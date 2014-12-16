#Grbl 0.8c/jog.2 

**c't Hacks GRBL-Jogger Version**

Update für GRBLize CNC-Steuerung aus c't Hacks/Make: 4/2014 mit verkürzter Koordinatenausgabe bei Jog und einigen Bugfixes

- Default-Parameter angepasst für CNC-Fräse aus Alu (c't Hacks 1/2013 mit TR12x6 Spindeln, grbl_alum.hex) 
- Default-Parameter angepasst für CNC-Fräse aus Multiplex-Holz mit Zahnriemen (c't Hacks 1/2014, grbl_wood.hex). Bitte Flash-Batch entsprechend Dateinamen anpassen.
- Bugfix Homing- und Jogging-Richtungen
- Parameter für invertiertes Enable-Signal eingeführt, war vorher ein C-Define in config.h
- Parameter Stepper-Port-Invertierung geändert auf Limit-Switch-Invertierung, bei falscher Drehrichtung eines Schrittmotors ist stattdessen eine der beiden Motorwicklungen umzupolen.

Bei Verwendung eines ATmega644 statt ATmega644P ist das Makefile anzupassen und die Firmware mit WinAVR/GCC-AVR neu zu 
kompilieren.

Bitte beachten Sie, dass bei einigen chinesischen Schrittmotortreibern die Belegungen der XYZ-Step/Direction-Pins 
vertauscht sind. Auf der GRBL-Jogger-Platine sind dann die Leiterbahnen unter den Pfostenleisten PL9 und 10 
aufzutrennen und die Verbindungen mit Jumper-Kabeln in der richtigen Reihenfolge herzustellen. Außerdem erwarten einige 
chinesische Schrittmotortreiber das Enable-Signal auf Pin 16 des D-Sub-Stecker und nicht auf Pin 1. Es dürfen dann die 
Jumper JP4 bis JP6 nicht gesteckt werden, stattdessen ist die Verbindung über ein Jumper-Kabel zwischen PL9 Pin 1 und 
PL10 Pin 16 herzustellen.

Dokumentationen zu verschiedenen Treiberkarten finden Sie im **[TB6560-BOARDS-Repository](https://github.com/heise/TB6560-BOARDS)**. 

Ein sehr komfortables Steuerprogramm mit HPGL- und Excellon-Import, Webcam-Unterstützung, Preview und 
CAM-Funktionen finden Sie in unserem **[GRBLize-Repository](https://github.com/heise/GRBLize)**. 

Carsten Meyer, Redaktion c't Hacks, Make Deutschland (cm@ct.de)


**Achtung:** Einige CAM-Programme liefern invertierte Z-Werte (positive Z-Werte im Werkstück) - ggf. Z-Skalierung oder Schrittmotor- und Homing-Richtung in GRBL-Port-Invertierungsparametern ändern:

Bitte beachten Sie unbedingt den Artikel in **[c't Hacks 1/2014](http://heise.de/-2109420)**.


**An embedded g-code interpreter and motion-controller for the Arduino/AVR328 microcontroller**

Note: Version 0.8c/j (prelimiary) for ATmega644 with jog control. Intended for 
use with **c't Hacks GRBL-Jogger PCB**. Default settings and Z scaling for **c't 
Hacks WOODMILL** project. Recompile for 644P or different Z scale setting - see 
'config.h' and 'pin_map.h' files for details.

------------

Grbl is a no-compromise, high performance, low cost alternative to parallel-port-based motion control for CNC milling. 
It will run on a vanilla Arduino (Duemillanove/Uno) as long as it sports an Atmega 328. 

The controller is written in highly optimized C utilizing every clever feature of the AVR-chips to achieve precise 
timing and asynchronous operation. It is able to maintain up to 30kHz of stable, jitter free control pulses.

It accepts standards-compliant G-code and has been tested with the output of several CAM tools with no problems. Arcs, 
circles and helical motion are fully supported, as well as, other basic functional g-code commands. Functions and 
variables are not currently supported, but may be included in future releases in a form of a pre-processor.

Grbl includes full acceleration management with look ahead. That means the controller will look up to 18 motions into 
the future and plan its velocities ahead to deliver smooth acceleration and jerk-free cornering.

_The project was initially inspired by the Arduino GCode Interpreter by Mike Ellery_

-------------

Grbl is an open-source project and fueled by the free-time of our intrepid administrators and altruistic users. If you'd 
like to donate, all proceeds will be used to help fund supporting hardware and testing equipment. Thank you!

[![Donate](https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=YNQZV7GRGQKVY)


