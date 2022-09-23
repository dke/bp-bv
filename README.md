# Backpacking Photovoltaics Solution

## Motivation, or: Limitations of existing products

The industry tries to sell PV "modules" which come with an USB output to charge a separate  power bank.

The issue with this approach is efficiency and reliability. For good results you want to have a charge controller which can operate the PV module in its optimal working point ("MPPT mode") and feed all the harvested power rawly into the battery. This concept does not fit well to the approach of using the USB standard to interface the PV module to the power bank. In the worst case the power bank stops charing after a few clouds when the "smart" electronics in the power bank considers the input power as "flaky".

So the superior approach is to use a good PV charge controller, connect it directly to a battery, and to a PV module. Just like building "normal" PV solutions, but focusing on lightweight portable components. This is a challenge because usually the components are much bigger and more powerful than what we need here.

Uhm, what we need here... let's discuss the requirements:

## Requirements

I want to be able to produce enough electricity for our backpacking / bikepacking tours of 4 adults with four smartphones plus some other smaller power consuming devices (e-book readers, small flash lights, smart watches, fitness trackers, etc). A smartphone battery typically has some 4.5Ah capacity with 3.7V voltage, means some 16.65Wh power capacity, and assuming to charge each device per day once fully gives a target daily power production of 66.6Wh of the solution.

## Component sizing

This (some 70-ish Wh) is the battery size I am aiming for in order to optimize weight. Of course if you chose the battery size bigger, you can survive multiple cloudy days, but you pay the price of a heavier battery.

For the PV module, I think it might be sufficient to aim to some 20 Watt peak power, however due to availability of small / lightweight modules on the market, the first implementation of this solution was using a 35W module, which was powerful enough to fill the battery in ideal conditions in few hours. As said before, smaller whould probably be sufficient as well, but this was fine as well.

For the charge controller, I failed to find something small an lightweight with "just enough" power for my solution. The smallest "reasonable" products start at 10A current rating (and some 500g), which typically means way more power than required for my PV module and the battery, but okay, I can live with the additional footprint / weight.

## Component selection

### PV module

For the PV module, I found a 35W ETFE module from Offgridtec which is about 800g and some 70x35cm in size. https://www.offgridtec.com/offgridtec-etfe-spr-f-30-v2-35w-marine-solarzelle-flexibel.html

It is attractive to look for the semi-flexible modules because these are way easier to handle and transport than their rigid frame counterparts.

There is an alternative product with 30W https://greenakku.de/Solarmodule/Flexible-Solarmodule/GreenAkku-Neptun-Semi-Flexibles-Solarmodul-doppel-ETFE-30Wp::2965.html which was not so attractive from its length / width ratio to me.

Looking back after the first travel with the solution, it might be preferrable to have two or more smaller modules which can be transported in a "folded" fashion.

Only later I found https://www.reichelt.de/solarpanel-semi-flex-20-32-zellen-12-v-20-w-phae-sf-20-p194536.html?&trstct=pol_14&nbc=1 which is nice and small (suitable for 2 to 4 serial or 2s2p configuration), but nearly twice the price per watt as the 35W module referenced above. ... 

### Battery

There is a small choice of 4-cell LiFePO4 batteries with 6Ah available which fit my requirements. I picked one from "EREMIT" https://www.eremit.de/p/12v-6ah-lifepo4-flach-mit-bms because it was a few € cheaper and looking back, the built-in BMS / short-circuit protection saved me at least once. Good product.

### Charge controller

The heart of the solution... I was using earlier for a solution for my car a cheap (pseudo-) MPPT charger from ebay, but now I wanted to spend some extra € to get a fancy product where I trust the producer and get nice addon features (like, bluetooth interface), so I picked the smallest https://www.victronenergy.de/solar-charge-controllers/smartsolar-mppt-75-10-75-15-100-15-100-20

I am absolutely happy with the module, its bluetooth interface and the app for the smartphone. Very satisfying to see the power production in real-time (after re-positioning it and when clouds emerge, etc). No need fo crouching to read a crappy LCD device in full sunlight (or dark shadow) conditions, and no accidential touching of buttons on the case. I never want to go back to a charge controller without this feature. However it is a bit pricey and if your budget is contrained, you might want to research for a cheaper simpler solution. (And if you find one which is if good quality and lower size / weight, let me know, I might be willing to skip the bluetooth interface then. Maybe.)

## Consumers

Well, strictly speaking not part of the PV solution, but still necessary: how to charge the smartphones with the (nominal) 12V of the 4s LiFePO4 battery?

Well, ebay is full of modules with names like "Type-C USB QC3.0 QC2.0 PD3.0 Quick Charge Fast Charging Board Circuit Module DC", e.g. (the link will probably break soon) https://www.ebay.de/itm/393391228378?mkcid=16&mkevt=1&mkrid=707-127634-2357-0&ssspo=73jfPPJpQPu&sssrc=2047675&ssuid=trlt9dk0S6C&widget_ver=artemis&media=COPY

These are cheap (some 5.5€ each) and work flawlessly, like, they worked with every (PD capable) device I connected them to, and seem to have high efficiency (not much noticeable heat production).

Some devices seem to be problematic, though, in particular those who don't properly speak USB-PD, but rather expect plain 5V (legacy USB). For these I also added some corresponding plain USB modules like "DC-DC USB Step Down Module 6-24V 12V/24V to 5V 3A USB Charger Module Arduino" https://www.ebay.de/itm/224602921105?mkcid=16&mkevt=1&mkrid=707-127634-2357-0&ssspo=WqmD4fmnRii&sssrc=2047675&ssuid=trlt9dk0S6C&widget_ver=artemis&media=COPY 

Another consumer is a LED lighting I added later, based on the Mean Well LDD-1500HW driver with 3x Nichia NVSL219CT LEDs and some heat sinks and optics, with a dimming feature based on a PWM signal generated by an Arduino, controlled by a smartphone app... but that's something for [a different article](./README.LED-Lamp.md).

## Implementation

### PV module modification

The PV module comes with the well-known nasty bulky high-footprint cables (aluminum, right?) and MC4 connectors. Of course I want to replace this by a smaller, less buly, more flexible cable I know from my previous RC model hobby, like 2x 1.5mm^2 silicon copper cable. To do so, I need to open the connector box on the PV module, scratch out the white "glue", remove the existing cables, and attach my new ones. Or, rather, I used 3.5mm banana plug female connectors which I soldered directly into the box, so I can transport the module with detached cables. I filled the box with glue afterwards so no water or dust can enter the connector box. However I am aware that the solution is not fully IP67 compliant or something like that, but I don't intend to run the PV module in the rain anyways and it will survive a few rain drops...

### Connectors

I picked the battery from the vendor with XT30 connectors (well-known from RC model stuff) and implemented the other connections also with XT30 connectors. They provide enough electrical conductivity with a neat and small footprint, and provide reverse polarity protection. I attached a cable with a corresponding connector to the controller.

Likewise I implemented the connection from the PV module to the controller with XT30 connectors.

The USB modules are provided with XT30 connectors as well.. XT30 all over the place... in theory it should also be possible to connect the USB modules directly to the battery or even directly to the PV module, but I never made use of that.

### Heat-shring tubing

The USB modules, after having been provided (soldered) with cables, are put into heat-shrinking tube for some isolation. Well, it is not "fully closed" and obviously still susceptible to splash water, but provides protection against accidential short circuits.

### Mechanical construction / PV module stand

The PV module comes with holes. I used T-connectors and carbon rods from DIY kite shops to build a nice stand for the PV module (like an easel) so it can stand free on its own. The T-connectors and carbon rods are connected to the PV module using cable ties. Initially I had it in "portrait" orientation, which makes no sense; later I converted the build to a "landscape" mode which is much more stable and less prone to wind gusts. In landscape mode, my easel like stand used two rods on the back, rather than only one in the original construction.

### Housing

Due to lack of time and ideas I did not put all the stuff in a nice housing, but just put it in a polyester bag I had from a sleeping mattress (remember, this is about backpacking / bikepacking). This worked out suprisingly well to protect against sand, dirt and humidity.

## Results

On a three week travel in mostly sunny conditions, the setup flawlessly provided enough power to feed all our devices. Actually we had mostly the situation where we had more electricitly available than we could consume. Only on a few mostly cloudy days we ran out of energy and I had to proclaim energy crisis and energy frugality.

As per reporting of the charge controller, on average we produced (and consumed) some 120Wh per day, which is roughly twice than estimated intially -- no clue whether we really consumed that much, or maybe this is imprecise measurement (given that the electronics are created for higher currents than we use them, I have the impression we suffer on arithmetics / integration of a lot of number with few significant digits, thus adding up a lot of rounding errors).

The PV module turned out to be oversized, but a "slightly" larger battery might have been nice (however, also bulkier / heavier).

The handling of the easel like stand was convenient.

Overall a totally successful project, meeting all its targets and requirements.

## Specs

Weights:

* Controller: 500g
* PV module: 850g
* Battery: 600g

Net weight for these three components: 1950g. (Sounds like a lot, but comparable to commecrial solutions outlined in the first paragraph, and way more powerful. Keep in mind this is for 4 adult persons who can share the load.)

Additional weight for cables, connectors, easel stand, USB modules, etc: not measured / no estimate

Electrical specs:

* 35W (peak) PV power
* 6 Ah / 76.8Wh battery capacity
* Enough discharge current to feed 4 USB-C PD modules at the same time, charging devices with a total power of some 60W

"Lighting specs":

* 2..3 hours of "perfect" light required for a full charge. Typically on a sunny morning we get the battery full while having breakfast and/or disassemble the tents.

## Outlook / Improvements

Not many. Quite satisfied.

Might add a second of these batteries and run them in parallel for charing and disconnected in discharging (we use multiple tents). This needs clarification whether disconnecting and reconnecting these batteries is valid also if they have different charging statuses. The description of the battery vendor indicates that.

Using all these cables and modules created a terrible Kabelsalat (sorry, unable to find a translation): https://de.wikipedia.org/wiki/Kabelsalat I recommend to look for "very" short USB C cables to reduce this pain.

For transportability, I am looking for e.g. 4x10W ETFE PV modules, to replace my (bulky, but otherwise fully satisfying) 35W module, to no avail yet.
