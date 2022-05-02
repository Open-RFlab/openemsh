///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

//******************************************************************************
class Coord {
private:
	double val;

public:
	template<typename T>
	Coord(T const& value) : val(value) {}
	Coord() = default;

	explicit operator double() const;
	double value() const;
};

//******************************************************************************
bool operator==(Coord const& a, Coord const& b);

//******************************************************************************
bool operator!=(Coord const& a, Coord const& b);

//******************************************************************************
bool operator<=(Coord const& a, Coord const& b);

//******************************************************************************
bool operator>=(Coord const& a, Coord const& b);

//******************************************************************************
bool operator<(Coord const& a, Coord const& b);

//******************************************************************************
bool operator>(Coord const& a, Coord const& b);

//******************************************************************************
Coord operator+(Coord const& a, Coord const& b);

//******************************************************************************
Coord operator-(Coord const& a, Coord const& b);

//******************************************************************************
Coord operator*(Coord const& a, Coord const& b);

//******************************************************************************
Coord operator/(Coord const& a, Coord const& b);

//******************************************************************************
template<typename T>
bool operator==(T const& a, Coord const& b) {
	return Coord(a) == b;
}

//******************************************************************************
template<typename T>
bool operator==(Coord const& a, T const& b) {
	return a == Coord(b);
}
