# Lorim Pinouts

## *THIS IS A PLACEHOLDER FILE AND IS SUBJECT TO CHANGE AT ANY TIME*

![NodeMCU Pin Definition](./devLayout.png "NodeMCU Pin Definition")

- A0 - Random seed

-----

## Arduino defined pin aliases
[common.h](./common.h)<br>
[pins_arduino.h](./pins_arduino.h)

-----

## Keyboard scanner CH450 (I<sup>2</sup>C)
*Will this work as an IO expander?*<br>
*To add a few CS pins for multiple SPI clients.*<br>
- D1 - SDA
- D2 - SCL

-----

## Display LX-12864L-1 (SPI)
| Arduino | Description  | Display |
|---------|--------------|---------|
| D5      | SCL          | 4       |
| D6      | A0 (D/C)     | 3       |
| D7      | SDA (MOSI)   | 5       |
| D8      | CS           | 1       |
| RST     | RST (unused) | 2       |

*Please note that SD1, CMD, SD0, CLK pins are already occupied for communications between*<br>
*the external flash chip and our esp8266 module, thus cannot work as user SPI port.*

-----

##  Reserved pins
- D4 (Built-in LED)
- D0 (Auxiliary built-in LED, USER, WAKE)
- D9 (RXD)
- D10 (TXD)

-----

# Lorim file system structure
LittleFS<br>
&emsp;│<br>
&emsp;├ settings.json<br>
&emsp;|<br>
&emsp;├ messages.json<br>
&emsp;|<br>
&emsp;└ contacts.json
