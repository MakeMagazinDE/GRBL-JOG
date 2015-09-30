avrdude.exe -C "avrdude.conf" -pm644p -cstk500v2 -B2 -P COM20 -V -U lfuse:w:0xf7:m -U hfuse:w:0xd2:m -U efuse:w:0xff:m -U flash:w:"grbl.hex":i -U lock:w:0xef:m
