///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

/// Let provide a default T value, alternatively, one can provide another
/// locator as a lambda function.
///*****************************************************************************
template<typename T>
T const& default_locator() {
	static T t;
	return t;
}
