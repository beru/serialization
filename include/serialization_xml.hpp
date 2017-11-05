#pragma once

namespace serialization {
namespace impl {

template <typename T, typename std::enable_if<!visit_struct::traits::is_visitable<T>::value>::type* = nullptr>
const char* get_name() {
  // I can has RTTI? A_A
  //                  oo
  //                   3	
  return typeid(T).name();
}
template <typename T, typename std::enable_if<visit_struct::traits::is_visitable<T>::value>::type* = nullptr>
const char* get_name() {
  return visit_struct::get_name<T>();
}

inline void escape_print(std::ostream& s, const char* str, size_t len = 0) {
  if (!len) len = strlen(str);
  for (size_t i=0; i<len; ++i) {
    char c = str[i];
    switch (c) {
    case '\"': s << "&quot;"; break;
    case '\'': s << "&apos;"; break;
    case '<': s << "&lt;"; break;
    case '>': s << "&gt;"; break;
    case '&': s << "&amp;"; break;
    default: s << c; break;
    }
  }
}

template <typename T>
struct tag_printer {
  tag_printer(std::ostream& s, const char* name, size_t depth = 0) : s(s), name(name), depth(depth) {
    indent();
    s << "<" << name << " type=\"";
    escape_print(s, get_name<T>());
    s << "\">";
    if (depth) {
      s << std::endl;
    }
  }
  ~tag_printer() {
    indent();
    s << "</" << name << ">";
    if (depth) {
      s << std::endl;
    }
  }
  void indent() {
    for (size_t i=0; i<depth; ++i) {
      s << "  ";
    }
  }
  const char* name;
  std::ostream& s;
  size_t depth;
};


struct save_xml_visitor {
  save_xml_visitor(std::ostream& s, size_t depth) : s(s), depth(depth) {}
  std::ostream& s;
  size_t depth;
  
  void indent() {
    for (size_t i=0; i<depth; ++i) {
      s << "  ";
    }
  }

  void operator()(const char* name, const int8_t& t) {
    indent();
    {
      tag_printer<int8_t> tag(s, name);
      s << signed(t);
    }
    s << std::endl;
  }
  void operator()(const char* name, const uint8_t& t) {
    indent();
    {
      tag_printer<uint8_t> tag(s, name);
      s << unsigned(t);
    }
    s << std::endl;
  }
  template <typename T, typename std::enable_if<!visit_struct::traits::is_visitable<T>::value>::type* = nullptr>
  void operator()(const char* name, const T& t) {
    indent();
    {
      tag_printer<T> tag(s, name);
      s << t;
    }
    s << std::endl;
  }
  template <typename T, typename std::enable_if<visit_struct::traits::is_visitable<T>::value>::type* = nullptr>
  void operator()(const char* name, const T& t) {
    tag_printer<T> tag(s, name, depth);
    save_xml_visitor v(s, depth + 1);
    visit_struct::apply_visitor(v, t);
  }
  template <typename T, typename std::enable_if<!visit_struct::traits::is_visitable<T>::value>::type* = nullptr>
  void operator()(const T& t) {
    indent();
    {
      tag_printer<T> tag(s, "entry");
      s << t;
    }
    s << std::endl;
  }
  template <typename T, typename std::enable_if<visit_struct::traits::is_visitable<T>::value>::type* = nullptr>
  void operator()(const T& t) {
    tag_printer<T> tag(s, "entry", depth);
    save_xml_visitor v(s, depth + 1);
    visit_struct::apply_visitor(v, t);
  }
  void operator()(const char* name, const std::string& t) {
    indent();
    {
      tag_printer<std::string> tag(s, name);
      escape_print(s, t.c_str(), t.size());
    }
    s << std::endl;
  }
  void operator()(const char* name, const std::wstring& t) {
    indent();
    {
      tag_printer<std::wstring> tag(s, name);
      //s << t;
    }
    s << std::endl;
  }
  template <typename T>
  void print_list_container(const char* name, const T& t) {
    tag_printer<T> tag(s, name, depth);
    save_xml_visitor v(s, depth + 1);
    for (const auto& e : t) {
      v(e);
    }
  }
  template <typename T>
  void operator()(const char* name, const std::vector<T> & t) {
    print_list_container(name, t);
  }
  template <typename T, size_t CNT> 
  void operator()(const char* name, const std::array<T, CNT> & t) {
    print_list_container(name, t);
  }
  template <typename T>
  void operator()(const char* name, const std::list<T> & t) {
    print_list_container(name, t);
  }
  template <typename KeyT, typename ValueT>
  void print_map_container(const char* name, const std::map<KeyT, ValueT>& t) {
    tag_printer<std::map<KeyT, ValueT>> tag(s, name, depth);
    save_xml_visitor v0(s, depth + 2);
    save_xml_visitor v1(s, depth + 3);
    for (const auto& e : t) {
      tag_printer<std::pair<KeyT, ValueT>> entry(s, "entry", depth + 1);
      v0("first", e.first);
      tag_printer<ValueT> value(s, "second", depth + 2);
      v1(get_name<ValueT>(), e.second);
    }
  }
  template <typename T>
  void operator()(const char* name, const std::map<int, T> & t) {
    print_map_container(name, t);
  }
  template <typename T>
  void operator()(const char* name, const std::map<std::string, T> & t) {
    print_map_container(name, t);
  }
};

template <typename T>
void save_xml(std::ostream& os, const T& value) {
  os << R"(<?xml version="1.0" standalone="yes"?>)" << std::endl;
  {
    tag_printer<T> root(os, "root");
    os << std::endl;
    save_xml_visitor v(os, 1);
    visit_struct::apply_visitor(v, value);
  }
  os << std::endl;
}

template <typename T>
void load_xml(std::istream& is, T& value) {
}

} // namespace impl
} // namespace serialization

