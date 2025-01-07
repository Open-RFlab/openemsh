///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QColor>

#include <vector>

#include "ui/qt/utils/nodegraph/container.hpp"
#include "ui/qt/utils/nodegraph/rect.hpp"
#include "ui/qt/utils/nodegraph/wire.hpp"
#include "processing_axis.hpp"
#include "processing_conflict_colinear_edges.hpp"
#include "processing_edge.hpp"
#include "processing_plane.hpp"
#include "processing_polygon.hpp"

namespace ui::qt {

//******************************************************************************
struct ProcessingStyle {
	QString name;

	QColor background = QColor(57, 57, 57);


	QColor node_background_regular              = QColor(27, 27, 27);
	QColor node_background_highlighted          = node_background_regular;
	QColor node_background_selected             = node_background_regular;
	QColor node_background_selected_highlighted = node_background_selected;

	QColor node_title_background_regular              = QColor(33, 33, 33);
	QColor node_title_background_highlighted          = node_title_background_regular;
	QColor node_title_background_selected             = node_title_background_regular;
	QColor node_title_background_selected_highlighted = node_title_background_selected;

	QColor node_title_text_regular              = Qt::white;
	QColor node_title_text_highlighted          = node_title_text_regular;
	QColor node_title_text_selected             = Qt::black;
	QColor node_title_text_selected_highlighted = node_title_text_selected;

	qreal node_radius = 10;


	QColor container_nested_zone_regular              = background;
	QColor container_nested_zone_highlighted          = container_nested_zone_regular;
	QColor container_nested_zone_selected             = container_nested_zone_regular;
	QColor container_nested_zone_selected_highlighted = container_nested_zone_selected;

	qreal container_nested_zone_opacity = 0.5;


	QColor wire_regular              = Qt::green;
	QColor wire_highlighted          = QColor(Qt::green).lighter();
	QColor wire_selected             = wire_regular;
	QColor wire_selected_highlighted = wire_regular;

	qreal wire_width_regular              = 2;
	qreal wire_width_highlighted          = 4;
	qreal wire_width_selected             = wire_width_regular;
	qreal wire_width_selected_highlighted = wire_width_highlighted;


	QColor port_text_regular              = Qt::white;
	QColor port_text_highlighted          = port_text_regular;
	QColor port_text_selected             = port_text_regular;
	QColor port_text_selected_highlighted = port_text_selected;

	QColor port_contour_regular              = Qt::black;
	QColor port_contour_highlighted          = port_contour_regular;
	QColor port_contour_selected             = port_contour_regular;
	QColor port_contour_selected_highlighted = port_contour_selected;

	QColor port_fill_regular              = QColor(255, 119, 0);
	QColor port_fill_highlighted          = port_fill_regular;
	QColor port_fill_selected             = port_fill_regular;
	QColor port_fill_selected_highlighted = port_fill_selected;

	qreal port_radius = 5;


	QColor text_normal_regular              = Qt::white;
	QColor text_normal_highlighted          = text_normal_regular;
	QColor text_normal_selected             = text_normal_regular;
	QColor text_normal_selected_highlighted = text_normal_selected;

	QColor text_enabled_regular              = Qt::darkGreen;
	QColor text_enabled_highlighted          = text_enabled_regular;
	QColor text_enabled_selected             = text_enabled_regular;
	QColor text_enabled_selected_highlighted = text_enabled_selected;

	QColor text_enabled_for_sure_regular              = Qt::green;
	QColor text_enabled_for_sure_highlighted          = text_enabled_for_sure_regular;
	QColor text_enabled_for_sure_selected             = text_enabled_for_sure_regular;
	QColor text_enabled_for_sure_selected_highlighted = text_enabled_for_sure_selected;

	QColor text_disabled_regular              = Qt::darkRed;
	QColor text_disabled_highlighted          = text_disabled_regular;
	QColor text_disabled_selected             = text_disabled_regular;
	QColor text_disabled_selected_highlighted = text_disabled_selected;
};

//******************************************************************************
class ProcessingStyleSelector {
	ProcessingStyle style;

	nodegraph::Wire::Params wire;
	nodegraph::Port::Params port;
	nodegraph::Node::Params node;
	nodegraph::Rect::Params nested_zone;
	nodegraph::Text::Params title;
	nodegraph::Text::Params text_normal;
	nodegraph::Text::Params text_enabled;
	nodegraph::Text::Params text_enabled_for_sure;
	nodegraph::Text::Params text_disabled;
	ProcessingEdge::Params const edge;
	ProcessingPolygon::Params const polygon;
	ProcessingPlane::Params const plane;
	ProcessingAxis::Params const axis;
	ProcessingConflictColinearEdges::Params const conflict_ce;

	nodegraph::Wire::Params make_wire(ProcessingStyle const& style) const;
	nodegraph::Port::Params make_port(ProcessingStyle const& style) const;
	nodegraph::Node::Params make_node(ProcessingStyle const& style) const;
	nodegraph::Rect::Params make_nested_zone(ProcessingStyle const& style) const;
	nodegraph::Text::Params make_title(ProcessingStyle const& style) const;
	nodegraph::Text::Params make_text_normal(ProcessingStyle const& style) const;
	nodegraph::Text::Params make_text_enabled(ProcessingStyle const& style) const;
	nodegraph::Text::Params make_text_enabled_for_sure(ProcessingStyle const& style) const;
	nodegraph::Text::Params make_text_disabled(ProcessingStyle const& style) const;
	ProcessingEdge::Params make_edge() const;
	ProcessingPolygon::Params make_polygon() const;
	ProcessingPlane::Params make_plane() const;
	ProcessingAxis::Params make_axis() const;
	ProcessingConflictColinearEdges::Params make_conflict_ce() const;

public:
	static std::vector<ProcessingStyle> const available_styles;

	ProcessingStyle const& get_style() const;
	nodegraph::Wire::Params const& get_wire() const;
	nodegraph::Port::Params const& get_port() const;
	nodegraph::Node::Params const& get_node() const;
	nodegraph::Rect::Params const& get_nested_zone() const;
	nodegraph::Text::Params const& get_title() const;
	nodegraph::Text::Params const& get_text_normal() const;
	nodegraph::Text::Params const& get_text_enabled() const;
	nodegraph::Text::Params const& get_text_enabled_for_sure() const;
	nodegraph::Text::Params const& get_text_disabled() const;
	ProcessingEdge::Params const& get_edge() const;
	ProcessingPolygon::Params const& get_polygon() const;
	ProcessingPlane::Params const& get_plane() const;
	ProcessingAxis::Params const& get_axis() const;
	ProcessingConflictColinearEdges::Params const& get_conflict_ce() const;

	ProcessingStyleSelector();
	ProcessingStyleSelector(ProcessingStyle style);

	ProcessingStyleSelector& operator=(ProcessingStyle const& style);
	ProcessingStyleSelector& operator=(ProcessingStyle&& style);
};

} // namespace ui::qt
