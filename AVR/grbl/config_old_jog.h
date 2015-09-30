/*
  config.h - compile time configuration
  Part of Grbl

  Copyright (c) 2011-2013 Sungeun K. Jeon
  Copyright (c) 2009-2011 Simen Svale Skogsrud

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

// This file contains compile-time configurations for Grbl's internal system. For the most part,
// users will not need to directly modify these, but they are here for specific needs, i.e.
// performance tuning or adjusting to non-typical machines.

// IMPORTANT: Any changes here requires a full re-compiling of the source code to propagate them.

#ifndef config_h
#define config_h

#define GRBL_VERSION "0.8c/jog.2"

// Default settings. Used when resetting EEPROM. Change to desired name in defaults.h
#define DEFAULTS_HACKSWOOD
// #define DEFAULTS_HACKSALUM

// Serial baud rate
#define BAUD_RATE 19200

// NOTE: All step bit and direction pins must be on the same port.
// Settings for Chinese Driver Board with Display (Board_2).
#define STEPPING_DDR      DDRD
#define STEPPING_PORT     PORTD
#define X_STEP_BIT        6  // DSUB Pin 3
#define Y_STEP_BIT        4  // DSUB Pin 5
#define Z_STEP_BIT        2  // DSUB Pin 7
#define X_DIRECTION_BIT   7  // DSUB Pin 2
#define Y_DIRECTION_BIT   5  // DSUB Pin 4
#define Z_DIRECTION_BIT   3  // DSUB Pin 6

// Settings for newer Chinese Driver Boards (Board_3). Enable must be re-wired from D-Sub Pin 1 to Pin 16
/*
#define X_STEP_BIT        7  // DSUB Pin 3
#define Y_STEP_BIT        5  // DSUB Pin 5
#define Z_STEP_BIT        3  // DSUB Pin 7
#define X_DIRECTION_BIT   6  // DSUB Pin 2
#define Y_DIRECTION_BIT   4  // DSUB Pin 4
#define Z_DIRECTION_BIT   2  // DSUB Pin 6
*/

#define STEPPERS_ENABLE_DDR    DDRB
#define STEPPERS_ENABLE_PORT   PORTB
#define STEPPERS_ENABLE_BIT    0  // DSUB Pin 1

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


// EEPROM Default values
#ifdef DEFAULTS_HACKSWOOD
  // Grbl default settings for C'T HACKS WOODMILL. 
  // Note Z scaling in gcode.c and report.c for different Z axis speed (well, its a kludge...)
  #define DEFAULT_Z_SCALE 5.0  // Trick um für Z kleineren Vorschub zu erzwingen
  #define DEFAULT_Z_GAUGE 10.0 // mm, Z Gauge Block Dicke
  #define DEFAULT_Z_PULLOFF 0.0 // mm, auto pull back Z after pressing ZERO center btn
  #define DEFAULT_X_STEPS_PER_MM 80.0
  #define DEFAULT_Y_STEPS_PER_MM 80.0
  #define DEFAULT_Z_STEPS_PER_MM 533.333/DEFAULT_Z_SCALE // note Z-scaling to reduce axis speed

  #define DEFAULT_STEP_PULSE_MICROSECONDS 10
  #define DEFAULT_MM_PER_ARC_SEGMENT 0.2
  #define DEFAULT_RAPID_FEEDRATE 5000.0 // mm/min
  #define DEFAULT_FEEDRATE 400.0
  #define DEFAULT_ACCELERATION (200.0*60*60) // 200 mm/min^2
  #define DEFAULT_JUNCTION_DEVIATION 0.1 // mm
  #define DEFAULT_LIMIT_INVERT_MASK 0
  #define DEFAULT_REPORT_INCHES 0 // false
  #define DEFAULT_AUTO_START 1 // true
  #define DEFAULT_INVERT_ST_ENABLE 0 // true
  #define DEFAULT_HARD_LIMIT_ENABLE 0  // false
  #define DEFAULT_HOMING_ENABLE 1  // true
  #define DEFAULT_HOMING_DIR_MASK ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)) // move negative dir
  #define DEFAULT_HOMING_RAPID_FEEDRATE 1500.0 // mm/min
  #define DEFAULT_HOMING_FEEDRATE 200.0 // mm/min
  #define DEFAULT_HOMING_DEBOUNCE_DELAY 100 // msec (0-65k)
  #define DEFAULT_HOMING_PULLOFF 5.0 // mm
  #define DEFAULT_STEPPER_IDLE_LOCK_TIME 25 // msec (0-255)
  #define DEFAULT_DECIMAL_PLACES 2
  #define DEFAULT_N_ARC_CORRECTION 25
#endif
#ifdef DEFAULTS_HACKSALUM
  // Grbl default settings for C'T HACKS ALUMINUM MILL. 
  // No Z Scaling.
  #define DEFAULT_Z_SCALE 1.0  // Trick um für Z kleineren Vorschub zu erzwingen
  #define DEFAULT_Z_GAUGE 10.0 // mm, Z Gauge Block Dicke
  #define DEFAULT_Z_PULLOFF 0.0 // mm, auto pull back Z after pressing ZERO center btn
  #define DEFAULT_X_STEPS_PER_MM 266.66
  #define DEFAULT_Y_STEPS_PER_MM 266.66
  #define DEFAULT_Z_STEPS_PER_MM 266.66

  #define DEFAULT_STEP_PULSE_MICROSECONDS 10
  #define DEFAULT_MM_PER_ARC_SEGMENT 0.2
  #define DEFAULT_RAPID_FEEDRATE 2500.0 // mm/min
  #define DEFAULT_FEEDRATE 400.0
  #define DEFAULT_ACCELERATION (150.0*60*60) // 200 mm/min^2
  #define DEFAULT_JUNCTION_DEVIATION 0.05 // mm
  #define DEFAULT_LIMIT_INVERT_MASK 14
  #define DEFAULT_REPORT_INCHES 0 // false
  #define DEFAULT_INVERT_ST_ENABLE 1 // true
  #define DEFAULT_AUTO_START 1 // true
  #define DEFAULT_HARD_LIMIT_ENABLE 0  // false
  #define DEFAULT_HOMING_ENABLE 1  // true
  #define DEFAULT_HOMING_DIR_MASK ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)) // move negative dir on XY
  #define DEFAULT_HOMING_RAPID_FEEDRATE 1500.0 // mm/min
  #define DEFAULT_HOMING_FEEDRATE 200.0 // mm/min
  #define DEFAULT_HOMING_DEBOUNCE_DELAY 100 // msec (0-65k)
  #define DEFAULT_HOMING_PULLOFF 5.0 // mm
  #define DEFAULT_STEPPER_IDLE_LOCK_TIME 25 // msec (0-255)
  #define DEFAULT_DECIMAL_PLACES 2
  #define DEFAULT_N_ARC_CORRECTION 25
#endif


// Increase Buffers to make use of extra SRAM
#define RX_BUFFER_SIZE    128
#define TX_BUFFER_SIZE    256
#define BLOCK_BUFFER_SIZE 32
#define LINE_BUFFER_SIZE  64

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

// Jog panel with speed pot and joystick switches by cm@ct.de

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

// Define runtime command special characters. These characters are 'picked-off' directly from the
// serial read data stream and are not passed to the grbl line execution parser. Select characters
// that do not and must not exist in the streamed g-code program. ASCII control characters may be 
// used, if they are available per user setup. Also, extended ASCII codes (>127), which are never in 
// g-code programs, maybe selected for interface programs.
// NOTE: If changed, manually update help message in report.c.
#define CMD_STATUS_REPORT '?'
#define CMD_FEED_HOLD '!'
#define CMD_CYCLE_START '~'
#define CMD_RESET 0x18 // ctrl-x

// The temporal resolution of the acceleration management subsystem. Higher number give smoother
// acceleration but may impact performance.
// NOTE: Increasing this parameter will help any resolution related issues, especially with machines 
// requiring very high accelerations and/or very fast feedrates. In general, this will reduce the 
// error between how the planner plans the motions and how the stepper program actually performs them.
// However, at some point, the resolution can be high enough, where the errors related to numerical 
// round-off can be great enough to cause problems and/or it's too fast for the Arduino. The correct
// value for this parameter is machine dependent, so it's advised to set this only as high as needed.
// Approximate successful values can range from 30L to 100L or more.
#define ACCELERATION_TICKS_PER_SECOND 50L

// Minimum planner junction speed. Sets the default minimum speed the planner plans for at the end
// of the buffer and all stops. This should not be much greater than zero and should only be changed
// if unwanted behavior is observed on a user's machine when running at very slow speeds.
#define MINIMUM_PLANNER_SPEED 0.0 // (mm/min)

// Minimum stepper rate. Sets the absolute minimum stepper rate in the stepper program and never runs
// slower than this value, except when sleeping. This parameter overrides the minimum planner speed.
// This is primarily used to guarantee that the end of a movement is always reached and not stop to
// never reach its target. This parameter should always be greater than zero.
#define MINIMUM_STEPS_PER_MINUTE 800 // (steps/min) - Integer value only

// Time delay increments performed during a dwell. The default value is set at 50ms, which provides
// a maximum time delay of roughly 55 minutes, more than enough for most any application. Increasing
// this delay will increase the maximum dwell time linearly, but also reduces the responsiveness of 
// run-time command executions, like status reports, since these are performed between each dwell 
// time step. Also, keep in mind that the Arduino delay timer is not very accurate for long delays.
#define DWELL_TIME_STEP 1 // Integer (1-255) (milliseconds)

// If homing is enabled, homing init lock sets Grbl into an alarm state upon power up. This forces
// the user to perform the homing cycle (or override the locks) before doing anything else. This is
// mainly a safety feature to remind the user to home, since position is unknown to Grbl.
// #define HOMING_INIT_LOCK // Comment to disable

// The homing cycle seek and feed rates will adjust so all axes independently move at the homing
// seek and feed rates regardless of how many axes are in motion simultaneously. If disabled, rates
// are point-to-point rates, as done in normal operation. For example in an XY diagonal motion, the
// diagonal motion moves at the intended rate, but the individual axes move at 70% speed. This option
// just moves them all at 100% speed.
#define HOMING_RATE_ADJUST // Comment to disable

// Define the homing cycle search patterns with bitmasks. The homing cycle first performs a search
// to engage the limit switches. HOMING_SEARCH_CYCLE_x are executed in order starting with suffix 0 
// and searches the enabled axes in the bitmask. This allows for users with non-standard cartesian 
// machines, such as a lathe (x then z), to configure the homing cycle behavior to their needs. 
// Search cycle 0 is required, but cycles 1 and 2 are both optional and may be commented to disable.
// After the search cycle, homing then performs a series of locating about the limit switches to hone
// in on machine zero, followed by a pull-off maneuver. HOMING_LOCATE_CYCLE governs these final moves,
// and this mask must contain all axes in the search.
// NOTE: Later versions may have this installed in settings.
#define HOMING_SEARCH_CYCLE_0 (1<<Z_AXIS)  // First move Z to clear workspace.
#define HOMING_SEARCH_CYCLE_1 (1<<Y_AXIS)  // Then move Y 
#define HOMING_SEARCH_CYCLE_2 (1<<X_AXIS)                       // Uncomment and add axes mask to enable
#define HOMING_LOCATE_CYCLE   ((1<<X_AXIS)|(1<<Y_AXIS)|(1<<Z_AXIS)) // Must contain ALL search axes

// Number of homing cycles performed after when the machine initially jogs to limit switches.
// This help in preventing overshoot and should improve repeatability. This value should be one or 
// greater.
#define N_HOMING_LOCATE_CYCLE 1 // Integer (1-128)

// Number of blocks Grbl executes upon startup. These blocks are stored in EEPROM, where the size
// and addresses are defined in settings.h. With the current settings, up to 5 startup blocks may
// be stored and executed in order. These startup blocks would typically be used to set the g-code
// parser state depending on user preferences.
#define N_STARTUP_LINE 2 // Integer (1-5)

// ---------------------------------------------------------------------------------------
// FOR ADVANCED USERS ONLY: 

// The number of linear motions in the planner buffer to be planned at any give time. The vast
// majority of RAM that Grbl uses is based on this buffer size. Only increase if there is extra 
// available RAM, like when re-compiling for a Teensy or Sanguino. Or decrease if the Arduino
// begins to crash due to the lack of available RAM or if the CPU is having trouble keeping
// up with planning new incoming motions as they are executed. 
// #define BLOCK_BUFFER_SIZE 32  // Uncomment to override default in planner.h.

// Line buffer size from the serial input stream to be executed. Also, governs the size of 
// each of the startup blocks, as they are each stored as a string of this size. Make sure
// to account for the available EEPROM at the defined memory address in settings.h and for
// the number of desired startup blocks.
// NOTE: 70 characters is not a problem except for extreme cases, but the line buffer size 
// can be too small and g-code blocks can get truncated. Officially, the g-code standards 
// support up to 256 characters. In future versions, this default will be increased, when 
// we know how much extra memory space we can re-invest into this.
// #define LINE_BUFFER_SIZE 70  // Uncomment to override default in protocol.h
  
// Serial send and receive buffer size. The receive buffer is often used as another streaming
// buffer to store incoming blocks to be processed by Grbl when its ready. Most streaming
// interfaces will character count and track each block send to each block response. So, 
// increase the receive buffer if a deeper receive buffer is needed for streaming and avaiable
// memory allows. The send buffer primarily handles messages in Grbl. Only increase if large
// messages are sent and Grbl begins to stall, waiting to send the rest of the message.
// #define RX_BUFFER_SIZE 128 // Uncomment to override defaults in serial.h
// #define TX_BUFFER_SIZE 64
  
// Toggles XON/XOFF software flow control for serial communications. Not officially supported
// due to problems involving the Atmega8U2 USB-to-serial chips on current Arduinos. The firmware
// on these chips do not support XON/XOFF flow control characters and the intermediate buffer 
// in the chips cause latency and overflow problems with standard terminal programs. However, 
// using specifically-programmed UI's to manage this latency problem has been confirmed to work.
// As well as, older FTDI FT232RL-based Arduinos(Duemilanove) are known to work with standard
// terminal programs since their firmware correctly manage these XON/XOFF characters. In any
// case, please report any successes to grbl administrators!
// #define ENABLE_XONXOFF // Default disabled. Uncomment to enable.

// Creates a delay between the direction pin setting and corresponding step pulse by creating
// another interrupt (Timer2 compare) to manage it. The main Grbl interrupt (Timer1 compare) 
// sets the direction pins, and does not immediately set the stepper pins, as it would in 
// normal operation. The Timer2 compare fires next to set the stepper pins after the step 
// pulse delay time, and Timer2 overflow will complete the step pulse, except now delayed 
// by the step pulse time plus the step pulse delay. (Thanks langwadt for the idea!)
//   This is an experimental feature that should only be used if your setup requires a longer
// delay between direction and step pin settings (some opto coupler based drivers), as it may
// adversely effect Grbl's high-end performance (>10kHz). Please notify Grbl administrators 
// of your successes or difficulties, as we will monitor this and possibly integrate this as a 
// standard feature for future releases. However, we suggest to first try our direction delay
// hack/solution posted in the Wiki involving inverting the stepper pin mask.
// NOTE: Uncomment to enable. The recommended delay must be > 3us and the total step pulse
// time, which includes the Grbl settings pulse microseconds, must not exceed 127us. Reported
// successful values for certain setups have ranged from 10 to 20us.
#define STEP_PULSE_DELAY 10 // Step pulse delay in microseconds. Default disabled.

// ---------------------------------------------------------------------------------------
// TODO: Install compile-time option to send numeric status codes rather than strings.

#endif
