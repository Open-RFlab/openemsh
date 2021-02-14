///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "board.hpp"

using namespace std;

//******************************************************************************
Board::Board(vector<unique_ptr<Polygon>>& _polygons)
: polygons(move(_polygons)) {
	for(unique_ptr<Polygon>& polygon : polygons)
		for(unique_ptr<Edge>& edge : polygon->edges)
			edges.push_back(edge.get());
}

//******************************************************************************
void Board::detect_disabled_edges() {
	for(size_t i = 0; i < polygons.size() ; ++i) {
		for(size_t j = i + 1; j < polygons.size(); ++j) {
			if(are_possibly_overlapping(*polygons[i], *polygons[j])) {

			}
		}
	}
}

//****************************************************************************** TODO handle conflicts
void Board::detect_halfs_rule_edges() {
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