///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <memory>
#include <vector>

#include "conflict.hpp"
#include "range.hpp"

class Edge;
class Polygon;

//******************************************************************************
class ConflictEdgeInPolygon : public Conflict {
public:
	/// Assumes a correspondance between polygons and ranges order.
	/// A nullptr range means the edge is totally in the polygon.
	///*************************************************************************
	Edge const* edge;
	std::vector<Polygon const*> polygons;
	std::vector<std::unique_ptr<Range const>> ranges;

	ConflictEdgeInPolygon(Edge const* _edge, Polygon const* _polygon, Range const* _range);


//	/// For all EDGE_PARTIALLY_IN_POLYGON conflict including the same edge,
//	/// detect overlapping ranges and if the edge is totally in polygons or not.
	//**************************************************************************
//	void solve() override;

//	bool is_between(IConflictOrigin* a, IConflictOrigin* b) const override;
	void append(Polygon const* polygon, Range const* range);
};