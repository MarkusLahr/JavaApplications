#include <iostream>
#include <ostream>
#include <string>
#include "../pchar.h"

enum class Color
{
	Red,
	Orange,
	Yellow,
	Blue,
	Indigo,
	Violet
};

// You can specify any underlying integral type you want, provided it fits.
enum Flavor : unsigned short int
{
	Vanilla,
	Chocolate,
	Strawberry,
	Mint,
};

int _pmain(int /*argc*/, _pchar* /*argv*/[])
{
	Flavor f = Vanilla;
	f = Mint; // This is legal since the Flavor enum is an unscoped enum.

	Color c = Color::Orange;
	//c = Orange; // This is illegal since the Color enum is a scoped enum.

	std::wstring flavor;
	std::wstring color;

	switch (c)
	{
	case Color::Red:
		color = L"Red";
		break;
	case Color::Orange:
		color = L"Orange";
		break;
	case Color::Yellow:
		color = L"Yellow";
		break;
	case Color::Blue:
		color = L"Blue";
		break;
	case Color::Indigo:
		color = L"Indigo";
		break;
	case Color::Violet:
		color = L"Violet";
		break;
	default:
		color = L"Unknown";
		break;
	}

	switch (f)
	{
	case Vanilla:
		flavor = L"Vanilla";
		break;
	case Chocolate:
		flavor = L"Chocolate";
		break;
	case Strawberry:
		flavor = L"Strawberry";
		break;
	case Mint:
		flavor = L"Mint";
		break;
	default:
		break;
	}

	std::wcout << L"Flavor is " << flavor.c_str() << L" (" << f <<
		L"). Color is " << color.c_str() << L" (" <<
		static_cast<int>(c) << L")." << std::endl <<
		L"The size of Flavor is " << sizeof(Flavor) <<
		L"." << std::endl <<
		L"The size of Color is " << sizeof(Color) <<
		L"." << std::endl;

	return 0;
}