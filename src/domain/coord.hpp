///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#ifdef UNITTEST
#define private public
#endif // UNITTEST

//******************************************************************************
class Coord {
private:
	double value;

public:
	template<typename T>
	Coord(T const& value) : value(value) {}
	operator double() const;
	bool operator==(Coord const& a) const;
};

#ifdef UNITTEST
#undef private
#endif // UNITTEST

//******************************************************************************
template<typename T>
bool operator==(T const& a, Coord const& b) {
	return (Coord(a) == b);
}

//******************************************************************************
template<typename T>
bool operator==(Coord const& a, T const& b) {
	return (a == Coord(b));
}
