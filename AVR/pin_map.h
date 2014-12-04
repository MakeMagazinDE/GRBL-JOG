/*
  pin_map.h - Pin mapping configuration file
  Part of Grbl

  Copyright (c) 2013 Sungeun K. Jeon

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

/* The pin_map.h file serves as a central pin mapping settings file for different processor
   types, i.e. AVR 328p or AVR Mega 2560. Grbl officially supports the Arduino Uno, but the 
   other supplied pin mappings are supplied by users, so your results may vary. */

#ifndef pin_map_h
#define pin_map_h

// ###################################################################################
// ####          c't HACKS GRBL Controller with ATmega644 and Jog Pad            #####
// ###################################################################################

#ifdef PIN_MAP_MEGA_644 // by CM

  // Serial port pins
  #define SERIAL_RX USART0_RX_vect
  #define SERIAL_UDRE USART0_UDRE_vect

  // Increase Buffers to make use of extra SRAM
  #define RX_BUFFER_SIZE    128
  #define TX_BUFFER_SIZE    256
  #define BLOCK_BUFFER_SIZE 36
  #define LINE_BUFFER_SIZE  50
  
  // NOTE: All step bit and direction pins must be on the same port.
  #define STEPPING_DDR      DDRD
  #define STEPPING_PORT     PORTD
  #define X_STEP_BIT        6  // DSUB Pin 3
  #define Y_STEP_BIT        4  // DSUB Pin 5
  #define Z_STEP_BIT        2  // DSUB Pin 7
  #define X_DIRECTION_BIT   7  // DSUB Pin 2
  #define Y_DIRECTION_BIT   5  // DSUB Pin 4
  #define Z_DIRECTION_BIT   3  // DSUB Pin 6
  #define STEP_MASK         ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits
  #define DIRECTION_MASK    ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)) // All direction bits
  #define STEPPING_MASK     (STEP_MASK | DIRECTION_MASK) // All stepping-related bits (step/direction)

  #define STEPPERS_DISABLE_DDR    DDRB
  #define STEPPERS_DISABLE_PORT   PORTB
  #define STEPPERS_DISABLE_BIT    0  // DSUB Pin 1
  #define STEPPERS_DISABLE_MASK (1<<STEPPERS_DISABLE_BIT)

  // NOTE: All limit bit pins must be on the same port
  #define LIMIT_DDR         DDRB
  #define LIMIT_PIN         PINB
  #define LIMIT_PORT        PORTB
  #define X_LIMIT_BIT       1  // DSUB Pin 11
  #define Y_LIMIT_BIT       2  // DSUB Pin 12
  #define Z_LIMIT_BIT       3  // DSUB Pin 13 // Anmerkung: Hier bei POSITIVEN Z-Werten!
  #define LIMIT_INT         PCIE1  // Pin change interrupt enable pin
  #define LIMIT_INT_vect    PCINT1_vect 
  #define LIMIT_PCMSK       PCMSK1 // Pin change interrupt register
  #define LIMIT_MASK        ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits


  #define SPINDLE_ENABLE_DDR   DDRC
  #define SPINDLE_ENABLE_PORT  PORTC
  #define SPINDLE_ENABLE_BIT   7  // DSUB Pin 14

  #define COOLANT_FLOOD_DDR   DDRC
  #define COOLANT_FLOOD_PORT  PORTC
  #define COOLANT_FLOOD_BIT   6  // DSUB Pin 16 (PWM)

  #define LED_PRESENT  // if defined, spindle direction and coolant mist may not be used
  #define LED_DDR   DDRC
  #define LED_PORT  PORTC
//  #define LED_AUX_BIT 2
  #define LED_RUN_BIT   4   // LED 3
  #define LED_ERROR_BIT 5   // LED 2

  // #define SPINDLE_DIRECTION_PRESENT  
  // Spindle direction disabled by default. Uncomment to enable. Used by LED_RUN_BIT
  #ifdef SPINDLE_DIRECTION_PRESENT
    #define SPINDLE_DIRECTION_DDR   DDRC
    #define SPINDLE_DIRECTION_PORT  PORTC
    #define SPINDLE_DIRECTION_BIT   5
  #endif  

  // #define ENABLE_M7  
  // Mist coolant disabled by default. Uncomment to enable. Used by LED_ERROR_BIT
  #ifdef ENABLE_M7
    #define COOLANT_MIST_DDR   DDRC
    #define COOLANT_MIST_PORT  PORTC
    #define COOLANT_MIST_BIT   4
  #endif  

  // NOTE: All pinouts pins must be on the same port
  #define PINOUT_DDR       DDRC
  #define PINOUT_PIN       PINC
  #define PINOUT_PORT      PORTC
  #define PIN_RESET        0
  #define PIN_FEED_HOLD    1
  #define PIN_CYCLE_START  2
  #define PIN_SPIN_TOGGLE  3
  #define PINOUT_INT       PCIE2  // Pin change interrupt enable pin
  #define PINOUT_INT_vect  PCINT2_vect
  #define PINOUT_PCMSK     PCMSK2 // Pin change interrupt register, without Spindle Button
  #define PINOUT_MASK ((1<<PIN_RESET)|(1<<PIN_FEED_HOLD)|(1<<PIN_CYCLE_START))

  // Jog panel with speed pot and joystick switches by cm@ct.de
  
  // #define JOG_SPI_PRESENT  // transmits current position data if requested TODO, does not work
  #define JOG_SPI_DDR     DDRB
  #define JOG_SPI_PORT    PORTB
  #define JOG_SPI_MASK    0xA0  // PB5, PB7 outputs; PB4 = /SS Input
  #define JOG_SPI_SS      4     // Bit PB4 = /SS
  
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
  
  #define JOG_MIN_SPEED   25    // Hz, kleinste Geschwindigkeit, > 10!
  #define JOG_MAX_SPEED   8000  // Hz, größte Schrittgeschwindigkeit
  #define JOG_RAMP        5     // Ramp speed inc/dec in µs per loop (higher=faster)

#endif

/* 
#ifdef PIN_MAP_CUSTOM_PROC
  // For a custom pin map or different processor, copy and paste one of the default pin map
  // settings above and modify it to your needs. Then, make sure the defined name is also
  // changed in the config.h file.
#endif
*/

#endif
