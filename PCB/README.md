#GRBL-Jogger PCB

###Schrittmotor-Controller mit ATmega644 und Jogpad


Gerber-Dateien zur Platinenfertigung sowie Layouts zum Selbstätzen. Zum Ansehen der PDFs "View raw" anklicken und 
downloaden. Bitte beachten Sie unbedingt den Artikel in **[c't Hacks 1/2014](http://heise.de/-2109420)**. Fertige 
Leerplatinen können Sie im **[c't Eurocircuits-Shop](http://ct.eurocircuits.de)** bestellen.

Bitte beachten Sie, dass bei einigen chinesischen Schrittmotortreibern die Belegungen der XYZ-Step/Direction-Pins 
vertauscht sind. Auf der GRBL- Jogger-Platine sind dann die Leiterbahnen unter den Pfostenleisten PL9 und 10 
aufzutrennen und die Verbindungen mit Jumper-Kabeln in der richtigen Reihenfolge herzustellen. Außerdem erwarten einige 
chinesische Schrittmotortreiber das Enable-Signal auf Pin 16 des D-Sub-Stecker und nicht auf Pin 1. Es dürfen dann die 
Jumper JP4 bis JP6 nicht gesteckt werden, stattdessen ist die Verbindung über ein Jumper-Kabel zwischen PL9 Pin 1 und 
PL10 Pin 16 herzustellen.

Anleitungen zu verschiedenen Treiberkarten finden Sie in unserem Github-Repository TB6560-BOARDS. 

Carsten Meyer, Redaktion c't Hacks, Make Deutschland (cm@ct.de)

Unser Leser Tom Martin hat freundlicherweise einen **[Reichelt-Warenkorb](https://secure.reichelt.de/index.html?&ACTION=20&AWKID=875060&PROVID=2084)** zusammengestellt. Evt. passt die 
DC-Buchse nicht perfekt; wir hatten unser Muster von Segor (Bestellnr: DCBU 2,1-PR) bezogen. Anmerkung: Der Heise 
Zeitschriften Verlag steht weder mit Reichelt, Eurocircuits noch mit Segor in einer Provisions- oder Geschäftsbeziehung, 
die über das Bereitstellen der Fertigungsdaten hinausgeht.

Note: "View Raw" to download PDFs. Gerber Files for PCB production. Double-sided PCB with silk position print available at [ct.eurocircuits.de](http://ct.eurocircuits.de/ "c't Hacks Eurocircuits Shop")

PCB artwork (c) by Carsten Meyer, c't Hacks editorial office, Heise Zeitschriften Verlag 2014.



