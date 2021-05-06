///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "conflict.hpp"
#include "range.hpp"

class Edge;
class Polygon;

//******************************************************************************
using Overlap = std::tuple<Polygon const*, std::unique_ptr<Range const>, std::optional<Edge const*>>;
enum OverlapIndex {
	POLYGON,
	RANGE,
	EDGE,
};

//******************************************************************************
class ConflictEdgeInPolygon : public Conflict {
public:
	/// The optional overlap edge is reserved for when edge is overlapping an
	/// edge of the polygon and not just the polygon itself.
	///*************************************************************************
	Edge const* edge;

	std::vector<Overlap> overlaps;

	ConflictEdgeInPolygon(Edge const* a, Polygon const* _polygon, Range const _range, std::optional<Edge const*> b);


//	/// For all EDGE_PARTIALLY_IN_POLYGON conflict including the same edge,
//	/// detect overlapping ranges and if the edge is totally in polygons or not.
	//**************************************************************************
//	void solve() override;

//	bool is_between(IConflictOrigin* a, IConflictOrigin* b) const override;
	void append(Polygon const* polygon, Range const range, std::optional<Edge const*> edge);

#ifdef DEBUG
	void print() const override;
#endif // DEBUG
};