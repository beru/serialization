
#include <stdio.h>
#include <limits>
#include <assert.h>

#include "example0.h"
#include "../include/serialization.h"

VISITABLE_STRUCT(Pointyo, ekkus, wahhi);
VISITABLE_STRUCT(ChildStruct, s1, s2, s4, s8, u1, u2, u4, u8, e, b, str, wstr, arr0, pts, pts2, pts3, pts4, pts5);
VISITABLE_STRUCT(TestStruct, a, f, d, e, b, str, wstr, s);

struct eq_visitor {
  bool result = true;
  template <typename T>
  void operator()(const char *, const T & t1, const T & t2) {
    result = result && (t1 == t2);
  }
};
template <typename T>
bool struct_eq(const T & t1, const T & t2) {
  eq_visitor vis;
  visit_struct::for_each(t1, t2, vis);
  return vis.result;
}

#define IMPL_EQOP(T) inline bool operator == (const T& lhs, const T& rhs) { return struct_eq(lhs, rhs); }
IMPL_EQOP(Pointyo);
IMPL_EQOP(ChildStruct);
IMPL_EQOP(TestStruct);

void init(TestStruct& s)
{
  s.a = 54321;
  s.f = 1.3142f;
  s.d = 0.9876;
  s.e = TestEnum_b;
  s.b = true;
  s.str = "abcdeZ";
  s.wstr = L"‚ ‚¢‚¤‚¦‚¨";
  auto& c = s.s;
  c.s1 = std::numeric_limits<int8_t>::min();
  c.s2 = std::numeric_limits<int16_t>::min();
  c.s4 = std::numeric_limits<int32_t>::min();
  c.s8 = std::numeric_limits<int64_t>::min();
  c.u1 = std::numeric_limits<uint8_t>::max();
  c.u2 = std::numeric_limits<uint16_t>::max();
  c.u4 = std::numeric_limits<uint32_t>::max();
  c.u8 = std::numeric_limits<uint64_t>::max();
  c.e = TestEnum_c;
  c.b = true;
  c.str = "ABCDEz";
  c.wstr = L"‚Ÿ‚¡‚£‚¥‚§";
  c.arr0 = {0, 1, 2};
  c.pts.resize(5);
  for (auto& p : c.pts) {
    p.ekkus = 10;
    p.wahhi = 20;
  }
  for (auto& p : c.pts2) {
    p.ekkus = 11;
    p.wahhi = 21;
  }
  for (size_t i=0; i<7; ++i) {
    c.pts3.emplace_back(Pointyo{12, 22});
  }
  c.pts4[3] = Pointyo{13, 23};
  c.pts4[4] = Pointyo{14, 24};
  c.pts5["san"] = Pointyo{113, 123};
  c.pts5["yon"] = Pointyo{114, 124};
}

int main(int argc, char* argv[])
{
  TestStruct s1, s2;
  init(s1);
  
  const char* filename;
  
#ifdef USE_BINARY_ARCHIVE
  filename = "example0.bin";
  serialization::serialize(filename, s1);
  serialization::deserialize(filename, s2);
  assert(s1 == s2);
#endif

#ifdef USE_JSON_ARCHIVE
  filename = "example0.json";
  serialization::serialize(filename, s1, serialization::archive_format_json);
  serialization::deserialize(filename, s2, serialization::archive_format_json);
  assert(s1 == s2);
#endif

#ifdef USE_XML_ARCHIVE
  filename = "example0.xml";
  serialization::serialize(filename, s1, serialization::archive_format_xml);
  serialization::deserialize(filename, s2, serialization::archive_format_xml);
  assert(s1 == s2);
#endif

#ifdef USE_INI_ARCHIVE
  filename = "example0.ini";
  serialization::serialize(filename, s1, serialization::archive_format_ini);
  serialization::deserialize(filename, s2, serialization::archive_format_ini);
  assert(s1 == s2);
#endif

  return 0;
}

