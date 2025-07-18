///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QGraphicsSceneMouseEvent>
#include <QMenu>

#include "domain/conflicts/conflict_colinear_edges.hpp"
#include "domain/conflicts/conflict_edge_in_polygon.hpp"
#include "domain/conflicts/conflict_too_close_meshline_policies.hpp"
#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/polygon.hpp"
#include "domain/geometrics/space.hpp"
#include "domain/mesh/interval.hpp"
#include "domain/mesh/meshline.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "ui/qt/data_keys.hpp"
#include "ui/qt/icons.hpp"
#include "ui/qt/edit/edit_dialog.hpp"
#include "ui/qt/edit/edit_model.hpp"
#include "ui/qt/utils/qlist_utils.hpp"
#include "utils/unreachable.hpp"
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

#include "processing_scene.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingScene::ProcessingScene(ProcessingStyleSelector& style_selector, QObject* parent)
: QGraphicsScene(parent)
, style_selector(style_selector)
, is_select_counterparts_locked(false)
, is_display_selected_chain_locked(false)
, display_mode(DisplayMode::SELECTED_CHAIN)
, plane_displayed_on_structure_view(domain::XY)
, axes_displayed_on_structure_view({ true, true })
{
	connect(
		this, &QGraphicsScene::selectionChanged,
		this, &ProcessingScene::on_selectionChanged);
}

//******************************************************************************
ProcessingScene::~ProcessingScene() {
	disconnect(
		this, &QGraphicsScene::selectionChanged,
		this, &ProcessingScene::on_selectionChanged);
}

//******************************************************************************
void ProcessingScene::init() {
	set_display_mode(display_mode);
}

//******************************************************************************
void ProcessingScene::set_wire_style(nodegraph::Wire::Style style) const {
	for(auto* wire : wires) {
		wire->style = style;
		wire->update_path();
	}
}

//******************************************************************************
void ProcessingScene::fit_containers() const {
	for(auto* container : polygons)
		container->fit();
	for(auto* container : planes)
		container->fit();
	for(auto* container : axes)
		container->fit();
}

//******************************************************************************
void ProcessingScene::fit_scene() const {
	qreal const margin_x = 1000;
	qreal const margin_y = 100;
	qreal x = 0;
	qreal y = 0;
	qreal next_x = 0;
	for(auto* plane : planes) {
		if(plane->isVisible()) {
			plane->setPos(x, y);
			next_x = qMax(next_x, plane->boundingRect().width());
			y += plane->boundingRect().height() + margin_y;
		}
	}

	x = next_x + margin_x;
	y = 0;

	for(auto* axis : axes) {
		if(axis->isVisible()) {
			axis->setPos(x, y);
			y += axis->boundingRect().height() + margin_y;
		}
	}
}

//******************************************************************************
void ProcessingScene::fit(QMarginsF margins) {
	auto const boundings = visible_items_bounding_rect();
	if(margins.isNull())
		margins += qMin(boundings.height(), boundings.width()) / 20;

	setSceneRect(boundings + margins);
}

//******************************************************************************
QRectF ProcessingScene::visible_items_bounding_rect() const {
	QRectF bounding;
	auto collect_bounding_rect = [&](QGraphicsItem const* item) {
		if(item && item->isVisible()) {
			bounding |= item->mapToScene(item->boundingRect()).boundingRect();
		}
	};

	for(auto const* container : planes)
		collect_bounding_rect(container);
	for(auto const* container : axes)
		collect_bounding_rect(container);

	return bounding;
}

//******************************************************************************
template<std::derived_from<nodegraph::Node> Node, Enum Space>
Node* ProcessingScene::add_node(Space space) {
	auto* node = new Node(space);
	addItem(node);
	nodes.push_back(node);
	return node;
}

//******************************************************************************
template<std::derived_from<nodegraph::Node> Node, std::derived_from<::Entity> Entity>
Node* ProcessingScene::add_node(Entity* entity) {
	auto* node = new Node(entity);
	addItem(node);
	nodes.push_back(node);
	index[entity] = node;
	return node;
}

//******************************************************************************
template<std::derived_from<nodegraph::Node> Node, std::derived_from<::Entity> Entity>
Node* ProcessingScene::add_node(Entity* entity, nodegraph::Container* to_container) {
	auto* node = new Node(entity);
	to_container->add(node);
	nodes.push_back(node);
	index[entity] = node;
	return node;
}

//******************************************************************************
ProcessingPlane* ProcessingScene::add(domain::Plane plane) {
	auto* processing_plane = add_node<ProcessingPlane>(plane);
	planes[plane] = processing_plane;
	processing_plane->locate_processing_plane_params = [this]() -> auto& {
		return style_selector.get_plane();
	};
	return processing_plane;
}

//******************************************************************************
ProcessingAxis* ProcessingScene::add(domain::Axis axis) {
	auto* processing_axis = add_node<ProcessingAxis>(axis);
	axes[axis] = processing_axis;
	processing_axis->locate_processing_axis_params = [this]() -> auto& {
		return style_selector.get_axis();
	};
	return processing_axis;
}

//******************************************************************************
ProcessingPolygon* ProcessingScene::add(domain::Polygon* polygon, ProcessingPlane* to_plane) {
	auto* processing_polygon = add_node<ProcessingPolygon>(polygon, to_plane);
	polygons.append(processing_polygon);
	processing_polygon->locate_processing_polygon_params = [this]() -> auto& {
		return style_selector.get_polygon();
	};
	return processing_polygon;
}

//******************************************************************************
ProcessingEdge* ProcessingScene::add(domain::Edge* edge, ProcessingPolygon* to_polygon) {
	auto* processing_edge = add_node<ProcessingEdge>(edge, to_polygon);
	edges.append(processing_edge);
	processing_edge->locate_processing_edge_params = [this]() -> auto& {
		return style_selector.get_edge();
	};
	processing_edge->updateGeometry();
	return processing_edge;
}

//******************************************************************************
ProcessingConflictColinearEdges* ProcessingScene::add(domain::ConflictColinearEdges* conflict, ProcessingAxis* to_axis) {
	auto* processing_conflict = add_node<ProcessingConflictColinearEdges>(conflict, to_axis);
	conflict_colinear_edges.append(processing_conflict);
	processing_conflict->locate_processing_conflict_ce_params = [this]() -> auto& {
		return style_selector.get_conflict_ce();
	};
	processing_conflict->updateGeometry();
	return processing_conflict;
}

//******************************************************************************
ProcessingConflictEdgeInPolygon* ProcessingScene::add(domain::ConflictEdgeInPolygon* conflict, ProcessingPlane* to_plane) {
	auto* processing_conflict = add_node<ProcessingConflictEdgeInPolygon>(conflict, to_plane);
	conflict_edge_in_polygons.append(processing_conflict);
	processing_conflict->locate_processing_conflict_eip_params = [this]() -> auto& {
		return style_selector.get_conflict_eip();
	};
	processing_conflict->updateGeometry();
	return processing_conflict;

}

//******************************************************************************
ProcessingConflictTooCloseMeshlinePolicies* ProcessingScene::add(domain::ConflictTooCloseMeshlinePolicies* conflict, ProcessingAxis* to_axis) {
	auto* processing_conflict = add_node<ProcessingConflictTooCloseMeshlinePolicies>(conflict, to_axis);
	conflict_too_close_meshline_policies.append(processing_conflict);
	processing_conflict->locate_processing_conflict_tcmlp_params = [this]() -> auto& {
		return style_selector.get_conflict_tcmlp();
	};
	processing_conflict->updateGeometry();
	return processing_conflict;
}

//******************************************************************************
ProcessingMeshlinePolicy* ProcessingScene::add(domain::MeshlinePolicy* policy, ProcessingAxis* to_axis) {
	auto* processing_policy = add_node<ProcessingMeshlinePolicy>(policy, to_axis);
	meshline_policies.append(processing_policy);
	processing_policy->locate_processing_meshline_policy_params = [this]() -> auto& {
		return style_selector.get_meshline_policy();
	};
	processing_policy->updateGeometry();
	return processing_policy;
}

//******************************************************************************
ProcessingInterval* ProcessingScene::add(domain::Interval* interval, ProcessingAxis* to_axis) {
	auto* processing_interval = add_node<ProcessingInterval>(interval, to_axis);
	intervals.append(processing_interval);
	processing_interval->locate_processing_interval_params = [this]() -> auto& {
		return style_selector.get_interval();
	};
	processing_interval->updateGeometry();
	return processing_interval;
}

//******************************************************************************
ProcessingMeshline* ProcessingScene::add(domain::Meshline* meshline, ProcessingAxis* to_axis) {
	auto* processing_meshline = add_node<ProcessingMeshline>(meshline, to_axis);
	meshlines.append(processing_meshline);
	processing_meshline->locate_processing_meshline_params = [this]() -> auto& {
		return style_selector.get_meshline();
	};
	processing_meshline->updateGeometry();
	return processing_meshline;
}

//******************************************************************************
nodegraph::Wire* ProcessingScene::wire_together(nodegraph::Port* begin, nodegraph::Port* end) {
	auto* wire = new nodegraph::Wire(begin, end);
	addItem(wire);
	wires.append(wire);
	wire->locate_wire_params = [this]() -> auto& {
		return style_selector.get_wire();
	};
	return wire;
}

//******************************************************************************
void ProcessingScene::wire_to_destination_first_output_port(nodegraph::Node* node) {
	for(auto const& to_wire : node->data(DataKeys::TO_WIRE).toList()) {
		auto const& [entity, port] = DataKeys::get_to_wire(to_wire);
		if(index.contains(entity)) {
			auto* item = index.at(entity);
			if(!port->is_wired_to(item->output_ports[0])) {
				wire_together(item->output_ports[0], port);
			}
		}
	}
	node->setData(DataKeys::TO_WIRE, QList<QVariant>());
}

//******************************************************************************
QList<nodegraph::Node*> ProcessingScene::selected_nodes() const {
	QList<nodegraph::Node*> ret;
	for(auto* node : nodes) {
		if(node && node->isSelected())
			ret.append(node);
	}
	return ret;
}

//******************************************************************************
QList<nodegraph::Node*> ProcessingScene::highlighted_nodes() const {
	QList<nodegraph::Node*> ret;
	for(auto* node : nodes) {
		if(node && node->is_highlighted())
			ret.append(node);
	}
	return ret;
}

//******************************************************************************
void ProcessingScene::reset_visibility(bool are_visible) const {
	for(auto* node : nodes) {
		if(node) {
			if(are_visible)
				node->show_after_parents();
			else
				node->hide();
		}
	}
}
//******************************************************************************
void ProcessingScene::set_display_view_axes(domain::ViewAxisSpace<bool> const& axes) {
	axes_displayed_on_structure_view = axes;
	if(display_mode == DisplayMode::STRUCTURE_VIEW)
		display_structure_view();
}

//******************************************************************************
void ProcessingScene::set_display_plane(domain::Plane plane) {
	plane_displayed_on_structure_view = plane;
	if(display_mode == DisplayMode::STRUCTURE_VIEW)
		display_structure_view();
}

//******************************************************************************
void ProcessingScene::set_display_mode(DisplayMode mode) {
	display_mode = mode;
	switch(display_mode) {
	case DisplayMode::EVERYTHING:
		reset_visibility();
		fit_containers();
		fit_scene();
		break;
	case DisplayMode::STRUCTURE_VIEW:
		display_structure_view();
		break;
	case DisplayMode::SELECTED_CHAIN:
		display_selected_chain();
		break;
	default:
		unreachable();
	}
}

//******************************************************************************
ProcessingScene::DisplayMode ProcessingScene::get_display_mode() const {
	return display_mode;
}

//******************************************************************************
void ProcessingScene::display_structure_view() {
	reset_visibility();

	for(domain::Plane const plane : domain::AllPlane) {
		if(plane != plane_displayed_on_structure_view)
			if(planes[plane])
				planes[plane]->hide();
	}

	for(domain::Axis const axis : domain::AllAxis) {
		if(auto const view_axis = domain::transpose(plane_displayed_on_structure_view, axis)
		; view_axis) {
			if(axes[axis])
				axes[axis]->setVisible(axes_displayed_on_structure_view[domain::reverse(view_axis.value())]);
		} else {
			if(axes[axis])
				axes[axis]->hide();
		}
	}

	fit_containers();
	fit_scene();
}

//******************************************************************************
void ProcessingScene::display_selected_chain() {
	auto const selected = selected_nodes();
	auto const highlighted = highlighted_nodes();
	reset_visibility(false);
	clearSelection();

	for(auto const& list : { selected, highlighted })
		for(auto* node : list)
			if(node)
				node->show_after_parents();

	for(auto* node : selected)
		if(node)
			node->setSelected(true);

	fit_containers();
	fit_scene();
}

//******************************************************************************
void ProcessingScene::on_selectionChanged() {
	// Avoid infinite recursion by being synchronized back.
	is_select_counterparts_locked = true;
	emit selection_changed(selectedItems());
	is_select_counterparts_locked = false;

	if(display_mode == DisplayMode::SELECTED_CHAIN) {
		if(!is_display_selected_chain_locked) {
			is_display_selected_chain_locked = true;
			display_selected_chain();
			emit requires_fit();
			is_display_selected_chain_locked = false;
		}
	}
}

//******************************************************************************
void ProcessingScene::select_counterparts(QList<QGraphicsItem*> foreign_items) {
	if(!is_select_counterparts_locked) {
		clearSelection();
		for(auto const* foreign_item : foreign_items) {
			auto const* entity = DataKeys::get_entity(foreign_item->data(DataKeys::ENTITY));
			if(index.contains(entity)) {
				auto* node = index.at(entity);
				node->show_after_parents();
				node->setSelected(true);
			}
		}
	}
}

//******************************************************************************
void ProcessingScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
	event->accept();
	edit(dynamic_to_qlist<nodegraph::Node*>(QGraphicsScene::items(event->scenePos())), event->screenPos());
}

//******************************************************************************
void ProcessingScene::edit_selected_nodes(QPoint const& pos) {
	edit(selected_nodes(), pos);
}

//******************************************************************************
void ProcessingScene::edit(QList<nodegraph::Node*> nodes, QPoint const& pos) {
	static QList<int> const type_index = {
		UserTypes::PROCESSING_EDGE,
		UserTypes::PROCESSING_INTERVAL,
		UserTypes::PROCESSING_MESHLINE_POLICY,
		UserTypes::PROCESSING_CONFLICT_CE,
		UserTypes::PROCESSING_CONFLICT_EIP,
		UserTypes::PROCESSING_CONFLICT_TCMLP
	};

//	items.removeIf([](auto const it) { return it->type() <= QGraphicsItem::UserType; }); // Wire::Type == UserType
	nodes.removeIf([](auto const it) { return !type_index.contains(it->type()); });

	// TODO isolate in own file
	auto const make_title = [](QGraphicsItem const* item) {
		QString title(item->data(DataKeys::TYPE).toString() + " - " + item->data(DataKeys::ID).toString());
		if(item->type() == ProcessingPolygon::Type)
			title.append(" - " + item->data(DataKeys::NAME).toString());
		return title;
	};

	auto const edit_node = [this](nodegraph::Node* node, QString const& title = QString()) {
		if(auto* model = EditModel::make(node); model) {
			EditDialog edit(model, title);
			model->setParent(&edit);
			connect(
				model, &EditModel::edit_from,
				this, &ProcessingScene::edit_from);
			edit.exec();
		}
	};

	if(nodes.isEmpty()) {
		clearSelection();
		emit edit_global_params();
	} else if(nodes.size() == 1) {
			edit_node(nodes.first(), make_title(nodes.first()));
	} else {
		QMenu menu;
		for(auto* node : nodes) {
//			// TODO add entity icon
			auto const title = make_title(node);
			auto* action = new QAction(Icons::select(node), title, &menu);
			menu.addAction(action);
			QObject::connect(action, &QAction::triggered, [&edit_node, node, title]() {
				edit_node(node, title);
			});
		}
		menu.exec(pos);
	}
}

} // namespace ui::qt
