# Relay_Switcher
DIY Relay bypass switcher for audio applications

Are you a Guitarist/Bassist?  Do you have a pedalboard that just keeps on getting more complicated?

Worried about losing the high end to the long capactive runs and buffers?

Or maybe you want the ability to control your Star-Trek level board from a small controller on your desk?

Or maybe you looked at the avaliable products, and practically fell out of your chair with the price tag?

Then look no further! In this project you'll find a modular capable, mono or stereo loop bypass switcher with wireless control capability.

## Notes:
V2 Jackless is up and running in a 6 bay stacked model. In the process a handful of lessons were picked up.
- One, for the jackless model a V3 utilizing probably 3 loops on a single panel with built in signal traces, and possibly breakaway functionality is likely. Attempting to wire the board interconnects resulted in a bit of a mess, one I'm not entirely happy with.
- Two, The original implementations use a completely separate power/control and signal path, not even the grounds are linked. This appears to be an error, and a future revision may include a solderable ground linking pad. In testing this linkage reduced the noise to virtually 0
- Three, the breakout board, while universal has a quirk. When used with the current V2 coms board, Power and ground on the 8 pins are not next to one another. Careful: This may fry some boards.
- Four, some of the pins used on the ESP-32 were linked to serial output, so when those pins are actie, the serial monitor and control are disabled. 
## PCB's
### Main Driver Board
### V2

![Screenshot 2024-06-09 213055](https://github.com/AnotherBuildCorner/Relay_Switcher/assets/112969078/63d05eaf-82f2-4c50-b386-1796e34f3956)

V2 makes some significant improvements over the V1 design. And introduces a second design.
the changes are as follows
- Length of board reduced to 61mm Long
- Reduced count of 2n7000 MOSFET drivers from 3 to 1
- Tied status LED directly to the control output for a net -1 2n7000
- Tied the two relay coils together, controlled by single 2n7000

  Among the boards are now the standard stereo jack model, and a pair of new "jackless" models
  The jackless models utilize either a 4x3.5mm pitch or 4x5mm pitch screw terminal block on the IO. this reduces the cost per loop by up $10 in full implementation
  Additionally, it appears the space consumption overall will be significantly reduced with the new design
  ![Screenshot 2024-06-09 213115](https://github.com/AnotherBuildCorner/Relay_Switcher/assets/112969078/ba27fbba-6481-434b-bd60-44a3bc659cf4)
![Screenshot 2024-06-09 213131](https://github.com/AnotherBuildCorner/Relay_Switcher/assets/112969078/98519d7d-2759-4e7d-a6ef-acc00c54d896)

![Screenshot 2024-06-09 212600](https://github.com/AnotherBuildCorner/Relay_Switcher/assets/112969078/600d4f9d-0146-45f4-bcbb-637c313b8c64)

  The reduced board size should enable either the w/ jack or jackless versions to fit inside a standard 1032L Aluminium looper case
  In testing, the relay coils consume 30mA each, well under the 2n7000's maximum 200mA current.
  the new status LED position can also function as a pulldown for the inputs, reduces the 2n7000 count. And the originals had some current leakage which caused the led to be partially illuminated all the time

### V1
![Screenshot 2024-06-09 213035](https://github.com/AnotherBuildCorner/Relay_Switcher/assets/112969078/3504aa3e-24b7-436a-bfec-2d540fdbd23e)


Pictured above is the main relay board. The core Arcitecture is one 5V coil DPDT Relay per signal channel. So for stereo configurations, two relays are required.
Each Relay is driven by a MOSFET 2n7000 switch circuit. As well as the status LED. All three MOSFET gates are tied to a common signal pin, and configured for 3.3V logic.

Each board is also equipped with a 74L05 5V regulator to drop the incoming 9V power, and provide supply for the relay switching.

Each board also has dual power connectors, as well as 3 pin 2.54mm spaced pins for the signal  this alows for easy connections between multiple boards in series.

Finally, the boards are set up to accept switched 1/4 inch stereo jacks. With the switches internally connected, so loops without connections pass signal in either active or inactive state.

(Add PCB wire photos here)

Of further note, was the consideration to keep audio, and control/ power wires as far away, and with minimal crossing as possible. This is to reduce crosstalk from the processor and power supplies into the audio path.

Future iterations will focus on compressing the PCB into a space suitable for a 16L Aluminium looper enclosure. Or possibly a design set up to directly connect to the mono/stereo signal wires, therby reducing the need for the 1/4 Barrel connectors, and reducing the required 1/4 Plugs by 1/2.  This also helps with space considerations.  Considering these plugs cost upwards of $4 each, cutting in half is most welcome, although this reduces how universal this device will be.

### Processor Board
### V2
![Screenshot 2024-06-09 212904](https://github.com/AnotherBuildCorner/Relay_Switcher/assets/112969078/9e084fd9-a3d6-4b84-a671-042f3508c44a)

Changes from V1 to V2 are a different footprint ESP32.  now the Wroom 32E is the standard, with 38 pins avaiable.
The individual pin headers have been replaced by 3x rj45 connectors. each is configured with 6 individual GPIO pins, as well as a 9V connection, and ground.
Additionally, a new breakout board for the rj45 on the receiver end has been created.
![Screenshot 2024-06-09 212836](https://github.com/AnotherBuildCorner/Relay_Switcher/assets/112969078/4937689f-c710-4c16-b608-3be36bcb5051)

### V1
![Screenshot 2024-06-09 212933](https://github.com/AnotherBuildCorner/Relay_Switcher/assets/112969078/59548987-45d4-424e-9344-db4188b86837)

The microprocessor controlling the relays is an ESP32 Devkit C V3 Chosen for it's relative availability, low price, access to Arduino IDE code, and most importantly the dedicated ESP_NOW wireless communication protocol, which will be used to communicate between boards.
This is a relatively simple IO board for connecting to the GPIO of the processor board. The only extra element is another 74L05 voltage regulator connected to the 5V input of the dev board and the 4 pullup resistors slotted in on GPIO pins which only provide input (not suitable for the relay control, but there are already plenty of pins available for most applications).

One shortcoming, there may not be enough GPIO pins to handle the control switches, status LED's, screen, and rotary encoder(s). But this is all currently under development.

This version has a single easily remedied flaw. I had to create a footprint for the ESP32 used as they were what I had to hand, but appear to no longer be made. Well I flipped a ground and power connection, shorting 3.3V power to ground. For now it's just a matter of cutting the trace.

Future versions will probably switch to using a V4 devkit, or the newer devkits based on the S3 processor for more GPIO, and to retain more current availability. 

### IO Board
### V2
![Screenshot 2024-06-09 213010](https://github.com/AnotherBuildCorner/Relay_Switcher/assets/112969078/468777dc-717f-4e4b-b924-c6f28aafe7a4)

The new V2 board corrects an error in the input switching, resulting in one of the inputs being completely unusable.
Additionally, the board size has been significantly reduced in length. and the IO board direct pinouts have been moved directly under each jack.

### V1
![Screenshot 2024-06-09 212951](https://github.com/AnotherBuildCorner/Relay_Switcher/assets/112969078/688ff221-724c-4b6b-b006-768c8b037788)

This is very similar to the main relay boards.
The differences are the use of the single relay, the dual LED indicators, and the large electrolytic capacitor on the 9V rail.
The Capacitor is a filtering cap, as most of the 9V power supplies will be switched mode supplies, and noise prone.
The relay is no longer used to enable/disable a loop. but instead is used to switch between two possible inputs/outputs. With the other half of the relay dedicated to controlling the indicator LED's
lastly there are direct pinouts on the back of each 1/4 Barrel connector. Should switching features not be desired, these can be used to link the relay loops directly to an output without extra parts.



## Housings
### Control Housing 
![Screenshot 2024-06-09 212513](https://github.com/AnotherBuildCorner/Relay_Switcher/assets/112969078/565f2fa7-e8dc-49e8-8bce-753956f41479)

### Relay housing V2 
JAckless model.
![Screenshot 2024-06-09 212331](https://github.com/AnotherBuildCorner/Relay_Switcher/assets/112969078/781447e6-f76a-4064-8620-b9911a61945c)


The latest relay switch is setup to accept 5 of the jackless relay boards, and one V2 IO board with the RJ45 connection board in front.  however this new model is half the height of V1, and is stackable. By splitting the units into modules of 5, this enables break points for things such as Volume pedals, and always on boosts between loop modules. Additional smaller size stackable modules will likely be produced in the future. No one single module can exceed more than 6 relay modules due to the RJ45 connection's configuration.

### Relay housing V1
(picure)
The current housing design uses horizontally stacked relay loops, with a total count of 10. Plus the input and output boards. The IO is located on the front for accesibility, with the loops located on the back.

### Control Housing
(picture
The current design is a single print, 6 switch control with provisions to add a 128X64 OLED.
At present this is in it's early stages, so no provisions to mount the control board, or a larger screen have been included. This will come around in V2  The footswitches are spaced to allow foot, or hand use currently. And the board can be purely powered from a 5V usb supply though the ESP32 board.

## Code
(Coming Soon...)


