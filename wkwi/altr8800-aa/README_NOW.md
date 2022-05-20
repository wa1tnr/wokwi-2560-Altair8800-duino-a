20 May 2022: Towards a validated wiring job. ;)

Fri 20 May 17:07:07 UTC 2022

  https://github.com/wa1tnr/wokwi-2560-Altair8800-duino-a/tree/dvlp-aa

  https://wokwi.com/projects/332196032517505618

Altair8800.ino:

   1743           // put PC on address bus LEDs
   1744           host_set_addr_leds(regPC);

Theory: this could be used to test wiring of address LEDs.
A15 seems wrong at the moment; can't quite suss why.


Note: flip both switches DOWN to run normal.  This seems correct
(by operation).  Not sure why (maybe some switches MUST be wired and they are not?)

Quick instrux:

Tap the 'u' key on your (real) keyboard.  Haven't got the nomenclature down,
but tapping 'u' (one of the AUX switches, simulated) seems to execute the
program named by the directory of such programs; program zero
(all S15-S0 switches down) is the directory listing itself.

Resize the terminal to max it out vertically to see 'everything'.

Kill-the-Bit works as-is, though unknown what it 'does' besides
make a nice animated LED display.

Meant to be a game using A15-A8 as input to it, iirc.

It's only listed as hex numbers in prog_games.cpp:

    20  static const byte PROGMEM killbits[] = {
    21  0x21, 0x00, 0x00, 0x16, 0x80, 0x01, 0x0E, 0x00, 0x1A, 0x1A, 0x1A, 0x1A, 0x09, 0xD2, 0x08, 0x00,
    22  0xDB, 0xFF, 0xAA, 0x0F, 0x57, 0xC3, 0x08, 0x00};

That's a hint as to how to program it from the front panel, manually,
perhaps. ;)


TO PLAY Kill-the-Bit:

Set the address switches to B10 (Set switch S1, all others reset) then tap 'u'
on your physical real) keyboard.

Rudamentary: 0 thru 9 and a thru f are keybindings that control S15 thru S0
(toggles them).  A caret ('^') represents a switch set UP (high, set).

A 'v' represents a switch that is cleared (DOWN) (low, reset).

So, to run Kill-the-Bit, S1 is toggled ON (UP/HIGH/SET) using the '1' key
of your (real) keyboard.

You can also save trouble by using the special slash ('/') keybinding,
to set all 16 address switches at once (using hexadecimal digits).

Literally (at your real keyboard):

    / F F F F    << flip all address switches (S15 thru S0) ON (or UP).

In the Linux (host PC) port, Control C pressed twice in rapid succession
ends the simulator (entirely).  Not sure how to do that on wokwi.

LINUX CHEAT:  type 'make' in the repository directory; will compile
a Linux (host PC) version that runs pretty good for basic checks,
and for practice.

There are 'make clean' and 'make run' targets in the local variant on github.

    https://github.com/wa1tnr/Altair8800-samd51-tnr-a/blob/terminal-linux-a/Makefile#L38-L39

A bang ('!') is some kind of a RESET function and (iirc) corresponds to
one of the front panel keys.

Have (accidentally) modified the PC (Program Counter) in the (recent)
past; it is expected that this very mechanism could be used to test the
wiring of the sixteen address LEDs.  Not at all sure this approach will
lead to a proper test, or not. ;)




19 May 2022: COMPILES AND RUNS.
Starting to look just like the Linux version.

Most of the LEDs are wired and .. seem .. to be correctly wired.

Don't have a lot of tests to run at the moment.

    https://github.com/wa1tnr/Altair8800-samd51-tnr-a/blob/altair-8800-wokwi-a/doc/NOTE.md

Presently on Serial rather than Serial1 and
pins 18, 19 are ignored (21:43z 19 May 2022).

see dhansel on github:

  https://github.com/dhansel/Altair8800


adwaterandstir.com as well - has several
useful references there.

(at times) unused serial stuff, below:

Serial1.begin(baud)
    [ "mega:18", "$serialMonitor:RX", "", [] ],
    [ "mega:19", "$serialMonitor:TX", "", [] ],


END.
