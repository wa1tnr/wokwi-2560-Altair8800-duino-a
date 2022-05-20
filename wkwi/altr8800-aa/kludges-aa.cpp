#include <Arduino.h>
#include "cpucore.h"
#include "host_mega.h"

// #define SHORT_DELAY 0x17ff
#define SHORT_DELAY 0x266 // slew 10x

// #define SIGNF_DELAY 0x47ff
#define SIGNF_DELAY 0x733 // slew 10x

#define VERY_SIGNF_DELAY SIGNF_DELAY * 3

// 0x7fffffff  is 2147483647

// unsigned long superdelay = 0x2fff; // about 12 seconds

unsigned long superdelay = 0x8fff; // about 12 seconds

void kludge_aa(void) {
    Serial.print("\r\nin view now is a print statement found in function kludge_aa() in file kludges-aa.cpp\r\n");
    delay(SHORT_DELAY);
    // host_set_addr_leds(regPC);
    host_set_addr_leds(22); // RESULT: decimal notation used and    .*.   **.  last six leds lit
    delay(SHORT_DELAY);
    host_set_addr_leds(255); // 8 bits set.
    delay(SIGNF_DELAY);
    host_set_addr_leds(0xaaaa); // every other one
    delay(SHORT_DELAY);
    host_set_addr_leds(0x9999); // 1 001 100 110 011  001
    delay(VERY_SIGNF_DELAY);
    Serial.print("END of very significant delay kludgefile.");
    delay(SIGNF_DELAY); // attenion span relief ;)
}

// END
