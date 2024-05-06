
#include "led.h"
#include "gpio.h"
#include "delays.h"
#include "util.h"

void led_init(void) {
    SET(GPFSEL4) |= 1<<21;
}

//backwards for newer pi versions
void led_off(void) {
    SET(GPSET1) = 1<<15;
}

void led_on(void) {
    SET(GPCLR1) = 1<<15;
}

void blink(uint32_t milisec){
    led_on();
    wait_msec(milisec * 1000);
    led_off();
}

void blinks(unsigned int blinks) {
    while(blinks) {
        led_on();
        wait_msec(500000);
        led_off();
        wait_msec(500000);
        blinks--;
    }
}