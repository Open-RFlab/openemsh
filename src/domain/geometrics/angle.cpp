///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "edge.hpp"

#include "angle.hpp"

namespace domain {

//******************************************************************************
Angle::Angle(Point const& p, Edge* e0, Edge* e1, Timepoint* t)
: Originator(t)
, plane(e0->plane)
, p(p)
, e0(e0)
, e1(e1)
, angle(domain::angle(e0->vec, e1->vec))
{}

} // namespace domain
