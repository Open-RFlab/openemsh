///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <utility>

// Example: TRY(func());
//******************************************************************************
#define TRY(x) \
	{ /* TRY */ \
		if(auto res = (x); !res.has_value()) \
			return std::unexpected(std::move(res.error())); \
	}

// Example: UNWRAP(func(), [&](auto& val) { var = val; });
//******************************************************************************
#define UNWRAP(x, unwrap) \
	{ /* UNWRAP */ \
		if(auto res = (x); res.has_value()) \
			unwrap(res.value()); \
		else \
			return std::unexpected(std::move(res.error())); \
	}
