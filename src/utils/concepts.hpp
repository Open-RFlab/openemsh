///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <concepts>
#include <type_traits>

//******************************************************************************
template<typename T>
concept Enum = std::is_enum_v<T>;

//******************************************************************************
template<typename T, typename R, typename... Args>
concept InvocableR = std::is_invocable_r_v<R, T, Args...>;

//******************************************************************************
template<typename P>
concept PointerLike = std::is_pointer_v<P> || requires (P p) {
	{ *p };
	{ static_cast<bool>(p) };
	{ p.operator->() } -> std::convertible_to<decltype(&*p)>;
};
