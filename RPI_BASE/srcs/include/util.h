#ifndef UTIL_H
#define UTIL_H

//cleaner syntax for setting mmio addresses' value
#define SET(x) *(x)

//Spin lock with nop code
#define SPIN_NOP(cond) do{asm volatile("nop");}while(cond);
#define NULL ((void*)0)

/**
 * TYPES
 **/
typedef unsigned char uint8_t;
typedef signed char int8_t; 
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned long long uint64_t;
typedef signed long long int64_t;
typedef float f32_t;
typedef double f64_t; 
typedef unsigned int size_t;

typedef enum { false, true } bool;

extern char _end;


/**
 * FUNCTION DEFINITIONS
 **/

/**
 * Frees the memory that it is pointing to. 
 * @param[in] p         Pointer to memory it needs to free
 * @retval              0 on success, -1 on failure.
 **/
int free(void *p);

void *memset(void *s, int c, uint32_t n);

/** Malloc without HEAP
 *  @param[in] size     length of the buffer
 *  @return             pointer to memory
 */ 
void *malloc (size_t size); 

/**
 * Compares the memory of two buffers and determines
 * @param[in] s1        Pointer to buffer to compare
 * @param[in] s2        Another pointer to a different buffer
 * @param[in] len       Number of bytes to compare
 * @return              Result of comparison
 **/
int memcmp(void *s1, void *s2, int len);

/** 
 * Copies memory from one buffer to another
 * @param[in] dest      Destination where data should go.
 * @param[in] src       Location of data
 * @param[in] n         Number of bytes to copy.
 **/ 
void memcpy(void *dest, void *src, size_t n);

/**
 * Gets the string length of the file 
 * @param[in] str       string to calculate length
 * @return              intetger containing the length
 **/
uint8_t strlen(unsigned char *str);
char* strcat(char* destination, const char* source, size_t n);
int division (int num1, int num2); 
int modulo(int num1, int num2);
int atoi(char *str);

/**
 * Returns a number (based on the seed)
 * @return  integer
 **/
int rand(void);

/**
 * Sets the seed to generate the random number
 *
 * @param[in] seed      Sets the seed
 **/ 
void srand(unsigned int seed); 

#endif
