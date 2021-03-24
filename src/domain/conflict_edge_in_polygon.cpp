///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "conflict_edge_in_polygon.hpp"

using namespace std;

//******************************************************************************
ConflictEdgeInPolygon::ConflictEdgeInPolygon(Edge const* a, Polygon const* _polygon, Range const _range, optional<Edge const*> b)
: Conflict(Kind::EDGE_IN_POLYGON)
, edge(a) {
	overlaps.emplace_back(_polygon, make_unique<Range const>(_range), b);
}

//******************************************************************************
void ConflictEdgeInPolygon::append(Polygon const* polygon, Range const range, optional<Edge const*> edge) {
	overlaps.emplace_back(polygon, make_unique<Range const>(range), edge);
}