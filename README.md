# sd_card_testing

are you an Arduino user? (like me...)

do you use the serial monitor for debug & status info? (like me...)

do you hate it when the USB cable gets wrapped around your robot, or it trips over it, or tugs on it, or having to run along behind your bot outdoors carrying your laptop (and hoping you don't trip and fall...) (defnly like me...)

Well, your days of misery and suffering are over!   =)

I've been looking into this for some time, and I think I may have found a great solution!

Think of it as the Arduino version of a ROS bag file. You collect up & locally store a bunch of data, readings, measurements, status info, or whatever you like, and then review it all after your test run at your leisure.

The basic idea I thought of was to use micro SD cards (or µSD). 

It turns out there are readily available adapter or breakout boards for these, but most depend on the MCU SPI port for their interface. This is way better than I2C but has some notable limitations. Writing a single INT can take 20-30 ms, which can be an issue at the wrong time, and can certainly add up fast if you're debugging a lot of sensors, etc.

Burst writes (up to 512 bytes) are a lot better, but can take 80 - 190 ms in my testing, see attached spreadsheet. This is more efficient, and the writes could perhaps be done when there are 'breaks in the action' so to speak, but that might not be the most convenient thing depending on what you're trying to do. You would have to monitor your buffer level and stop when it's full, write it to the SD, then resume, etc. 

Or just keep going and 'fly blind' and pray you don't hit anything while writing out the buffer.    =)

The best solution I've found is called OpenLog, and it basically uses another 'Arduino' ST MCU to receive your serial port writes and manages buffering and writing the data to the µSD card for you. The buffer is only 2 kB, so is has some limitations as well, but it turns out the performance greatly depends on the speed of the card rather than the MCU you're using.

I've found that you can stream error free continuously at 57,600 baud with a class 10 card (10 MB/s), and 115,200 baud error free with a class 30 or v30 card (30 MB/s).

Since I normally like to run my serial monitor at the latter rate, that is perfect for what I need. 

Plus this only takes one IO pin (serial TX) vs. four for SPI, and the adapter is about the size of a postage stamp. Level shifting is optional, only needed for reads but should really work fine without it (plus I don't do reads anyway).

I plan on putting an OpenLog on all my bots from now on.    =)

Code and test results attached. Let me know if you have any Qs.

Ralph



