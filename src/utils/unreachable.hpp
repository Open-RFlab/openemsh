/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

/// @brief Backport of C++23 std::unreachable.
///*****************************************************************************
[[noreturn]] inline void unreachable() {
#ifdef __GNUC__ // GCC, Clang, ICC
	__builtin_unreachable();
#else
#ifdef _MSC_VER // MSVC
	__assume(false);
#endif
#endif
}
