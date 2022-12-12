///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

//#include <vector>

#include "domain/mesh/i_meshline_origin.hpp"
#include "domain/utils/entity_visitor.hpp"
#include "utils/entity.hpp"
#include "i_conflict_origin.hpp"

namespace domain {

class IConflictSolution;
//class MeshlinePolicy;
class MeshlinePolicyManager;

//******************************************************************************
class Conflict : public Entity, public IMeshLineOrigin {
public:
	enum class Kind {
		USER_WILL, // TODO multiple cases ?
		EDGE_IN_POLYGON,
//		OVERLAPPING_EDGES, // TODO this or EDGE_IN_POLYGON subcase? try EOP first
		COLINEAR_EDGES,
		TOO_CLOSE_MESHLINE_POLICIES
	} kind;

	bool is_solved;
//	MeshlinePolicy* solution;
	IConflictSolution* solution;
//	std::vector<IConflictOrigin*> between;

	explicit Conflict(Kind const kind);

	virtual void auto_solve(MeshlinePolicyManager& line_policy_manager) = 0;

#ifdef DEBUG
	virtual void print() const = 0;
#endif // DEBUG
};

} // namespace domain
