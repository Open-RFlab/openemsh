///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "meshline.hpp"

namespace domain {

//******************************************************************************
Meshline::Meshline(Coord coord, Interval const* interval, MeshlinePolicy const* policy) noexcept
: coord(coord)
, interval(interval)
, policy(policy)
{}

//******************************************************************************
Meshline::Meshline(Coord coord, Interval const* interval) noexcept
: coord(coord)
, interval(interval)
, policy(nullptr)
{}

//******************************************************************************
Meshline::Meshline(Coord coord, MeshlinePolicy const* policy) noexcept
: coord(coord)
, interval(nullptr)
, policy(policy)
{}

//******************************************************************************
bool operator<(Meshline const& a, Meshline const& b) noexcept {
	return (a.coord < b.coord);
}

} // namespace domain
