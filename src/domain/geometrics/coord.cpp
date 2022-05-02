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
	return val;
}

//******************************************************************************
double Coord::value() const {
	return val;
}

//******************************************************************************
bool operator==(Coord const& a, Coord const& b) {
	return abs(a.value() - b.value()) < equality_tolerance;
}

//******************************************************************************
bool operator!=(Coord const& a, Coord const& b) {
	return !(a == b);
}

//******************************************************************************
bool operator<=(Coord const& a, Coord const& b) {
	return a.value() <= b.value();
}

//******************************************************************************
bool operator>=(Coord const& a, Coord const& b) {
	return a.value() >= b.value();
}

//******************************************************************************
bool operator<(Coord const& a, Coord const& b) {
	return a.value() < b.value();
}

//******************************************************************************
bool operator>(Coord const& a, Coord const& b) {
	return a.value() > b.value();
}

//******************************************************************************
Coord operator+(Coord const& a, Coord const& b) {
	return a.value() + b.value();
}

//******************************************************************************
Coord operator-(Coord const& a, Coord const& b) {
	return a.value() - b.value();
}

//******************************************************************************
Coord operator*(Coord const& a, Coord const& b) {
	return a.value() * b.value();
}

//******************************************************************************
Coord operator/(Coord const& a, Coord const& b) {
	return a.value() / b.value();
}
