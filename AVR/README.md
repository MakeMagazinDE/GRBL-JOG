#Grbl 0.8c/j 

**c't Hacks GRBL-Jogger Version**

GRBL-Sourcen und HEX-File f�r ATmega644, Default-Parameter angepasst f�r [c't Hacks Holzfr�se](https://github.com/heise/WOODMILL). Bei Verwendung eines ATmega644P ist ein hierf�r kompiliertes HEX-File n�tig!

**Achtung:** Ausgelegt f�r positive Z-Werte (negative Z-Werte im Werkst�ck). Einige CAM-Programme liefern invertierte Z-Werte (positive Z-Werte im Werkst�ck) - ggf. Z-Skalierung oder Schrittmotor-Richtung in GRBL-Port-Invertierungsparametern �ndern:

Normal (Z+ oben): $6=8  $18=160
Invert (Z+ down): $6=0  $18=168

Bitte beachten Sie unbedingt den Artikel in **[c't Hacks 1/2014](http://heise.de/-2109420)**.

**An embedded g-code interpreter and motion-controller for the Arduino/AVR328 microcontroller**

Note: Version 0.8c/j (prelimiary) for ATmega644 with jog control. Intended for 
use with **c't Hacks GRBL-Jogger PCB**. Default settings and Z scaling for **c't 
Hacks WOODMILL** project. Recompile for 644P or different Z scale setting - see 
'config.h' and 'pin_map.h' files for details.

------------

Grbl is a no-compromise, high performance, low cost alternative to parallel-port-based motion control for CNC milling. It will run on a vanilla Arduino (Duemillanove/Uno) as long as it sports an Atmega 328. 

The controller is written in highly optimized C utilizing every clever feature of the AVR-chips to achieve precise timing and asynchronous operation. It is able to maintain up to 30kHz of stable, jitter free control pulses.

It accepts standards-compliant G-code and has been tested with the output of several CAM tools with no problems. Arcs, circles and helical motion are fully supported, as well as, other basic functional g-code commands. Functions and variables are not currently supported, but may be included in future releases in a form of a pre-processor.

Grbl includes full acceleration management with look ahead. That means the controller will look up to 18 motions into the future and plan its velocities ahead to deliver smooth acceleration and jerk-free cornering.

_The project was initially inspired by the Arduino GCode Interpreter by Mike Ellery_

-------------

Grbl is an open-source project and fueled by the free-time of our intrepid administrators and altruistic users. If you'd like to donate, all proceeds will be used to help fund supporting hardware and testing equipment. Thank you!

[![Donate](https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=YNQZV7GRGQKVY)


