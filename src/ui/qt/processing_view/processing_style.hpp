///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QColor>

#include <vector>
#include <type_traits>

#include "ui/qt/utils/nodegraph/container.hpp"
#include "ui/qt/utils/nodegraph/rect.hpp"
#include "ui/qt/utils/nodegraph/wire.hpp"
#include "processing_axis.hpp"
#include "processing_conflict_colinear_edges.hpp"
#include "processing_conflict_edge_in_polygon.hpp"
#include "processing_conflict_too_close_meshline_policies.hpp"
#include "processing_edge.hpp"
#include "processing_interval.hpp"
#include "processing_meshline.hpp"
#include "processing_meshline_policy.hpp"
#include "processing_plane.hpp"
#include "processing_polygon.hpp"

namespace ui::qt {

//******************************************************************************
struct ProcessingStyle {
	QString name;

	QColor background = QColor(57, 57, 57);


	QColor node_background_regular             = QColor(27, 27, 27);
	QColor node_background_highlighted         = node_background_regular;
	QColor node_background_selected            = node_background_regular;
	QColor node_background_regular_hovered     = node_background_regular.lighter();
	QColor node_background_highlighted_hovered = node_background_highlighted.lighter();
	QColor node_background_selected_hovered    = node_background_selected.darker();

	QColor node_title_background_regular             = QColor(33, 33, 33);
	QColor node_title_background_highlighted         = node_title_background_regular;
	QColor node_title_background_selected            = node_title_background_regular;
	QColor node_title_background_regular_hovered     = node_title_background_regular.lighter();
	QColor node_title_background_highlighted_hovered = node_title_background_highlighted.lighter();
	QColor node_title_background_selected_hovered    = node_title_background_selected.darker();

	QColor node_title_text_regular             = Qt::white;
	QColor node_title_text_highlighted         = node_title_text_regular;
	QColor node_title_text_selected            = Qt::black;
	QColor node_title_text_regular_hovered     = node_title_text_regular;
	QColor node_title_text_highlighted_hovered = node_title_text_highlighted;
	QColor node_title_text_selected_hovered    = node_title_text_selected;

	qreal node_radius = 10;


	QColor container_nested_zone_regular             = background;
	QColor container_nested_zone_highlighted         = container_nested_zone_regular;
	QColor container_nested_zone_selected            = container_nested_zone_regular;
	QColor container_nested_zone_regular_hovered     = container_nested_zone_regular;
	QColor container_nested_zone_highlighted_hovered = container_nested_zone_highlighted;
	QColor container_nested_zone_selected_hovered    = container_nested_zone_selected;

	qreal container_nested_zone_opacity = 0.5;


	QColor wire_regular             = Qt::green;
	QColor wire_highlighted         = wire_regular;
	QColor wire_selected            = wire_regular;
	QColor wire_regular_hovered     = wire_regular.lighter();
	QColor wire_highlighted_hovered = wire_highlighted.lighter();
	QColor wire_selected_hovered    = wire_selected.lighter();

	qreal wire_width_regular             = 2;
	qreal wire_width_highlighted         = wire_width_regular;
	qreal wire_width_selected            = wire_width_regular;
	qreal wire_width_regular_hovered     = wire_width_regular * 2;
	qreal wire_width_highlighted_hovered = wire_width_highlighted * 2;
	qreal wire_width_selected_hovered    = wire_width_selected * 2;


	QColor port_text_regular             = Qt::white;
	QColor port_text_highlighted         = port_text_regular;
	QColor port_text_selected            = port_text_regular;
	QColor port_text_regular_hovered     = port_text_regular;
	QColor port_text_highlighted_hovered = port_text_highlighted;
	QColor port_text_selected_hovered    = port_text_selected;

	QColor port_contour_regular             = Qt::black;
	QColor port_contour_highlighted         = port_contour_regular;
	QColor port_contour_selected            = port_contour_regular;
	QColor port_contour_regular_hovered     = port_contour_regular;
	QColor port_contour_highlighted_hovered = port_contour_highlighted;
	QColor port_contour_selected_hovered    = port_contour_selected;

	QColor port_fill_regular             = QColor(255, 119, 0);
	QColor port_fill_highlighted         = port_fill_regular;
	QColor port_fill_selected            = port_fill_regular;
	QColor port_fill_regular_hovered     = port_fill_regular;
	QColor port_fill_highlighted_hovered = port_fill_highlighted;
	QColor port_fill_selected_hovered    = port_fill_selected;

	qreal port_radius = 5;


	QColor text_normal_regular             = Qt::white;
	QColor text_normal_highlighted         = text_normal_regular;
	QColor text_normal_selected            = text_normal_regular;
	QColor text_normal_regular_hovered     = text_normal_regular;
	QColor text_normal_highlighted_hovered = text_normal_highlighted;
	QColor text_normal_selected_hovered    = text_normal_selected;

	QColor text_enabled_regular             = Qt::darkGreen;
	QColor text_enabled_highlighted         = text_enabled_regular;
	QColor text_enabled_selected            = text_enabled_regular;
	QColor text_enabled_regular_hovered     = text_enabled_regular;
	QColor text_enabled_highlighted_hovered = text_enabled_highlighted;
	QColor text_enabled_selected_hovered    = text_enabled_selected;

	QColor text_enabled_for_sure_regular             = Qt::green;
	QColor text_enabled_for_sure_highlighted         = text_enabled_for_sure_regular;
	QColor text_enabled_for_sure_selected            = text_enabled_for_sure_regular;
	QColor text_enabled_for_sure_regular_hovered     = text_enabled_for_sure_regular;
	QColor text_enabled_for_sure_highlighted_hovered = text_enabled_for_sure_highlighted;
	QColor text_enabled_for_sure_selected_hovered    = text_enabled_for_sure_selected;

	QColor text_disabled_regular             = Qt::darkRed;
	QColor text_disabled_highlighted         = text_disabled_regular;
	QColor text_disabled_selected            = text_disabled_regular;
	QColor text_disabled_regular_hovered     = text_disabled_regular;
	QColor text_disabled_highlighted_hovered = text_disabled_highlighted;
	QColor text_disabled_selected_hovered    = text_disabled_selected;
};

//******************************************************************************
#define GETTER_DEF(VAR_NAME) \
	decltype(VAR_NAME) const& get_ ##VAR_NAME () const { return VAR_NAME; }

//******************************************************************************
#define BASIC_MAKER_DECL(VAR_NAME) \
	decltype(VAR_NAME) make_ ##VAR_NAME (ProcessingStyle const& style) const

//******************************************************************************
#define BASIC_MAKER_DEF(SCOPE, VAR_NAME) \
	decltype(SCOPE::VAR_NAME) SCOPE::make_ ##VAR_NAME (ProcessingStyle const& style) const

//******************************************************************************
#define COMPOUNDED_MAKER_DECL(VAR_NAME) \
	std::remove_const_t<decltype(VAR_NAME)> make_ ##VAR_NAME () const

//******************************************************************************
#define COMPOUNDED_MAKER_DEF(SCOPE, VAR_NAME) \
	std::remove_const_t<decltype(SCOPE::VAR_NAME)> SCOPE::make_ ##VAR_NAME () const

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
	ProcessingMeshlinePolicy::Params const meshline_policy;
	ProcessingInterval::Params const interval;
	ProcessingMeshline::Params const meshline;
	ProcessingConflictColinearEdges::Params const conflict_ce;
	ProcessingConflictEdgeInPolygon::Params const conflict_eip;
	ProcessingConflictTooCloseMeshlinePolicies::Params const conflict_tcmlp;

	BASIC_MAKER_DECL(wire);
	BASIC_MAKER_DECL(port);
	BASIC_MAKER_DECL(node);
	BASIC_MAKER_DECL(nested_zone);
	BASIC_MAKER_DECL(title);
	BASIC_MAKER_DECL(text_normal);
	BASIC_MAKER_DECL(text_enabled);
	BASIC_MAKER_DECL(text_enabled_for_sure);
	BASIC_MAKER_DECL(text_disabled);

	COMPOUNDED_MAKER_DECL(edge);
	COMPOUNDED_MAKER_DECL(polygon);
	COMPOUNDED_MAKER_DECL(plane);
	COMPOUNDED_MAKER_DECL(axis);
	COMPOUNDED_MAKER_DECL(meshline_policy);
	COMPOUNDED_MAKER_DECL(interval);
	COMPOUNDED_MAKER_DECL(meshline);
	COMPOUNDED_MAKER_DECL(conflict_ce);
	COMPOUNDED_MAKER_DECL(conflict_eip);
	COMPOUNDED_MAKER_DECL(conflict_tcmlp);

public:
	static std::vector<ProcessingStyle> const available_styles;

	GETTER_DEF(style)
	GETTER_DEF(wire)
	GETTER_DEF(port)
	GETTER_DEF(node)
	GETTER_DEF(nested_zone)
	GETTER_DEF(title)
	GETTER_DEF(text_normal)
	GETTER_DEF(text_enabled)
	GETTER_DEF(text_enabled_for_sure)
	GETTER_DEF(text_disabled)
	GETTER_DEF(edge)
	GETTER_DEF(polygon)
	GETTER_DEF(plane)
	GETTER_DEF(axis)
	GETTER_DEF(meshline_policy)
	GETTER_DEF(interval)
	GETTER_DEF(meshline)
	GETTER_DEF(conflict_ce)
	GETTER_DEF(conflict_eip)
	GETTER_DEF(conflict_tcmlp)

	ProcessingStyleSelector();
	ProcessingStyleSelector(ProcessingStyle style);

	ProcessingStyleSelector& operator=(ProcessingStyle const& style);
	ProcessingStyleSelector& operator=(ProcessingStyle&& style);
};

} // namespace ui::qt
