///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

#include "point.hpp"

#include "range.hpp"

using namespace std;

//******************************************************************************
Range::Range(Point const* _p0, Point const* _p1)
: p0(_p0)
, p1(_p1)
{}
