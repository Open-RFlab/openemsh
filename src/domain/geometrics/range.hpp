///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "domain/conflicts/i_conflict_origin.hpp"
#include "domain/mesh/i_meshline_origin.hpp"
#include "point.hpp"
#include "segment.hpp"

namespace domain {

//******************************************************************************
class Range : public Segment/*, public IConflictOrigin*//*, public IMeshLineOrigin*/ {
private:
	Point _p0;
	Point _p1;

public:
	Range(Point const p0, Point const p1) noexcept;

	Point const& p0() const noexcept override;
	Point const& p1() const noexcept override;
};

//******************************************************************************
bool operator==(Range const& a, Range const& b) noexcept;

} // namespace domain
