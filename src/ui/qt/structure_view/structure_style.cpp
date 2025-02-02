///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "structure_style.hpp"

namespace ui::qt {

//******************************************************************************
StructureStyleSelector::StructureStyleSelector()
: edge(make_edge(style))
, polygon_shape(make_polygon_shape(style))
, polygon_port(make_polygon_port(style))
, polygon_ground(make_polygon_ground(style))
, polygon_substrate(make_polygon_substrate(style))
, meshline(make_meshline(style))
{}

//******************************************************************************
StructureStyleSelector::StructureStyleSelector(StructureStyle style)
: style(style)
, edge(make_edge(style))
, polygon_shape(make_polygon_shape(style))
, polygon_port(make_polygon_port(style))
, polygon_ground(make_polygon_ground(style))
, polygon_substrate(make_polygon_substrate(style))
, meshline(make_meshline(style))
{}

//******************************************************************************
StructureStyleSelector& StructureStyleSelector::operator=(StructureStyle const& style) {
	this->style = style;
	edge = make_edge(style);
	polygon_shape = make_polygon_shape(style);
	polygon_port = make_polygon_port(style);
	polygon_ground = make_polygon_ground(style);
	polygon_substrate = make_polygon_substrate(style);
	meshline = make_meshline(style);
	return *this;
}

//******************************************************************************
StructureStyleSelector& StructureStyleSelector::operator=(StructureStyle&& style) {
	this->style = style;
	edge = make_edge(style);
	polygon_shape = make_polygon_shape(style);
	polygon_port = make_polygon_port(style);
	polygon_ground = make_polygon_ground(style);
	polygon_substrate = make_polygon_substrate(style);
	meshline = make_meshline(style);
	return *this;
}

//******************************************************************************
MAKER_DEF(StructureStyleSelector, edge, StructureStyle const& style) {
	return {
		.regular = QPen(style.edge_regular, style.edge_width_regular, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.selected = QPen(style.edge_selected, style.edge_width_selected, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.regular_hovered = QPen(style.edge_regular_hovered, style.edge_width_regular_hovered, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.selected_hovered = QPen(style.edge_selected_hovered, style.edge_width_selected_hovered, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin)
	};
}

//******************************************************************************
MAKER_DEF(StructureStyleSelector, polygon_shape, StructureStyle const& style) {
	return {
		.contour_regular = QPen(style.polygon_shape_contour_regular, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.contour_selected = QPen(style.polygon_shape_contour_selected, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.contour_regular_hovered = QPen(style.polygon_shape_contour_regular_hovered, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.contour_selected_hovered = QPen(style.polygon_shape_contour_selected_hovered, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.fill_regular = QBrush(style.polygon_shape_fill_regular),
		.fill_selected = QBrush(style.polygon_shape_fill_selected),
		.fill_regular_hovered = QBrush(style.polygon_shape_fill_regular_hovered),
		.fill_selected_hovered = QBrush(style.polygon_shape_fill_selected_hovered)
	};
}

//******************************************************************************
MAKER_DEF(StructureStyleSelector, polygon_port, StructureStyle const& style) {
	return {
		.contour_regular = QPen(style.polygon_port_contour_regular, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.contour_selected = QPen(style.polygon_port_contour_selected, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.contour_regular_hovered = QPen(style.polygon_port_contour_regular_hovered, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.contour_selected_hovered = QPen(style.polygon_port_contour_selected_hovered, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.fill_regular = QBrush(style.polygon_port_fill_regular),
		.fill_selected = QBrush(style.polygon_port_fill_selected),
		.fill_regular_hovered = QBrush(style.polygon_port_fill_regular_hovered),
		.fill_selected_hovered = QBrush(style.polygon_port_fill_selected_hovered)
	};
}

//******************************************************************************
MAKER_DEF(StructureStyleSelector, polygon_ground, StructureStyle const& style) {
	return {
		.contour_regular = QPen(style.polygon_ground_contour_regular, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.contour_selected = QPen(style.polygon_ground_contour_selected, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.contour_regular_hovered = QPen(style.polygon_ground_contour_regular_hovered, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.contour_selected_hovered = QPen(style.polygon_ground_contour_selected_hovered, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.fill_regular = QBrush(style.polygon_ground_fill_regular),
		.fill_selected = QBrush(style.polygon_ground_fill_selected),
		.fill_regular_hovered = QBrush(style.polygon_ground_fill_regular_hovered),
		.fill_selected_hovered = QBrush(style.polygon_ground_fill_selected_hovered)
	};
}

//******************************************************************************
MAKER_DEF(StructureStyleSelector, polygon_substrate, StructureStyle const& style) {
	return {
		.contour_regular = QPen(style.polygon_substrate_contour_regular, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.contour_selected = QPen(style.polygon_substrate_contour_selected, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.contour_regular_hovered = QPen(style.polygon_substrate_contour_regular_hovered, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.contour_selected_hovered = QPen(style.polygon_substrate_contour_selected_hovered, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.fill_regular = QBrush(style.polygon_substrate_fill_regular),
		.fill_selected = QBrush(style.polygon_substrate_fill_selected),
		.fill_regular_hovered = QBrush(style.polygon_substrate_fill_regular_hovered),
		.fill_selected_hovered = QBrush(style.polygon_substrate_fill_selected_hovered)
	};
}

//******************************************************************************
MAKER_DEF(StructureStyleSelector, meshline, StructureStyle const& style) {
	return {
		.regular = QPen(style.meshline_regular, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.selected = QPen(style.meshline_selected, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.regular_hovered = QPen(style.meshline_regular_hovered, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin),
		.selected_hovered = QPen(style.meshline_selected_hovered, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin)
	};
}

} // namespace ui::qt
