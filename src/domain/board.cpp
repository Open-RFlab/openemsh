///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

#include "board.hpp"

using namespace std;

//******************************************************************************
Board::Board(vector<unique_ptr<Polygon>>& _polygons)
: polygons(move(_polygons)) {
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
				for(size_t k = 0; k < polygons[i]->edges.size(); ++k) {
					for(size_t l = k + 1; l < polygons[j]->edges.size(); ++l) {
						relation::EdgeEdge e = polygons[i]->edges[k]->relation_to(polygons[j]->edges[l].get());
						polygons[i]->edges[k]->print();
						polygons[j]->edges[l]->print();
						switch(e) {
						case relation::EdgeEdge::APART: cout << "i: " << i << "\tj: " << j << "\t\tAPART" << endl; break;
						case relation::EdgeEdge::CROSSING: cout << "i: " << i << "\tj: " << j << "\t\tCROSSING" << endl; break;
						case relation::EdgeEdge::COLINEAR: cout << "i: " << i << "\tj: " << j << "\t\tCOLINEAR" << endl; break;
						case relation::EdgeEdge::COLINEAR_CROSSING: cout << "i: " << i << "\tj: " << j << "\t\tCOLINEAR_CROSSING" << endl; break;
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