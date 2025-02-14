///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "processing_style.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingStyleSelector::ProcessingStyleSelector()
: wire(make_wire(style))
, port_normal(make_port_normal(style))
, port_enabled(make_port_enabled(style))
, port_disabled(make_port_disabled(style))
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
: style(std::move(style))
, wire(make_wire(style))
, port_normal(make_port_normal(style))
, port_enabled(make_port_enabled(style))
, port_disabled(make_port_disabled(style))
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
	port_normal = make_port_normal(style);
	port_enabled = make_port_enabled(style);
	port_disabled = make_port_disabled(style);
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
	this->style = std::move(style);
	wire = make_wire(style);
	port_normal = make_port_normal(style);
	port_enabled = make_port_enabled(style);
	port_disabled = make_port_disabled(style);
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
MAKER_DEF(ProcessingStyleSelector, wire, ProcessingStyle const& style) {
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
MAKER_DEF(ProcessingStyleSelector, port_normal, ProcessingStyle const& style) {
	return {
		.radius = style.port_radius,
		.text_regular = QPen(style.port_text_normal_regular),
		.text_highlighted = QPen(style.port_text_normal_highlighted),
		.text_selected = QPen(style.port_text_normal_selected),
		.text_regular_hovered = QPen(style.port_text_normal_regular_hovered),
		.text_highlighted_hovered = QPen(style.port_text_normal_highlighted_hovered),
		.text_selected_hovered = QPen(style.port_text_normal_selected_hovered),
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
MAKER_DEF(ProcessingStyleSelector, port_enabled, ProcessingStyle const& style) {
	return {
		.radius = style.port_radius,
		.text_regular = QPen(style.port_text_enabled_regular),
		.text_highlighted = QPen(style.port_text_enabled_highlighted),
		.text_selected = QPen(style.port_text_enabled_selected),
		.text_regular_hovered = QPen(style.port_text_enabled_regular_hovered),
		.text_highlighted_hovered = QPen(style.port_text_enabled_highlighted_hovered),
		.text_selected_hovered = QPen(style.port_text_enabled_selected_hovered),
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
MAKER_DEF(ProcessingStyleSelector, port_disabled, ProcessingStyle const& style) {
	return {
		.radius = style.port_radius,
		.text_regular = QPen(style.port_text_disabled_regular),
		.text_highlighted = QPen(style.port_text_disabled_highlighted),
		.text_selected = QPen(style.port_text_disabled_selected),
		.text_regular_hovered = QPen(style.port_text_disabled_regular_hovered),
		.text_highlighted_hovered = QPen(style.port_text_disabled_highlighted_hovered),
		.text_selected_hovered = QPen(style.port_text_disabled_selected_hovered),
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
MAKER_DEF(ProcessingStyleSelector, node, ProcessingStyle const& style) {
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
MAKER_DEF(ProcessingStyleSelector, nested_zone, ProcessingStyle const& style) {
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
MAKER_DEF(ProcessingStyleSelector, title, ProcessingStyle const& style) {
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
MAKER_DEF(ProcessingStyleSelector, text_normal, ProcessingStyle const& style) {
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
MAKER_DEF(ProcessingStyleSelector, text_enabled, ProcessingStyle const& style) {
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
MAKER_DEF(ProcessingStyleSelector, text_enabled_for_sure, ProcessingStyle const& style) {
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
MAKER_DEF(ProcessingStyleSelector, text_disabled, ProcessingStyle const& style) {
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
MAKER_DEF(ProcessingStyleSelector, edge) {
	return {
		.node = get_node(),
		.port = get_port_normal(),
		.title = get_title(),
		.main = get_text_normal(),
		.enabled = get_text_enabled(),
		.enabled_for_sure = get_text_enabled_for_sure(),
		.disabled = get_text_disabled()
	};
}

//******************************************************************************
MAKER_DEF(ProcessingStyleSelector, meshline_policy) {
	return {
		.node = get_node(),
		.port = get_port_normal(),
		.title = get_title(),
		.main = get_text_normal(),
		.enabled = get_text_enabled(),
		.enabled_for_sure = get_text_enabled_for_sure(),
		.disabled = get_text_disabled()
	};
}

//******************************************************************************
MAKER_DEF(ProcessingStyleSelector, interval) {
	return {
		.node = get_node(),
		.port = get_port_normal(),
		.title = get_title(),
		.main = get_text_normal()
	};
}

//******************************************************************************
MAKER_DEF(ProcessingStyleSelector, meshline) {
	return {
		.node = get_node(),
		.port = get_port_normal(),
		.title = get_title(),
		.main = get_text_normal()
	};
}

//******************************************************************************
MAKER_DEF(ProcessingStyleSelector, polygon) {
	return {
		.node = get_node(),
		.port = get_port_normal(),
		.title = get_title(),
		.nested_zone = get_nested_zone()
	};
}

//******************************************************************************
MAKER_DEF(ProcessingStyleSelector, plane) {
	return {
		.node = get_node(),
		.title = get_title(),
		.nested_zone = get_nested_zone()
	};
}

//******************************************************************************
MAKER_DEF(ProcessingStyleSelector, axis) {
	return {
		.node = get_node(),
		.title = get_title(),
		.nested_zone = get_nested_zone()
	};
}

//******************************************************************************
MAKER_DEF(ProcessingStyleSelector, conflict_ce) {
	return {
		.node = get_node(),
		.title = get_title(),
		.port_normal = get_port_normal(),
		.port_enabled = get_port_enabled(),
		.port_disabled = get_port_disabled()
	};
}

//******************************************************************************
MAKER_DEF(ProcessingStyleSelector, conflict_eip) {
	return {
		.node = get_node(),
		.title = get_title(),
		.port = get_port_normal(),
		.main = get_text_normal(),
		.enabled = get_text_enabled(),
		.disabled = get_text_disabled()
	};
}

//******************************************************************************
MAKER_DEF(ProcessingStyleSelector, conflict_tcmlp) {
	return {
		.node = get_node(),
		.title = get_title(),
		.port = get_port_normal()
	};
}

} // namespace ui::qt
