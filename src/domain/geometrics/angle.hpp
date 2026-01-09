///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "domain/conflicts/i_conflict_origin.hpp"
#include "domain/mesh/i_meshline_origin.hpp"
#include "domain/utils/entity_visitor.hpp"
#include "utils/entity.hpp"
#include "utils/state_management.hpp"
//#include "normal.hpp"
#include "point.hpp"
#include "space.hpp"

namespace domain {

class Edge;

//******************************************************************************
struct AngleState final
: public IConflictOriginState
, public IMeshLineOriginState {
	ViewAxisSpace<bool> to_mesh = { false, false };
};

// TODO IMeshLineOrigin allow 1 MLP while Angle will end in 2 MLPs !!!

//******************************************************************************
class Angle
: public Originator<AngleState const>
, public Visitable<Angle, EntityVisitor>
, public Entity
, public IConflictOrigin
, public IMeshLineOrigin {
public:

	Plane const plane;
	Point const p;
	Edge* const e0;
	Edge* const e1;
	Coord const angle; // In degree.
//	ViewAxisSpace<Normal> normals; // from e0 & e1, assuming not colinear : if H or V -> take, else -> NONE -> HALFS

	Angle(Point const& p, Edge* e0, Edge* e1, Timepoint* t);
};

} // namespace domain
