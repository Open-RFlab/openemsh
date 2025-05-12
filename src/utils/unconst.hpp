///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <type_traits>

//******************************************************************************
template<typename P>
P* unconst(P const* p) {
	return const_cast
		<std::add_pointer_t
		<std::remove_const_t
		<std::remove_pointer_t
		<P
		>>>>(p);
}

//******************************************************************************
template<typename P>
P& unconst(P const& p) {
	return const_cast
		<std::add_lvalue_reference_t
		<std::remove_const_t
		<std::remove_reference_t
		<P
		>>>>(p);
}
