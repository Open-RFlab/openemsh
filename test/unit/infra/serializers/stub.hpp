///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "domain/board.hpp"

//******************************************************************************
std::unique_ptr<domain::Board> create_stub() {
	using namespace domain;
	using Type = Polygon::Type;

	Board::Builder builder;
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS1", { 31.1, -44.8024 }, { 51.3539, -42.0164 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS2", { 120.029, -44.8024 }, { 140.283, -42.0164 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS3", { 51.6077, -42.0164 }, { 51.3539, -31 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS4", { 120.024, -42.0164 }, { 119.78, -31 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS5", { 109.208, -43.9276 }, { 119.775, -42.8913 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS6", { 51.6077, -43.9276 }, { 62.1753, -42.8913 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS7", { 70.3673, -42.8674 }, { 62.1753, -33.2952 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS8", { 109.208, -42.8674 }, { 101.016, -33.2952 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS9", { 90.96, -42.8674 }, { 80.4229, -33.1334 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS10", { 70.3673, -43.9514 }, { 80.4229, -42.8674 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS11", { 90.96, -43.9514 }, { 101.016, -42.8674 });
	builder.add_polygon(XY, Type::SHAPE, "MS12", {
		{ 51.6077, -42.0164 },
		{ 51.3539, -42.0164 },
		{ 51.3539, -44.8024 },
		{ 51.4808, -44.8024 },
		{ 51.4808, -43.9276 },
		{ 51.6077, -43.9276 }});
	builder.add_polygon(XY, Type::SHAPE, "MS13", {
		{ 70.3673, -42.8674 },
		{ 62.1753, -42.8674 },
		{ 62.1753, -43.9276 },
		{ 66.2713, -43.9276 },
		{ 66.2713, -43.9514 },
		{ 70.3673, -43.9514 }});
	builder.add_polygon(XY, Type::SHAPE, "MS14", {
		{ 90.96, -42.8674 },
		{ 80.4229, -42.8674 },
		{ 80.4229, -43.9514 },
		{ 85.6915, -43.9514 },
		{ 85.6915, -43.9514 },
		{ 90.96, -43.9514 }});
	builder.add_polygon(XY, Type::SHAPE, "MS15", {
		{ 109.208, -42.8674 },
		{ 101.016, -42.8674 },
		{ 101.016, -43.9514 },
		{ 105.112, -43.9514 },
		{ 105.112, -43.9276 },
		{ 109.208, -43.9276 }});
	builder.add_polygon(XY, Type::SHAPE, "MS16", {
		{ 120.029, -42.0164 },
		{ 119.775, -42.0164 },
		{ 119.775, -43.9276 },
		{ 119.902, -43.9276 },
		{ 119.902, -44.8024 },
		{ 120.029, -44.8024 }});

	return builder.build();
}
