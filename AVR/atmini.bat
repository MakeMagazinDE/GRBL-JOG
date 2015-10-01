batch for flashing with bootloader on ATMINI Arduino compatible board from MikroElektronika on COM%1
avrdude -C "avrdude.conf" -pm328p -cstk500v1 -P COM%1 -b57600 -D -U flash:w:"grbl.hex":i