#pragma once

#include <stdint.h>

// return value is address of 0 terminator position
char* ntoa_int16(int16_t value, char* str, int base);
char* ntoa_int32(int32_t value, char* str, int base);
char* ntoa_int64(int32_t value, char* str, int base);

wchar_t* ntow_int32(int32_t value, wchar_t* str, int base);

