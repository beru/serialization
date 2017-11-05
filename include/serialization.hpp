#pragma once

#include <fstream>

#include "visit_struct.hpp"
#include "serialization.h"

#ifdef USE_BINARY_ARCHIVE
#include "serialization_binary.hpp"
#endif

#ifdef USE_JSON_ARCHIVE
#include "serialization_json.hpp"
#endif

#ifdef USE_XML_ARCHIVE
#include "serialization_xml.hpp"
#endif

#ifdef USE_INI_ARCHIVE
#include "serialization_ini.hpp"
#endif

namespace serialization {

template <typename T>
void serialize(std::ostream& os, const T& value, archive_format format) {
  switch (format) {
#ifdef USE_BINARY_ARCHIVE
  case archive_format_binary:   impl::save_binary(os, value); break;
#endif

#ifdef USE_JSON_ARCHIVE
  case archive_format_json:     impl::save_json(os, value); break;
#endif

#ifdef USE_XML_ARCHIVE
  case archive_format_xml:      impl::save_xml(os, value); break;
#endif

#ifdef USE_INI_ARCHIVE
  case archive_format_ini:      impl::save_ini(os, value); break;
#endif

  }
}

template <typename T>
void serialize(const char* file_path, const T& value, archive_format format) {
  serialize(std::ofstream(file_path, std::ios::binary), value, format);
}

template <typename T>
void deserialize(std::istream& is, T& value, archive_format format) {
  switch (format) {
#ifdef USE_BINARY_ARCHIVE
  case archive_format_binary:   impl::load_binary(is, value); break;
#endif

#ifdef USE_JSON_ARCHIVE
  case archive_format_json:     impl::load_json(is, value); break;
#endif

#ifdef USE_XML_ARCHIVE
  case archive_format_xml:      impl::load_xml(is, value); break;
#endif

#ifdef USE_INI_ARCHIVE
  case archive_format_ini:      impl::load_ini(is, value); break;
#endif

  }
}

template <typename T>
void deserialize(const char* file_path, T& value, archive_format format) {
  deserialize(std::ifstream(file_path, std::ios::binary), value, format);
}

} // namespace serialization

