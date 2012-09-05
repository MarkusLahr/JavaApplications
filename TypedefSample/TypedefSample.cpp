#include <iostream>
#include <ostream>
#include <vector>
#include <algorithm>
#include "../pchar.h"

// This makes WidgetIdVector is an alias for std::vector<int>. This has 
// more meaning than std::vector<int> would have, since now we know that 
// something which uses this this alias expects a vector of widget IDs
// rather than some other vector of integers.
typedef std::vector<int> WidgetIdVector;

bool ContainsWidgetId(WidgetIdVector idVector, int id)
{
	return (std::end(idVector) != 
		std::find(std::begin(idVector), std::end(idVector), id)
		);
}

int _pmain(int /*argc*/, _pchar* /*argv*/[])
{
	WidgetIdVector idVector;
	
	// Add some "id" numbers to the vector.
	idVector.push_back(5);
	idVector.push_back(8);

	// Output a result letting us know if the id is in the 
	// WidgetIdVector
	std::wcout << L"Contains 8: " <<
		(ContainsWidgetId(idVector, 8) ? L"true." : L"false.") <<
		std::endl;

	return 0;
}