///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <cstdint>

//******************************************************************************
template<typename T>
constexpr int8_t signum(T const& a) noexcept(noexcept(T(0))) {
	return (int8_t) (T(0) < a) - (int8_t) (a < T(0));
}
