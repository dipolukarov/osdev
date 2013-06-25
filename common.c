/**
 * common.c
 * @version $Id$
 */

#include "common.h"

// write byte to port
void outb(u16int port, u8int value)
{
	asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

// get byte from port
u8int inb(u16int port)
{
	u8int ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

// get word from port
u16int inw(u16int port)
{
	u16int ret;
	asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

// Copy len bytes from src to dest.
void memcpy(u8int *dest, const u8int *src, u32int len)
{
	const u8int *sp = (const u8int *)src;
	u8int *dp = (u8int *)dest;
	for ( ; len != 0; len-- )
		*dp++ = *sp++;
}

// Write len copies of val  into dest.
void memset(u8int *dest, u8int val, u32int len)
{
	u8int *temp = (u8int *)dest;
	for ( ; len != 0; len--)
		*temp++ = val;
}

// Compare two strings. Should return -1 if
// str1 < str2, 0 if they equal or 1 otherwise.
int strcmp(char *str1, char *str2)
{
	int i = 0;
	int failed = 0;
	while (str1[i] != '\0' && str2[i] != '\0') {
		if (str1[i] != str2[i]) {
			failed = 1;
			break;
		}
		i++;
	}
	// why did the loop exit ?
	if ( (str1[i] == '\0' && str2[i] == '\0') || (str1[i] != '\0' && str2[i] != '\0') )
		failed = 1;

	return failed;
}

// Copy the null-terminated string src into dest, and
// return dest.
char *strcpy(char *dest, const char *src)
{
	do {
		*dest++ = *src++;
	} while (*src != 0);
}

// Concatenate the null-terminated string src onto
// the end of dest, and return dest.
char *strcat(char *dest, const char *src)
{
	while (*dest != 0)
		*dest = *dest++;

	do
		*dest++ = *src++;
	while (*src != 0);

	return dest;
}
