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
		EDGE_IN_POLYGON, // TODO EDGE_IN_MULTIPLE_POLYGONS : store cross point, portion in / out ?
		COLINEAR_EDGES
	} kind;

	bool is_solved;
	MeshlineManager* solution;
//	std::vector<IConflictOrigin*> between;

	Conflict(Kind const _kind);

//	virtual void solve();
//	virtual bool is_between(IConflictOrigin* a, IConflictOrigin* b) const = 0;

};
