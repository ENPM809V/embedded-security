
#include "util.h"

#ifndef UART_H
#define UART_H

#define	MAX_UART_ARRAY_SIZE	200
#define ZERO 0
#define ZERO_CHAR '0'
#define SPACE ' '
#define		MAX_INT_SIZE			(8*sizeof(int))


void uart_init();
void uart_send(unsigned int c);
char uart_getc();
void uart_getsn(unsigned char *, uint32_t);
void uart_puts(char *s);
void uart_hex(unsigned int d);
#endif