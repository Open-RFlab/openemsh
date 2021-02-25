///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

#include <utility>

#include "conflict_edge_in_polygon.hpp"

#include "board.hpp"

using namespace std;

//******************************************************************************
Board::Board(vector<unique_ptr<Polygon>>& _polygons)
: conflict_manager(conflicts)
, polygons(move(_polygons)) {
	for(unique_ptr<Polygon>& polygon : polygons)
		for(unique_ptr<Edge>& edge : polygon->edges)
			edges.push_back(edge.get());
}

/// Detect all EDGE_IN_POLYGON. Will also detect some COLINEAR_EDGES.
///*****************************************************************************
void Board::detect_edges_in_polygons() {
	for(size_t i = 0; i < polygons.size() ; ++i) {
		for(size_t j = i + 1; j < polygons.size(); ++j) {
			if(are_possibly_overlapping(*polygons[i], *polygons[j])) {
//				for(unique_ptr<Edge>& edge_i : polygons[i]->edges) {
//					for(unique_ptr<Edge>& edge_j : polygons[j]->edges) {
//						relation::EdgeEdge e = edge_i->relation_to(edge_j.get());

				relation::PolygonEdge side; // TODO check if first point is in polygon

				for(size_t k = 0; k < polygons[i]->edges.size(); ++k) {
					for(size_t l = k + 1; l < polygons[j]->edges.size(); ++l) { // TODO k+1 -> broken for?


						relation::EdgeEdge e = polygons[i]->edges[k]->relation_to(polygons[j]->edges[l].get()); // TODO e better name
						polygons[i]->edges[k]->print();
						polygons[j]->edges[l]->print();
						switch(e) {
						case relation::EdgeEdge::APART: {

							cout << "i: " << i << "\tj: " << j << "\t\tAPART" << endl;
							break;
						} case relation::EdgeEdge::CROSSING: {
							if(optional<Point> p = intersection(polygons[i]->edges[k].get(), polygons[j]->edges[l].get()))
								p.value().print();

//							if(p)
							cout << "i: " << i << "\tj: " << j << "\t\tCROSSING" << endl;
							break;
						} case relation::EdgeEdge::COLINEAR: {
							conflict_manager.add_colinear_edges(polygons[i]->edges[k].get(), polygons[j]->edges[l].get());

							cout << "i: " << i << "\tj: " << j << "\t\tCOLINEAR" << endl;
							break;
						} case relation::EdgeEdge::OVERLAPPING: {
							if(optional<Range> r = overlap(polygons[i]->edges[k].get(), polygons[j]->edges[l].get()));

							cout << "i: " << i << "\tj: " << j << "\t\tOVERLAPPING" << endl;
							break;
						}
						}
					}
				}
			}
		}
	}
}

//****************************************************************************** TODO handle conflicts
void Board::detect_colinear_edges() {
	for(size_t i = 0; i < edges.size(); ++i) {
		if(edges[i]->direction == Edge::Direction::DIAGONAL)
			continue;
/*
		for(unsigned long j = i + 1; j < edges.size(); ++j) {
			if((edges[j]->direction == edges[i]->direction)    // Different axis.
			&& ((edges[i]->normal != edges[j]->normal)         // Opposite directions.
			|| (edges[i]->policy == Edge::Policy::HALFS)       // More than 2 conflicting edges.
			|| (edges[j]->policy == Edge::Policy::HALFS))) {
				edges[i]->policy = Edge::Policy::HALFS;
				edges[j]->policy = Edge::Policy::HALFS;
			}
		}
*/
	}
}

//******************************************************************************
void Board::print() {
	for(unique_ptr<Polygon>& polygon : polygons)
		polygon->print();
}

// TODO use add_conflict_edge_in_polygon instead of interface type & switch ?
// TODO wrap all in a factory
//******************************************************************************
void Board::add_conflict(Conflict::Kind kind, IConflictOrigin* a, IConflictOrigin* b) {
	switch(kind) {
	case Conflict::Kind::EDGE_IN_POLYGON: {
		bool exist = false;
		for(unique_ptr<Conflict>& conflict : conflicts) {
			if(conflict->kind == kind && dynamic_cast<ConflictEdgeInPolygon*>(conflict.get())->edge == a) {
//				dynamic_cast<ConflictEdgeInPolygon*>(conflict.get())->append(b);
				exist = true;
				break;
			}
		}
		if(!exist)
//			conflicts.push_back(make_unique<ConflictEdgeInPolygon>(dynamic_cast<Edge*>(a), dynamic_cast<Polygon*>(b)));
		break;
	} default: {
		break;
	}
	}
}
