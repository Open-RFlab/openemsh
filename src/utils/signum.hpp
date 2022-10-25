///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <cstdint>

//******************************************************************************
template<typename T>
int8_t signum(T const& a) noexcept(noexcept(T(0))) {
	return (T(0) < a) - (a < T(0));
}
