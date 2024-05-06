

#include "gpio.h"
#include "util.h"
#include "uart.h"
#include "led.h"
#include "delays.h"

/* Auxilary mini UART registers */
#define AUX_ENABLE      ((volatile unsigned int*)(MMIO_BASE+0x00215004))
#define AUX_MU_IO       ((volatile unsigned int*)(MMIO_BASE+0x00215040))
#define AUX_MU_IER      ((volatile unsigned int*)(MMIO_BASE+0x00215044)) 
#define AUX_MU_IIR      ((volatile unsigned int*)(MMIO_BASE+0x00215048)) 
#define AUX_MU_LCR      ((volatile unsigned int*)(MMIO_BASE+0x0021504C))
#define AUX_MU_MCR      ((volatile unsigned int*)(MMIO_BASE+0x00215050))
#define AUX_MU_LSR      ((volatile unsigned int*)(MMIO_BASE+0x00215054))
#define AUX_MU_MSR      ((volatile unsigned int*)(MMIO_BASE+0x00215058))
#define AUX_MU_SCRATCH  ((volatile unsigned int*)(MMIO_BASE+0x0021505C))
#define AUX_MU_CNTL     ((volatile unsigned int*)(MMIO_BASE+0x00215060))
#define AUX_MU_STAT     ((volatile unsigned int*)(MMIO_BASE+0x00215064))
#define AUX_MU_BAUD     ((volatile unsigned int*)(MMIO_BASE+0x00215068))

#define BAUD 270   // 115200
//#define BAUD  3254  // 9600
#define BLINK_DELAY 0

uint32_t use_zero_padding = 0;


/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
void uart_init()
{
    register unsigned int r;

    /* initialize UART */
    SET(AUX_ENABLE) |= 1;       // enable UART1, AUX mini uart
    SET(AUX_MU_LCR) = 3;       // 8 bits
    SET(AUX_MU_MCR) = 0;
    SET(AUX_MU_IER) = 0;
    SET(AUX_MU_IIR) = 0xc6;    // disable interrupts
    SET(AUX_MU_BAUD) = BAUD;

    /* map UART1 to GPIO pins */
    r=*GPFSEL1;
    r &=~(7<<12);
    r |=(2<<12); 
    r &=~(7<<15); // gpio14, gpio15
    r |=(2<<15);
    *GPFSEL1 = r;
    
    *GPPUD = 0;            // enable pins 14 and 15
    wait_cycles(150);
    *GPPUDCLK0 = (1<<14)|(1<<15);
    wait_cycles(150);
    *GPPUDCLK0 = 0;        // flush GPIO setup

    *AUX_MU_CNTL = 3;      // enable Tx, Rx
}

/**
 * Send a character
 */
void uart_send(unsigned int c) {
    /* wait until we can send */
    SPIN_NOP(!(*AUX_MU_LSR&0x20))
    /* write the character to the buffer */
    SET(AUX_MU_IO)=c;
}

/**
 * Receive a character
 */
char uart_getc() {
    char r;
    /* wait until something is in the buffer */
    SPIN_NOP(!(*AUX_MU_LSR&0x01))
    /* read it and return */
    r=(char)(*AUX_MU_IO);
    /* convert carrige return to newline */
    
    /* echo */
    if (r == '\r')
        uart_send('\n'); 
    uart_send(r);

    return r=='\r'?'\n':r;
}

void uart_getsn(unsigned char *buf, uint32_t max) {
    uint32_t curr = 0;
    uint8_t c = 0;
    while (curr < max && (c = uart_getc()) != '\n') {
        buf[curr] = c;
        curr++;
    }
    buf[curr] = 0;
}

/**
 * Display a string
 */
void uart_puts(char *s) {
    while(*s) {
        /* convert newline to carrige return + newline */
        if(*s=='\n') {
            uart_send('\r');
        }
        uart_send(*s++);
    }
    blink(500);
}

/**
 * Display a binary value in hexadecimal
 */
void uart_hex(unsigned int d) {
    unsigned int n;
    int c;
    for(c=28;c>=0;c-=4) {
        // get highest tetrad
        n=(d>>c)&0xF;
        // 0-9 => '0'-'9', 10-15 => 'A'-'F'
        n+=n>9?0x37:0x30;
        uart_send(n);
    }
}
