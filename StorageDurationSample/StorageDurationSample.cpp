#include <iostream>
#include <ostream>
#include <sstream>
#include <thread>
#include <memory>
#include <cstddef>
#include "SomeClass.h"
#include "../pchar.h"

using namespace std;

struct SomeStruct
{
	int Value;
};

namespace Value
{
	// Visual C++ does not support thread_local as of VS 2012 RS. We can 
	// partially mimic thread_local with _declspec(thread) but we cannot have 
	// things like as classes with functions (including constructors and 
	// destructors) with _declspec(thread).
	_declspec(thread) SomeStruct ThreadLocalSomeStruct = {};

	// g_staticSomeClass has static duration. It exists until the program 
	// ends or until a different value is assigned to it. Even if you left 
	// off the static keyword, in this case it would still be static since 
	// it is not a local variable, is not dynamic, and is not a thread local 
	// variable.
	static SomeClass g_staticSomeClass = SomeClass(20, L"g_staticSomeClass");
}

// This creates a SomeClass instance then changes the value of the 
void ChangeAndPrintValue(int value)
{
	// Create an identifier string.
	wstringstream wsStr(L"");
	wsStr << L"ChangeAndPrintValue thread id: '" << this_thread::get_id()
		<< L"'";
	// Create a SomeClass instance to demonstrate function-level block scope.
	SomeClass sc(value, wsStr.str().c_str());

	// Demonstrate _declspec(thread).
	wcout << L"Old value is " << Value::ThreadLocalSomeStruct.Value <<
		L". Thread id: '" << this_thread::get_id() << L"'." << endl;
	Value::ThreadLocalSomeStruct.Value = value;
	wcout << L"New value is " << Value::ThreadLocalSomeStruct.Value <<
		L". Thread id: '" << this_thread::get_id() << L"'." << endl;
}

void LocalStatic(int value)
{
	static SomeClass sc(value, L"LocalStatic sc");

	//// If you wanted to reinitialize sc every time you would have to 
	//// uncomment the following line. This would defeat the purpose of 
	//// having a local static though. But you could do something similar 
	//// if you only wanted to reinitialize it in certain circumstances 
	//// since that would make having a local static make sense.
	//sc = SomeClass(value, L"LocalStatic reinitialize");

	wcout << L"Local Static sc value: '" << sc.GetValue() <<
		L"'." << endl << endl;
}

int _pmain(int /*argc*/, _pchar* /*argv*/[])
{
	// Automatic storage; destroyed when this function ends.
	SomeClass sc1(1, L"_pmain sc1");
	wcout << L"sc1 value: '" << sc1.GetValue() <<
		L"'." << endl << endl;
	{
		// The braces here create a new block. This means that 
		// sc2 only survives until the matching closing brace since 
		// it also has automatic storage.
		SomeClass sc2(2, L"_pmain sc2");
		wcout << L"sc2 value: '" << sc2.GetValue() <<
			L"'." << endl << endl;
	}

	LocalStatic(1000);
	// Note: The local static in LocalStatic will not be reinitialized
	// with 5000. See the function definition for more info.
	LocalStatic(5000);

	// To demonstrate _declspec(thread) we change the value of this
	// thread's Value::ThreadLocalSomeStruct to 20 from its default 0.
	ChangeAndPrintValue(20);

	// We now create a new thread (which automatically starts), which 
	// changes the value of Value::ThreadLocalSomeStruct to 40. If it 
	// were shared between threads then it would be 20 from the 
	// previous call to ChangeAndPrintValue. But it's not. Instead it 
	// is the default 0 that we would expect as a result of this being 
	// a new thread.
	auto thr = thread(ChangeAndPrintValue, 40);

	// Wait for the thread we just created to finish executing. Note that 
	// calling join from a UI thread is a bad idea since it blocks 
	// the current thread from running until the thread we are calling 
	// join on completes. For WinRT programming you want to make use 
	// of the PPLTasks API instead.
	thr.join();

	// Dynamic storage. WARNING: This is a 'naked' pointer, which is a VERY 
	// BAD PRACTICE. It is here only to clarify dynamic storage and so 
	// that you will see an example of it. Normally you should use either 
	// std::unique_ptr or std::shared_ptr to wrap any memory allocated with 
	// the 'new' keyword or the 'new[]' keyword.
	SomeClass* p_dsc = new SomeClass(1000, L"_pmain p_dsc");

	const std::size_t arrIntSize = 5;

	// Dynamic storage array. THE SAME WARNING APPLIES.
	int* p_arrInt = new int[arrIntSize];

	// Note that there's no way to find out how many elements arrInt
	// has other than to manually track it. Note also that the values in
	// arrInt are not initialized (i.e. it's not arrIntSize zeroes, it's
	// arrIntSize arbitrary integer values).

	for (int i = 0; i < arrIntSize; i++)
	{
		wcout << L"i: '" << i << L"'. p_arrInt[i] = '" <<
			p_arrInt[i] << L"'." << endl;

		// Assign a value of i to this index.
		p_arrInt[i] = i;
	}

	wcout << endl;

	//// If you wanted to zero out your dynamic array, you could do this:
	//uninitialized_fill_n(p_arrInt, arrIntSize, 0);

	for (int i = 0; i < arrIntSize; i++)
	{
		wcout << L"i: '" << i << L"'. p_arrInt[i] = '" <<
			p_arrInt[i] << L"'." << endl;
	}

	// If you forgot this you would have a memory leak.
	delete p_dsc;

	//// If you uncommented this then you would have a double delete, which 
	//// would crash your program.
	//delete p_dsc;

	//// If you did this you would have a program error which may or may not 
	//// crash your program. Since dsc is not an array, it should not use the 
	//// array delete, i.e. delete[], but should the non-array delete shown 
	//// above.
	//delete[] p_dsc;

	// You should always set a pointer to nullptr after deleting it to prevent 
	// any accidental use of it (since what it points to is unknown at this 
	// point).
	p_dsc = nullptr;

	// If you forgot this you would have a memory leak. If you used 
	// 'delete' instead of 'delete[]' unknown bad things might happen. Some 
	// implementations will overlook it while others would crash or do who 
	// knows what else.
	delete[] p_arrInt;
	p_arrInt = nullptr;

	wcout << L"Ending program." << endl;
	return 0;
}