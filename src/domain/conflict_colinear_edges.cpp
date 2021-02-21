///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "edge.hpp"

#include "conflict_colinear_edges.hpp"

using namespace std;

//******************************************************************************
ConflictColinearEdges::ConflictColinearEdges(Edge* a, Edge* b)
: Conflict(Kind::COLINEAR_EDGES)
, edges({ a, b })
{}

//******************************************************************************
void ConflictColinearEdges::append(Edge* edge) {
	edges.push_back(edge);
}
