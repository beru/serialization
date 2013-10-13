
#include "strreverse.h"

#include <ctype.h>
#include "ntoa.h"

/**
 * Ansi C "itoa" based on Kernighan & Ritchie's "Ansi C"
 */
void strreverse(char* begin, char* end) {
	char aux;
	while(end>begin)
		aux=*end, *end--=*begin, *begin++=aux;
}

void wstrreverse(wchar_t* begin, wchar_t* end) {
	wchar_t aux;
	while(end>begin)
		aux=*end, *end--=*begin, *begin++=aux;
}

