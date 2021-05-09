///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "meshline.hpp"

//******************************************************************************
Meshline::Meshline(double _coord, MeshlinePolicy* const _policy)
: coord(_coord)
, policy(_policy)
{}

//******************************************************************************
bool operator<(Meshline const& a, Meshline const& b) {
	return (a.coord < b.coord);
}