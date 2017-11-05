#pragma once

namespace serialization {
namespace impl {

template <typename DerivedT, bool saving>
struct binary_visitor_base {
private:
  template <typename T>
  void serialize(T& t) {
    static_cast<typename DerivedT*>(this)->serialize(t);
  }

  template <typename T>
  void serialize(T* values, size_t cnt) {
    for (size_t i=0; i<cnt; ++i) {
      serialize(values[i]);
    }
  }

  template <typename T, typename std::enable_if<!visit_struct::traits::is_visitable<T>::value>::type* = nullptr>
  void operator()(T& t) {
    serialize(t);
  }

  template <typename T, typename std::enable_if<visit_struct::traits::is_visitable<T>::value>::type* = nullptr>
  void operator()(T& t) {
    visit_struct::apply_visitor(*this, t);
  }

  template <typename T>
  void operator()(std::vector<T> & t) {
    size_t sz;
    if (saving) {
      sz = t.size();
    }
    serialize(sz);
    if (!saving) {
      t.resize(sz);
    }
    serialize(&t[0], sz);
  }

  template <typename T, size_t CNT>
  void operator()(std::array<T, CNT>& t) {
    serialize(&t[0], CNT);
  }

  template <typename T>
  void operator()(std::list<T>& t) {
    size_t sz;
    if (saving) {
      sz = t.size();
    }
    serialize(sz);
    if (saving) {
      for (auto& v : t) {
        serialize(v);
      }
    }else {
      for (size_t i=0; i<sz; ++i) {
        T v;
        serialize(v);
        t.emplace_back(v);
      }
    }
  }

  template <typename T>
  void operator()(std::map<int, T>& t) {
    size_t sz;
    if (saving) {
      sz = t.size();
    }
    serialize(sz);
    if (saving) {
      for (auto& v : t) {
        serialize(v.first);
        serialize(v.second);
      }
    }else {
      for (size_t i=0; i<sz; ++i) {
        int key;
        T value;
        serialize(key);
        serialize(value);
        t[key] = value;
      }
    }
  }

  template <typename T>
  void operator()(std::map<std::string, T>& t) {
    size_t sz;
    if (saving) {
      sz = t.size();
    }
    serialize(sz);
    if (saving) {
      for (auto& v : t) {
        serialize(const_cast<std::string&>(v.first));
        serialize(v.second);
      }
    }else {
      for (size_t i=0; i<sz; ++i) {
        std::string key;
        T value;
        serialize(key);
        serialize(value);
        t[key.c_str()] = value;
      }
    }
  }

  void operator()(std::string& t) {
    serialize(t);
  }
  void operator()(std::wstring& t) {
    serialize(t);
  }

public:
  template <typename T>
  void operator()(const char* name, T& t) {
    (void)name;
    this->operator()(t);
  }

};

struct save_binary_visitor : public binary_visitor_base<save_binary_visitor, true> {
  save_binary_visitor(std::ostream& os) : os(os) {}
  std::ostream& os;
  
  template <typename T>
  void serialize(const T& t) { os.write((const char*)&t, sizeof(T)); }
  void serialize(const void* data, size_t bytes) { os.write((const char*)data, bytes); }

  void serialize(const std::string& t) {
    size_t sz = t.size();
    serialize(sz);
    serialize((void*)t.c_str(), sz + 1);
  }
};

struct load_binary_visitor : public binary_visitor_base<load_binary_visitor, false> {
  load_binary_visitor(std::istream& is) : is(is) {}
  std::istream& is;
  
  template <typename T>
  void serialize(T& t) { is.read((char*)&t, sizeof(T)); }
  void serialize(void* data, size_t bytes) { is.read((char*)data, bytes); }

  void serialize(std::string& t) {
    size_t sz;
    serialize(sz);
    t.reserve(sz + 1);
    serialize((void*)&t[0], sz + 1);
  }
};

template <typename T>
void save_binary(std::ostream& os, const T& value) {
  visit_struct::apply_visitor(save_binary_visitor(os), const_cast<T&>(value));
}

template <typename T>
void load_binary(std::istream& is, T& value) {
  visit_struct::apply_visitor(load_binary_visitor(is), value);
}

} // namespace impl
} // namespace serialization

