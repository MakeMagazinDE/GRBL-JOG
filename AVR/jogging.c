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
#include "report.h"
#include <avr/pgmspace.h>
#include "report.h"
#include "print.h"

#define JOG_SPEED_FAC (JOG_MAX_SPEED-JOG_MIN_SPEED)/255
#define ADCSRA_init 0x83  // AD enable, IRQ off, Prescaler 8
#define ADMUX_init  0x20  // ADLAR =1 (left adjustet, 8-Bit-Result on ADCH)


void jog_init() {
// Initialize jog switch port bits and DDR
  #ifdef LED_PRESENT
    LED_DDR |= ((1<<LED_RUN_BIT) | (1<<LED_ERROR_BIT));
    LED_PORT |= ((1<<LED_RUN_BIT) | (1<<LED_ERROR_BIT)); // active low, so set high
  #endif
  JOGSW_DDR &= ~(JOGSW_MASK); // Set as input pins
  JOGSW_PORT |= (JOGSW_MASK); // Enable internal pull-up resistors. Active low operation.

  PINOUT_DDR &= ~(1<<PIN_SPIN_TOGGLE);  // Set as input pin
  PINOUT_PORT|= (1<<PIN_SPIN_TOGGLE);   // Pullup
  
  ADCSRA = ADCSRA_init;
  ADMUX = ADMUX_init | JOG_POT;     // Kanal, ADLAR =1 (left adjustet, 8-Bit-Result on ADCH)

  #ifdef JOG_SPI_PRESENT
    JOG_SPI_DDR |= JOG_SPI_MASK; // Set MOSI, SCK and /SS output
    SPCR = (1<<SPE)|(1<<MSTR); // Enable SPI, Master, set clock rate fck/2.
    SPSR = (1<<SPI2X); // Double speed
    JOG_SPI_PORT |= (1<<JOG_SPI_SS);  // Slave select pullup on
  #endif
}

#ifdef JOG_SPI_PRESENT
// TODO: send SPI on /SS request. Dummy data for now

  void spi_transmit(uint8_t SPI_data) {
    /* Start transmission */
    SPDR = SPI_data;
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));
  }
  
  void send_spi_position(uint8_t axis_idx) {
    JOG_SPI_PORT &= ~(1<<JOG_SPI_SS); // Slave select active low
    spi_transmit(axis_idx); // TEST DATA
    spi_transmit(255);
    spi_transmit(85);
    spi_transmit(0);
    JOG_SPI_PORT |= (1<<JOG_SPI_SS);
  }
#endif

void jog_btn_release() {
  uint8_t jog_bits;
  do {
    jog_bits = (~JOGSW_PIN) & JOGSW_MASK; // active low         
    protocol_process(); // process the serial protocol while waiting
    protocol_execute_runtime();
  }
  while (jog_bits); // until released
}    

void spindle_btn_release() {
  uint8_t spindle_bits;
  do {
    spindle_bits = (~PINOUT_PIN) & (1<<PIN_SPIN_TOGGLE); // active low          
    protocol_process(); // process the serial protocol while waiting
    protocol_execute_runtime();
  }
  while (spindle_bits); // until released
}

void jogging() 
// Tests jog port pins and moves steppers
{
  uint8_t jog_bits, jog_bits_old, out_bits0, jog_exit, last_sys_state;
  uint8_t i, limit_state, spindle_bits;
  
  uint32_t dest_step_rate, step_rate, step_delay; // Step delay after pulse 

  switch (sys.state) {
    case STATE_CYCLE: case STATE_HOMING: case STATE_INIT:
      LED_PORT |= (1<<LED_ERROR_BIT);     
      return;
    case STATE_ALARM: 
      LED_PORT &= ~(1<<LED_ERROR_BIT);  break;
    default: 
      LED_PORT |= (1<<LED_ERROR_BIT);                 
  }
  last_sys_state = sys.state;
  
  spindle_bits = (~PINOUT_PIN) & (1<<PIN_SPIN_TOGGLE); // active low          
  if (spindle_bits) {
    if (spindle_status) {
//      gc.spindle_direction = 0; 
      spindle_run(0);
    }
    else {
//      gc.spindle_direction = 1;   // also update gcode spindle status
      spindle_run(1);
    } 
    spindle_btn_release();  
    delay_ms(20);
  }

  jog_bits = (~JOGSW_PIN) & JOGSW_MASK; // active low
  if (!jog_bits) { return; }  // nothing pressed
  
  // At least one jog/joystick switch is active 
  
  ADCSRA = ADCSRA_init | (1<<ADIF); //0x93, clear ADIF

  uint8_t reverse_flag = 0;
  uint8_t out_bits = 0; 
  uint8_t jog_select = 0; 
  out_bits0 = (0) ^ (settings.invert_mask); 
  
  ADCSRA = ADCSRA_init | (1<<ADIF); //0x93, clear ADIF
  ADCSRA = ADCSRA_init | (1<<ADSC); //0xC3; start conversion  
  
  sys.state = STATE_JOG;
  
  // check for reverse switches 
  if (jog_bits & (1<<JOGREV_X_BIT)) { // X reverse switch on
    out_bits0 ^= (1<<X_DIRECTION_BIT);
    out_bits = out_bits0 ^ (1<<X_STEP_BIT); 
    reverse_flag = 1;
  }                                                            
  if (jog_bits & (1<<JOGREV_Y_BIT)) { // Y reverse switch on
    out_bits0 ^= (1<<Y_DIRECTION_BIT);
    out_bits = out_bits0 ^ (1<<Y_STEP_BIT);
    reverse_flag = 1;
    jog_select = 1;
  }                                                            
  if (jog_bits & (1<<JOGREV_Z_BIT)) { // Z reverse switch on
    out_bits0 ^= (1<<Z_DIRECTION_BIT);
    out_bits = out_bits0 ^ (1<<Z_STEP_BIT);
    reverse_flag = 1;
    jog_select = 2;
  } 
  
  // check for forward switches 
  if (jog_bits & (1<<JOGFWD_X_BIT)) { // X forward switch on
    out_bits = out_bits0 ^ (1<<X_STEP_BIT);
  }                                                            
  if (jog_bits & (1<<JOGFWD_Y_BIT)) { // Y forward switch on
    out_bits = out_bits0 ^ (1<<Y_STEP_BIT);
    jog_select = 1;
  }                                                            
  if (jog_bits & (1<<JOGFWD_Z_BIT)) { // Z forward switch on
    out_bits = out_bits0 ^ (1<<Z_STEP_BIT);
    jog_select = 2;
  } 

  if (jog_bits & (1<<JOG_ZERO)) {     // Zero-Button gedrückt
    jog_btn_release();
    sys.state = last_sys_state;
    if (bit_isfalse(PINOUT_PIN,bit(PIN_RESET))) { // RESET und zusätzlich ZERO gedrückt: Homing  
      if (bit_istrue(settings.flags,BITFLAG_HOMING_ENABLE)) { 
        // Only perform homing if Grbl is idle or lost.
        if ( sys.state==STATE_IDLE || sys.state==STATE_ALARM ) { 
          mc_go_home(); 
          if (!sys.abort) { protocol_execute_startup(); } // Execute startup scripts after successful homing.
        }
      }
    } else { // Zero current work position
//      gc.coord_system[i]    Current work coordinate system (G54+). Stores offset from absolute machine
//                            position in mm. Loaded from EEPROM when called.
//      gc.coord_offset[i]    Retains the G92 coordinate offset (work coordinates) relative to
//                            machine zero in mm. Non-persistent. Cleared upon reset and boot.  
      for (i=0; i<=2; i++) { // Axes indices are consistent, so loop may be used.
        gc.coord_offset[i] = gc.position[i]-gc.coord_system[i];
        #ifdef JOG_SPI_PRESENT
          send_spi_position(i);
        #endif
      } 
    }                   
    return;
  } 
  dest_step_rate = ADCH;    // set initial dest_step_rate according to analog input
  dest_step_rate = (dest_step_rate * JOG_SPEED_FAC) + JOG_MIN_SPEED;  
  step_rate = JOG_MIN_SPEED;   // set initial step rate
  jog_exit = 0; 
  while (!(ADCSRA && (1<<ADIF))) {} // warte bis ADIF-Bit gesetzt 
  ADCSRA = ADCSRA_init; // exit conversion
  st_wake_up();
  // prepare direction with small delay, direction settle time
  STEPPING_PORT = (STEPPING_PORT & ~STEPPING_MASK) | (out_bits0 & STEPPING_MASK);
  delay_us(10);
  jog_bits_old = jog_bits;
  i = 0;  // now index for sending position data 
  
  for(;;) { // repeat until button/joystick released  
    // report_realtime_status();
    ADCSRA = ADCSRA_init | (1<<ADIF); //0x93, clear ADIF

    // Get limit pin state
    #ifdef LIMIT_SWITCHES_ACTIVE_HIGH
      // When in an active-high switch configuration
      limit_state = LIMIT_PIN;
    #else
      limit_state = LIMIT_PIN ^ LIMIT_MASK;
    #endif
    if ((limit_state & LIMIT_MASK) && reverse_flag) { jog_exit = 1; } // immediate stop
 
    jog_bits = (~JOGSW_PIN) & JOGSW_MASK; // active low
    if (jog_bits == jog_bits_old) { // nothing changed
      if (step_rate < (dest_step_rate - 5)) { // Hysterese für A/D-Wandlung
        step_rate += JOG_RAMP; // accellerate
      }                                    
      if (step_rate > (dest_step_rate + 5)) { // Hysterese für A/D-Wandlung
        step_rate -= JOG_RAMP; // brake
      }                                    
    }
    else {
      if (step_rate > (JOG_MIN_SPEED*2)) {  // switch change happened, fast brake to complete stop
        step_rate = ((step_rate * 99) / 100) - 10;
      }
      else { jog_exit = 1; } // finished to stop and exit
    }
    
   
    // stop and exit if done
    if (jog_exit || (sys.execute & EXEC_RESET)) { 
      st_go_idle(); 
      sys.state = last_sys_state;
      sys_sync_current_position();
      return; 
    }
    
    // update position registers
    if (reverse_flag) {
      sys.position[jog_select]--;
    } 
    else {
      sys.position[jog_select]++; 
    }
    ADCSRA = ADCSRA_init | (1<<ADSC); //0xC3; start ADC conversion
    // Both direction and step pins appropriately inverted and set. Perform one step
    STEPPING_PORT = (STEPPING_PORT & ~STEPPING_MASK) | (out_bits & STEPPING_MASK);
    delay_us(settings.pulse_microseconds);
    STEPPING_PORT = (STEPPING_PORT & ~STEPPING_MASK) | (out_bits0 & STEPPING_MASK);
    step_delay = (1000000/step_rate) - settings.pulse_microseconds - 100; // 100 = fester Wert für Schleifenzeit
  
    if (sys.execute & EXEC_STATUS_REPORT) { // status report requested, print short msg only
      printPgmString(PSTR("Jog\r\n"));
      sys.execute = 0;
    }
    delay_us(step_delay);
    
    send_spi_position(i); // bei jedem Durchlauf nur eine Achse übertragen
    i++;
    if (i>2) {i=0;}
      
    while (!(ADCSRA && (1<<ADIF))) {} // warte ggf. bis ADIF-Bit gesetzt  
    ADCSRA = ADCSRA_init;     // exit conversion
    dest_step_rate = ADCH;    // set next dest_step_rate according to analog input
    dest_step_rate = (dest_step_rate * JOG_SPEED_FAC) + JOG_MIN_SPEED;  

  }
}
