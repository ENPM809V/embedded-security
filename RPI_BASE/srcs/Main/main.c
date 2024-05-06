
#include "uart.h"
#include "led.h"
#include "util.h"

void main()
{
    // set up serial console
    uart_init();

    uart_puts("Press to start...\n");
    uart_getc();

    uart_puts("You are accessing a secure device...\n");
    uart_puts("If you are not authorized, please GTFO.\n");
  
    uart_puts("Finished\n");
    return;
}
