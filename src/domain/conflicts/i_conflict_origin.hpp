///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <vector>

namespace domain {

class Conflict;
class ConflictColinearEdges;
class ConflictEdgeInPolygon;

//******************************************************************************
class IConflictOrigin {
public:
	std::vector<Conflict*> conflicts; //TODO rename like resulting_conflict

	virtual ~IConflictOrigin() = default;

	std::vector<ConflictColinearEdges*> look_for_conflict_colinear_edges();
	ConflictEdgeInPolygon* look_for_conflict_edge_in_polygon();
};

} // namespace domain
