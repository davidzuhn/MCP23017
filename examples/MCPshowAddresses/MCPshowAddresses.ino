#include <Wire.h>
#include <MCP23017.h>

#define KNOWN_DEVICE 0x20


void setup()
{
    Serial.begin(9600);

    while (!Serial);

    delay(5000);

    for (int i = 0x20; i < 0x28; i++) {
	Serial.print("checking for device at address 0x");
	Serial.print(i, HEX);

	if (MCP23017.exists(i)) {
	    Serial.println(" exists");
	    MCP23017.debugDump(Serial, i);
	} else {
	    Serial.println(" does not exist");
	}
    }

    for (int bit = 0; bit < 8; bit++) {
	MCP23017.setMode(KNOWN_DEVICE, 0x0, bit, OUTPUT);
    }

}

void loop()
{

    for (int bit = 0; bit < 8; bit++) {
	MCP23017.setGPIO(KNOWN_DEVICE, 0x0, bit, HIGH);
	delay(50);
    }

    delay(1000);
    for (int bit = 0; bit < 8; bit++) {
	MCP23017.setGPIO(KNOWN_DEVICE, 0x0, bit, LOW);
	delay(50);
    }

}
