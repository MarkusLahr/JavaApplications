#include <memory>
//// See the comment to the first use of assert() in _pmain below.
//#define NDEBUG 1
#include <cassert>
#include "../pchar.h"

using namespace std;

void SetValueToZero(int& value)
{
	value = 0;
}

void SetValueToZero(int* value)
{
	*value = 0;
}

int _pmain(int /*argc*/, _pchar* /*argv*/[])
{
	int value = 0;

	const int intArrCount = 20;
	// Create a pointer to int.
	int* p_intArr = new int[intArrCount];

	// Create a const pointer to int.
	int* const cp_intArr = p_intArr;

	// These two statements are fine since we can modify the data that a 
	// const pointer points to.
	// Set all elements to 5.
	uninitialized_fill_n(cp_intArr, intArrCount, 5);
	// Sets the first element to zero.
	*cp_intArr = 0;

	//// This statement is illegal because we cannot modify what a const 
	//// pointer points to.
	//cp_intArr = nullptr;

	// Create a pointer to const int.
	const int* pc_intArr = nullptr;

	// This is fine because we can modify what a pointer to const points 
	// to.
	pc_intArr = p_intArr;

	// Make sure we "use" pc_intArr.
	value = *pc_intArr;

	//// This statement is illegal since we cannot modify the data that a
	//// pointer to const points to.
	//*pc_intArr = 10;

	const int* const cpc_intArr = p_intArr;

	//// These two statements are illegal because we can modify neither 
	//// what a const pointer to const points to nor the data that it 
	//// points to.
	//cpc_intArr = p_intArr;
	//*cpc_intArr = 20;

	// Make sure we "use" cpc_intArr.
	value = *cpc_intArr;

	*p_intArr = 6;

	SetValueToZero(*p_intArr);

	// From <cassert>, this macro will display a diagnostic message if the 
	// expression in the parentheses evaluates to anything other than zero.
	// Unlike the _ASSERTE macro, this will run during Release builds. To
	// disable it, define NDEBUG before including the <cassert> header.
	assert(*p_intArr == 0);

	*p_intArr = 9;

	int& r_first = *p_intArr;

	SetValueToZero(r_first);

	assert(*p_intArr == 0);

	const int& cr_first = *p_intArr;

	//// This statement is illegal because cr_first is a const reference 
	//// but SetValueToZero does not take a const reference, only a 
	//// non-const reference (which makes sense since it wants to modify 
	//// the value.
	//SetValueToZero(cr_first);

	value = cr_first;

	// We can initialize a pointer using the address-of operator. 
	// Just be wary because local non-static variables become 
	// invalid when you exit their scope so any pointers to them 
	// would also thus become invalid.
	int* p_firstElement = &r_first;

	*p_firstElement = 10;

	SetValueToZero(*p_firstElement);

	assert(*p_firstElement == 0);

	// This will call the SetValueToZero(int*) overload because we 
	// are using the address-of operator to turn the reference into 
	// a pointer.
	SetValueToZero(&r_first);

	*p_intArr = 3;

	SetValueToZero(&(*p_intArr));

	assert(*p_firstElement == 0);

	// Create a function pointer. Notice how we need to put the 
	// variable name in parentheses with a * before it.
	void (*FunctionPtrToSVTZ)(int&) = nullptr;

	// Set the function pointer to point to SetValueToZero. It picks 
	// the correct overload automatically.
	FunctionPtrToSVTZ = &SetValueToZero;

	*p_intArr = 20;

	// Call the function pointed to by FunctionPtrToSVTZ, i.e. 
	// SetValueToZero(int&).
	FunctionPtrToSVTZ(*p_intArr);

	assert(*p_intArr == 0);

	*p_intArr = 50;

	// We can also call a function pointer like this. This is 
	// closer to what is actually happening behind the scenes;
	// FunctionPtrToSVTZ is being dereferenced with the result
	// being the function that is pointed to, which we then 
	// call using the value(s) specified in the second set of 
	// parentheses, i.e. *p_intArr here.
	(*FunctionPtrToSVTZ)(*p_intArr);

	assert(*p_intArr == 0);

	// Make sure that we get value set to 0 so we can "use" it.
	*p_intArr = 0;
	value = *p_intArr;

	// Delete the p_intArray using the delete[] operator since it is a 
	// dynamic p_intArray.
	delete[] p_intArr;
	p_intArr = nullptr;
	return value;
}