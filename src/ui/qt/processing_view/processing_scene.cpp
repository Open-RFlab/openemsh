///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <type_traits>

#include "domain/conflicts/conflict_colinear_edges.hpp"
#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/polygon.hpp"
#include "domain/geometrics/space.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "ui/qt/data_keys.hpp"
#include "processing_axis.hpp"
#include "processing_conflict_colinear_edges.hpp"
#include "processing_edge.hpp"
#include "processing_meshline_policy.hpp"
#include "processing_plane.hpp"
#include "processing_polygon.hpp"

#include "processing_scene.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingScene::ProcessingScene(QObject* parent)
: QGraphicsScene(parent)
, style_selector(ProcessingStyleSelector::available_styles[0])
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
void ProcessingScene::set_wire_style(nodegraph::Wire::Style style) {
	for(auto* wire : wires) {
		wire->style = style;
		wire->update_path();
	}
}

//******************************************************************************
void ProcessingScene::fit_containers() {
	for(auto* container : polygons)
		container->fit();
	for(auto* container : planes)
		container->fit();
	for(auto* container : axes)
		container->fit();
}

//******************************************************************************
template<typename Item, typename Space>
Item* ProcessingScene::add_node(Space space) {
	static_assert(std::is_base_of<nodegraph::Node, Item>::value, "Item must derive from nodegraph::Node");
	static_assert(std::is_enum<Space>::value, "Space must be an enum");
	Item* item = new Item(space);
	addItem(item);
	return item;
}

//******************************************************************************
template<typename Item, typename Entity>
Item* ProcessingScene::add_node(Entity* entity) {
	static_assert(std::is_base_of<nodegraph::Node, Item>::value, "Item must derive from nodegraph::Node");
	static_assert(std::is_base_of<::Entity, Entity>::value, "Entity must derive from ::Entity");
	Item* item = new Item(entity);
	addItem(item);
	index[entity] = item;
	return item;
}

//******************************************************************************
template<typename Item, typename Entity>
Item* ProcessingScene::add_node(Entity* entity, nodegraph::Container* to_container) {
	static_assert(std::is_base_of<nodegraph::Node, Item>::value, "Item must derive from nodegraph::Node");
	static_assert(std::is_base_of<::Entity, Entity>::value, "Entity must derive from ::Entity");
	Item* item = new Item(entity);
	to_container->add(item);
	index[entity] = item;
	return item;
}

//******************************************************************************
ProcessingPlane* ProcessingScene::add(domain::Plane plane) {
	auto* processing_plane = add_node<ProcessingPlane>(plane);
	planes[plane] = processing_plane;
	processing_plane->locate_processing_plane_params = [&]() -> auto& {
		return style_selector.get_plane();
	};
	return processing_plane;
}

//******************************************************************************
ProcessingAxis* ProcessingScene::add(domain::Axis axis) {
	auto* processing_axis = add_node<ProcessingAxis>(axis);
	axes[axis] = processing_axis;
	processing_axis->locate_processing_axis_params = [&]() -> auto& {
		return style_selector.get_axis();
	};
	return processing_axis;
}

//******************************************************************************
ProcessingPolygon* ProcessingScene::add(domain::Polygon* polygon, ProcessingPlane* to_plane) {
	auto* processing_polygon = add_node<ProcessingPolygon>(polygon, to_plane);
	polygons.append(processing_polygon);
	processing_polygon->locate_processing_polygon_params = [&]() -> auto& {
		return style_selector.get_polygon();
	};
	return processing_polygon;
}

//******************************************************************************
ProcessingEdge* ProcessingScene::add(domain::Edge* edge, ProcessingPolygon* to_polygon) {
	auto* processing_edge = add_node<ProcessingEdge>(edge, to_polygon);
	edges.append(processing_edge);
	processing_edge->locate_processing_edge_params = [&]() -> auto& {
		return style_selector.get_edge();
	};
	processing_edge->updateGeometry();
	return processing_edge;
}

//******************************************************************************
ProcessingConflictColinearEdges* ProcessingScene::add(domain::ConflictColinearEdges* conflict, ProcessingAxis* to_axis) {
	auto* processing_conflict = add_node<ProcessingConflictColinearEdges>(conflict, to_axis);
	conflict_colinear_edges.append(processing_conflict);
	processing_conflict->locate_processing_conflict_ce_params = [&]() -> auto& {
		return style_selector.get_conflict_ce();
	};
	processing_conflict->updateGeometry();
	return processing_conflict;
}

//******************************************************************************
ProcessingMeshlinePolicy* ProcessingScene::add(domain::MeshlinePolicy* policy, ProcessingAxis* to_axis) {
	auto* processing_policy = add_node<ProcessingMeshlinePolicy>(policy, to_axis);
	meshline_policies.append(processing_policy);
	processing_policy->locate_processing_meshline_policy_params = [&]() -> auto& {
		return style_selector.get_meshline_policy();
	};
	processing_policy->updateGeometry();
	return processing_policy;
}

//******************************************************************************
nodegraph::Wire* ProcessingScene::wire_together(nodegraph::Port* begin, nodegraph::Port* end) {
	nodegraph::Wire* wire = new nodegraph::Wire(begin, end);
	addItem(wire);
	wires.append(wire);
	wire->locate_wire_params = [&]() -> auto& {
		return style_selector.get_wire();
	};
	return wire;
}

//******************************************************************************
void ProcessingScene::on_selectionChanged() {
	// Avoid infinite recursion by being synchronized back.
	is_select_counterparts_locked = true;
	emit selection_changed(selectedItems());
	is_select_counterparts_locked = false;
}

//******************************************************************************
void ProcessingScene::select_counterparts(QList<QGraphicsItem*> foreign_items) {
	if(!is_select_counterparts_locked) {
		clearSelection();
		for(auto* foreign_item : foreign_items) {
			Entity const* entity = static_cast<Entity const*>(foreign_item->data(DataKeys::ENTITY).value<void const*>());
			if(index.contains(entity)) {
				index.at(entity)->setSelected(true);
			}
		}
	}
}

} // namespace ui::qt
