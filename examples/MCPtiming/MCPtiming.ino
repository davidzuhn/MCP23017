// emacs should treat this as -*- c++ -*-

#include <Wire.h>
#include <MCP23017.h>


#define KNOWN_DEVICE 0x20

void setup()
{
    Serial.begin(9600);

    Serial.println("registers before configuration:");
    MCP23017.debugDump(Serial, KNOWN_DEVICE);

    for (uint8_t bit = 0; bit < 6; bit++) {
        MCP23017.setMode(KNOWN_DEVICE, 0x0, bit, OUTPUT);
        MCP23017.setGPIO(KNOWN_DEVICE, 0, bit, LOW);
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
