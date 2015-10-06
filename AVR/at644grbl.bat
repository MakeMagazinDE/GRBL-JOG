echo off
echo ISP GRBL flash
echo Usage: at644grbl XX with XX = COM# of DIAMEX ISP programmer
avrdude.exe -C "avrdude.conf" -pm644p -cstk500v2 -B2 -P COM%1 -V -U flash:w:"grbl.hex":i
