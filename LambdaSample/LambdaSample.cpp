#include <iostream>
#include <ostream>
#include "../pchar.h"

using namespace std;

int _pmain(int /*argc*/, _pchar* /*argv*/[])
{
	// Create a lambda expression closure.
	auto lm1 = []()
	{
		wcout << L"No capture, parameterless lambda." << endl;
	};

	// Invoke the lambda.
	lm1();

	// Create a lambda closure with parameters.
	auto lm2 = [](int a, int b)
	{
		wcout << a << L" + " << b << " = " << (a + b) << endl;
	};

	lm2(3,4);

	// Create a lambda closure with a trailing return type
	auto lm3 = [](int a, int b) -> int
	{
		wcout << a << L" % " << b << " = ";
		return a % b;
	};

	wcout << lm3(7, 5) << endl;

	int a = 5;
	int b = 6;
	// Capture by copy all variables that are in scope currently. 
	// Note also that we do not need to capture the closure; 
	// here we simply invoke the anonymous lambda with the 
	// () after the closing brace.
	[=]()
	{
		wcout << a << L" + " << b << " = " << (a + b) << endl;
		//// It's illegal to modify a here because we have 
		//// captured by value and have not specified that 
		//// this lambda should be treated as mutable.
		//a = 10;
	}();

	[=]() mutable -> void
	{
		wcout << a << L" + " << b << " = " << (a + b) << endl;
		// By marking this lambda as mutable we can now modify a. 
		// But since we are capturing by value, the modifications 
		// will not propagate outside.
		a = 10;
	}();

	wcout << L"The value of a is " << a << L"." << endl;

	[&]()
	{
		wcout << a << L" + " << b << " = " << (a + b) << endl;
		// By capturing by reference we have now made it so 
		// that we do not need to mark this as mutable. 
		// Because it is a reference, though, changes now 
		// propagate out.
		a = 10;
	}();

	wcout << L"The value of a is " << a << L"." << endl;

	// Here we specify explictly that we are capturing a by 
	// value and b as a reference.
	[a,&b]()
	{
		b = 12;
		wcout << a << L" + " << b << " = " << (a + b) << endl;
	}();

	// Here we specify explictly that we are capturing b as 
	// a reference and that all other captures should be by 
	// value.
	[=,&b]()
	{
		b = 15;
		wcout << a << L" + " << b << " = " << (a + b) << endl;
	}();

	// Here we specify explictly that we are capturing a by 
	// value and that all other captures should be by 
	// reference.
	[&,a]()
	{
		b = 18;
		wcout << a << L" + " << b << " = " << (a + b) << endl;
	}();

	return 0;
}