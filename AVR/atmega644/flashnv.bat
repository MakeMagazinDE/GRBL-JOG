rem Flashes ATmega644P with DIAMEX PROG-S on serial port [Param1], no verify
avrdude -C "avrdude.conf" -pm644 -cstk500v2 -P COM%1 -B4  -V -U flash:w:"GRBL.HEX":i -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:0xfd:m 
