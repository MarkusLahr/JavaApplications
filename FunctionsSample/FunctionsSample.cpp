#include "Utility.h"
#include "../pchar.h"

using namespace Utility;

int _pmain(int /*argc*/, _pchar* /*argv*/[])
{
	int i1 = 3;
	int i2 = 4;

	long long ll1 = 6;
	long long ll2 = 7;
	
	bool b1 = IsEven(i1);
	
	PrintBool(b1);

	PrintIsEvenResult(i1);
	PrintIsEvenResult(i2);
	PrintIsEvenResult(ll1);
	PrintIsEvenResult(ll2);


	return 0;
}