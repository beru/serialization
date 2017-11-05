#pragma once

namespace serialization {
namespace impl {


template <typename T>
void save_xml(std::ostream& os, const T& value) {
}

template <typename T>
void load_xml(std::istream& is, T& value) {
}

} // namespace impl
} // namespace serialization

