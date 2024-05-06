#include "util.h"
#include "gpio.h" 
#include "delays.h"
#define MEM_START &_end
#define MEM_END   MMIO_BASE

unsigned long freeptr = (unsigned long) MEM_START;
static unsigned long int next = 1;
uint32_t canaryValue = 0;

int free(void *p) {
    return 0; 
}

void *memset(void *s, int c, uint32_t n) {
    uint8_t *p = (uint8_t *)s;

    /* c should only be a byte's worth of information anyway, but let's mask out
     * everything else just in case.
     */
    uint8_t x = c & 0xff;

    while (n--)
        *p++ = x;
    return s;
}

uint8_t strlen(unsigned char *str) {
    uint32_t c = 0;
    while (str[c++] != '\0');
    return c-1;
}

// Function to implement strcat() function in C
char* strcat(char* destination, const char* source, size_t n)
{
    // make ptr point to the end of destination string
    int i = 0;
    char* ptr = destination + strlen((unsigned char *) destination);
 
    // Appends characters of source to the destination string
    while (*source != '\0' && i < n) {
        *ptr++ = *source++;
        i++;
    }
 
    // null terminate destination string
    *ptr = '\0';
 
    // destination is returned by standard strcat()
    return destination;
}

void *malloc(size_t size)
{
    if (size < 1 || freeptr + size > MEM_END)
        return (void*)0;
    freeptr += size;
    return (void*)(freeptr - size);
}

int memcmp(void *s1, void *s2, int len)
{
    unsigned char *p = s1;
    unsigned char *q = s2;
    int charCompareStatus = 0;
    //If both pointer pointing same memory block
    if (s1 == s2)
    {
        return charCompareStatus;
    }
    while (len > 0)
    {
        if (*p != *q)
        {  //compare the mismatching character
            charCompareStatus = (*p >*q)?1:-1;
            break;
        }
        len--;
        p++;
        q++;
    }
    return charCompareStatus;
}

/**
 * Memory Copy 
 **/

void memcpy(void *dest, void *src, size_t n) {
    char *csrc = (char *) src;
    char *cdest = (char *) dest;

    for (int i = 0; i < n; i++) {
        cdest[i] = csrc[i];
    }
}

/**
 * Division since we don't have division without importing a library
 **/ 

int division(int num1, int num2)
{
    int temp = 1, quotient = 0;
    while (num2 <= num1)
    {
        num2 <<= 1;
        temp <<= 1;
    }
    while (temp > 1)
    {
        num2 >>= 1;
        temp >>= 1;
        if (num1 >= num2)
        {
            num1 -= num2;
            quotient += temp;
        }
    }
    return quotient;
}

/**
 * Modulo
 **/ 
int modulo(int num1, int num2){
	//variables
	int p, q, m;

	q = division(num1, num2);	//finding quotient (integer part only)
	p = q * num2;	//finding product
	m = num1 - p;	//finding modulus
	
	return m;
}

/** 
 * Convert string containing numbers to int
 **/

int atoi(char* str)
{
    // Initialize result
    int res = 0;
 
    // Iterate through all characters
    // of input string and update result
    // take ASCII character of corosponding digit and
    // subtract the code from '0' to get numerical
    // value and multiply res by 10 to shuffle
    // digits left to update running total
    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';
 
    // return result.
    return res;
}

int rand(void) // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed)
{
    next = seed;
}

int setCanary(void) {
    srand(get_system_timer()); 
    canaryValue = rand();
    return canaryValue;
}