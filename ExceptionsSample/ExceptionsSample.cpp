#include <iostream>
#include <ostream>
#include <memory>
#include <exception>
#include <stdexcept>
#include <typeinfo>
#include <algorithm>
#include <cstdlib>
#include "InvalidArgumentException.h"
#include "../pchar.h"

using namespace CppForCsExceptions;
using namespace std;

class ThrowClass
{
public:
	ThrowClass(void)
		: m_shouldThrow(false)
	{
		wcout << L"Constructing ThrowClass." << endl;
	}

	explicit ThrowClass(bool shouldThrow)
		: m_shouldThrow(shouldThrow)
	{
		wcout << L"Constructing ThrowClass. shouldThrow = " <<
			(shouldThrow ? L"true." : L"false.") << endl;
		if (shouldThrow)
		{
			throw InvalidArgumentException<const char*>(
				"ThrowClass",
				"ThrowClass(bool shouldThrow)",
				"shouldThrow",
				"true"
				);
		}
	}

	~ThrowClass(void)
	{
		wcout << L"Destroying ThrowClass." << endl;
	}

	const wchar_t* GetShouldThrow(void) const
	{
		return (m_shouldThrow ? L"True" : L"False");
	}

private:
	bool		m_shouldThrow;
};

class RegularClass
{
public:
	RegularClass(void)
	{
		wcout << L"Constructing RegularClass." << endl;
	}
	~RegularClass(void)
	{
		wcout << L"Destroying RegularClass." << endl;
	}
};

class ContainStuffClass
{
public:
	ContainStuffClass(void) :
		m_regularClass(new RegularClass()),
		m_throwClass(new ThrowClass())
	{
		wcout << L"Constructing ContainStuffClass." << endl;
	}

	ContainStuffClass(const ContainStuffClass& other) :
		m_regularClass(new RegularClass(*other.m_regularClass)),
		m_throwClass(other.m_throwClass)
	{
		wcout << L"Copy constructing ContainStuffClass." << endl;
	}

	~ContainStuffClass(void)
	{
		wcout << L"Destroying ContainStuffClass." << endl;
	}

	const wchar_t* GetString(void) const
	{
		return L"I'm a ContainStuffClass.";
	}

private:

	unique_ptr<RegularClass>	m_regularClass;
	shared_ptr<ThrowClass>		m_throwClass;
};

void TerminateHandler(void)
{
	wcout << L"Terminating due to unhandled exception." << endl;

	// If you call abort (from <cstdlib>) the program will exit 
	// abnormally. It will also exit abnormally if you do not call 
	// anything to cause it to exit from this method.
	abort();

	//// If you were instead to call exit(0) (also from <cstdlib>) 
	//// then your program would instead exit as though nothing had 
	//// gone wrong. This is bad since something did go wrong. But 
	//// I present this so that you know that it is possible for 
	//// a program to throw an uncaught exception and yet still 
	//// exit in a way that isn't interpreted as a crash since 
	//// you may someday find yourself trying to find out why a 
	//// program keeps abruptly exiting yet isn't "crashing". This 
	//// would be one such cause for that. 
	//exit(0);
}

int _pmain(int /*argc*/, _pchar* /*argv*/[])
{
	// Set a custom handler for std::terminate. Note that this handler 
	// won't run unless you run this from a command prompt. The debugger 
	// will intercept the unhandled exception and will present you with 
	// debugging options when you run this from Visual Studio.
	set_terminate(&TerminateHandler);
	
	try
	{
		ContainStuffClass cSC;
		wcout << cSC.GetString() << endl;

		ThrowClass tC(false);
		wcout << L"tC should throw? " << tC.GetShouldThrow() << endl;

		tC = ThrowClass(true);
		wcout << L"tC should throw? " << tC.GetShouldThrow() << endl;
	}
	// One downside to using templates for exceptions is that you need a 
	// catch handler for each specialization you might have. Unless you 
	// have a base class they all inherit from, that is. To avoid catching 
	// other std::invalid_argument exceptions, we created an abstract 
	// class called InvalidArgumentExceptionBase which serves solely to
	// act as the base class for all specializations of 
	// InvalidArgumentException<T>. Now we can catch them all (if desired) 
	// without needing a catch handler for each. If you wanted to, you 
	// could still have a handler for a particular specialization though.
	catch (InvalidArgumentExceptionBase& e)
	{
		wcout << L"Caught '" << typeid(e).name() << L"'." << endl <<
			L"Message: " << e.what() << endl;
	}
	// Catch anything derived from std::exception that we haven't already
	// had a specialized handler. Since you don't know what this is, you
	// should catch it, log it, and rethrow it.
	catch (std::exception& e)
	{
		wcout << L"Caught '" << typeid(e).name() << L"'." << endl <<
			L"Message: " << e.what() << endl;
		// Just a plain throw statement like this is a rethrow.
		throw;
	}
	// This next catch catches everything, regardless of type. Like
	// catching System.Exception, you should only ever catch this to
	// rethrow it.
	catch (...)
	{
		wcout << L"Caught unknown exception type." << endl;
		throw;
	}

	// This will cause our custom terminate handler to run.
	wcout << L"tC should throw? " <<
		ThrowClass(true).GetShouldThrow() << endl;

	return 0;
}