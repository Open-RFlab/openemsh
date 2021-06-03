///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

//#include <memory>
#include <optional>
#include <vector>

//#include "conflict.hpp"
#include "coord.hpp"
#include "global.hpp"
#include "i_conflict_origin.hpp"
#include "i_conflict_solution.hpp"
#include "i_meshline_origin.hpp"
#include "segment.hpp"
#include "types.hpp"

class Conflict;
class Meshline;

/// This class is an interface between a mesh line and its origin because
/// multiples edges can be responsible for the same lines and some lines can
/// be produced by other entities than edges, like a whole polygon (port) or
/// a conflict between lines that can require a modification of other lines.
///*****************************************************************************
class MeshlinePolicy : public IConflictOrigin, public IConflictSolution {
public:
	enum class Axis {
		H,
		V
	} const axis;

	/// Describe meshing policy to apply to the edge.
	///*************************************************************************
	enum class Policy {
//		DISABLED,    ///< Do not mesh this edge : if the edge is totally overlapping another
//		             /// or if the edge is inside a polygon.
		ONELINE,     ///< Place one line on the coord. typically produced by ports.
		HALFS,       ///< Apply halfs rule while meshing : when edges conflict on the direction.
		THIRDS,       ///< Apply thirds rule while meshing : normal case for edges.
		INTERVAL
	} policy; // TODO rename meshing_rule

	Normal normal;

	Params& params;
	Coord const coord;
	bool is_enabled;
	double res_factor;

	std::vector<IMeshLineOrigin*> origins;
	std::vector<Meshline*> meshlines; // TODO unique_ptr or ptr?
//	std::vector<Conflict*> conflicts;

	MeshlinePolicy(
		Axis const axis,
		Policy const policy,
		Normal normal,
		Params& params,
		Coord const coord,
		bool const is_enabled = true,
		double const res_factor = 1);

	Meshline mesh();
};

//******************************************************************************
std::optional<MeshlinePolicy::Axis> cast(Segment::Axis const a);

//******************************************************************************
Coord coord(Point const& point, MeshlinePolicy::Axis const axis);
