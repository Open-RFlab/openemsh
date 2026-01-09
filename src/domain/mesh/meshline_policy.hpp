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
#include "domain/conflicts/i_conflict_origin.hpp"
#include "domain/conflicts/i_conflict_solution.hpp"
#include "domain/geometrics/coord.hpp"
#include "domain/geometrics/normal.hpp"
#include "domain/geometrics/space.hpp"
#include "domain/utils/entity_visitor.hpp"
#include "domain/global.hpp"
#include "utils/entity.hpp"
#include "utils/state_management.hpp"
#include "i_meshline_origin.hpp"

namespace domain {

class Conflict;
class Meshline;

struct MeshlinePolicyState;

/// This class is an interface between a mesh line and its origin because
/// multiples edges can be responsible for the same lines and some lines can
/// be produced by other entities than edges, like a whole polygon (port) or
/// a conflict between lines that can require a modification of other lines.
///*****************************************************************************
class MeshlinePolicy
: public Originator<MeshlinePolicyState const>
, public Visitable<MeshlinePolicy, EntityVisitor>
, public Entity
, public IConflictOrigin
, public IConflictSolution {
public:
	Axis const axis;

	/// Describe meshing policy to apply to the edge.
	///*************************************************************************
	enum class Policy {
//		DISABLED,    ///< Do not mesh this edge : if the edge is totally overlapping another
//		             /// or if the edge is inside a polygon.
		ONELINE,     ///< Place one line on the coord. typically produced by ports.
		HALFS,       ///< Apply halfs rule while meshing : when edges conflict on the direction.
		THIRDS       ///< Apply thirds rule while meshing : normal case for edges.
	};

	enum class Normal {
		NONE,
		MIN,
		MAX
	};

	GlobalParams* global_params;
	Coord const coord;
//	std::vector<Conflict*> conflicts;

	MeshlinePolicy(
		Axis const axis,
		Policy const policy,
		Normal normal,
		GlobalParams* global_params,
		Coord const coord,
		Timepoint* t,
		std::vector<IMeshLineOrigin*> const& origins = {},
		bool const is_enabled = true,
		double const res_factor = 2);

	std::optional<Meshline> mesh();
};

//******************************************************************************
struct MeshlinePolicyState final
: public IConflictOriginState
, public IConflictSolutionState {
	MeshlinePolicy::Policy policy;
	MeshlinePolicy::Normal normal;
	bool is_enabled;
	double res_factor; // TODO useful? d directly? come from params
	double d; ///< Distance betwen two lines (HALFS and THIRDS only).
	std::vector<IMeshLineOrigin*> origins;
	std::vector<Meshline*> meshlines;
};

//******************************************************************************
MeshlinePolicy::Normal cast(Normal const normal) noexcept;

} // namespace domain
