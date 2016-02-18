/*
  cpu_map_atmega744p.h - CPU and pin mapping configuration file
  Part of Grbl

  Copyright (c) 2012-2015 Sungeun K. Jeon

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

/* This cpu_map file serves as a central pin mapping settings file for AVR Mega644 */
// angepasst für GRBL-JOG

#ifdef GRBL_PLATFORM
#error "cpu_map already defined: GRBL_PLATFORM=" GRBL_PLATFORM
#endif


#define GRBL_PLATFORM "Atmega644p"

// Serial port pins
#define SERIAL_RX USART0_RX_vect
#define SERIAL_UDRE USART0_UDRE_vect

// Increase Buffers to make use of extra SRAM
//#define RX_BUFFER_SIZE		256
//#define TX_BUFFER_SIZE		128
//#define BLOCK_BUFFER_SIZE	36
//#define LINE_BUFFER_SIZE	100

// Define step pulse output pins. NOTE: All step bit pins must be on the same port.
// Settings for Chinese Driver Board with Display (Board_2).

#define STEP_DDR      DDRD
#define STEP_PORT     PORTD
#define STEP_PIN      PIND
#define X_STEP_BIT    6 // 
#define Y_STEP_BIT    4 // 
#define Z_STEP_BIT    2 // 
#define STEP_MASK ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits

// Define step direction output pins. NOTE: All direction pins must be on the same port.
#define DIRECTION_DDR     DDRD
#define DIRECTION_PORT    PORTD
#define DIRECTION_PIN     PIND
#define X_DIRECTION_BIT   7 // 
#define Y_DIRECTION_BIT   5 // 
#define Z_DIRECTION_BIT   3 // 
#define DIRECTION_MASK ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)) // All direction bits

// Define stepper driver enable/disable output pin.
#define STEPPERS_DISABLE_DDR   DDRB
#define STEPPERS_DISABLE_PORT  PORTB
#define STEPPERS_DISABLE_BIT   0 // 
#define STEPPERS_DISABLE_MASK (1<<STEPPERS_DISABLE_BIT)

// Define homing/hard limit switch input pins and limit interrupt vectors. 
// NOTE: All limit bit pins must be on the same port
#define LIMIT_DDR       DDRB
#define LIMIT_PORT      PORTB
#define LIMIT_PIN       PINB
#define X_LIMIT_BIT     1 // 
#define Y_LIMIT_BIT     2 // 
#define Z_LIMIT_BIT     3 // 
#define LIMIT_INT         PCIE1  // Pin change interrupt enable pin
#define LIMIT_INT_vect    PCINT1_vect 
#define LIMIT_PCMSK       PCMSK1 // Pin change interrupt register
#define LIMIT_MASK ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits




// Define spindle enable and spindle direction output pins.
#define SPINDLE_ENABLE_DDR    DDRC
#define SPINDLE_ENABLE_PORT   PORTC
// Z Limit pin and spindle PWM/enable pin swapped to access hardware PWM on Pin 11.
#ifdef VARIABLE_SPINDLE 
  #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
    // If enabled, spindle direction pin now used as spindle enable, while PWM remains on D11.
    #define SPINDLE_ENABLE_BIT    7  // Uno Digital Pin 13 (NOTE: D13 can't be pulled-high input due to LED.)
  #else
    #define SPINDLE_ENABLE_BIT    7  // Uno Digital Pin 11
  #endif
#else
  #define SPINDLE_ENABLE_BIT    7  // Uno Digital Pin 12
#endif
#ifndef USE_SPINDLE_DIR_AS_ENABLE_PIN
  #define SPINDLE_DIRECTION_DDR   DDRC
  #define SPINDLE_DIRECTION_PORT  PORTC
  #define SPINDLE_DIRECTION_BIT   7  // Uno Digital Pin 13 (NOTE: D13 can't be pulled-high input due to LED.)
#endif
  
// Define flood and mist coolant enable output pins.
// NOTE: Uno analog pins 4 and 5 are reserved for an i2c interface, and may be installed at
// a later date if flash and memory space allows.
#define COOLANT_FLOOD_DDR   DDRC
#define COOLANT_FLOOD_PORT  PORTC
#define COOLANT_FLOOD_BIT   6  // Uno Analog Pin 3
#ifdef ENABLE_M7 // Mist coolant disabled by default. See config.h to enable/disable.
  #define COOLANT_MIST_DDR   DDRC
  #define COOLANT_MIST_PORT  PORTC
  #define COOLANT_MIST_BIT   6 // Uno Analog Pin 4
#endif  


#define LED_PRESENT  // if defined, spindle direction and coolant mist may not be used
#define LED_DDR   DDRC
#define LED_PORT  PORTC
//  #define LED_AUX_BIT 2
#define LED_RUN_BIT   4   // LED 3
#define LED_ERROR_BIT 5   // LED 2


// Define user-control controls (cycle start, reset, feed hold) input pins.
// NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
#define CONTROL_DDR       DDRC
#define CONTROL_PIN       PINC
#define CONTROL_PORT      PORTC
#define RESET_BIT         0  // Uno Analog Pin 0
#define FEED_HOLD_BIT     1  // Uno Analog Pin 1
#define CYCLE_START_BIT   2  // Uno Analog Pin 2

#define SPIN_TOGGLE_PIN  3

#define SAFETY_DOOR_BIT   1  // Uno Analog Pin 1 NOTE: Safety door is shared with feed hold. Enabled by config define.
#define CONTROL_INT       PCIE2  // Pin change interrupt enable pin
#define CONTROL_INT_vect  PCINT2_vect
#define CONTROL_PCMSK     PCMSK2 // Pin change interrupt register
#define CONTROL_MASK ((1<<RESET_BIT)|(1<<FEED_HOLD_BIT)|(1<<CYCLE_START_BIT)|(1<<SAFETY_DOOR_BIT))
#define CONTROL_INVERT_MASK CONTROL_MASK // May be re-defined to only invert certain control pins.

  
// Define probe switch input pin.
#define PROBE_DDR       DDRB
#define PROBE_PIN       PINB
#define PROBE_PORT      PORTB
#define PROBE_BIT       4  // SS pin -cm
#define PROBE_MASK      (1<<PROBE_BIT)

// Start of PWM & Stepper Enabled Spindle
#ifdef VARIABLE_SPINDLE
  // Advanced Configuration Below You should not need to touch these variables
  #define PWM_MAX_VALUE    255.0
  #define TCCRA_REGISTER	 TCCR2A
  #define TCCRB_REGISTER	 TCCR2B
  #define OCR_REGISTER     OCR2A
  
  #define COMB_BIT	     COM2A1
  #define WAVE0_REGISTER	 WGM20
  #define WAVE1_REGISTER	 WGM21
  #define WAVE2_REGISTER	 WGM22
  #define WAVE3_REGISTER	 WGM23
      
  // NOTE: On the 328p, these must be the same as the SPINDLE_ENABLE settings.
  #define SPINDLE_PWM_DDR	  DDRB
  #define SPINDLE_PWM_PORT  PORTB
  #define SPINDLE_PWM_BIT	  3    // Uno Digital Pin 11
#endif // End of VARIABLE_SPINDLE



#define JOGSW_DDR       DDRA
#define JOGSW_PIN       PINA
#define JOGSW_PORT      PORTA
#define JOGSW_MASK      0x7F  // Bits 0..6 used
#define JOGREV_X_BIT    0     // Switch input bits
#define JOGFWD_X_BIT    1  
#define JOGREV_Y_BIT    2
#define JOGFWD_Y_BIT    3
#define JOGREV_Z_BIT    4
#define JOGFWD_Z_BIT    5
#define JOG_ZERO        6
#define JOG_POT         7     // analog pin

