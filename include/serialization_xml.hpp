#pragma once

#include "rapidxml.hpp"

namespace serialization {
namespace impl {

template <typename T>
bool save_xml(std::istream& is, const T& value) {
  ;
}

template <typename T>
bool load_xml(std::istream& is, T& value) {
  ;
}

} // namespace impl
} // namespace serialization

