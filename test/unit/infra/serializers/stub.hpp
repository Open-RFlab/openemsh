///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "domain/board.hpp"

//******************************************************************************
std::shared_ptr<domain::Board> create_stub() {
	using namespace domain;
	using RangeZ = Polygon::RangeZ;
	auto material = std::make_shared<Material>(Material::Type::CONDUCTOR, "");

	Board::Builder builder;
	builder.add_polygon_from_box(XY, material, "MS1", 0, RangeZ { 0, 0 }, { 31.1, -44.8024 }, { 51.3539, -42.0164 });
	builder.add_polygon_from_box(XY, material, "MS2", 0, RangeZ { 0, 0 }, { 120.029, -44.8024 }, { 140.283, -42.0164 });
	builder.add_polygon_from_box(XY, material, "MS3", 0, RangeZ { 0, 0 }, { 51.6077, -42.0164 }, { 51.3539, -31 });
	builder.add_polygon_from_box(XY, material, "MS4", 0, RangeZ { 0, 0 }, { 120.024, -42.0164 }, { 119.78, -31 });
	builder.add_polygon_from_box(XY, material, "MS5", 0, RangeZ { 0, 0 }, { 109.208, -43.9276 }, { 119.775, -42.8913 });
	builder.add_polygon_from_box(XY, material, "MS6", 0, RangeZ { 0, 0 }, { 51.6077, -43.9276 }, { 62.1753, -42.8913 });
	builder.add_polygon_from_box(XY, material, "MS7", 0, RangeZ { 0, 0 }, { 70.3673, -42.8674 }, { 62.1753, -33.2952 });
	builder.add_polygon_from_box(XY, material, "MS8", 0, RangeZ { 0, 0 }, { 109.208, -42.8674 }, { 101.016, -33.2952 });
	builder.add_polygon_from_box(XY, material, "MS9", 0, RangeZ { 0, 0 }, { 90.96, -42.8674 }, { 80.4229, -33.1334 });
	builder.add_polygon_from_box(XY, material, "MS10", 0, RangeZ { 0, 0 }, { 70.3673, -43.9514 }, { 80.4229, -42.8674 });
	builder.add_polygon_from_box(XY, material, "MS11", 0, RangeZ { 0, 0 }, { 90.96, -43.9514 }, { 101.016, -42.8674 });
	builder.add_polygon(XY, material, "MS12", 0, RangeZ { 0, 0 }, {
		{ 51.6077, -42.0164 },
		{ 51.3539, -42.0164 },
		{ 51.3539, -44.8024 },
		{ 51.4808, -44.8024 },
		{ 51.4808, -43.9276 },
		{ 51.6077, -43.9276 }});
	builder.add_polygon(XY, material, "MS13", 0, RangeZ { 0, 0 }, {
		{ 70.3673, -42.8674 },
		{ 62.1753, -42.8674 },
		{ 62.1753, -43.9276 },
		{ 66.2713, -43.9276 },
		{ 66.2713, -43.9514 },
		{ 70.3673, -43.9514 }});
	builder.add_polygon(XY, material, "MS14", 0, RangeZ { 0, 0 }, {
		{ 90.96, -42.8674 },
		{ 80.4229, -42.8674 },
		{ 80.4229, -43.9514 },
		{ 85.6915, -43.9514 },
		{ 85.6915, -43.9514 },
		{ 90.96, -43.9514 }});
	builder.add_polygon(XY, material, "MS15", 0, RangeZ { 0, 0 }, {
		{ 109.208, -42.8674 },
		{ 101.016, -42.8674 },
		{ 101.016, -43.9514 },
		{ 105.112, -43.9514 },
		{ 105.112, -43.9276 },
		{ 109.208, -43.9276 }});
	builder.add_polygon(XY, material, "MS16", 0, RangeZ { 0, 0 }, {
		{ 120.029, -42.0164 },
		{ 119.775, -42.0164 },
		{ 119.775, -43.9276 },
		{ 119.902, -43.9276 },
		{ 119.902, -44.8024 },
		{ 120.029, -44.8024 }});

	return builder.build();
}
