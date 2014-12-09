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

// Serial port pins
#define SERIAL_RX USART0_RX_vect
#define SERIAL_UDRE USART0_UDRE_vect

#define LIMIT_MASK        ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits
#define STEP_MASK         ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits
#define DIRECTION_MASK    ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)) // All direction bits
#define STEPPING_MASK     (STEP_MASK | DIRECTION_MASK) // All stepping-related bits (step/direction)

#define STEPPERS_ENABLE_MASK (1<<STEPPERS_ENABLE_BIT)


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

#define PINOUT_MASK ((1<<PIN_RESET)|(1<<PIN_FEED_HOLD)|(1<<PIN_CYCLE_START))

#endif
