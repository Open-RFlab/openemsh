///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "bounding.hpp"

/// Check if two bounding boxes overlap or just touch each other.
///*****************************************************************************
bool does_overlap(Bounding const& a, Bounding const&b) {
	return((b[XMIN] >= a[XMIN] && b[XMIN] <= a[XMAX])
	    || (b[XMAX] >= a[XMIN] && b[XMAX] <= a[XMAX])
	    || (a[XMIN] >= b[XMIN] && a[XMIN] <= b[XMAX])
	    || (a[XMAX] >= b[XMIN] && a[XMAX] <= b[XMAX]))
	    &&((b[YMIN] >= a[YMIN] && b[YMIN] <= a[YMAX])
	    || (b[YMAX] >= a[YMIN] && b[YMAX] <= a[YMAX])
	    || (a[YMIN] >= b[YMIN] && a[YMIN] <= b[YMAX])
	    || (a[YMAX] >= b[YMIN] && a[YMAX] <= b[YMAX]));
}
