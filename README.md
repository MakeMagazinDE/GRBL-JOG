![GitHub Logo](http://www.heise.de/make/icons/make_logo.png)

Maker Media GmbH und c't, Heise Zeitschriften Verlag

***

#Grbl 0.9j/jog

Update für GRBLize CNC-Steuerung aus c't Hacks/Make: 4/2014

- Angepasst auf GRBL Version 0.9j
- Standard-Format der Statusmeldung und Befehlsverarbeitung wie im offiziellen Grbl-Build 
- Jog-Routinen nun Interrupt-gesteuert, keine Störungen des Motorlaufs
- stark verbesserte Performance
- Beschleunigungswerte und und Seek-Geschwindigkeit für jede Achse getrennt einstellbar
- keine Z-Skalierung mehr erforderlich bei der Make: Sperrholzfräse
- Schnittstelle jetzt 115200 Bd, 8n1
- Z-Probe-Eingang ist Pin 5 (PB4, SPI SS) des ATmega644. Dieser Eingang ist an PL7 (ISP) Pin 3 zugänglich. Für erhöhte Störsicherheit sollte ein Pull-up-Widerstand 4k7 nach +5V und ggf. ein Eingangsfilter wie an den anderen Schalter-Eingängen vorgesehen werden. Eine überarbeitete GRBL-JOG-Platine ist in Planung.


#Grbl 0.8c/jog.2 

- Default-Parameter angepasst für CNC-Fräse aus Alu (c't Hacks 1/2013 mit TR12x6 Spindeln, grbl_alum.hex) 
- Default-Parameter angepasst für CNC-Fräse aus Multiplex-Holz mit Zahnriemen (c't Hacks 1/2014, grbl_wood.hex). Bitte Flash-Batch entsprechend Dateinamen anpassen.
- Bugfix Homing- und Jogging-Richtungen
- Parameter für invertiertes Enable-Signal eingeführt, war vorher ein C-Define in config.h
- Parameter Stepper-Port-Invertierung geändert auf Limit-Switch-Invertierung, bei falscher Drehrichtung eines Schrittmotors ist stattdessen eine der beiden Motorwicklungen umzupolen.

Bei Verwendung eines ATmega644 statt ATmega644P ist das Makefile anzupassen und die Firmware mit WinAVR/GCC-AVR neu zu 
kompilieren.

***

#Schrittmotortreiber

Bitte beachten Sie, dass bei einigen chinesischen Schrittmotortreibern die Belegungen der XYZ-Step/Direction-Pins 
vertauscht sind. Auf der GRBL-Jogger-Platine sind dann die Leiterbahnen unter den Pfostenleisten PL9 und 10 
aufzutrennen und die Verbindungen mit Jumper-Kabeln in der richtigen Reihenfolge herzustellen. Außerdem erwarten einige 
chinesische Schrittmotortreiber das Enable-Signal auf Pin 16 des D-Sub-Stecker und nicht auf Pin 1. Es dürfen dann die 
Jumper JP4 bis JP6 nicht gesteckt werden, stattdessen ist die Verbindung über ein Jumper-Kabel zwischen PL9 Pin 1 und 
PL10 Pin 16 herzustellen.

Dokumentationen zu verschiedenen Treiberkarten finden Sie im **[TB6560-BOARDS-Repository](https://github.com/heise/TB6560-BOARDS)**. 

Ein sehr komfortables Steuerprogramm mit HPGL- und Excellon-Import, Webcam-Unterstützung, Preview und 
CAM-Funktionen finden Sie in unserem **[GRBLize-Repository](https://github.com/heise/GRBLize)**. 

Carsten Meyer, Redaktion Make Deutschland (cm@ct.de)

**Achtung:** Einige CAM-Programme liefern invertierte Z-Werte (positive Z-Werte im Werkstück) - ggf. Z-Skalierung oder Schrittmotor- und Homing-Richtung in GRBL-Port-Invertierungsparametern ändern:

Bitte beachten Sie unbedingt den Artikel in **[c't Hacks 1/2014](http://heise.de/-2109420)**.
