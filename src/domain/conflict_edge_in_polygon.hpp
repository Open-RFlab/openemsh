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

/// The optional overlap edge is reserved for when edge is overlapping an
/// edge of the polygon and not just the polygon itself.
///*****************************************************************************
using Overlap = std::tuple<Polygon const*, std::unique_ptr<Range const>, std::optional<Edge const*>>;
enum OverlapIndex {
	POLYGON,
	RANGE,
	EDGE,
};

//******************************************************************************
class ConflictEdgeInPolygon : public Conflict {
public:
	Edge* const edge;

	std::vector<Overlap> overlaps;

	ConflictEdgeInPolygon(Edge* a, Polygon const* _polygon, Range const _range, std::optional<Edge const*> b);

	void append(Polygon const* polygon, Range const range, std::optional<Edge const*> edge);

	void auto_solve(MeshlinePolicyManager& line_policy_manager) override;

#ifdef DEBUG
	void print() const override;
#endif // DEBUG
};

#ifdef UNITTEST
void sort_overlaps_by_p0_by_vector_orientation(std::vector<Overlap>& overlaps, Point const& vector);
#endif // UNITTEST
