#include "IWriteData.h"
#include "ConsoleWriteData.h"
#include "../pchar.h"

int _pmain(int /*argc*/, _pchar* /*argv*/[])
{
	//// The following line is illegal since IWriteData is abstract.
	//IWriteData iwd = IWriteData();

	//// The following line is also illegal. You cannot have an
	//// instance of IWriteData. 
	//IWriteData iwd = ConsoleWriteData();

	ConsoleWriteData cwd = ConsoleWriteData();

	// You can create an IWriteData reference to an instance of a class
	// that derives from IWriteData.
	IWriteData& r_iwd = cwd;

	// You can also create an IWriteData pointer to an instance of a
	// class that derives from IWriteData.
	IWriteData* p_iwd = &cwd;

	cwd.WriteLine(10);
	r_iwd.WriteLine(14.6);
	p_iwd->WriteLine(L"Hello Abstract World!");

	return 0;
}