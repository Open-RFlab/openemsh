///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <algorithm>

#include "utils/unreachable.hpp"

#include "bounding.hpp"

namespace domain {

using namespace std;

///*****************************************************************************
bool does_overlap(Bounding1D const& a, Coord const& b) noexcept {
	return b >= a[XMIN] && b <= a[XMAX];
}

/// Check if two bounding boxes overlap or just touch each other.
///*****************************************************************************
bool does_overlap(Bounding1D const& a, Bounding1D const& b) noexcept {
	return min(a[XMAX], b[XMAX]) >= max(a[XMIN], b[XMIN]);
}

/// Check if two bounding boxes overlap or just touch each other.
///*****************************************************************************
bool does_overlap(Bounding2D const& a, Bounding2D const& b) noexcept {
	return min(a[XMAX], b[XMAX]) >= max(a[XMIN], b[XMIN])
	    && min(a[YMAX], b[YMAX]) >= max(a[YMIN], b[YMIN]);
}

/// Check if two bounding boxes overlap.
///*****************************************************************************
bool does_overlap_strict(Bounding2D const& a, Bounding2D const& b) noexcept {
	return min(a[XMAX], b[XMAX]) > max(a[XMIN], b[XMIN])
	    && min(a[YMAX], b[YMAX]) > max(a[YMIN], b[YMIN]);
}

//******************************************************************************
Bounding1D cast(ViewAxis axis, Bounding2D const& a) noexcept {
	switch(axis) {
	case H: return { a[XMIN], a[XMAX] };
	case V: return { a[YMIN], a[YMAX] };
	default: ::unreachable();
	}
}


} // namespace domain
