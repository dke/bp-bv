# LED Lamp for the BP-PV solution

## Goals

* LED Lamp
* for the tent
* power-supplied by the BP-PV battery
* dimmable by Android Smarthpone via bluetooth Low Energy.

## Implementation

* Power Supply: 12.8v 4s LiFePo4 Battery from the BP-PV solution
* LEDs: 3x Nichia NVSL219CT
* LED Driver: Mean Well LDD-1500HW
* PWM generator with Bluetooth Low Energy Interface: Arduino Nano 33 BLE (also called µC in the following)
* Interface µC - Driver: Opto-Coupler ILD74
* Power Supply for the µC: 5V Step-Down Module power-supplied by the 12.8V battery

## Discussion

### Power supply

Aka main battery. Well, this thing is supposed to run on the battery of the BP-PV solution, so this was pre-defined.

### LEDs

Picked the LEDs just to try Nichia because I had Cree the last time and wanted to try Japanese Nichia. Ontop, they are cheaper.

Picked these because they have a "warm-white" light temperature and can run high currents (up to 3A?!). For cooling reasons and because I don't want to empty the 6Ah battery in one evening, targetting max for 1.5A and dimmable to be able to consume even less (and it turned out later that much less current is actually needed to illuminate a tent).

Picked 3 of them because of my voltage of 12.8V (nominal, depending on the battery level) and to give the driver some headroom.

Bought them soldered on 10x10 boards because that's easier for me to solder.

Got some heat sinks "Fischer Elektronik SK 46 10" (star-shaped, some 51x51x10 mm) which turn out to be capable to dissipate the LEDs running on the full (anticipated) power of 1.5A without getting "too" warm, while they are getting "very" warm (so warm that you just can't touch them "forever"). Feels like pretty much in a sweet spot: Even on full power still sub-critical, while in partial-power loads I am aiming for it is nice and cold.

### Driver

Loooking for a driver which can do PWM and fits in voltage and current. Picked the Mean Well LDD-1500HW because of this (and because cheap) and because I had good experience with other Mean Well stuff before.

### Bluetooth-controllable PWM generator

Arduino Nano 33 BLE

Well, Arduino because obviously and Nano 33 BLE because it comes with Bluetooth onboard rather than requiring an additional module. Less power consumption, less hassle in the build. BLE rather than "legacy" Bluetooth because less power consumption and I guess easier to program.

### Interface via Optocoupler

I guess it would not be strictly required to use an optocoupler (OC), most people skip this and by the electrical specs it would not be required, but is an additional safety and a best practice I learned from my father.

Picking the ILD74 because I knew it from earlier projects.

The actual circuit is slightly less trivial than directly connecting the secondary side of the OC to the LED driver (which works as well), because it has the advantage that in this circuit a disconnected / offline µC will make "dark" LEDs, while the "trivial" circuit would make "full-power" LEDs, and I preferred the "dark" variant for a bunch of reasons, mostly security / thermal reasons, but also to not accidently drain the battery empty.

See the [circuit.svg](./circuit.svg) for details.

### µC power supply

I read we could connect it directly to 12.8V, but want to be sure and not overload internal voltage controllers...

So, got from ebay such a small module which accepts a wide range of input voltages and converts (step-down) to 5V.

## Arduino program

Doing the usual "programming" like googling for examples and copy-pasting stuff from suitable examples into a combined solutiong.

See the file [fade_d2_ble.ino](./fade_d2_ble.ino) for the full file.

I would give the sources of my copy-pasting... if I had them still around.

There are some details worth mentioning:

* The "magic" numbers in the BLE code are, I think, just random numbers which are later exposed / visible when browsing the code from BLE clients, like service and metric identifiers
* The `while (!Serial);` line is an artifact often found in example code which is supposed to run interactively while the Arduino is connected to the programming device (thus, PC). In disconnected / "production" mode this endless loop will loop forever, thus, break your program, thefore disabled in the "prod" version.
* In principle all the BLE handling code is boilerplated from a sample program, but adjusted to my desired logic. In particular I chose to hook to the BLE disconnect to write the value to NVM because I don't want to write it "always" when it's updated because supposedly the NVM writes only have a limited number and I anticipated writing an App with a slider which could potentially fire *very lots* of subsequent update events
* The key-value stuff is actually one of the few (the only?) solution to store the PWM value persistently (thus, surviving power-offs) on the device, because this one does not feature an eeprom, which seems to have been standard on earlier flavors of Arduino

## Client App

Unfortunately I failed to wite a nice slider-based app.

In principle that would be yet another copy-paste-merge work, identify a sample slider app, and a sample BLE client app.

Both of these exist:

* https://github.com/PunchThrough/ble-starter-android iirc for the BLE side
* (lost the reference for a slider demo app)

But I failed yet to "frankenstein" them into one.

Using currently the PunchThrough LightBlue app. It is a generic app and I can't slider to control the PWM, but hey, that's okay.

## Build

Soldered the µC - driver interface on a "breadboard".

Picking XT30 connectors for the connections from battery to my circuit, and from the led driver output to the LED lamp.

Soldered wires (1.5mm squared -- a bit overkill, but I like it) to implement the 3s connection of the LEDs.

Glued the LEDs on their circuit boards on the heat sinks.

Added some reflectors / optics to have better use of the light rather than illuminating to 2*pi steradians.
