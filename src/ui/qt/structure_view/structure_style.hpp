///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QColor>

#include "utils/class_utils.hpp"
#include "structure_edge.hpp"
#include "structure_meshline.hpp"
#include "structure_polygon.hpp"

namespace ui::qt {

//******************************************************************************
struct StructureStyle {
	QColor background = Qt::white;


	qreal edge_width_regular = 0;
	qreal edge_width_selected = edge_width_regular;
	qreal edge_width_regular_hovered = edge_width_regular * 2;
	qreal edge_width_selected_hovered = edge_width_selected * 2;

	QColor edge_regular = Qt::transparent;
	QColor edge_selected = Qt::red;
	QColor edge_regular_hovered = edge_regular;
	QColor edge_selected_hovered = edge_selected;


	QColor polygon_shape_contour_regular = Qt::black;
	QColor polygon_shape_contour_selected = polygon_shape_contour_regular;
	QColor polygon_shape_contour_regular_hovered = polygon_shape_contour_regular;
	QColor polygon_shape_contour_selected_hovered = polygon_shape_contour_selected;

	QColor polygon_shape_fill_regular = Qt::cyan;
	QColor polygon_shape_fill_selected = polygon_shape_fill_regular;
	QColor polygon_shape_fill_regular_hovered = polygon_shape_fill_regular;
	QColor polygon_shape_fill_selected_hovered = polygon_shape_fill_selected;

	QColor polygon_port_contour_regular = Qt::black;
	QColor polygon_port_contour_selected = polygon_port_contour_regular;
	QColor polygon_port_contour_regular_hovered = polygon_port_contour_regular;
	QColor polygon_port_contour_selected_hovered = polygon_port_contour_selected;

	QColor polygon_port_fill_regular = Qt::yellow;
	QColor polygon_port_fill_selected = polygon_port_fill_regular;
	QColor polygon_port_fill_regular_hovered = polygon_port_fill_regular;
	QColor polygon_port_fill_selected_hovered = polygon_port_fill_selected;

	QColor polygon_ground_contour_regular = Qt::black;
	QColor polygon_ground_contour_selected = polygon_ground_contour_regular;
	QColor polygon_ground_contour_regular_hovered = polygon_ground_contour_regular;
	QColor polygon_ground_contour_selected_hovered = polygon_ground_contour_selected;

	QColor polygon_ground_fill_regular = QColor(99, 78, 45);
	QColor polygon_ground_fill_selected = polygon_ground_fill_regular;
	QColor polygon_ground_fill_regular_hovered = polygon_ground_fill_regular;
	QColor polygon_ground_fill_selected_hovered = polygon_ground_fill_selected;

	QColor polygon_substrate_contour_regular = Qt::black;
	QColor polygon_substrate_contour_selected = polygon_substrate_contour_regular;
	QColor polygon_substrate_contour_regular_hovered = polygon_substrate_contour_regular;
	QColor polygon_substrate_contour_selected_hovered = polygon_substrate_contour_selected;

	QColor polygon_substrate_fill_regular = Qt::darkGreen;
	QColor polygon_substrate_fill_selected = polygon_substrate_fill_regular;
	QColor polygon_substrate_fill_regular_hovered = polygon_substrate_fill_regular;
	QColor polygon_substrate_fill_selected_hovered = polygon_substrate_fill_selected;


	QColor meshline_regular = Qt::black;
	QColor meshline_selected = Qt::red;
	QColor meshline_regular_hovered = meshline_regular;
	QColor meshline_selected_hovered = meshline_selected;
};

//******************************************************************************
class StructureStyleSelector {
	StructureStyle style;

	StructureEdge::Params edge;
	StructurePolygon::Params polygon_shape;
	StructurePolygon::Params polygon_port;
	StructurePolygon::Params polygon_ground;
	StructurePolygon::Params polygon_substrate;
	StructureMeshline::Params meshline;

	MAKER_DECL(edge, StructureStyle const& style);
	MAKER_DECL(polygon_shape, StructureStyle const& style);
	MAKER_DECL(polygon_port, StructureStyle const& style);
	MAKER_DECL(polygon_ground, StructureStyle const& style);
	MAKER_DECL(polygon_substrate, StructureStyle const& style);
	MAKER_DECL(meshline, StructureStyle const& style);

public:
	GETTER(style)
	GETTER(edge)
	GETTER(polygon_shape)
	GETTER(polygon_port)
	GETTER(polygon_ground)
	GETTER(polygon_substrate)
	GETTER(meshline)

	StructureStyleSelector();
	StructureStyleSelector(StructureStyle style);

	StructureStyleSelector& operator=(StructureStyle const& style);
	StructureStyleSelector& operator=(StructureStyle&& style);
};

} // namespace ui::qt
