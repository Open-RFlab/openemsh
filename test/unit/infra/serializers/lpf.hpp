///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <memory>

#include "domain/board.hpp"

//******************************************************************************
std::unique_ptr<domain::Board> create_lpf() {
	using namespace domain;
	using Type = Polygon::Type;

	Board::Builder builder;
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS1", { 16.1, -26.5 }, { 20.6, -26 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS2", { 20.6, -36.5 }, { 22.1, -16 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS3", { 22.1, -26.5 }, { 40.1, -26 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS4", { 40.1, -36.5 }, { 42.6, -16 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS5", { 42.6, -26.5 }, { 60.6, -26 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS6", { 60.6, -36.5 }, { 62.1, -16 });
	builder.add_polygon_from_box(XY, Type::SHAPE, "MS7", { 62.1, -26.5 }, { 66.6, -26 });

	return builder.build();
}
