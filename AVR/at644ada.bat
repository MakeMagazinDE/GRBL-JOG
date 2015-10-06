echo off
echo Serial GRBL flasher for Adafruit 644 Bootloader
echo Works only with installed bootloader.hex on ATmega644p!
echo Usage: at644ada XX with XX = # of virtual USB COM port
avrdude -C "avrdude.conf" -pm644p -cstk500v1 -P COM%1 -b57600 -D -U flash:w:"grbl.hex":i
