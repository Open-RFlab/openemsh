///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/polygon.hpp"
#include "domain/geometrics/types.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "domain/meshline_policy_manager.hpp"

//******************************************************************************
std::string to_string(Edge::Direction const direction);
std::string to_string(GridAxis const axis);
std::string to_string(Normal const normal);
std::string to_string(MeshlinePolicy::Policy const policy);
std::string to_string(Polygon::Rotation const rotation);
std::string to_string(Segment::Axis const axis);
