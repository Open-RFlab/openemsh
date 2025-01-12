///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <concepts>

/// Cast a pointer from a base UpFrom of an object to another one UpTo, trying
/// to travel through different derived Down classes. Returns it through the
/// first derived class that succeed. Returns nullptr if none matches.
///
/// Cf. https://www.foonathan.net/2020/05/fold-tricks/
///
/// If the cast predicate succeed, assign its output to result variable and
/// return true, which will stop folding due to the operator ||.
/// Otherwise, fold to the next Down value.
///*****************************************************************************
template<typename UpTo, std::derived_from<UpTo> ...Down, typename UpFrom>
UpTo* down_up_cast(UpFrom* from) {
	UpTo* result = nullptr;

	[[maybe_unused]] bool const succeed = (
		( (dynamic_cast<Down*>(from) != nullptr)
		? (result = dynamic_cast<Down*>(from), true)
		: false)
	|| ...);

	return result;
}
