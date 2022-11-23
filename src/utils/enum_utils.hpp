///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <array>

/// @brief Counts variadic arguments
///*****************************************************************************
template<typename ...Args>
constexpr std::size_t va_count(Args&&...) {
	return sizeof...(Args);
}

/// @brief Defines an enum <ENUM> and a constexpr array All<ENUM> that contains
/// all the enumerators in correct order.
///*****************************************************************************
#define ITERABLE_ENUM(ENUM, ...) \
	enum ENUM : std::size_t { __VA_ARGS__ }; \
	constexpr std::array<ENUM const, va_count(__VA_ARGS__)> All ##ENUM { __VA_ARGS__ };

/// @brief Defines an enum <ENUM> and a constexpr array All<ENUM> that contains
/// all the enumerators in correct order, plus an array alias that is sized for
/// the number of enumerators.
///*****************************************************************************
#define ARRAY_WITH_ITERABLE_ENUM_INDEX(ARRAY, ENUM, ...) \
	enum ENUM : std::size_t { __VA_ARGS__ }; \
	template<typename T> \
	using ARRAY = typename std::array<T, va_count(__VA_ARGS__)>; \
	constexpr ARRAY<ENUM const> All ##ENUM { __VA_ARGS__ };
