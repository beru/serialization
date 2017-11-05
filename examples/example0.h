#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>

enum TestEnum {
  TestEnum_a,
  TestEnum_b,
  TestEnum_c,
};

struct Pointyo {
  int ekkus;
  int wahhi;
};

struct ChildStruct {
  int8_t s1;
  int16_t s2;
  int32_t s4;
  int64_t s8;
  uint8_t u1;
  uint16_t u2;
  uint32_t u4;
  uint64_t u8;
  TestEnum e;
  bool b;
  std::string str;
  std::wstring wstr;
  std::vector<Pointyo> pts;
  std::array<Pointyo, 8> pts2;
  std::list<Pointyo> pts3;
  std::map<int, Pointyo> pts4;
  std::map<std::string, Pointyo> pts5;
};

struct TestStruct {
  int a;
  float f;
  double d;
  TestEnum e;
  bool b;
  std::string str;
  std::wstring wstr;
  ChildStruct s;
};

