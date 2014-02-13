#Grbl 0.8c/j c't Hacks GRBL-Jogger Version

_An embedded g-code interpreter and motion-controller for the Arduino/AVR328 microcontroller_

Note: Version 0.8c/j (prelimiary) for ATmega644 with jog control. Intended for 
use with _c't Hacks GRBL-Jogger PCB_. Default settings and Z scaling for _c't 
Hacks WOODMILL_ project. Recompile for 644P or different Z scale setting - see 
_config.h_ and _pin_map.h_ files for details.

------------

Grbl is a no-compromise, high performance, low cost alternative to parallel-port-based motion control for CNC milling. It will run on a vanilla Arduino (Duemillanove/Uno) as long as it sports an Atmega 328. 

The controller is written in highly optimized C utilizing every clever feature of the AVR-chips to achieve precise timing and asynchronous operation. It is able to maintain up to 30kHz of stable, jitter free control pulses.

It accepts standards-compliant G-code and has been tested with the output of several CAM tools with no problems. Arcs, circles and helical motion are fully supported, as well as, other basic functional g-code commands. Functions and variables are not currently supported, but may be included in future releases in a form of a pre-processor.

Grbl includes full acceleration management with look ahead. That means the controller will look up to 18 motions into the future and plan its velocities ahead to deliver smooth acceleration and jerk-free cornering.

_The project was initially inspired by the Arduino GCode Interpreter by Mike Ellery_

-------------

Grbl is an open-source project and fueled by the free-time of our intrepid administrators and altruistic users. If you'd like to donate, all proceeds will be used to help fund supporting hardware and testing equipment. Thank you!

[![Donate](https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=YNQZV7GRGQKVY)


