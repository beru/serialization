#pragma once

#include <stdint.h>
#include <tchar.h>

struct POINT
{
	int32_t x;
	int32_t y;
};

struct RECT
{
	int32_t left;
	int32_t top;
	int32_t right;
	int32_t bottom;
};

struct Data
{
	POINT pt;
	RECT rec;
};

struct Sample
{
	POINT* pP;
	POINT pts[10];
	POINT a;
	RECT b;

	uint32_t arrLen;
	RECT* arr;
	int c;
	Data d;

	float f;
	double dbl;
};

bool Save(const TCHAR* path, const Sample& data);
bool Load(const TCHAR* path, Sample& data);

