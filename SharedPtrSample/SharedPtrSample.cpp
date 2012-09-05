#include <memory>
#include <iostream>
#include <ostream>
#include "../pchar.h"

using namespace std;

struct TwoInts
{
	TwoInts(void) : A(), B() { }
	TwoInts(int a, int b) : A(a), B(b) { }

	int A;
	int B;
};

wostream& operator<<(wostream& stream, TwoInts* v)
{
	stream << v->A << L" " << v->B;
	return stream;
}

int _pmain(int /*argc*/, _pchar* /*argv*/[])
{
	//// Bad: results in double free.
	//try
	//{
	//	TwoInts* p_i = new TwoInts(10, 20);

	//	auto sp1 = shared_ptr<TwoInts>(p_i);
	//	auto sp2 = shared_ptr<TwoInts>(p_i);
	//	p_i = nullptr;

	//	wcout << L"sp1 count is " << sp1.use_count() << L"." << endl <<
	//		L"sp2 count is " << sp2.use_count() << L"." << endl;
	//}
	//catch(exception& e)
	//{
	//	wcout << L"There was an exception." << endl;
	//	wcout << e.what() << endl << endl;
	//}
	//catch(...)
	//{
	//	wcout << L"There was an exception due to a double free " <<
	//		L"because we tried freeing p_i twice!" << endl;
	//}

	// This is one right way to create shared_ptrs.
	{
		auto sp1 = shared_ptr<TwoInts>(new TwoInts(10, 20));
		auto sp2 = shared_ptr<TwoInts>(sp1);

		wcout << L"sp1 count is " << sp1.use_count() << L"." << endl <<
			L"sp2 count is " << sp2.use_count() << L"." << endl;

		wcout << L"sp1 value is " << sp1 << L"." << endl <<
			L"sp2 value is " << sp2 << L"." << endl;
	}

	// This is another right way. The std::make_shared function takes the 
	// type as its template argument and then the argument value(s) to the 
	// constructor you want as its parameters and automatically constructs 
	// the object for you. This is usually more memory efficient as the 
	// reference count can be stored right with the shared_ptr's pointed to 
	// object at the time of the object's creation.
	{
		auto sp1 = make_shared<TwoInts>(10, 20);
		auto sp2 = shared_ptr<TwoInts>(sp1);

		wcout << L"sp1 count is " << sp1.use_count() << L"." << endl <<
			L"sp2 count is " << sp2.use_count() << L"." << endl;

		wcout << L"sp1 value is " << sp1 << L"." << endl <<
			L"sp2 value is " << sp2 << L"." << endl;
	}
	
	return 0;
}