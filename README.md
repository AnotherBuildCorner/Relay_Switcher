# Relay_Switcher
DIY Relay bypass switcher for audio applications

Are you a Guitarist/Bassist?  Do you have a pedalboard that just keeps on getting more complicated?

Worried about losing the high end to the long capacive runs and buffers?

Or maybe you want the ability to control your Star-Trek level board from a small controller on your desk?

Or maybe you looked at the avaliable products, and practically fell out of your chair with the price tag?

Then look no further! In this project you'll find a modular capable, mono or stereo loop bypass switcher with wireless control capability.
## PCB's
###Main Driver Board
V1
(Insert Picture here)

Pictured above is the main relay board. The core Arcitecture is one 5V coil DPDT Relay per signal channel. So for stereo configurations, two relays are required.
Each Relay is driven by a MOSFET 2n7000 switch circuit. As well as the status LED. All three MOSFET gates are tied to a common signal pin, and configured for 3.3V logic.

Each board is also equipped with a 74L05 5V regulator to drop the incoming 9V power, and provide supply for the relay switching.

Each board also has dual power connectors, as well as 3 pin 2.54mm spaced pins for the signal  this alows for easy connections between multiple boards in series.

Finally, the boards are set up to accept switched 1/4 inch stereo jacks. With the switches internally connected, so loops without connections pass signal in either active or inactive state.

(Add PCB wire photos here)

Of further note, was the consideration to keep audio, and control/ power wires as far away, and with minimal crossing as possible. This is to reduce crosstalk from the processor and power supplies into the audio path.

Future iterations will focus on compressing the PCB into a space suitable for a 16L Aluminium looper enclosure. Or possibly a design set up to directly connect to the mono/stereo signal wires, therby reducing the need for the 1/4 Barrel connectors, and reducing the required 1/4 Plugs by 1/2.  This also helps with space considerations.  Considering these plugs cost upwards of $4 each, cutting in half is most welcome, although this reduces how universal this device will be.

### Processor Board
V1
(Insert Picture Here)
The microprocessor controlling the relays is an ESP32 Devkit C V3 Chosen for it's relative availability, low price, access to Arduino IDE code, and most importantly the dedicated ESP_NOW wireless communication protocol, which will be used to communicate between boards.
This is a relatively simple IO board for connecting to the GPIO of the processor board. The only extra element is another 74L05 voltage regulator connected to the 5V input of the dev board and the 4 pullup resistors slotted in on GPIO pins which only provide input (not suitable for the relay control, but there are already plenty of pins available for most applications).

One shortcoming, there may not be enough GPIO pins to handle the control switches, status LED's, screen, and rotary encoder(s). But this is all currently under development.

This version has a single easily remedied flaw. I had to create a footprint for the ESP32 used as they were what I had to hand, but appear to no longer be made. Well I flipped a ground and power connection, shorting 5V power to ground. For now it's just a matter of cutting the trace.

Future versions will probably switch to using a V4 devkit, or the newer devkits based on the S3 processor for more GPIO, and to retain more current availability. 

### IO Board
V1
(Insert Picture Here)
This is very similar to the main relay boards.
The differences are the use of the single relay, the dual LED indicators, and the large electrolytic capacitor on the 9V rail.
The Capacitor is a filtering cap, as most of the 9V power supplies will be switched mode supplies, and noise prone.
The relay is no longer used to enable/disable a loop. but instead is used to switch between two possible inputs/outputs. With the other half of the relay dedicated to controlling the indicator LED's
lastly there are direct pinouts on the back of each 1/4 Barrel connector. Should switching features not be desired, these can be used to link the relay loops directly to an output without extra parts.

## Housings
### Relay housing
(picure)
The current housing design uses horizontally stacked relay loops, with a total count of 10. Plus the input and output boards. The IO is located on the front for accesibility, with the loops located on the back.

### Control Housing
(picture
The current design is a single print, 6 switch control with provisions to add a 128X64 OLED.
At present this is in it's early stages, so no provisions to mount the control board, or a larger screen have been included. This will come around in V2  The footswitches are spaced to allow foot, or hand use currently. And the board can be purely powered from a 5V usb supply though the ESP32 board.

## Code
(Coming Soon...)


