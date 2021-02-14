///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <vector>

#include "i_conflict_origin.hpp"
#include "i_meshline_origin.hpp"
//#include "meshline_manager.hpp"

class MeshlineManager;

//******************************************************************************
class Conflict : public IMeshLineOrigin {
public:
	enum class Kind {
		EDGE_EDGE_MERGE
	} kind;

	bool is_solved;
	MeshlineManager* solution;
	std::vector<IConflictOrigin*> between;
};
