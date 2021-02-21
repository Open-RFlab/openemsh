///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "conflict_edge_in_polygon.hpp"

using namespace std;

//******************************************************************************
ConflictEdgeInPolygon::ConflictEdgeInPolygon(Edge const* _edge, Polygon const* _polygon, Range const* _range)
: Conflict(Kind::EDGE_IN_POLYGON)
, edge(_edge)
, polygons({ _polygon }) {
//, ranges({ unique_ptr<Range const>(_range) }) { // TODO why?
	ranges.push_back(unique_ptr<Range const>(_range));
}

//******************************************************************************
void ConflictEdgeInPolygon::append(Polygon const* polygon, Range const* range) {
	polygons.push_back(polygon);
	ranges.push_back(unique_ptr<Range const>(range));
}