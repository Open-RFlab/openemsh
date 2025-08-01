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

#include "domain/geometrics/range.hpp"
#include "domain/geometrics/space.hpp"
#include "utils/state_management.hpp"
#include "conflict.hpp"

namespace domain {

class Edge;
class Polygon;

/// The optional overlap edge is reserved for when edge is overlapping an
/// edge of the polygon and not just the polygon itself.
///*****************************************************************************
using Overlap = std::tuple<Polygon const*, Range, std::optional<Edge const*>>;
enum OverlapIndex {
	POLYGON,
	RANGE,
	EDGE,
};

//******************************************************************************
struct ConflictEdgeInPolygonState final : public ConflictState {
	std::vector<Overlap> overlaps;
};

//******************************************************************************
class ConflictEdgeInPolygon
: public Originator<ConflictEdgeInPolygonState const>
, public Visitable<ConflictEdgeInPolygon, EntityVisitor>
, public Conflict {
public:
	Plane const plane;
	Edge* const edge;

	ConflictEdgeInPolygon(Plane plane, Edge* a, Polygon const* polygon, Range const range, std::optional<Edge const*> b, Timepoint* t);

	void append(Polygon const* polygon, Range const range, std::optional<Edge const*> edge, Timepoint* t = nullptr);

	void auto_solve(MeshlinePolicyManager& line_policy_manager) override;
};

#ifdef UNITTEST
void sort_overlaps_by_p0_by_vector_orientation(std::vector<Overlap>& overlaps, Point const& vector);
#endif // UNITTEST

} // namespace domain
