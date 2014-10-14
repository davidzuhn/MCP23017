/* emacs should treat this as -*- c++ -*-  */

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

#ifndef MCP23017_H
#define MCP23017_H

#include "Arduino.h"

#define INPUT_INVERTED 0x3
#define INPUT_PULLUP_INVERTED 0x4

class MCPIOIntf {
  public:
    enum mcp_register { IODIR = 0x00, IPOL = 0x02, GPINTEN = 0x04,
			DEFVAL = 0x06, INTCON = 0x08, IOCON = 0x0A,
			GPPU = 0x0C, INTF = 0x0D, INTCAP = 0x10,
			GPIO = 0x12, OLAT = 0x14
    };

    /* returns true if a device is readable at this address */
    bool exists(uint8_t address);

    /* sets the mode of the given I/O line:
     *   OUTPUT, INPUT, INPUT_PULLUP, INPUT_INVERTED, INPUT_PULLUP_INVERTED
     */
    bool setMode(uint8_t address, uint8_t port, uint8_t bit, uint8_t mode);

    /* returns the value of the given GPIO line
     *   if the bit argument is specified, the value will be 0 or 1
     *   if no bit argument is given, return 0-255 (0 or 1 for each of the 8 bits)
     */
    int getGPIO(uint8_t address, uint8_t port, uint8_t bit);
    int getGPIO(uint8_t address, uint8_t port);

    /* set the value of the given GPIO line
     *    if the bit argument is specified, only that bit is modified
     *    if no bit argument is specified, all 8 bits on that port will be set
     */
    bool setGPIO(uint8_t address, uint8_t port, uint8_t bit, bool value);
    bool setGPIO(uint8_t address, uint8_t port, uint8_t value);

    /* read or write a given register from the MCP23017 device
     *   see the datasheet for register details.  Specify the "A" value
     *   for the register or use the mcp_register enum values, and the
     *   correct register will be used for the specified port
     */
    int readRegister(uint8_t address, uint8_t port, uint8_t reg);
    void writeRegister(uint8_t address, uint8_t port, uint8_t reg, uint8_t value);

    /* print the current MCP23017 register state to the given Stream (such as Serial) */
    void debugDump(Stream & s, uint8_t address);

private:
    bool initialized = false;
    void init();
};


// preinstantiate an object for ready use
extern MCPIOIntf MCP23017;

#endif
