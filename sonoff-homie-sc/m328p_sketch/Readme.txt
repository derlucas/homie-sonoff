##About this file folder

In code folder, there is ATMEGA328P code

In libraries folder, there is the related library.



##Flash Firmware Notes: 

 - When flashing a new firmware, you must discounnect the serial short jumper cap(please refer to the picture)
 - Firstly, burn bootloader for ATMEGA328P with an AVR ISP Programmer/Another Arduino  (optional, its already installed)
 - Next, start burning program for ATMEGA328P
   connect your USB-RS232 adapter to to Pin Header next to the Atmel ATmega328p
   
   Sonoff      Adapter
    Reset  ->    DTR
    GND    ->    GND
    RX     ->    RX
    TX     ->    TX
    5V     ->    VCC
 - in Arduino IDE select "Arduino Mini"


This firmware is mostly taken from ITead Wiki: https://www.itead.cc/wiki/Sonoff_SC
