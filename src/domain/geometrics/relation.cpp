///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "utils/unreachable.hpp"

#include "relation.hpp"

namespace domain {
namespace relation {

//******************************************************************************
PolygonPoint cast(PolygonSegment const a) {
	switch(a) {
	case PolygonSegment::IN: return PolygonPoint::IN;
	case PolygonSegment::ON: return PolygonPoint::ON;
	case PolygonSegment::OUT: return PolygonPoint::OUT;
	default: unreachable();
	}
}

//******************************************************************************
PolygonSegment cast(PolygonPoint const a) {
	switch(a) {
	case PolygonPoint::IN: return PolygonSegment::IN;
	case PolygonPoint::ON: return PolygonSegment::ON;
	case PolygonPoint::OUT: return PolygonSegment::OUT;
	default: unreachable();
	}
}

//******************************************************************************
void toggle(PolygonSegment& a) {
	switch(a) {
	case PolygonSegment::IN: a = PolygonSegment::OUT; break;
	case PolygonSegment::OUT: a = PolygonSegment::IN; break;
	default: break;
	}
}

//******************************************************************************
void toggle(PolygonPoint& a) {
	switch(a) {
	case PolygonPoint::IN: a = PolygonPoint::OUT; break;
	case PolygonPoint::OUT: a = PolygonPoint::IN; break;
	default: break;
	}
}

} // namespace relation
} // namespace domain
