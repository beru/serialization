
#include "sample.h"

#include "serialization/serialization.h"

// many thanks to http://stackoverflow.com/a/5530998/4699324
#define EXPAND(x) x
#define PP_NARGS(...) EXPAND(_xPP_NARGS_IMPL(__VA_ARGS__,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))
#define _xPP_NARGS_IMPL(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,N,...) N

#define _PP_0(_1, ...) _1			// (a,b,c,d) => a
#define _PP_X(_1, ...) __VA_ARGS__	// (a,b,c,d) => (b,c,d)

#define PP_JOIN(a,b) a ## b

#define PP_TRANSFORM_(f, NARG, ...) PP_JOIN(PP_TRANSFORM_, NARG)(f, __VA_ARGS__)
#define PP_TRANSFORM(f, ...) PP_TRANSFORM_(f, PP_NARGS(__VA_ARGS__), __VA_ARGS__)

#define CALL_F(f, ...) f(__VA_ARGS__)

// many thanks to http://stackoverflow.com/a/13637426/4699324
#define PP_TRANSFORM_0(f, ...)
#define PP_TRANSFORM_1(f, a) CALL_F(f, _PP_0(a)); PP_TRANSFORM_0(f, _PP_X(a))
#define PP_TRANSFORM_2(f, a) CALL_F(f, _PP_0(a)); PP_TRANSFORM_1(f, _PP_X(a))
#define PP_TRANSFORM_3(f, a) CALL_F(f, _PP_0(a)); PP_TRANSFORM_2(f, _PP_X(a))
#define PP_TRANSFORM_4(f, a) CALL_F(f, _PP_0(a)); PP_TRANSFORM_3(f, _PP_X(a))
#define PP_TRANSFORM_5(f, a) CALL_F(f, _PP_0(a)); PP_TRANSFORM_4(f, _PP_X(a))
#define PP_TRANSFORM_6(f, a) CALL_F(f, _PP_0(a)); PP_TRANSFORM_5(f, _PP_X(a))

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
	#define BIND_MULTI(...) PP_TRANSFORM(BIND, __VA_ARGS__)
	
	void bind(POINT& v) {
		BIND_MULTI(
			x,
			y
		);
	}
	
	void bind(RECT& v) {
		BIND_MULTI(
			left,
			top,
			right,
			bottom
		);
	}
	
	void bind(Data& v) {
		BIND_MULTI(
			pt,
			rec
		);
	}

	void bind(Sample& v) {
		BIND_MULTI(
			pP,
			pts,
			a,
			b,
			arrLen);
		if (v.arrLen) {
			BIND_ARRAY(arr, v.arrLen);
		}
		BIND_MULTI(
			c,
			d,
			f,
			dbl
		);
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

