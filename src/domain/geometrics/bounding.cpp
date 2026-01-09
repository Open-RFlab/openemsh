///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "utils/unreachable.hpp"

#include "bounding.hpp"

namespace domain {

///*****************************************************************************
bool does_overlap(Bounding1D const& a, Coord const& b) noexcept {
	return b >= a[XMIN] && b <= a[XMAX];
}

/// Check if two bounding boxes overlap or just touch each other.
///*****************************************************************************
bool does_overlap(Bounding1D const& a, Bounding1D const& b) noexcept {
	return (b[XMIN] >= a[XMIN] && b[XMIN] <= a[XMAX])
	    || (b[XMAX] >= a[XMIN] && b[XMAX] <= a[XMAX])
	    || (a[XMIN] >= b[XMIN] && a[XMIN] <= b[XMAX])
	    || (a[XMAX] >= b[XMIN] && a[XMAX] <= b[XMAX]);
}

/// Check if two bounding boxes overlap or just touch each other.
///*****************************************************************************
bool does_overlap(Bounding2D const& a, Bounding2D const& b) noexcept {
	return((b[XMIN] >= a[XMIN] && b[XMIN] <= a[XMAX])
	    || (b[XMAX] >= a[XMIN] && b[XMAX] <= a[XMAX])
	    || (a[XMIN] >= b[XMIN] && a[XMIN] <= b[XMAX])
	    || (a[XMAX] >= b[XMIN] && a[XMAX] <= b[XMAX]))
	    &&((b[YMIN] >= a[YMIN] && b[YMIN] <= a[YMAX])
	    || (b[YMAX] >= a[YMIN] && b[YMAX] <= a[YMAX])
	    || (a[YMIN] >= b[YMIN] && a[YMIN] <= b[YMAX])
	    || (a[YMAX] >= b[YMIN] && a[YMAX] <= b[YMAX]));
}

//******************************************************************************
Bounding1D cast(ViewAxis axis, Bounding2D const& a) noexcept {
	switch(axis) {
	case H: return { a[XMIN], a[XMAX] };
	case V: return { a[YMIN], a[YMAX] };
	default: unreachable();
	}
}


} // namespace domain
