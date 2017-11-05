#pragma once

#include <iostream>

#define USE_BINARY_ARCHIVE
#define USE_JSON_ARCHIVE
#define USE_XML_ARCHIVE
//#define USE_INI_ARCHIVE

namespace serialization {

enum archive_format {
#ifdef USE_BINARY_ARCHIVE
  archive_format_binary,
#endif
#ifdef USE_JSON_ARCHIVE
  archive_format_json,
#endif
#ifdef USE_XML_ARCHIVE
  archive_format_xml,
#endif
#ifdef USE_INI_ARCHIVE
  archive_format_ini,
#endif

  //archive_format_default = archive_format_binary,
  archive_format_default = archive_format_xml,
};

template <typename T>
void serialize(std::ostream& os, const T& value, archive_format format = archive_format_default);
template <typename T>
void serialize(const char* file_path, const T& value, archive_format format = archive_format_default);

template <typename T>
void deserialize(std::istream& is, T& value, archive_format format = archive_format_default);
template <typename T>
void deserialize(const char* file_path, T& value, archive_format format = archive_format_default);

} // namespace serialization

#include "serialization.hpp"
