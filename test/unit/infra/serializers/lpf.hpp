///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <memory>

#include "domain/board.hpp"

//******************************************************************************
std::shared_ptr<domain::Board> create_lpf() {
	using namespace domain;
	using RangeZ = Polygon::RangeZ;
	auto material = std::make_shared<Material>(Material::Type::CONDUCTOR, "");

	Board::Builder builder;
	builder.add_polygon_from_box(XY, material, "MS1", 0, RangeZ { 0, 0 }, { 16.1, -26.5 }, { 20.6, -26 });
	builder.add_polygon_from_box(XY, material, "MS2", 0, RangeZ { 0, 0 }, { 20.6, -36.5 }, { 22.1, -16 });
	builder.add_polygon_from_box(XY, material, "MS3", 0, RangeZ { 0, 0 }, { 22.1, -26.5 }, { 40.1, -26 });
	builder.add_polygon_from_box(XY, material, "MS4", 0, RangeZ { 0, 0 }, { 40.1, -36.5 }, { 42.6, -16 });
	builder.add_polygon_from_box(XY, material, "MS5", 0, RangeZ { 0, 0 }, { 42.6, -26.5 }, { 60.6, -26 });
	builder.add_polygon_from_box(XY, material, "MS6", 0, RangeZ { 0, 0 }, { 60.6, -36.5 }, { 62.1, -16 });
	builder.add_polygon_from_box(XY, material, "MS7", 0, RangeZ { 0, 0 }, { 62.1, -26.5 }, { 66.6, -26 });

	return builder.build();
}
