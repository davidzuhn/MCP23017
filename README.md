MCP23017
========

Arduino support library for MCP23017 I2C I/O expander chips


This library allows the user to configure I/O lines on the chip without
direct use of I2C/Wire commands.  I/O lines default to input (as per the
chip spec), but can be configured as output lines.  Input ines can be
configured to use the pullup resistors for inputs, or to invert the logical
state reported to the sketch.

Outputs can be modified on a port level (8 lines at a time), or one by one.
Modifications to a single bit on a port should not affect the state of any
other bit on that port.



These chips are used on the cpNode's IOX I/O expander boards.
