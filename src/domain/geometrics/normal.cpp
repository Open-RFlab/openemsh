///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "utils/unreachable.hpp"

#include "normal.hpp"

namespace domain {

//******************************************************************************
Normal reverse(Normal normal) noexcept {
	switch(normal) {
	case Normal::XMIN: return Normal::XMAX;
	case Normal::XMAX: return Normal::XMIN;
	case Normal::YMIN: return Normal::YMAX;
	case Normal::YMAX: return Normal::YMIN;
	case Normal::ZMIN: return Normal::ZMAX;
	case Normal::ZMAX: return Normal::ZMIN;
	case Normal::NONE: return Normal::NONE;
	default: ::unreachable();
	}
}

} // namespace domain
