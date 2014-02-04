/* Interface library for MCP23017 16 bit i2c I/O expander
 *
 * The MCP23017 implements 16 configurable I/O lines via an I2C
 * interface.  This library is designed to make it possible to use
 * this device without specific I2C/Wire commands.
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

#include <MCP23017.h>
#include <Wire.h>

/* common error checking routines, to make sure that we don't act when
 * unreasonable port or bit values are passed in
 */
#define checkAddress(address,error) do { if(address < 0x20 || address > 0x27) { return error; } } while(0)
#define checkPort(port,error) do { if(port!=0 && port !=1) { return error; } } while(0)
#define checkBit(bit,error)   do { if(bit<0 || bit>7) { return error; } } while(0)


MCPIOIntf::MCPIOIntf()
{
    Wire.begin();
}


bool MCPIOIntf::exists(uint8_t address)
{
    int config = readRegister(address, 0, MCPIOIntf::IOCON);

    if (config >= 0) {
        return true;
    } else {
        return false;
    }
}


bool MCPIOIntf::setMode(uint8_t address, uint8_t port, uint8_t bit, uint8_t mode)
{
    checkAddress(address, false);
    checkPort(port, false);
    checkBit(bit, false);

    int chipMode = readRegister(address, port, MCPIOIntf::IODIR);
    if (chipMode < 0) {
        //Serial.println("Error reading chipMode");
        return false;
    }

    int newMode = chipMode;
    int pullup = -1;
    int polarity = -1;

    switch (mode) {
    case OUTPUT:
        bitClear(newMode, bit);
        break;

    case INPUT:
        bitSet(newMode, bit);
        break;

    case INPUT_PULLUP:
        bitSet(newMode, bit);

        pullup = readRegister(address, port, MCPIOIntf::GPPU);
        if (pullup >= 0) {
            //Serial.println("pullup set for "); Serial.println(bit);
            bitSet(pullup, bit);
        } else {
            //Serial.println("error reading GPPU");
            return false;
        }
        break;

    case INPUT_INVERTED:
        bitSet(newMode, bit);

        polarity = readRegister(address, port, MCPIOIntf::IPOL);
        if (polarity >= 0) {
            bitSet(polarity, 1);
        } else {
            return false;
        }
        break;

    case INPUT_PULLUP_INVERTED:
        bitSet(newMode, bit);

        pullup = readRegister(address, port, MCPIOIntf::GPPU);
        if (pullup >= 0) {
            bitSet(pullup, bit);
        } else {
            return false;
        }

        polarity = readRegister(address, port, MCPIOIntf::IPOL);
        if (polarity >= 0) {
            bitSet(polarity, 1);
        } else {
            return false;
        }
        break;

    default:
        return false;
    }


    // We write the mode only if it's changed
    if (newMode != chipMode) {
        writeRegister(address, port, MCPIOIntf::IODIR, newMode);
    }
    // If we read a pullup value, then write it no matter what
    if (pullup >= 0) {
        writeRegister(address, port, MCPIOIntf::GPPU, pullup);
    }
    // If we read a polarity value, then write it no matter what
    if (polarity >= 0) {
        writeRegister(address, port, MCPIOIntf::IPOL, pullup);
    }
}


int MCPIOIntf::getGPIO(uint8_t address, uint8_t port)
{
    checkAddress(address, -1);
    checkPort(port, -1);
    checkBit(bit, -1);

    int val = readRegister(address, port, MCPIOIntf::GPIO);
    return val;
}


int MCPIOIntf::getGPIO(uint8_t address, uint8_t port, uint8_t bit)
{
    checkAddress(address, -1);
    checkPort(port, -1);
    checkBit(bit, -1);

    int val = readRegister(address, port, MCPIOIntf::GPIO);
    return bitRead(val, bit);
}


bool MCPIOIntf::setGPIO(uint8_t address, uint8_t port, uint8_t value)
{
    checkAddress(address, false);
    checkPort(port, false);
    checkBit(bit, false);

    writeRegister(address, port, MCPIOIntf::GPIO, value);
    return true;
}

bool MCPIOIntf::setGPIO(uint8_t address, uint8_t port, uint8_t bit, bool value)
{
    checkAddress(address, false);
    checkPort(port, false);
    checkBit(bit, false);

    int gpioVal = readRegister(address, port, MCPIOIntf::GPIO);
    if (gpioVal < 0) {
        return false;
    }

    uint8_t newVal = gpioVal;
    bitWrite(newVal, bit, value);
    if (newVal != gpioVal) {
        writeRegister(address, port, MCPIOIntf::GPIO, newVal);
    }
    return true;
}




int MCPIOIntf::readRegister(uint8_t address, uint8_t port, uint8_t reg)
{
    Wire.beginTransmission(address);
    Wire.write(reg + port);
    Wire.endTransmission();

    Wire.requestFrom(address, (uint8_t) 1);

    int val = Wire.read();

#if 0
    Serial.print("readReg ");
    Serial.print(address, HEX);
    Serial.print(" register ");
    Serial.print(reg + port, HEX);
    Serial.print("  ==> ");
    Serial.println(val, HEX);
#endif

    return val;
}


void MCPIOIntf::writeRegister(uint8_t address, uint8_t port, uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(address);
    Wire.write(reg + port);
    Wire.write(value);
    Wire.endTransmission();
}


static void printVal(Stream& s, char *name, uint8_t addr, uint8_t v1, uint8_t v2)
{
    s.print(name);
    s.print("A ");
    s.print(addr, HEX);
    s.print(" ");
    s.println(v1, BIN);
    s.print(name);
    s.print("B ");
    s.print(addr + 1, HEX);
    s.print(" ");
    s.println(v2, BIN);
}


void MCPIOIntf::debugDump(Stream& s, uint8_t address)
{
    uint8_t r[0x16];

    s.print("MCP23017 address 0x");
    s.print(address, HEX);

    for (int i = 0; i <= 0x1A; i += 2) {
        int v = readRegister(address, 0, i);
        int v2 = readRegister(address, 1, i);

        if (v < 0 || v2 < 0) {
            s.print(": error reading register ");
            s.print(i, HEX);
            s.println("\n");
            return;
        }
        r[i] = (uint8_t) v;
        r[i + 1] = (uint8_t) v2;
    }

    s.println("");
    printVal(s, "IODIR", 0x00, r[0], r[1]);
    printVal(s, "IOPOL", 0x02, r[2], r[3]);
    printVal(s, "GPINTEN", 0x04, r[4], r[5]);
    printVal(s, "DEFVAL", 0x06, r[6], r[7]);
    printVal(s, "INTCON", 0x08, r[8], r[9]);
    printVal(s, "IOCON", 0x0A, r[0xA], r[0xB]);
    printVal(s, "GPPU", 0x0C, r[0xC], r[0xD]);
    printVal(s, "INTF", 0x0E, r[0xE], r[0xF]);
    printVal(s, "INTCAP", 0x10, r[0x10], r[0x11]);
    printVal(s, "GPIO", 0x12, r[0x12], r[0x13]);
    printVal(s, "OLAT", 0x14, r[0x14], r[0x15]);


    s.println("------------\n");
}

// Now, preinstantiate the object for this API

MCPIOIntf MCP23017;
