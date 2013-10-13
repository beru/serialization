
#include "sample.h"

#include "serialization/serialization.h"

template <
	template <typename DerivedT>
	class BaseT
>
struct Connector
	:
	public BaseT <
		Connector<BaseT>
	>
{
	using BaseT <Connector<BaseT>>::bind;
	#define BIND(name) bind(v.name, #name);
	#define BIND_ARRAY(name, count) bind(v.name, count, #name)

	void bind(POINT& v) {
		BIND(x);
		BIND(y);
	}
	
	void bind(RECT& v) {
		BIND(left);
		BIND(top);
		BIND(right);
		BIND(bottom);
	}
	
	void bind(Data& v) {
		BIND(pt);
		BIND(rec);
	}

	void bind(Sample& v) {
		BIND(pP);
		BIND(pts);
		BIND(a);
		BIND(b);
		BIND(arrLen);
		if (v.arrLen) {
			BIND_ARRAY(arr, v.arrLen);
		}
		BIND(c);
		BIND(d);

		BIND(f);
		BIND(dbl);
	}
	
};


bool Save(const TCHAR* path, const Sample& data)
{
	return Serialization::Save<Connector>(path, data);
}

bool Load(const TCHAR* path, Sample& data)
{
	return Serialization::Load<Connector>(path, data);
}

bool Print(const TCHAR* path, Sample& data)
{
	return Serialization::Print<Connector>(path, data);
}

int main(int argc, char* argv[])
{
	{
		Sample s = {0};
		s.arr = new RECT[256];
		s.arrLen = 10;
		s.arr[0].left = 1;
		s.arr[0].top = 2;
		s.a.x = 10;
		s.a.y = 20;
		Save(L"data.bin", s);
		delete s.arr;
	}
	{
		Sample s = {0};
		s.arr = new RECT[256];
		Load(L"data.bin", s);
		int hoge = 0;

		Print(L"data.txt", s);
		delete s.arr;
	}
	return 0;
}

