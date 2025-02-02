///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <type_traits>

//******************************************************************************
#define GETTER(VAR_NAME) \
	std::add_const_t<decltype(VAR_NAME)>& get_ ##VAR_NAME () const { return VAR_NAME; }

//******************************************************************************
#define SETTER(VAR_NAME) \
	void set_ ##VAR_NAME (std::add_const_t<decltype(VAR_NAME)>& var) { VAR_NAME = var; }

//******************************************************************************
#define MAKER_DECL(VAR_NAME, ...) \
	std::remove_const_t<decltype(VAR_NAME)> make_ ##VAR_NAME (__VA_ARGS__) const

//******************************************************************************
#define MAKER_DEF(SCOPE, VAR_NAME, ...) \
	std::remove_const_t<decltype(SCOPE::VAR_NAME)> SCOPE::make_ ##VAR_NAME (__VA_ARGS__) const
