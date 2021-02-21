///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "conflict_colinear_edges.hpp"
#include "conflict_edge_in_polygon.hpp"
#include "edge.hpp"

#include "conflict_manager.hpp"

using namespace std;

//******************************************************************************
ConflictManager::ConflictManager(vector<unique_ptr<Conflict>>& _conflicts)
: conflicts(_conflicts)
{}

// TODO add a filter a & b are H or V
// TODO register confglict in edges
//******************************************************************************
void ConflictManager::add_colinear_edges(Edge* a, Edge* b) {
	bool does_conflict_exist = false;
	for(unique_ptr<Conflict>& conflict : conflicts) {
		if(conflict->kind == Conflict::Kind::COLINEAR_EDGES) {
			auto c = dynamic_cast<ConflictColinearEdges*>(conflict.get());
			bool is_a_registered = false;
			bool is_b_registered = false;

			for(Edge* edge : c->edges) {
				if(edge == a) {
					is_a_registered = true;
					if(is_b_registered)
						break;
				} else if(edge == b) {
					is_b_registered = true;
					if(is_a_registered)
						break;
				}
			}

			if(is_a_registered && !is_b_registered) {
				does_conflict_exist = true;
				c->append(b);
				b->conflicts.push_back(conflict.get());
				break;
			} else if(!is_a_registered && is_b_registered) {
				does_conflict_exist = true;
				c->append(a);
				a->conflicts.push_back(conflict.get());
				break;
			} else if(is_a_registered && is_b_registered) {
				does_conflict_exist = true;
				break;
			}
		}
	}
	if(!does_conflict_exist) {
		conflicts.push_back(make_unique<ConflictColinearEdges>(a, b));
		a->conflicts.push_back(conflicts.back().get());
		b->conflicts.push_back(conflicts.back().get());
	}
}