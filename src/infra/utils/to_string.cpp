///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "utils/unreachable.hpp"

#include "to_string.hpp"

using namespace domain;
using namespace std;

//******************************************************************************
string to_string(Edge::Direction const direction) noexcept {
	switch(direction) {
	case Edge::Direction::XMIN: return "XMIN";
	case Edge::Direction::XMAX: return "XMAX";
	case Edge::Direction::YMIN: return "YMIN";
	case Edge::Direction::YMAX: return "YMAX";
	case Edge::Direction::NONE: return "NONE";
	default: ::unreachable();
	}
}

//******************************************************************************
string to_string(Normal const normal) noexcept {
	switch(normal) {
	case Normal::XMIN: return "XMIN";
	case Normal::XMAX: return "XMAX";
	case Normal::YMIN: return "YMIN";
	case Normal::YMAX: return "YMAX";
	case Normal::ZMIN: return "ZMIN";
	case Normal::ZMAX: return "ZMAX";
	case Normal::NONE: return "NONE";
	default: ::unreachable();
	}
}

//******************************************************************************
string to_string(MeshlinePolicy::Normal const normal) noexcept {
	switch(normal) {
	case MeshlinePolicy::Normal::MIN: return "MIN";
	case MeshlinePolicy::Normal::MAX: return "MAX";
	case MeshlinePolicy::Normal::NONE: return "NONE";
	default: ::unreachable();
	}
}

//******************************************************************************
string to_string(MeshlinePolicy::Policy const policy) noexcept {
	switch(policy) {
	case MeshlinePolicy::Policy::ONELINE: return "ONELINE";
	case MeshlinePolicy::Policy::HALFS: return "HALFS";
	case MeshlinePolicy::Policy::THIRDS: return "THIRDS";
	default: ::unreachable();
	}
}

//******************************************************************************
string to_string(Polygon::Rotation const rotation) noexcept {
	switch(rotation) {
	case Polygon::Rotation::CW: return "CW";
	case Polygon::Rotation::CCW: return "CCW";
	case Polygon::Rotation::COLINEAR: return "COLINEAR";
	default: ::unreachable();
	}
}

//******************************************************************************
string to_string(Segment::Axis const axis) noexcept {
	switch(axis) {
	case Segment::Axis::H: return "H";
	case Segment::Axis::V: return "V";
	case Segment::Axis::DIAGONAL: return "DIAGONAL";
	case Segment::Axis::POINT: return "POINT";
	default: ::unreachable();
	}
}

//******************************************************************************
string to_string(Axis const axis) noexcept {
	switch(axis) {
	case X: return "X";
	case Y: return "Y";
	case Z: return "Z";
	default: ::unreachable();
	}
}

//******************************************************************************
string to_string(Plane const plane) noexcept {
	switch(plane) {
	case YZ: return "YZ";
	case ZX: return "ZX";
	case XY: return "XY";
	default: ::unreachable();
	}
}
