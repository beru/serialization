#pragma once

#include <stdio.h>
#include <type_traits>
#include <string>
#include <stdint.h>

#include "toString.h"

namespace BinaryBinding {

struct File
{
	template <typename T>
	void read(T& v) {
		fread(&v, sizeof(v), 1, f_);
	}
	
	template <typename T>
	void write(const T& v) {
		fwrite(&v, sizeof(v), 1, f_);
	}
	FILE* f_;
};

template <typename DerivedT>
struct Writer : public File
{
	static const bool isWriter = true;
	static const bool isReader = false;
	
	template <typename T>
	void bindInDerived(const T& v) {
		static_cast<DerivedT*>(this)->bind(
			const_cast<T&>(v)
		);
	}

	template <typename T>
	void bind(const T* v, size_t count) {
		for (size_t i=0; i<count; ++i) {
			bindInDerived(v[i]);
		}
	}
	
	template <typename T>
	void bind_checkClass(const T& v, std::false_type isClassType) {
		write(v);
	}

	template <typename T>
	void bind_checkClass(const T& v, std::true_type isClassType) {
		bindInDerived(v);
	}

	template <typename T>
	void bind_checkPointer(const T& v, std::true_type isPointerType) {
		// pointer's validity must be recorded
		uint8_t pValid = v ? 1 : 0;
		write(pValid);
		// do not record additional info if pointer is null
		if (pValid) {
			bindInDerived(*v);
		}
	}
	
	template <typename T>
	void bind_checkPointer(const T& v, std::false_type isPointerType) {
		bind_checkClass(v, std::is_class<T>());
	}
	
	template <typename T, std::size_t N>
	void bind_checkArray(const T (&arr)[N], std::true_type isArrayType) {
		bind(arr, N);
	}

	template <typename T>
	void bind_checkArray(const T& v, std::false_type isArrayType) {
		bind_checkPointer(v, std::is_pointer<T>());
	}

	// generic
	template <typename T>
	void bind(const T& v, const char* name) {
		bind_checkArray(v, std::is_array<T>());
	}

	// generic
	template <typename T>
	void bind(const T& v, size_t count, const char* name) {
		bind(v, count);
	}

};

template <typename DerivedT>
struct Reader : public File
{
	static const bool isWriter = false;
	static const bool isReader = true;
	
	template <typename T>
	void bindInDerived(T& v) {
		static_cast<DerivedT*>(this)->bind(v);
	}

	template <typename T>
	void bind(T* v, size_t count) {
		for (size_t i=0; i<count; ++i) {
			bindInDerived(v[i]);
		}
	}
	
	template <typename T>
	void bind_checkClass(T& v, std::false_type isClassType) {
		read(v);
	}

	template <typename T>
	void bind_checkClass(T& v, std::true_type isClassType) {
		bindInDerived(v);
	}

	template <typename T>
	void bind_checkPointer(T*& v, std::true_type isPointerType) {
		uint8_t pValid;
		read(pValid);
		if (pValid) {
			v = new T();
			bindInDerived(*v);
		}
	}
	
	template <typename T>
	void bind_checkPointer(T& v, std::false_type isPointerType) {
		bind_checkClass(v, std::is_class<T>());
	}
	
	template <typename T, std::size_t N>
	void bind_checkArray(T (&arr)[N], std::true_type isArrayType) {
		bind(arr, N);
	}

	template <typename T>
	void bind_checkArray(T& v, std::false_type isArrayType) {
		bind_checkPointer(v, std::is_pointer<T>());
	}

	// generic
	template <typename T>
	void bind(T& v, const char* name) {
		bind_checkArray(v, std::is_array<T>());
	}

	template <typename T>
	void bind(T& v, size_t count, const char* name) {
		bind(v, count);
	}	

};

template <typename DerivedT>
struct Printer
{
	Printer()
		:
		depth_(0)
	{
	}

	size_t depth_;
	void indent()
	{
		for (size_t i=0; i<depth_; ++i) {
			fputc('\t', f_);
		}
	}
	void nl()
	{
		fputs("\r\n", f_);;
	}
	
	FILE* f_;
	char buff_[1024];
	
	template <typename T>
	void bindInDerived(const T& v) {
		static_cast<DerivedT*>(this)->bind(
			const_cast<T&>(v)
		);
	}

	template <typename T>
	void bind(const T* v, size_t count) {
		++depth_;
		nl();
		for (size_t i=0; i<count; ++i) {
			indent();
			fprintf(f_, "[%d]", i);
			nl();
			++depth_;
			bindInDerived(v[i]);
			--depth_;
		}
		--depth_;
	}
	
	template <typename T>
	void bind_checkClass(const T& v, std::false_type isClassType) {
		fprintf(f_, "%s", toString(buff_, v));
		nl();
	}

	template <typename T>
	void bind_checkClass(const T& v, std::true_type isClassType) {
		++depth_;
		nl();
		bindInDerived(v);
		--depth_;
	}

	template <typename T>
	void bind_checkPointer(const T& v, std::true_type isPointerType) {
		if (v) {
			bindInDerived(*v);
		}else {
			indent();
			fprintf(f_, "nullptr");
			nl();
		}
	}
	
	template <typename T>
	void bind_checkPointer(const T& v, std::false_type isPointerType) {
		bind_checkClass(v, std::is_class<T>());
	}
	
	template <typename T, std::size_t N>
	void bind_checkArray(const T (&arr)[N], std::true_type isArrayType) {
		bind(arr, N);
	}

	template <typename T>
	void bind_checkArray(const T& v, std::false_type isArrayType) {
		bind_checkPointer(v, std::is_pointer<T>());
	}

	// generic
	template <typename T>
	void bind(const T& v, const char* name) {
		indent();
		fprintf(f_, "%s : ", name);
		bind_checkArray(v, std::is_array<T>());
	}

	// generic
	template <typename T>
	void bind(const T& v, size_t count, const char* name) {
		indent();
		fprintf(f_, "%s : ", name);
		bind(v, count);
	}

};

template <
	template <
		template <typename DerivedT>
		class BaseT
	>
	class ConnectorT,
	typename T
>
bool Save(const TCHAR* path, const T& data)
{

	ConnectorT<Writer> writer;
	writer.f_ = _tfopen(path, _T("wb"));
	if (!writer.f_) {
		return false;
	}
	writer.bind(const_cast<T&>(data));
	fclose(writer.f_);
	
	return true;
}

template <
	template <
		template <typename DerivedT>
		class BaseT
	>
	class ConnectorT,
	typename T
>
bool Load(const TCHAR* path, T& data)
{
	ConnectorT<Reader> reader;
	reader.f_ = _tfopen(path, _T("rb"));
	if (!reader.f_) {
		return false;
	}
	bool bFailed = false;
	try {
		reader.bind(data);
	}catch (...) {
		bFailed = true;
	}
	fclose(reader.f_);
	return !bFailed;
}

template <
	template <
		template <typename DerivedT>
		class BaseT
	>
	class ConnectorT,
	typename T
>
bool Print(const TCHAR* path, const T& data)
{
	ConnectorT<Printer> printer;
	printer.f_ = _tfopen(path, _T("wb"));
	if (!printer.f_) {
		return false;
	}
	bool bFailed = false;
	try {
		printer.bind(const_cast<T&>(data));
	}catch (...) {
		bFailed = true;
	}
	fclose(printer.f_);
	return !bFailed;
}

} // namespace BinaryBinding

