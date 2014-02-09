/* this program runs a series of performance tests on IOX hardware.
 *
 * First, 1000 single bit reads are performed from a single address.  This
 * is timed, with the full elapsed time being for 1000 operations.  A
 * simple division shows the average results for a single operation.
 *
 * Then 1000 single bit writes are performed, again timing the entire set
 * of operations.  This should take approximately twice as long as 1000 reads,
 * single a bit write involves a full byte read, a bit modification, and then
 * a full byte write operation.
 *
 * On my cpNode IOX16 hardware, I found that a single bit read takes about
 * 0.43ms and a single bit write is about 0.86ms.  If you find wildly
 * differing values on your devices, please let me know.
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
 *
 */
#include <Wire.h>
#include <MCP23017.h>


// change this if your IOX is at a different address
#define KNOWN_DEVICE 0x20
#define OUTPUT_PORT 0x0

void setup()
{
    Serial.begin(9600);

    Serial.println("registers before configuration:");
    MCP23017.debugDump(Serial, KNOWN_DEVICE);


    for (uint8_t bit = 0; bit < 6; bit++) {
        MCP23017.setMode(KNOWN_DEVICE, OUTPUT_PORT, bit, OUTPUT);
        MCP23017.setGPIO(KNOWN_DEVICE, OUTPUT_PORT, bit, LOW);
    }

    for (uint8_t bit = 0; bit < 6; bit++) {
        bool err = MCP23017.setMode(KNOWN_DEVICE, 0x1, bit, INPUT_PULLUP);
        if (err) {
            Serial.print("error setting mode on bit ");
            Serial.println(bit);
        }
    }

    Serial.println("registers after configuration:");
    MCP23017.debugDump(Serial, 0x21);
}


void loop()
{
    unsigned long start = millis();
    for (int i = 0; i < 1000; i++) {
        bool v = MCP23017.getGPIO(KNOWN_DEVICE, 1, 0);
    }

    unsigned long now = millis();

    Serial.print("1000 reads took ");
    Serial.print(now - start);
    Serial.print(" milliseconds, or ");
    Serial.print((now - start) / 1000.0);
    Serial.println(" milliseconds per read");

    int state = 0;

    start = millis();
    for (int i = 0; i < 1000; i++) {
        MCP23017.setGPIO(KNOWN_DEVICE, 0, 0, state);
        state = !state;
    }
    now = millis();


    Serial.print("1000 writes took ");
    Serial.print(now - start);
    Serial.print(" milliseconds, or ");
    Serial.print((now - start) / 1000.0);
    Serial.println(" milliseconds per write");
}
