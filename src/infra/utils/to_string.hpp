///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/polygon.hpp"
#include "domain/geometrics/space.hpp"
#include "domain/geometrics/normal.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "domain/meshline_policy_manager.hpp"

//******************************************************************************
std::string to_string(domain::Edge::Direction direction) noexcept;
std::string to_string(domain::MeshlinePolicy::Normal normal) noexcept;
std::string to_string(domain::MeshlinePolicy::Policy policy) noexcept;
std::string to_string(domain::Polygon::Rotation rotation) noexcept;
std::string to_string(domain::Segment::Axis axis) noexcept;
std::string to_string(domain::Axis axis) noexcept;
std::string to_string(domain::Normal normal) noexcept;
std::string to_string(domain::Plane plane) noexcept;
