///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <cmath>

#include "domain/global.hpp"

#include "coord.hpp"

using namespace std;

//******************************************************************************
Coord::operator double() const {
	return value;
}

//******************************************************************************
bool Coord::operator==(Coord const& a) const {
	return (abs(value - (double) a) < equality_tolerance);
}
