///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <cstdlib>

#include "to_string.hpp"

using namespace std;

//******************************************************************************
string to_string(Edge::Direction direction) {
	switch(direction) {
	case Edge::Direction::XMIN: return "XMIN";
	case Edge::Direction::XMAX: return "XMAX";
	case Edge::Direction::YMIN: return "YMIN";
	case Edge::Direction::YMAX: return "YMAX";
	case Edge::Direction::NONE: return "NONE";
	default: abort();
	}
}

//******************************************************************************
string to_string(GridAxis const axis) {
	switch(axis) {
	case H: return "H";
	case V: return "V";
	default: abort();
	}
}

//******************************************************************************
string to_string(Normal const normal) {
	switch(normal) {
	case Normal::XMIN: return "XMIN";
	case Normal::XMAX: return "XMAX";
	case Normal::YMIN: return "YMIN";
	case Normal::YMAX: return "YMAX";
	case Normal::NONE: return "NONE";
	default: abort();
	}
}

//******************************************************************************
string to_string(MeshlinePolicy::Policy const policy) {
	switch(policy) {
	case MeshlinePolicy::Policy::ONELINE: return "ONELINE";
	case MeshlinePolicy::Policy::HALFS: return "HALFS";
	case MeshlinePolicy::Policy::THIRDS: return "THIRDS";
	default: abort();
	}
}

//******************************************************************************
string to_string(Polygon::Rotation const rotation) {
	switch(rotation) {
	case Polygon::Rotation::CW: return "CW";
	case Polygon::Rotation::CCW: return "CCW";
	case Polygon::Rotation::COLINEAR: return "COLINEAR";
	default: abort();
	}
}

//******************************************************************************
string to_string(Segment::Axis const axis) {
	switch(axis) {
	case Segment::Axis::H: return "H";
	case Segment::Axis::V: return "V";
	case Segment::Axis::DIAGONAL: return "DIAGONAL";
	case Segment::Axis::POINT: return "POINT";
	default: abort();
	}
}
