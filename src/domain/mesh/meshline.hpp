///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "domain/geometrics/coord.hpp"
#include "domain/utils/entity_visitor.hpp"
#include "utils/entity.hpp"

namespace domain {

class Interval;
class MeshlinePolicy;

//******************************************************************************
class Meshline
: public Visitable<Meshline, EntityVisitor>
, public Entity {
public:
	Coord const coord;
	Interval const* const interval;
	MeshlinePolicy const* const policy;

	Meshline(Coord coord, Interval const* interval, MeshlinePolicy const* policy) noexcept;
	Meshline(Coord coord, Interval const* interval) noexcept;
	Meshline(Coord coord, MeshlinePolicy const* policy) noexcept;
};

//******************************************************************************
bool operator<(Meshline const& a, Meshline const& b) noexcept;

} // namespace domain
