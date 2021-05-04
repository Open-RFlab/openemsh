///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <memory>
#include <vector>

//#include "conflict.hpp"
#include "global.hpp"
#include "i_conflict_origin.hpp"
#include "i_meshline_origin.hpp"
#include "types.hpp"

class Conflict;
class Meshline;

/// This class is an interface between a mesh line and its origin because
/// multiples edges can be responsible for the same lines and some lines can
/// be produced by other entities than edges, like a whole polygon (port) or
/// a conflict between lines that can require a modification of other lines.
///*****************************************************************************
class MeshlineManager : public IConflictOrigin {
public:
	Params& params;
	double coord;
	bool is_enabled;
	double res_factor;

	/// Describe meshing policy to apply to the edge.
	///*************************************************************************
	enum class Policy {
//		DISABLED,    ///< Do not mesh this edge : if the edge is totally overlapping another
//		             /// or if the edge is inside a polygon.
		ONELINE,     ///< Place one line on the coord. typically produced by ports.
		HALFS,       ///< Apply halfs rule while meshing : when edges conflict on the direction.
		THIRDS,       ///< Apply thirds rule while meshing : normal case for edges.
		RANGE
	} policy; // TODO rename meshing_rule

	Normal normal;

//	bool axis;
	std::vector<IMeshLineOrigin*> origins;
//	std::vector<std::unique_ptr<Meshline>> meshlines;
	std::vector<Conflict*> conflicts;

	MeshlineManager(Params& _params, double const _coord, bool const _is_enabled = true, double const _res_factor = 1);

};