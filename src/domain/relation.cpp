///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "relation.hpp"

namespace relation {

//******************************************************************************
PolygonPoint cast(PolygonEdge const a) {
	switch(a) {
	case PolygonEdge::IN: return PolygonPoint::IN;
	case PolygonEdge::ON: return PolygonPoint::ON;
	case PolygonEdge::OUT: return PolygonPoint::OUT;
	}
}

//******************************************************************************
PolygonEdge cast(PolygonPoint const a) {
	switch(a) {
	case PolygonPoint::IN: return PolygonEdge::IN;
	case PolygonPoint::ON: return PolygonEdge::ON;
	case PolygonPoint::OUT: return PolygonEdge::OUT;
	}
}

//******************************************************************************
void toggle(PolygonEdge& a) {
	switch(a) {
	case PolygonEdge::IN: a = PolygonEdge::OUT; break;
	case PolygonEdge::OUT: a = PolygonEdge::IN; break;
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

}; // namespace relation