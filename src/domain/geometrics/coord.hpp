///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

namespace domain {

//******************************************************************************
class Coord {
private:
	double val;

public:
	template<typename T>
	Coord(T const& value) noexcept : val(value) {}
	Coord() = default;

	explicit operator double() const noexcept;
	double value() const noexcept;

	template<typename T>
	Coord& operator=(T const& a) noexcept {
		val = (double) a;
		return *this;
	}
	template<typename T>
	Coord& operator+=(T const& a) noexcept {
		val += (double) a;
		return *this;
	}
	template<typename T>
	Coord& operator-=(T const& a) noexcept {
		val -= (double) a;
		return *this;
	}
	template<typename T>
	Coord& operator*=(T const& a) noexcept {
		val *= (double) a;
		return *this;
	}
	template<typename T>
	Coord& operator/=(T const& a) noexcept {
		val /= a;
		return *this;
	}
};

//******************************************************************************
bool operator==(Coord const& a, Coord const& b) noexcept;

//******************************************************************************
bool operator!=(Coord const& a, Coord const& b) noexcept;

//******************************************************************************
bool operator<=(Coord const& a, Coord const& b) noexcept;

//******************************************************************************
bool operator>=(Coord const& a, Coord const& b) noexcept;

//******************************************************************************
bool operator<(Coord const& a, Coord const& b) noexcept;

//******************************************************************************
bool operator>(Coord const& a, Coord const& b) noexcept;

//******************************************************************************
Coord operator+(Coord const& a, Coord const& b) noexcept;

//******************************************************************************
Coord operator-(Coord const& a, Coord const& b) noexcept;

//******************************************************************************
Coord operator*(Coord const& a, Coord const& b) noexcept;

//******************************************************************************
Coord operator/(Coord const& a, Coord const& b) noexcept;

//******************************************************************************
template<typename T>
bool operator==(T const& a, Coord const& b) noexcept {
	return Coord(a) == b;
}

//******************************************************************************
template<typename T>
bool operator==(Coord const& a, T const& b) noexcept {
	return a == Coord(b);
}

//******************************************************************************
Coord mid(Coord const& a, Coord const& b) noexcept;

//******************************************************************************
Coord distance(Coord const& a, Coord const& b) noexcept;

} // namespace domain
