#pragma once

#include "json.hpp"

namespace serialization {
namespace impl {

struct save_json_visitor {
  save_json_visitor(nlohmann::json& j) : j(j) {}
  nlohmann::json& j;
  
  template <typename T, typename std::enable_if<std::is_scalar<T>::value>::type* = nullptr>
  void operator()(const char* name, const T& t) {
    j[name] = t;
  }
  template <typename T, typename std::enable_if<std::is_scalar<T>::value>::type* = nullptr>
  void operator()(const T& t) {
    j.push_back(t);
  }
  template <typename T, typename std::enable_if<std::is_class<T>::value>::type* = nullptr>
  void operator()(const char* name, const T& t) {
    visit_struct::apply_visitor(save_json_visitor(j[name]), t);
  }
  template <typename T, typename std::enable_if<std::is_class<T>::value>::type* = nullptr>
  void operator()(const T& t) {
    nlohmann::json j2;
    visit_struct::apply_visitor(save_json_visitor(j2), t);
    j.push_back(j2);
  }
  void operator()(const char* name, const std::string& t) {
    j[name] = t;
  }
  void operator()(const char* name, const std::wstring& t) {
    j[name] = t;
  }
  template <typename It>
  void operator()(It begin, It end) {
    for (It it=begin; it!=end; ++it) {
      this->operator()(*it);
    }
  }
  template <typename T>
  void operator()(const char* name, const std::vector<T> & t) {
    save_json_visitor v(j[name]);
    v(t.begin(), t.end());
  }
  template <typename T, size_t CNT> 
  void operator()(const char* name, const std::array<T, CNT> & t) {
    save_json_visitor v(j[name]);
    v(t.begin(), t.end());
  }
  template <typename T>
  void operator()(const char* name, const std::list<T> & t) {
    save_json_visitor v(j[name]);
    v(t.begin(), t.end());
  }
  template <typename T>
  void operator()(const char* name, const std::map<int, T> & t) {
    auto& jj = j[name];
    save_json_visitor keys(jj["keys"]);
    save_json_visitor values(jj["values"]);
    for (const auto& p : t) {
      keys(p.first);
      values(p.second);
    }
  }
  template <typename T>
  void operator()(const char* name, const std::map<std::string, T> & t) {
    save_json_visitor v(j[name]);
    for (const auto& p : t) {
      v(p.first.c_str(), p.second);
    }
  }
};

struct load_json_visitor {
  load_json_visitor(const nlohmann::json& j) : j(j) {}
  const nlohmann::json& j;
  
  template <typename T, typename std::enable_if<std::is_scalar<T>::value>::type* = nullptr>
  void operator()(const char* name, T& t) {
    t = j[name];
  }
  template <typename T, typename std::enable_if<std::is_scalar<T>::value>::type* = nullptr>
  void operator()(T& t) {
    t = j;
  }
  template <typename T, typename std::enable_if<std::is_class<T>::value>::type* = nullptr>
  void operator()(const char* name, T& t) {
    visit_struct::apply_visitor(load_json_visitor(j[name]), t);
  }
  template <typename T, typename std::enable_if<std::is_class<T>::value>::type* = nullptr>
  void operator()(T& t) {
    visit_struct::apply_visitor(load_json_visitor(j), t);
  }
  template <typename T>
  void operator()(const char* name, std::vector<T> & t) {
    const auto& arr = j[name];
    t.resize(arr.size());
    for (size_t i=0; i<arr.size(); ++i) {
      load_json_visitor v(arr[i]);
      v(t[i]);
    }
  }
  template <typename T, size_t CNT> 
  void operator()(const char* name, std::array<T, CNT> & t) {
    const auto& arr = j[name];
    assert(arr.size() == CNT);
    for (size_t i=0; i<arr.size(); ++i) {
      load_json_visitor v(arr[i]);
      v(t[i]);
    }
  }
  template <typename T>
  void operator()(const char* name, std::list<T> & t) {
    const auto& arr = j[name];
    t.clear();
    for (size_t i=0; i<arr.size(); ++i) {
      load_json_visitor v(arr[i]);
      T e;
      v(e);
      t.emplace_back(e);
    }
  }
  template <typename T>
  void operator()(const char* name, std::map<int, T> & t) {
    const auto& jj = j[name];
    const auto& keys = jj["keys"];
    const auto& values = jj["values"];
    assert(keys.size() == values.size());
    t.clear();
    for (size_t i=0; i<keys.size(); ++i) {
      int key = keys[i];
      load_json_visitor v(values[i]);
      T e;
      v(e);
      t[key] = e;
    }
  }
  template <typename T>
  void operator()(const char* name, std::map<std::string, T> & t) {
    auto& o = j[name];
    t.clear();
    for (auto it=o.begin(); it!=o.end(); ++it) {
      load_json_visitor v(it.value());
      v(t[it.key()]);
    }
  }
  void operator()(const char* name, std::string& t) {
    t = j[name];
  }
  void operator()(const char* name, std::wstring& t) {
    auto a = j[name];
    t.clear();
    for (auto it=a.begin(); it!=a.end(); ++it) {
      wchar_t c = (wchar_t)*it;
      t.push_back(c);
    }
  }
};

template <typename T>
void save_json(std::ostream& os, const T& value) {
  nlohmann::json j;
  visit_struct::apply_visitor(save_json_visitor(j), value);
  os << j.dump(2);
}

template <typename T>
void load_json(std::istream& is, T& value) {
  nlohmann::json j;
  is >> j; 
  visit_struct::apply_visitor(load_json_visitor(j), value);
}

} // namespace impl
} // namespace serialization

