///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

//#include <vector>

#include "i_conflict_origin.hpp"
#include "i_meshline_origin.hpp"

class MeshlineManager;

//******************************************************************************
class Conflict : public IMeshLineOrigin {
public:
	enum class Kind {
		USER_WILL, // TODO multiple cases ?
		EDGE_IN_POLYGON,
//		OVERLAPPING_EDGES, // TODO this or EDGE_IN_POLYGON subcase? try EOP first
		COLINEAR_EDGES
	} kind;

	bool is_solved;
	MeshlineManager* solution;
//	std::vector<IConflictOrigin*> between;

	Conflict(Kind const _kind);

	virtual void print() const = 0;
//	virtual void solve();
//	virtual bool is_between(IConflictOrigin* a, IConflictOrigin* b) const = 0;

};
