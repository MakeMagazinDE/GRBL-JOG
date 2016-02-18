/*
  jogging.c - code pertaining to  jog switches
  Copyright (c) 2013 Carsten Meyer, cm@ct.de

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/
  
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "stepper.h"
#include "settings.h"
#include "nuts_bolts.h"
#include "gcode.h"
#include "config.h"
#include "spindle_control.h"
#include "motion_control.h"
#include "planner.h"
#include "protocol.h"
#include "limits.h"
#include "jogging.h"
#include "serial.h"
#include "report.h"
#include <avr/pgmspace.h>
#include "report.h"
#include "print.h"

#define ADCSRA_init 0x83  // AD enable, IRQ off, Prescaler 8
#define ADMUX_init  0x20  // ADLAR =1 (left adjustet, 8-Bit-Result on ADCH)

uint16_t led_count = 0;
uint8_t led_toggle;
uint32_t dest_step_rate, step_rate; // Step delay after pulse 
static uint8_t jog_bits, jog_bits_old, jog_exit, last_sys_state;
static uint8_t jstep_bits; 
static uint8_t jdir_bits; 
static uint8_t reverse_flag;
static uint8_t homedir_flag;
static uint8_t jog_select; 
static uint16_t skip_max; 
static volatile long work_position;
static volatile uint16_t skip_count; 
static uint32_t step_delay;
static uint8_t limit_state;




void jog_waitmsg() {
//  Falls während Tastendruck von Spindel oder Zero eine Status-Anfrage kommt
    if (sys.rt_exec_state & EXEC_STATUS_REPORT) {
       // status report requested, print short msg only
      printPgmString(PSTR("<JogF>\r\n"));
      sys.rt_exec_state = 0;
    }
}


void jog_init() {
// Initialize jog switch port bits and DDR
  #ifdef LED_PRESENT
    LED_DDR |= ((1<<LED_RUN_BIT) | (1<<LED_ERROR_BIT));
    LED_PORT |= ((1<<LED_RUN_BIT) | (1<<LED_ERROR_BIT)); // active low, so set high
  #endif
  JOGSW_DDR &= ~(JOGSW_MASK); // Set as input pins
  JOGSW_PORT |= (JOGSW_MASK); // Enable internal pull-up resistors. Active low operation.

  CONTROL_DDR &= ~(1<<SPIN_TOGGLE_PIN);  // Set as input pin
  CONTROL_PORT|= (1<<SPIN_TOGGLE_PIN);   // Pullup
  
  ADCSRA = ADCSRA_init;
  ADMUX = ADMUX_init | JOG_POT;     // Kanal, ADLAR =1 (left adjustet, 8-Bit-Result on ADCH)
}

void jog_btn_release() {
  uint8_t jog_bits;
  do {
    jog_bits = (~JOGSW_PIN) & JOGSW_MASK; // active low
    // geht nicht, weil wir daraus aufgerufen werden:         
    // protocol_execute_realtime(); // process the serial protocol while waiting
    jog_waitmsg();
    delay_ms(10);
  }
  while (jog_bits); // until released
}    

void spindle_btn_release() {
  uint8_t spindle_bits;
  do {
    spindle_bits = (~CONTROL_PIN) & (1<<SPIN_TOGGLE_PIN); // active low          
    // geht nicht, weil wir daraus aufgerufen werden:         
    // protocol_execute_realtime(); // process the serial protocol while waiting
    jog_waitmsg();
    delay_ms(10);
  }
  while (spindle_bits); // until released
}


void jogpad_check() 
// Tests jog port pins and moves steppers
{
	jstep_bits = 0; 
	jdir_bits = 0; 
	reverse_flag = 0;
	homedir_flag = 0;
	jog_select = 0; 
  jog_bits = 0;
  
	uint8_t i, spindle_bits;
	
#ifdef LED_PRESENT
  switch (sys.state) {
    case STATE_ALARM: case STATE_SAFETY_DOOR:
      LED_PORT &= ~(1<<LED_ERROR_BIT);  
      LED_PORT &= ~(1<<LED_RUN_BIT);                 
      break;
    case STATE_HOLD: case STATE_MOTION_CANCEL: 
      if (led_toggle) {
      	LED_PORT &= ~(1<<LED_ERROR_BIT);
      	LED_PORT |= (1<<LED_RUN_BIT); 
      } else {
      	LED_PORT &= ~(1<<LED_ERROR_BIT);
      	LED_PORT &= ~(1<<LED_RUN_BIT);                 
      }                
      break;
    case STATE_CYCLE: case STATE_HOMING:
      LED_PORT &= ~(1<<LED_RUN_BIT); 
      LED_PORT |= (1<<LED_ERROR_BIT);                 
      return;
    case STATE_IDLE:
      LED_PORT |= ((1<<LED_RUN_BIT) | (1<<LED_ERROR_BIT));                 
  }
#endif

	led_count += 1;
	if (led_count > 7000) { 
		led_toggle = ~led_toggle;
		led_count = 0; 
		}
		
  last_sys_state = sys.state;

  spindle_bits = (~CONTROL_PIN) & (1<<SPIN_TOGGLE_PIN); // active low          
  if (spindle_bits) {
    if (spindle_status) {
//      gc.spindle_direction = 0; 
      spindle_set_state(SPINDLE_DISABLE, gc_state.spindle_speed);  // Dummy-RPM
    }
    else {
//      gc.spindle_direction = 1;   // also update gcode spindle status
      spindle_set_state(SPINDLE_ENABLE_CW, gc_state.spindle_speed);  // RPM-Wert aus GCode-Status
    } 
    spindle_btn_release();  
    delay_ms(20);
  }

  jog_bits = (~JOGSW_PIN) & JOGSW_MASK; // active low
  if (!jog_bits) { return; }  // nothing pressed
  
  // At least one jog/joystick switch is active 
  if (jog_bits & (1<<JOG_ZERO)) {     // Zero-Button gedrückt
    jog_btn_release();
    sys.state = last_sys_state;
    if (bit_isfalse(CONTROL_PIN,bit(RESET_BIT))) { // RESET und zusätzlich ZERO gedrückt: Homing  
      if (bit_istrue(settings.flags,BITFLAG_HOMING_ENABLE)) { 
        // Only perform homing if Grbl is idle or lost.
        if ( sys.state==STATE_IDLE || sys.state==STATE_ALARM ) { 
// Search to engage all axes limit switches at faster homing seek rate.
  					limits_go_home(HOMING_CYCLE_0);  // Homing cycle 0
						#ifdef HOMING_CYCLE_1
    					limits_go_home(HOMING_CYCLE_1);  // Homing cycle 1
						#endif
						#ifdef HOMING_CYCLE_2
				    	limits_go_home(HOMING_CYCLE_2);  // Homing cycle 2
						#endif
         if (!sys.abort) { protocol_auto_cycle_start(); } // Execute startup scripts after successful homing.
        }
      }
    } else {
			zero_request_flag=1;
/*
// Nicht mehr selbsttätige Nullung, sondern <Zero...> beim nächsten Status-Request senden, siehe report.c
    	
//  gc_state.position[N_AXIS];      // Where the interpreter considers the tool to be at this point in the code
//  gc_state.coord_system[N_AXIS];  // Current work coordinate system (G54+). Stores offset from absolute machine
                                  	// position in mm. Loaded from EEPROM when called.  
//  gc_state.coord_offset[N_AXIS];  // Retains the G92 coordinate offset (work coordinates) relative to
                                  	// machine zero in mm. Non-persistent. Cleared upon reset and boot.    
    	
      for (i=0; i<N_AXIS; i++) { // Axes indices are consistent, so loop may be used.      	
        if (i == Z_AXIS) 
        	{ 
        		gc_state.coord_offset[i] = gc_state.position[i] - 10; 
        	}
        else
        	{ 
        		gc_state.coord_offset[i] = gc_state.position[i]; 
        	}
      	}  

    	return;
*/
    }	
  }
  ADCSRA = ADCSRA_init | (1<<ADIF); //0x93, clear ADIF
  ADCSRA = ADCSRA_init | (1<<ADIF); //0x93, clear ADIF
  ADCSRA = ADCSRA_init | (1<<ADSC); //0xC3; start conversion  
  
  sys.state = STATE_JOG;
  
  // check for reverse switches 
  if (jog_bits & (1<<JOGREV_X_BIT)) { // X reverse switch on
    jdir_bits ^= (1<<X_DIRECTION_BIT);
    jstep_bits = jdir_bits ^ (1<<X_STEP_BIT); 
    reverse_flag = 1;
  }                                                            
  if (jog_bits & (1<<JOGREV_Y_BIT)) { // Y reverse switch on
    jdir_bits ^= (1<<Y_DIRECTION_BIT);
    jstep_bits = jdir_bits ^ (1<<Y_STEP_BIT);
    reverse_flag = 1;
    jog_select = 1;
  }                                                            
  if (jog_bits & (1<<JOGREV_Z_BIT)) { // Z reverse switch on
    jdir_bits ^= (1<<Z_DIRECTION_BIT);
    jstep_bits = jdir_bits ^ (1<<Z_STEP_BIT);
    reverse_flag = 1; // positive Z dir!
    jog_select = 2;
  } 
  
  // check for forward switches 
  if (jog_bits & (1<<JOGFWD_X_BIT)) { // X forward switch on
    jstep_bits = jdir_bits ^ (1<<X_STEP_BIT);
  }                                                            
  if (jog_bits & (1<<JOGFWD_Y_BIT)) { // Y forward switch on
    jstep_bits = jdir_bits ^ (1<<Y_STEP_BIT);
    jog_select = 1;
  }                                                            
  if (jog_bits & (1<<JOGFWD_Z_BIT)) { // Z forward switch on
    jstep_bits = jdir_bits ^ (1<<Z_STEP_BIT);
    jog_select = 2;
  } 
  homedir_flag = reverse_flag;  // Richtung der Schalter, Begrenzung Jogging invertrieren wenn positive Richtung
  if (!bit_istrue(settings.homing_dir_mask,bit(jog_select))) {
    homedir_flag = homedir_flag ^ 1; }

	// (JOG_MAX_SPEED-JOG_MIN_SPEED)/256
	uint32_t max_frequ;
	uint32_t jog_speed_fac;
	 
	max_frequ = (settings.max_rate[jog_select] * settings.steps_per_mm[jog_select]) / 60; // max_rate war in mm/min, max_freq in Hz
	jog_speed_fac = (max_frequ - JOG_MIN_SPEED)/256;	 	

  dest_step_rate = ADCH;    // set initial dest_step_rate according to analog input
  dest_step_rate = (dest_step_rate * jog_speed_fac) + JOG_MIN_SPEED;  
  step_rate = JOG_MIN_SPEED;   // set initial step rate
  jog_exit = 0; 
  while (!(ADCSRA && (1<<ADIF))) {} // warte bis ADIF-Bit gesetzt 
  ADCSRA = ADCSRA_init; // exit conversion
  
  jog_bits_old = jog_bits;
  i = 0;  // now index for sending position data 
 
 	float mm_per_step;
 
  if (bit_istrue(settings.flags,BITFLAG_REPORT_INCHES)) {
    mm_per_step = 1/(settings.steps_per_mm[jog_select] * INCH_PER_MM);
  } else {
    mm_per_step = 1/settings.steps_per_mm[jog_select];
  }  
  	
  work_position = sys.position[jog_select];  // Ausgangswert
  plan_reset(); // Reset planner buffer to zero planner current position and to clear previous motions.

	stepper_init();  // initialisiert Timer 1 OC
  TIMSK0 &= ~(1<<OCIE0A); // Disable Timer0 Compare Match A interrupt

  TCCR1B = (TCCR1B & ~((1<<CS12) | (1<<CS11))) | (1<<CS10); // Set in st_go_idle().
  TIMSK1 &= ~(1<<OCIE1A);  // Timer 1 zunächst abgeschaltet

  LED_PORT &= ~(1<<LED_RUN_BIT); 

  uint8_t idx;
  uint8_t step_port_invert_mask = 0;
  uint8_t dir_port_invert_mask = 0;
  for (idx=0; idx<N_AXIS; idx++) {
    if (bit_istrue(settings.step_invert_mask,bit(idx))) { step_port_invert_mask |= get_step_pin_mask(idx); }
    if (bit_istrue(settings.dir_invert_mask,bit(idx))) { dir_port_invert_mask |= get_direction_pin_mask(idx); }
  }

  DIRECTION_PORT = (DIRECTION_PORT & ~DIRECTION_MASK) | ((jdir_bits ^ dir_port_invert_mask) & DIRECTION_MASK);
  STEP_PORT = (STEP_PORT & ~STEP_MASK) | (step_port_invert_mask & STEP_MASK);  // noch nicht aktiv

  delay_us(settings.pulse_microseconds);

  // Enable stepper drivers.
  if (bit_istrue(settings.flags,BITFLAG_INVERT_ST_ENABLE)) 
  	{ STEPPERS_DISABLE_PORT |= (1<<STEPPERS_DISABLE_BIT); }
  else 
  	{ STEPPERS_DISABLE_PORT &= ~(1<<STEPPERS_DISABLE_BIT); }

	jog_active = true;	// set jogging flag, -cm

  skip_count = 0;
  skip_max = 8;			// Prescaler nicht ändern, deshalb so
  limit_state = limits_get_state();  //  LIMIT_MASK & (LIMIT_PIN ^ settings.invert_mask);
	step_delay = 10000;
	OCR1A = step_delay;
	TIMSK1 |= (1<<OCIE1A);  // Timer 1 starten	  
  	
  for(;;) { // repeat until button/joystick released  
    
    ADCSRA = ADCSRA_init | (1<<ADIF); //0x93, clear ADIF

    // Get limit pin state
    limit_state = limits_get_state();  //  LIMIT_MASK & (LIMIT_PIN ^ settings.invert_mask);
    jog_bits = (~JOGSW_PIN) & JOGSW_MASK; // active low, neu abfragen weil in der Schleife
    step_delay = (1710000/step_rate); //
    // Maximaler Wert für 16-Bit-Timer darf nicht überschritten werden
    if (step_delay > 65535) {
    		step_delay = 65535;
    	}
        
    ADCSRA = ADCSRA_init | (1<<ADSC); //0xC3; start ADC conversion

    // jog_exit wird im ISR gesetzt sobald Stopp erreicht
    if (jog_exit || (sys.rt_exec_state & EXEC_RESET)) {
   		TIMSK1 &= ~(1<<OCIE1A);	// Timer disable
			jog_active = false;	// reset jogging flag, -cm
      LED_PORT |= (1<<LED_RUN_BIT);                 
      st_reset(); 
      stepper_init(); 
      sys.state = last_sys_state;
      sys.position[jog_select] = work_position;
      plan_sync_position();  	//	sys.position --> pl.position
    	gc_sync_position();  		//	system_convert_array_steps_to_mpos(gc_state.position,sys.position);
      return; 
    }

//  report_realtime_status()  benötigt viel Zeit, deshalb Minimalmeldung
    if (sys.rt_exec_state & EXEC_STATUS_REPORT) {
       // status report requested, print short msg only
      printPgmString(PSTR("<Jog"));
      serial_write(88 + jog_select); // 88 = X + 1 = Y etc.
      serial_write(44);
      // Relativposition zum Werkstück-Nullpunkt ermitteln aud ausgeben
      printFloat_CoordValue(work_position * mm_per_step - gc_state.coord_offset[jog_select]);
      serial_write(62); // ">" char
      serial_write(13);
      serial_write(10);
      sys.rt_exec_state = 0;
    }
    
    delay_us(100);	// wg. AD-Wandlung ohnehin benötigt
      
    while (!(ADCSRA && (1<<ADIF))) {} // warte ggf. bis ADIF-Bit gesetzt  
    ADCSRA = ADCSRA_init;     // exit conversion
    dest_step_rate = ADCH;    // set next dest_step_rate according to analog input
    dest_step_rate = (dest_step_rate * jog_speed_fac) + JOG_MIN_SPEED;  

  }
}


// ISR(TIMER1_COMPA_vect) wird hierhin umgeleitet wenn jog_active true ist
// Interupt setzt eigenen Counterwert neu und berechnet gewünschte Steprate.
void jog_isr() {
  if (skip_count == 0) {

		OCR1A = step_delay; 	// Counter ist jetzt auf Null, darf neu gesetzt werden

    if (limit_state && homedir_flag) { jog_exit = 1; } // immediate stop on any switch
 
    if (jog_bits == jog_bits_old) { // nothing changed
      if (step_rate < (dest_step_rate - 5)) { // Hysterese für A/D-Wandlung
        step_rate += JOG_RAMP; // accellerate
      }                                    
      if (step_rate > (dest_step_rate + 5)) { // Hysterese für A/D-Wandlung
        step_rate -= JOG_RAMP; // brake
      }                                    
    }
    else {
      if (step_rate > (JOG_MIN_SPEED*2)) {  // fast brake to complete stop
        step_rate = ((step_rate * 99) / 100) - JOG_MIN_SPEED;
      }
      else { jog_exit = 1; } // finished to stop and exit
    }

		// Set stepping pins, nochmal invertieren
  	STEP_PORT = STEP_PORT ^ (jstep_bits & STEP_MASK);

	  if (reverse_flag) {
	    work_position -= 1;
	  } 
	  else {
	    work_position += 1;    // relative print_position in mm since last report
	  }
	  skip_count = skip_max;

  	// Delay für Step-Impuls im Interrupt, nicht so sehr schön, aber einfach
  	delay_us(settings.pulse_microseconds); 

	 	// Reset stepping pins, nochmal invertieren
	  STEP_PORT = STEP_PORT ^ (jstep_bits & STEP_MASK);

	} else {
	  skip_count -= 1;
	}
}

