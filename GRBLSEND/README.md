#GRBLize

###CNC-Steuerung für GRBL-JOG Projekt

Erstellt mit Delphi 2005 PE. Bitte beachten Sie unbedingt den Artikel in **[c't Hacks 4/2014](http://heise.de/-2109420)**.



Sources to be compiled with Borland Delphi 2005 Personal Edition (and up) for those interested in improving it. This is 
more or less a quick & dirty (but working) approach, so be forgiving. Borland Delphi 2005 Personal Edition was 
downloadable for free some time ago, also included on some computer magazine CDs/DVDs. 

Delphi 2005 still works fine on Windows 7 if you add the new string value "$(BDS)\Bin\delphicoreide90.bpl" = "Delphi Core IDE" 
to the "HKEY_CURRENT_USER\Software\Borland\BDS\3.0\Known IDE Packages" registry key. Otherwise you'll get an access 
violation error on rtl90.bpl on exit of IDE. 

On Windows 7 latest update, the DZ Line Editor Fix (http://sourceforge.net/projects/dzeditorlineendsfix/) must be installed.

GRBLize uses ftdiclass component from Michael "Zipplet" Nixon and GLscene OpenGL component.

(c) by Carsten Meyer, Redaktion c't Hacks/Make Deutschland, cm@ct.de, 12/2014.
