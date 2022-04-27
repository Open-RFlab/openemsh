///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "meshline.hpp"

//******************************************************************************
Meshline::Meshline(Coord coord, MeshlinePolicy* const policy)
: coord(coord)
, policy(policy)
{}

//******************************************************************************
bool operator<(Meshline const& a, Meshline const& b) {
	return (a.coord < b.coord);
}
