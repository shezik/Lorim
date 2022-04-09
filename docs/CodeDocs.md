Hi. Thanks for joining me on this project.

Before I attempt to *deobfuscate* everything, let's get your codebase building first.

## Step 1: Building Prerequisites

*(Make sure you have ESP32 board support installed in advance.)*

Install **GEM** from Library Manager in Arduino IDE.<br>
This repository comes with a slightly modified version of LoRaLayer2, there should be no problems with that.<br>

Then, some hacks to GEM are required in order to make things work as expected.

### Step 1.1: GEM Hacks

**Please note that the following tutorial is based on GEM version 1.3.4.**

Navigate to ```%USERPROFILE%\Documents\Arduino\libraries\GEM\src```. We'll be editing ```config.h``` and ```GEM_u8g2.h```.

Let's take a look at ```config.h``` first. If you don't have *AltSerialGraphicLCD* and *Adafruit GFX*, and are unwilling to add compiler flags, please comment out the corresponding ```#include``` lines, which are line 4 and 16 here.

Save and close this file.

Then open ```GEM_u8g2.h```. Things are getting complicated here.
- Please change the second member access control keyword in the declaration of GEM_u8g2 class from **private** to **protected** at line 141.<br>
- Also append word **virtual** at the beginning of ```void drawMenu();``` at line 134.

Finally, save and close this file. Now you can try building and flashing this project in Arduino IDE!

-----

Now that your codebase is building fine, let's put the device together.

## Step 2: Hardware Connections

Lorim utilizes a keyboard scanner and segmented LED display driver CH340 to read keyboard inputs. The chip communicates with ESP32 through non-standard I<sup>2</sup>C protocol, so [a custom driver](../Kbd_8x5_CH450.hpp) had to be implemented. Please refer to [Pinouts.md](./Pinouts.md#keyboard-scanner-ch450-i2c) for all the wirings.

For the LoRa part, my two devices are using SX1278 modules via SPI, though many other models are also supported, see [sudomesh/LoRaLayer2](https://github.com/sudomesh/LoRaLayer2#description-of-library-files). Undoubtedly, you guessed it, please refer to manufacturer datasheet and [Pinouts.md](./Pinouts.md#lora-module-sx1278-hspi) for power supply, wirings and stuff.

The display is merely a common U8g2-compatible 12864 dot matrix LCD via SPI.

-----

Here comes the real code documentation part.

## Module dependency tree

Well, I ought to draw something here but nah, maybe later.
