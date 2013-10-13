#include "ntoa.h"

#include "strreverse.h"

// free bsd license, public domain

static const char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";

char* ntoa_int16(int16_t value, char* str, int base)
{
	char* wstr = str;
	
	if (base<2 || base>35){ *wstr='\0'; return wstr; }

	// Take care of sign
	uint16_t n = (value < 0) ? (-(value + 1) + 1) : value;
	
	// Conversion. Number is reversed.
	do {
		uint16_t tmp = n / base;
		uint16_t r = n % base;
		n = tmp;
		*wstr++ = num[r];
	}while (n);
	if (value < 0) *wstr++ = '-';
	*wstr = '\0';

	// Reverse string
	strreverse(str, wstr-1);
	return wstr;
}

char* ntoa_int32(int32_t value, char* str, int base)
{
	char* wstr = str;
	
	if (base<2 || base>35){ *wstr='\0'; return wstr; }

	// Take care of sign
	uint32_t n = (value < 0) ? (-(value + 1) + 1) : value;
	
	// Conversion. Number is reversed.
	do {
		uint32_t tmp = n / base;
		uint32_t r = n % base;
		n = tmp;
		*wstr++ = num[r];
	}while (n);
	if (value < 0) *wstr++ = '-';
	*wstr = '\0';

	// Reverse string
	strreverse(str, wstr-1);
	return wstr;
}


wchar_t* ntow_int32(int32_t value, wchar_t* str, int base)
{
	wchar_t* wstr = str;
	
	if (base<2 || base>35){ *wstr='\0'; return wstr; }

	// Take care of sign
	uint32_t n = (value < 0) ? (-(value + 1) + 1) : value;
	
	// Conversion. Number is reversed.
	do {
		uint32_t tmp = n / base;
		uint32_t r = n % base;
		n = tmp;
		*wstr++ = num[r];
	}while (n);
	if (value < 0) *wstr++ = '-';
	*wstr = '\0';
	
	// Reverse string
	wstrreverse(str, wstr-1);
	return wstr;
}

