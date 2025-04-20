///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "conflict.hpp"
#include "conflict_colinear_edges.hpp"
#include "conflict_edge_in_polygon.hpp"

#include "i_conflict_origin.hpp"

namespace domain {

using namespace std;

//******************************************************************************
//vector<ConflictColinearEdges*> IConflictOrigin::look_for_conflict_colinear_edges() {
//	vector<ConflictColinearEdges*> ret;
//	for(Conflict* conflict : conflicts)
////		if(conflict->kind == Conflict::Kind::COLINEAR_EDGES)
////			ret.push_back(dynamic_cast<ConflictColinearEdges*>(conflict));
//		if(auto c = dynamic_cast<ConflictColinearEdges*>(conflict))
//			ret.push_back(c);
//	return ret;
//}

//******************************************************************************
//ConflictEdgeInPolygon* IConflictOrigin::look_for_conflict_edge_in_polygon() {
//	for(Conflict* conflict : conflicts)
////		if(conflict->kind == Conflict::Kind::EDGE_IN_POLYGON)
////		if(typeid(conflict) == typeid(ConflictEdgeInPolygon))
////			return dynamic_cast<ConflictEdgeInPolygon*>(conflict);
//		if(auto c = dynamic_cast<ConflictEdgeInPolygon*>(conflict))
//			return c;
//	return nullptr;
//}

} // namespace domain
