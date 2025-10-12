/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#ifdef DEBUG
#include <cstdlib>
#endif

#include <utility>

//******************************************************************************
[[noreturn]] inline void unreachable() {
#ifdef DEBUG
	std::abort();
#else
	std::unreachable();
#endif
}
