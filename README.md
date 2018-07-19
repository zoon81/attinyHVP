# attinyHVP

Last week I bought a lot of attiny13a on ebay for an upcommig project but when I tried to use them, I failed.
Later, I recognised they had a serious proble, maybe it's all faulty.
My last chance was a wrong FUSE bit configuration and it is.
Somehow the SPIEN bit in FUSE bits was disabled in all attiny.
I found an arduino impementation of HVSP done by Paul Willoughby.
I ported the code to an atmega88 since I had not an arduino.
