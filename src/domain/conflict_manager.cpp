///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "conflict_colinear_edges.hpp"
#include "conflict_edge_in_polygon.hpp"
#include "edge.hpp"
#include "polygon.hpp"

#include "conflict_manager.hpp"

using namespace std;

//******************************************************************************
ConflictManager::ConflictManager(vector<unique_ptr<Conflict>>& _conflicts)
: conflicts(_conflicts)
{}

// TODO register conflict in edges
//******************************************************************************
void ConflictManager::add_colinear_edges(Edge* a, Edge* b) {
	if((a->axis == Edge::Axis::X && b->axis == Edge::Axis::X)
	|| (a->axis == Edge::Axis::Y && b->axis == Edge::Axis::Y)) {
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
			Conflict* conflict = conflicts.emplace_back(
				make_unique<ConflictColinearEdges>(a, b)).get();
			a->conflicts.push_back(conflict);
			b->conflicts.push_back(conflict);
		}
	}
}

//******************************************************************************
void ConflictManager::add_edge_in_polygon(Edge* a, Polygon* polygon, optional<Edge const*> b) {
	add_edge_in_polygon(a, polygon, Range(*a->p0, *a->p1), b);
}

//******************************************************************************
void ConflictManager::add_edge_in_polygon(Edge* a, Polygon* polygon, Range const range, optional<Edge const*> b) {
	bool does_conflict_exist = false;
	for(unique_ptr<Conflict>& conflict : conflicts) {
		if(conflict->kind == Conflict::Kind::EDGE_IN_POLYGON) {
			auto c = dynamic_cast<ConflictEdgeInPolygon*>(conflict.get());
			bool is_a_registered = false;
			bool is_overlap_registered = false;

			if(c->edge == a) {
				is_a_registered = true;

				using Overlap = tuple<Polygon const*, std::unique_ptr<Range const>, std::optional<Edge const*>>;
				for(Overlap& overlap : c->overlaps) {
					if((get<0>(overlap) == polygon && *get<1>(overlap) == range)
					&& (!get<2>(overlap) || (get<2>(overlap) && get<2>(overlap).value() == b))) {
						is_overlap_registered = true;
						break;
					}
				}
			}

			if(is_a_registered && !is_overlap_registered) {
				does_conflict_exist = true;
				c->append(polygon, range, b);
//				b->conflicts.push_back(conflict.get()); // TODO needed?
				polygon->conflicts.push_back(conflict.get());
				break;
			} else if(is_a_registered && is_overlap_registered) {
				does_conflict_exist = true;
				break;
			}
		}
	}

	if(!does_conflict_exist) {
		Conflict* conflict = conflicts.emplace_back(
			make_unique<ConflictEdgeInPolygon>(a, polygon, range, b)).get();
		a->conflicts.push_back(conflict);
//		b->conflicts.push_back(conflict); // TODO needed?
		polygon->conflicts.push_back(conflict);
	}
}