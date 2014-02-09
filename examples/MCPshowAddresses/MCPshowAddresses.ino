/*
 * show the presence & status of any MCP23017 device found on the I2C bus.
 * There can be up to 8 such chips on the bus, with the address ranging
 * from 0x20 to 0x27.
 *
 * Check for the existence of a device at the given address, and if one is
 * found, dump all the registers for it to the Serial monitor.  If it's not
 * an MCP23017, well, all bets are off as to what the memory values
 * represent.  I don't know know if/how I can be certain it's an MCP23017
 * at the specified address.
 *
 * Once that's done, loop through the bits of the MCP23017 specified with
 * an I2C address defined in KNOWN_DEVICE and the port defined in
 * OUTPUT_PORT, and turn them all on in sequence, and then turn them all
 * off.
 *
 * This is done over and over again until you get tired and run another
 * program.
 *
 *
 * Copyright 2013, 2014 by david d zuhn <zoo@whitepineroute.org>
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike
 * 4.0 International License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-sa/4.0/deed.en_US.
 *
 * You may use this work for any purposes, provided that you make your
 * version available to anyone else.  
 */
#include <Wire.h>
#include <MCP23017.h>


/* these define OUTPUT lines that you are expected to have in place.
 *
 * if you don't, then nothing bad should happen, but you don't get to
 * see as many blinky lights as you could
 *
 * change them if your IOX device is at another address
 */

#define KNOWN_DEVICE 0x20
#define OUTPUT_PORT  0x0


void setup()
{
    Serial.begin(9600);

    // this makes it easier to see the output of the address checks
    while (!Serial);
    delay(5000);

    // check for an MCp23017 at each of the possible addresses
    for (int i = 0x20; i < 0x28; i++) {
        Serial.print("checking for device at address 0x");
        Serial.print(i, HEX);

        if (MCP23017.exists(i)) {
            // if one is found, then dump the registers to the debug console
            Serial.println(" exists");
            MCP23017.debugDump(Serial, i);
        } else {
            Serial.println(" does not exist");
        }
    }


    // initialize the output bits you have defined, one by one
    for (int bit = 0; bit < 8; bit++) {
        MCP23017.setMode(KNOWN_DEVICE, OUTPUT_PORT, bit, OUTPUT);
    }

}

void loop()
{
    // turn on each bit in sequence
    for (int bit = 0; bit < 8; bit++) {
        MCP23017.setGPIO(KNOWN_DEVICE, OUTPUT_PORT, bit, HIGH);
        delay(50);
    }

    delay(1000);

    // and now turn them back off again
    for (int bit = 0; bit < 8; bit++) {
        MCP23017.setGPIO(KNOWN_DEVICE, OUTPUT_PORT, bit, LOW);
        delay(50);
    }

}
