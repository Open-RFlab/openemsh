///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "processing_style.hpp"

namespace ui::qt {

//******************************************************************************
std::vector<ProcessingStyle> const ProcessingStyleSelector::available_styles {
	{
		.name = "Main",
		.background = QColor(57, 57, 57),
		.node_background_regular             = QColor(27, 27, 27),
//		.node_background_highlighted         = node_background_regular,
		.node_background_selected            = Qt::white,
//		.node_background_regular_hovered     = node_background_regular.lighter(),
//		.node_background_highlighted_hovered = node_background_highlighted.lighter(),
		.node_background_selected_hovered    = QColor(Qt::white).darker(150),
		.node_title_background_regular             = QColor(33, 33, 33),
		.node_title_background_highlighted         = QColor(222, 222, 222),
		.node_title_background_selected            = QColor(222, 222, 222),
		.node_title_background_regular_hovered     = QColor(33, 33, 33).lighter(150),
		.node_title_background_highlighted_hovered = QColor(222, 222, 222).darker(150),
		.node_title_background_selected_hovered    = QColor(222, 222, 222).darker(150),
		.node_title_text_regular             = Qt::white,
		.node_title_text_highlighted         = Qt::black,
		.node_title_text_selected            = Qt::black,
//		.node_title_text_regular_hovered     = node_title_text_regular,
//		.node_title_text_highlighted_hovered = node_title_text_highlighted,
//		.node_title_text_selected_hovered    = node_title_text_selected,
//		.node_radius = 10,
//		.container_nested_zone_regular             = background,
//		.container_nested_zone_highlighted         = container_nested_zone_regular,
//		.container_nested_zone_selected            = container_nested_zone_regular,
//		.container_nested_zone_regular_hovered     = container_nested_zone_regular,
//		.container_nested_zone_highlighted_hovered = container_nested_zone_highlighted,
//		.container_nested_zone_selected_hovered    = container_nested_zone_selected,
//		.container_nested_zone_opacity = 0.5,
//		.wire_regular             = Qt::green,
//		.wire_highlighted         = wire_regular,
//		.wire_selected            = wire_regular,
//		.wire_regular_hovered     = wire_regular.lighter(),
//		.wire_highlighted_hovered = wire_highlighted.lighter(),
//		.wire_selected_hovered    = wire_selected.lighter(),
//		.wire_width_regular             = 2,
//		.wire_width_highlighted         = wire_width_regular,
//		.wire_width_selected            = wire_width_regular,
//		.wire_width_regular_hovered     = wire_width_regular * 2,
//		.wire_width_highlighted_hovered = wire_width_highlighted * 2,
//		.wire_width_selected_hovered    = wire_width_selected * 2,
		.port_text_regular             = Qt::white,
//		.port_text_highlighted         = port_text_regular,
		.port_text_selected            = Qt::black,
//		.port_text_regular_hovered     = port_text_regular,
//		.port_text_highlighted_hovered = port_text_highlighted,
//		.port_text_selected_hovered    = port_text_selected,
//		.port_contour_regular             = Qt::black,
//		.port_contour_highlighted         = port_contour_regular,
//		.port_contour_selected            = port_contour_regular,
//		.port_contour_regular_hovered     = port_contour_regular,
//		.port_contour_highlighted_hovered = port_contour_highlighted,
//		.port_contour_selected_hovered    = port_contour_selected,
//		.port_fill_regular             = QColor(255, 119, 0),
//		.port_fill_highlighted         = port_fill_regular,
//		.port_fill_selected            = port_fill_regular,
//		.port_fill_regular_hovered     = port_fill_regular,
//		.port_fill_highlighted_hovered = port_fill_highlighted,
//		.port_fill_selected_hovered    = port_fill_selected,
//		.port_radius = 5,
		.text_normal_regular             = Qt::white,
//		.text_normal_highlighted         = text_normal_regular,
		.text_normal_selected            = Qt::black,
//		.text_normal_regular_hovered     = text_normal_regular,
//		.text_normal_highlighted_hovered = text_normal_highlighted,
//		.text_normal_selected_hovered    = text_normal_selected,
//		.text_enabled_regular             = Qt::darkGreen,
//		.text_enabled_highlighted         = text_enabled_regular,
//		.text_enabled_selected            = text_enabled_regular,
//		.text_enabled_regular_hovered     = text_enabled_regular,
//		.text_enabled_highlighted_hovered = text_enabled_highlighted,
//		.text_enabled_selected_hovered    = text_enabled_selected,
//		.text_enabled_for_sure_regular             = Qt::green,
//		.text_enabled_for_sure_highlighted         = text_enabled_for_sure_regular,
//		.text_enabled_for_sure_selected            = text_enabled_for_sure_regular,
//		.text_enabled_for_sure_regular_hovered     = text_enabled_for_sure_regular,
//		.text_enabled_for_sure_highlighted_hovered = text_enabled_for_sure_highlighted,
//		.text_enabled_for_sure_selected_hovered    = text_enabled_for_sure_selected,
//		.text_disabled_regular             = Qt::darkRed,
//		.text_disabled_highlighted         = text_disabled_regular,
//		.text_disabled_selected            = text_disabled_regular,
//		.text_disabled_regular_hovered     = text_disabled_regular,
//		.text_disabled_highlighted_hovered = text_disabled_highlighted,
//		.text_disabled_selected_hovered    = text_disabled_selected
	}
};


//******************************************************************************
ProcessingStyleSelector::ProcessingStyleSelector()
: wire(make_wire(style))
, port(make_port(style))
, node(make_node(style))
, nested_zone(make_nested_zone(style))
, title(make_title(style))
, text_normal(make_text_normal(style))
, text_enabled(make_text_enabled(style))
, text_enabled_for_sure(make_text_enabled_for_sure(style))
, text_disabled(make_text_disabled(style))
, edge(make_edge())
, polygon(make_polygon())
, plane(make_plane())
, axis(make_axis())
, meshline_policy(make_meshline_policy())
, interval(make_interval())
, meshline(make_meshline())
, conflict_ce(make_conflict_ce())
, conflict_eip(make_conflict_eip())
, conflict_tcmlp(make_conflict_tcmlp())
{}

//******************************************************************************
ProcessingStyleSelector::ProcessingStyleSelector(ProcessingStyle style)
: style(style)
, wire(make_wire(style))
, port(make_port(style))
, node(make_node(style))
, nested_zone(make_nested_zone(style))
, title(make_title(style))
, text_normal(make_text_normal(style))
, text_enabled(make_text_enabled(style))
, text_enabled_for_sure(make_text_enabled_for_sure(style))
, text_disabled(make_text_disabled(style))
, edge(make_edge())
, polygon(make_polygon())
, plane(make_plane())
, axis(make_axis())
, meshline_policy(make_meshline_policy())
, interval(make_interval())
, meshline(make_meshline())
, conflict_ce(make_conflict_ce())
, conflict_eip(make_conflict_eip())
, conflict_tcmlp(make_conflict_tcmlp())
{}

//******************************************************************************
ProcessingStyleSelector& ProcessingStyleSelector::operator=(ProcessingStyle const& style) {
	this->style = style;
	wire = make_wire(style);
	port = make_port(style);
	node = make_node(style);
	nested_zone = make_nested_zone(style);
	title = make_title(style);
	text_normal = make_text_normal(style);
	text_enabled = make_text_enabled(style);
	text_enabled_for_sure = make_text_enabled_for_sure(style);
	text_disabled = make_text_disabled(style);
	return *this;
}

//******************************************************************************
ProcessingStyleSelector& ProcessingStyleSelector::operator=(ProcessingStyle&& style) {
	this->style = style;
	wire = make_wire(style);
	port = make_port(style);
	node = make_node(style);
	nested_zone = make_nested_zone(style);
	title = make_title(style);
	text_normal = make_text_normal(style);
	text_enabled = make_text_enabled(style);
	text_enabled_for_sure = make_text_enabled_for_sure(style);
	text_disabled = make_text_disabled(style);
	return *this;
}

//******************************************************************************
nodegraph::Wire::Params ProcessingStyleSelector::make_wire(ProcessingStyle const& style) const {
	return {
		.regular = QPen(style.wire_regular, style.wire_width_regular, Qt::SolidLine, Qt::RoundCap),
		.highlighted = QPen(style.wire_highlighted, style.wire_width_highlighted, Qt::SolidLine, Qt::RoundCap),
		.selected = QPen(style.wire_selected, style.wire_width_selected, Qt::SolidLine, Qt::RoundCap),
		.regular_hovered = QPen(style.wire_regular_hovered, style.wire_width_regular_hovered, Qt::SolidLine, Qt::RoundCap),
		.highlighted_hovered = QPen(style.wire_highlighted_hovered, style.wire_width_highlighted_hovered, Qt::SolidLine, Qt::RoundCap),
		.selected_hovered = QPen(style.wire_selected_hovered, style.wire_width_selected_hovered, Qt::SolidLine, Qt::RoundCap)
	};
}

//******************************************************************************
nodegraph::Port::Params ProcessingStyleSelector::make_port(ProcessingStyle const& style) const {
	return {
		.radius = style.port_radius,
		.text_regular = QPen(style.port_text_regular),
		.text_highlighted = QPen(style.port_text_highlighted),
		.text_selected = QPen(style.port_text_selected),
		.text_regular_hovered = QPen(style.port_text_regular_hovered),
		.text_highlighted_hovered = QPen(style.port_text_highlighted_hovered),
		.text_selected_hovered = QPen(style.port_text_selected_hovered),
		.contour_regular = QPen(style.port_contour_regular),
		.contour_highlighted = QPen(style.port_contour_highlighted),
		.contour_selected = QPen(style.port_contour_selected),
		.contour_regular_hovered = QPen(style.port_contour_regular_hovered),
		.contour_highlighted_hovered = QPen(style.port_contour_highlighted_hovered),
		.contour_selected_hovered = QPen(style.port_contour_selected_hovered),
		.fill_regular = QBrush(style.port_fill_regular),
		.fill_highlighted = QBrush(style.port_fill_highlighted),
		.fill_selected = QBrush(style.port_fill_selected),
		.fill_regular_hovered = QBrush(style.port_fill_regular_hovered),
		.fill_highlighted_hovered = QBrush(style.port_fill_highlighted_hovered),
		.fill_selected_hovered = QBrush(style.port_fill_selected_hovered)
	};
}

//******************************************************************************
nodegraph::Node::Params ProcessingStyleSelector::make_node(ProcessingStyle const& style) const {
	return {
		.radius = style.node_radius,
		.title_background_regular = QBrush(style.node_title_background_regular),
		.title_background_highlighted = QBrush(style.node_title_background_highlighted),
		.title_background_selected = QBrush(style.node_title_background_selected),
		.title_background_regular_hovered = QBrush(style.node_title_background_regular_hovered),
		.title_background_highlighted_hovered = QBrush(style.node_title_background_highlighted_hovered),
		.title_background_selected_hovered = QBrush(style.node_title_background_selected_hovered),
		.background_regular = QBrush(style.node_background_regular),
		.background_highlighted = QBrush(style.node_background_highlighted),
		.background_selected = QBrush(style.node_background_selected),
		.background_regular_hovered = QBrush(style.node_background_regular_hovered),
		.background_highlighted_hovered = QBrush(style.node_background_highlighted_hovered),
		.background_selected_hovered = QBrush(style.node_background_selected_hovered)
	};
}

//******************************************************************************
nodegraph::Rect::Params ProcessingStyleSelector::make_nested_zone(ProcessingStyle const& style) const {
	return {
		.opacity_regular = style.container_nested_zone_opacity,
		.opacity_highlighted = style.container_nested_zone_opacity,
		.opacity_selected = style.container_nested_zone_opacity,
		.opacity_regular_hovered = style.container_nested_zone_opacity,
		.opacity_highlighted_hovered = style.container_nested_zone_opacity,
		.opacity_selected_hovered = style.container_nested_zone_opacity,
		.fill_regular = QBrush(style.container_nested_zone_regular),
		.fill_highlighted = QBrush(style.container_nested_zone_highlighted),
		.fill_selected = QBrush(style.container_nested_zone_selected),
		.fill_regular_hovered = QBrush(style.container_nested_zone_regular_hovered),
		.fill_highlighted_hovered = QBrush(style.container_nested_zone_highlighted_hovered),
		.fill_selected_hovered = QBrush(style.container_nested_zone_selected_hovered),
		.contour_regular = QPen(style.container_nested_zone_regular),
		.contour_highlighted = QPen(style.container_nested_zone_highlighted),
		.contour_selected = QPen(style.container_nested_zone_selected),
		.contour_regular_hovered = QPen(style.container_nested_zone_regular_hovered),
		.contour_highlighted_hovered = QPen(style.container_nested_zone_highlighted_hovered),
		.contour_selected_hovered = QPen(style.container_nested_zone_selected_hovered)
	};
}

//******************************************************************************
nodegraph::Text::Params ProcessingStyleSelector::make_title(ProcessingStyle const& style) const {
	return {
		.regular = QPen(style.node_title_text_regular),
		.highlighted = QPen(style.node_title_text_highlighted),
		.selected = QPen(style.node_title_text_selected),
		.regular_hovered = QPen(style.node_title_text_regular_hovered),
		.highlighted_hovered = QPen(style.node_title_text_highlighted_hovered),
		.selected_hovered = QPen(style.node_title_text_selected_hovered)
	};
}

//******************************************************************************
nodegraph::Text::Params ProcessingStyleSelector::make_text_normal(ProcessingStyle const& style) const {
	return {
		.regular = QPen(style.text_normal_regular),
		.highlighted = QPen(style.text_normal_highlighted),
		.selected = QPen(style.text_normal_selected),
		.regular_hovered = QPen(style.text_normal_regular_hovered),
		.highlighted_hovered = QPen(style.text_normal_highlighted_hovered),
		.selected_hovered = QPen(style.text_normal_selected_hovered)
	};
}

//******************************************************************************
nodegraph::Text::Params ProcessingStyleSelector::make_text_enabled(ProcessingStyle const& style) const {
	return {
		.regular = QPen(style.text_enabled_regular),
		.highlighted = QPen(style.text_enabled_highlighted),
		.selected = QPen(style.text_enabled_selected),
		.regular_hovered = QPen(style.text_enabled_regular_hovered),
		.highlighted_hovered = QPen(style.text_enabled_highlighted_hovered),
		.selected_hovered = QPen(style.text_enabled_selected_hovered)
	};
}

//******************************************************************************
nodegraph::Text::Params ProcessingStyleSelector::make_text_enabled_for_sure(ProcessingStyle const& style) const {
	return {
		.regular = QPen(style.text_enabled_for_sure_regular),
		.highlighted = QPen(style.text_enabled_for_sure_highlighted),
		.selected = QPen(style.text_enabled_for_sure_selected),
		.regular_hovered = QPen(style.text_enabled_for_sure_regular_hovered),
		.highlighted_hovered = QPen(style.text_enabled_for_sure_highlighted_hovered),
		.selected_hovered = QPen(style.text_enabled_for_sure_selected_hovered)
	};
}

//******************************************************************************
nodegraph::Text::Params ProcessingStyleSelector::make_text_disabled(ProcessingStyle const& style) const {
	return {
		.regular = QPen(style.text_disabled_regular),
		.highlighted = QPen(style.text_disabled_highlighted),
		.selected = QPen(style.text_disabled_selected),
		.regular_hovered = QPen(style.text_disabled_regular_hovered),
		.highlighted_hovered = QPen(style.text_disabled_highlighted_hovered),
		.selected_hovered = QPen(style.text_disabled_selected_hovered)
	};
}

//******************************************************************************
ProcessingEdge::Params ProcessingStyleSelector::make_edge() const {
	return {
		.node = get_node(),
		.port = get_port(),
		.title = get_title(),
		.main = get_text_normal(),
		.enabled = get_text_enabled(),
		.enabled_for_sure = get_text_enabled_for_sure(),
		.disabled = get_text_disabled()
	};
}

//******************************************************************************
ProcessingMeshlinePolicy::Params ProcessingStyleSelector::make_meshline_policy() const {
	return {
		.node = get_node(),
		.port = get_port(),
		.title = get_title(),
		.main = get_text_normal(),
		.enabled = get_text_enabled(),
		.enabled_for_sure = get_text_enabled_for_sure(),
		.disabled = get_text_disabled()
	};
}

//******************************************************************************
ProcessingInterval::Params ProcessingStyleSelector::make_interval() const {
	return {
		.node = get_node(),
		.port = get_port(),
		.title = get_title(),
		.main = get_text_normal()
	};
}

//******************************************************************************
ProcessingMeshline::Params ProcessingStyleSelector::make_meshline() const {
	return {
		.node = get_node(),
		.port = get_port(),
		.title = get_title(),
		.main = get_text_normal()
	};
}

//******************************************************************************
ProcessingPolygon::Params ProcessingStyleSelector::make_polygon() const {
	return {
		.node = get_node(),
		.port = get_port(),
		.title = get_title(),
		.nested_zone = get_nested_zone()
	};
}

//******************************************************************************
ProcessingPlane::Params ProcessingStyleSelector::make_plane() const {
	return {
		.node = get_node(),
		.title = get_title(),
		.nested_zone = get_nested_zone()
	};
}

//******************************************************************************
ProcessingAxis::Params ProcessingStyleSelector::make_axis() const {
	return {
		.node = get_node(),
		.title = get_title(),
		.nested_zone = get_nested_zone()
	};
}

//******************************************************************************
ProcessingConflictColinearEdges::Params ProcessingStyleSelector::make_conflict_ce() const {
	return {
		.node = get_node(),
		.title = get_title(),
		.port = get_port()
	};
}

//******************************************************************************
ProcessingConflictEdgeInPolygon::Params ProcessingStyleSelector::make_conflict_eip() const {
	return {
		.node = get_node(),
		.title = get_title(),
		.port = get_port(),
		.main = get_text_normal(),
		.enabled = get_text_enabled(),
		.disabled = get_text_disabled()
	};
}

//******************************************************************************
ProcessingConflictTooCloseMeshlinePolicies::Params ProcessingStyleSelector::make_conflict_tcmlp() const {
	return {
		.node = get_node(),
		.title = get_title(),
		.port = get_port()
	};
}

//******************************************************************************
ProcessingStyle const& ProcessingStyleSelector::get_style() const {
	return style;
}

//******************************************************************************
nodegraph::Wire::Params const& ProcessingStyleSelector::get_wire() const {
	return wire;
}

//******************************************************************************
nodegraph::Port::Params const& ProcessingStyleSelector::get_port() const {
	return port;
}

//******************************************************************************
nodegraph::Node::Params const& ProcessingStyleSelector::get_node() const {
	return node;
}

//******************************************************************************
nodegraph::Rect::Params const& ProcessingStyleSelector::get_nested_zone() const {
	return nested_zone;
}

//******************************************************************************
nodegraph::Text::Params const& ProcessingStyleSelector::get_title() const {
	return title;
}

//******************************************************************************
nodegraph::Text::Params const& ProcessingStyleSelector::get_text_normal() const {
	return text_normal;
}

//******************************************************************************
nodegraph::Text::Params const& ProcessingStyleSelector::get_text_enabled() const {
	return text_enabled;
}

//******************************************************************************
nodegraph::Text::Params const& ProcessingStyleSelector::get_text_enabled_for_sure() const {
	return text_enabled_for_sure;
}

//******************************************************************************
nodegraph::Text::Params const& ProcessingStyleSelector::get_text_disabled() const {
	return text_disabled;
}

//******************************************************************************
ProcessingEdge::Params const& ProcessingStyleSelector::get_edge() const {
	return edge;
}

//******************************************************************************
ProcessingMeshlinePolicy::Params const& ProcessingStyleSelector::get_meshline_policy() const {
	return meshline_policy;
}

//******************************************************************************
ProcessingInterval::Params const& ProcessingStyleSelector::get_interval() const {
	return interval;
}

//******************************************************************************
ProcessingMeshline::Params const& ProcessingStyleSelector::get_meshline() const {
	return meshline;
}

//******************************************************************************
ProcessingPolygon::Params const& ProcessingStyleSelector::get_polygon() const {
	return polygon;
}

//******************************************************************************
ProcessingPlane::Params const& ProcessingStyleSelector::get_plane() const {
	return plane;
}

//******************************************************************************
ProcessingAxis::Params const& ProcessingStyleSelector::get_axis() const {
	return axis;
}

//******************************************************************************
ProcessingConflictColinearEdges::Params const& ProcessingStyleSelector::get_conflict_ce() const {
	return conflict_ce;
}

//******************************************************************************
ProcessingConflictEdgeInPolygon::Params const& ProcessingStyleSelector::get_conflict_eip() const {
	return conflict_eip;
}

//******************************************************************************
ProcessingConflictTooCloseMeshlinePolicies::Params const& ProcessingStyleSelector::get_conflict_tcmlp() const {
	return conflict_tcmlp;
}

} // namespace ui::qt
