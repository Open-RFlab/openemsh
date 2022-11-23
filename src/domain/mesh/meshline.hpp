///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "utils/entity.hpp"

#include "domain/geometrics/coord.hpp"

class Interval;
class MeshlinePolicy;

//******************************************************************************
class Meshline : public Entity, public Visitable<Meshline, EntityVisitor> {
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
