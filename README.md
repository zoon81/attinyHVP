# attinyHVP

Backgound:
Last week, I bought a lot of ATtiny13A for an upcommig project on ebay. But when I tried to use them, I couldn't program any of them.My last chance was the wrong FUSE bit configuration and luckily it was.
Somehow the SPIEN bit in FUSE bits was disabled in all ATtiny13A.
I found an arduino impementation of HVSP done by Paul Willoughby.
I ported the code to an atmega88 since I do not have an arduino.
