# How it works?
### I cant really tell right now, I am still developing it. But I can tell something.
### There is raspberry pi pico W (you can use raspberry pi pico, but I have used pico W because I want it to have more than one functional wifi modules (main is ESP32's wifi module and its modified anthena I made))
### Raspberry pi pico is connected to 420x380 ILI9488 SPI TFT display, but cardkb doest't work with raspberry pi pico, so I have connected it in ESP32 and I made my own serial bus to tell raspberry pi pico what to show on the display
### My plans are making ESP32 communicate over the network (raspberry pi pico sometimes too, but less), giving it an micro SD card reader to store some user data in non-volatile storage to make better user experience. After prooving that my serial bus works and maybe improving it I will add gyroscope to automatically turn the display and I will create some extension modules
