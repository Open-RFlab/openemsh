/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#ifdef DEBUG
#include <cstdlib>
#endif

/// @brief Backport of C++23 std::unreachable.
///*****************************************************************************
[[noreturn]] inline void unreachable() {
#ifndef DEBUG

#ifdef __GNUC__ // GCC, Clang, ICC
	__builtin_unreachable();
#else
#ifdef _MSC_VER // MSVC
	__assume(false);
#endif
#endif

#else // DEBUG

	std::abort();

#endif // DEBUG
}
