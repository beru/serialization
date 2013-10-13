#pragma once

#include "ntoa.h"

const char* toString(char* str, uint8_t v)
{
	char* zp = ntoa_int16(v, str, 10);
	*zp = 0;
	return str;
}

const char* toString(char* str, int8_t v)
{
	char* zp = ntoa_int16(v, str, 10);
	*zp = 0;
	return str;
}

const char* toString(char* str, uint16_t v)
{
	char* zp = ntoa_int32(v, str, 10);
	*zp = 0;
	return str;
}

const char* toString(char* str, int16_t v)
{
	char* zp = ntoa_int32(v, str, 10);
	*zp = 0;
	return str;
}

const char* toString(char* str, uint32_t v)
{
	char* zp = ntoa_int32(v, str, 10);
	*zp = 0;
	return str;
}

const char* toString(char* str, int32_t v)
{
	char* zp = ntoa_int32(v, str, 10);
	*zp = 0;
	return str;
}

const char* toString(char* str, bool v)
{
	return v ? "true" : "false";
}

const char* toString(char* str, float v)
{
	sprintf(str, "%f", v);
	return str;
}

const char* toString(char* str, double v)
{
	sprintf(str, "%f", v);
	return str;
}
