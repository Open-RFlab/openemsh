///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QAction>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QMenu>
#include <QPainter>

#include <algorithm>

#include "domain/conflicts/conflict_colinear_edges.hpp"
#include "domain/conflicts/conflict_too_close_meshline_policies.hpp"
#include "domain/geometrics/polygon.hpp"
#include "domain/geometrics/edge.hpp"
#include "domain/mesh/interval.hpp"
#include "domain/mesh/meshline.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "utils/unreachable.hpp"
#include "ui/qt/data_keys.hpp"
#include "ui/qt/user_types.hpp"
#include "structure_conflict_colinear_edges.hpp"
#include "structure_conflict_too_close_meshline_policies.hpp"
#include "structure_edge.hpp"
#include "structure_interval.hpp"
#include "structure_meshline.hpp"
#include "structure_meshline_policy.hpp"
#include "structure_polygon.hpp"

#include "structure_scene.hpp"

namespace ui::qt {

//******************************************************************************
StructureGroup::StructureGroup(QGraphicsItem* parent)
: QGraphicsItem(parent)
{
	setFiltersChildEvents(false);
	setFlags(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable);
}

//******************************************************************************
QRectF StructureGroup::boundingRect() const {
	return childrenBoundingRect();
}

//******************************************************************************
void StructureGroup::paint(QPainter* /*painter*/, QStyleOptionGraphicsItem const* /*option*/, QWidget* /*widget*/) {
}

//******************************************************************************
StructureScene::StructureScene(StructureStyleSelector& style_selector, QObject* parent)
: QGraphicsScene(parent)
, style_selector(style_selector)
, edges(new StructureGroup())
, polygons(new StructureGroup())
, conflict_colinear_edges{{ new StructureGroup(), new StructureGroup() }}
, conflict_too_close_meshline_policies{{ new StructureGroup(), new StructureGroup() }}
, intervals{{ new StructureGroup(), new StructureGroup() }}
, meshlines{{ new StructureGroup(), new StructureGroup() }}
, meshline_policies{{ new StructureGroup(), new StructureGroup() }}
{
	// Adding order matters.
	addItem(edges);
	addItem(polygons);
	for(auto const& list : {
		meshlines,
		intervals,
		meshline_policies,
		conflict_colinear_edges,
		conflict_too_close_meshline_policies
	})
		for(auto* group : list)
			addItem(group);

	polygons->stackBefore(edges);
	for(auto const& list : {
		meshlines,
		intervals,
		meshline_policies,
		conflict_colinear_edges,
		conflict_too_close_meshline_policies
	})
		for(auto const* group : list)
			edges->stackBefore(group);

	connect(
		this, &QGraphicsScene::selectionChanged,
		this, &StructureScene::on_selectionChanged);
}

//******************************************************************************
StructureScene::~StructureScene() {
	disconnect(
		this, &QGraphicsScene::selectionChanged,
		this, &StructureScene::on_selectionChanged);
}

//******************************************************************************
StructureEdge* StructureScene::add(domain::Edge const* edge) {
	auto* item = new StructureEdge(edge, edges);
	index[edge] = item;
	item->locate_structure_edge_params = [this]() -> auto& {
		return style_selector.get_edge();
	};
	return item;
}

//******************************************************************************
StructurePolygon* StructureScene::add(domain::Polygon const* polygon) {
	auto* item = new StructurePolygon(polygon, polygons);
	index[polygon] = item;
	switch(polygon->type) {
	case domain::Polygon::Type::SHAPE:
		item->locate_structure_polygon_params = [this]() -> auto& {
			return style_selector.get_polygon_shape();
		};
		break;
	case domain::Polygon::Type::PORT:
		item->locate_structure_polygon_params = [this]() -> auto& {
			return style_selector.get_polygon_port();
		};
		break;
	case domain::Polygon::Type::GROUND:
		item->locate_structure_polygon_params = [this]() -> auto& {
			return style_selector.get_polygon_ground();
		};
		break;
	case domain::Polygon::Type::SUBSTRATE:
		item->locate_structure_polygon_params = [this]() -> auto& {
			return style_selector.get_polygon_substrate();
		};
		break;
	default:
		unreachable();
	}
	return item;
}

//******************************************************************************
StructureConflictColinearEdges* StructureScene::add(domain::ConflictColinearEdges const* conflict, domain::ViewAxis view_axis, QRectF const& scene_rect) {
	auto const meshline_axis = reverse(view_axis); // Let stick to meshline axis definition.
	auto* item = new StructureConflictColinearEdges(meshline_axis, conflict, scene_rect, conflict_colinear_edges[meshline_axis]);
	index[conflict] = item;
	item->locate_structure_conflict_ce_params = [this]() ->auto& {
		return style_selector.get_conflict_ce();
	};
	return item;
}

//******************************************************************************
StructureConflictTooCloseMeshlinePolicies* StructureScene::add(domain::ConflictTooCloseMeshlinePolicies const* conflict, domain::ViewAxis view_axis, QRectF const& scene_rect) {
	auto const meshline_axis = reverse(view_axis); // Let stick to meshline axis definition.
	auto* item = new StructureConflictTooCloseMeshlinePolicies(meshline_axis, conflict, scene_rect, conflict_too_close_meshline_policies[meshline_axis]);
	index[conflict] = item;
	item->locate_structure_conflict_tcmlp_params = [this]() ->auto& {
		return style_selector.get_conflict_tcmlp();
	};
	return item;
}

//******************************************************************************
StructureInterval* StructureScene::add(domain::Interval const* interval, domain::ViewAxis view_axis, QRectF const& scene_rect) {
	auto const meshline_axis = reverse(view_axis); // Let stick to meshline axis definition.
	auto* item = new StructureInterval(meshline_axis, interval, scene_rect, intervals[meshline_axis]);
	index[interval] = item;
	item->locate_structure_interval_params = [this]() ->auto& {
		return style_selector.get_interval();
	};
	return item;
}

//******************************************************************************
StructureMeshline* StructureScene::add(domain::Meshline const* meshline, domain::ViewAxis view_axis, QRectF const& scene_rect) {
	auto const meshline_axis = reverse(view_axis);
	auto* item = new StructureMeshline(meshline_axis, meshline, scene_rect, meshlines[meshline_axis]);
	index[meshline] = item;
	item->locate_structure_meshline_params = [this]() ->auto& {
		return style_selector.get_meshline();
	};
	return item;
}

//******************************************************************************
StructureMeshlinePolicy* StructureScene::add(domain::MeshlinePolicy const* policy, domain::ViewAxis view_axis, QRectF const& scene_rect) {
	auto const meshline_axis = reverse(view_axis); // Let stick to meshline axis definition.
	auto* item = new StructureMeshlinePolicy(meshline_axis, policy, scene_rect, meshline_policies[meshline_axis]);
	index[policy] = item;
	if(policy->is_enabled) {
		item->locate_structure_meshline_policy_params = [this]() ->auto& {
			return style_selector.get_meshline_policy_enabled();
		};
	} else {
		item->locate_structure_meshline_policy_params = [this]() ->auto& {
			return style_selector.get_meshline_policy_disabled();
		};
	}
	return item;
}

//******************************************************************************
void StructureScene::clear_edges() {
	auto const items = edges->childItems();
	std::erase_if(index, [&](auto const& item) {
		return items.contains(item.second);
	});
	removeItem(edges);
	delete edges;
	edges = new StructureGroup();
	addItem(edges);

	polygons->stackBefore(edges);
	for(auto const* group : meshlines)
		edges->stackBefore(group);
}

//******************************************************************************
void StructureScene::clear_polygons() {
	auto const items = polygons->childItems();
	std::erase_if(index, [&](auto const& item) {
		return items.contains(item.second);
	});
	removeItem(polygons);
	delete polygons;
	polygons = new StructureGroup();
	addItem(polygons);

	polygons->stackBefore(edges);
}

//******************************************************************************
void StructureScene::clear_meshlines() {
	for(auto const view_axis : domain::AllViewAxis) {
		auto const items = meshlines[view_axis]->childItems();
		std::erase_if(index, [&](auto const& item) {
			return items.contains(item.second);
		});
		removeItem(meshlines[view_axis]);
		delete meshlines[view_axis];
		meshlines[view_axis] = new StructureGroup();
		addItem(meshlines[view_axis]);

		edges->stackBefore(meshlines[view_axis]);
	}
}

//******************************************************************************
void StructureScene::clear_all() {
	index.clear();
	clear_edges();
	clear_polygons();
	clear_meshlines();
}

//******************************************************************************
void StructureScene::set_mesh_visibility(MeshVisibility mesh_visibility) {
	switch(mesh_visibility) {
	case MeshVisibility::NONE:
		meshlines[domain::H]->setVisible(false);
		meshlines[domain::V]->setVisible(false);
		break;
	case MeshVisibility::VERTICAL:
		meshlines[domain::H]->setVisible(false);
		meshlines[domain::V]->setVisible(true);
		break;
	case MeshVisibility::HORIZONTAL:
		meshlines[domain::H]->setVisible(true);
		meshlines[domain::V]->setVisible(false);
		break;
	case MeshVisibility::FULL:
		meshlines[domain::H]->setVisible(true);
		meshlines[domain::V]->setVisible(true);
		break;
	default:
		unreachable();
	}
}

// TODO use simple click + double click to select / deselect
//******************************************************************************
void StructureScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	event->accept();

	static QList<int> const ordered_type_index = {
		UserTypes::STRUCTURE_EDGE,
		UserTypes::STRUCTURE_POLYGON,
		UserTypes::STRUCTURE_MESHLINE
	};

	QList<QGraphicsItem*> items = QGraphicsScene::items(event->scenePos());
	items.removeIf([](auto const it) { return it->type() <= QGraphicsItem::UserType; });
//	items.removeIf([](auto const it) { return !ordered_type_index.contains(it->type()); });

	std::ranges::sort(items, [](auto const* a, auto const* b) {
		static auto constexpr index_of = [&](int const type) {
			for(qsizetype i = 0; i < ordered_type_index.size(); ++i)
				if(type == ordered_type_index[i])
					return i;
			return std::numeric_limits<qsizetype>::max();
		};

		return index_of(a->type()) < index_of(b->type());
	});

	if(items.isEmpty()) {
		clearSelection();
	} else if(items.size() == 1) {
		items.first()->setSelected(true);
	} else {
		QMenu menu;
		for(auto* item : items) {
			QString title(item->data(DataKeys::TYPE).toString() + " - " + item->data(DataKeys::ID).toString());
			if(item->type() == StructurePolygon::Type)
				title.append(" - " + item->data(DataKeys::NAME).toString());

			auto* action = new QAction(title, &menu);
			menu.addAction(action);
			QObject::connect(action, &QAction::triggered, [item]() {
				item->setSelected(true);
			});
		}
		menu.exec(event->screenPos());
	}
}

//******************************************************************************
void StructureScene::on_selectionChanged() {
	// Avoid infinite recursion by being synchronized back.
	is_select_counterparts_locked = true;
	emit selection_changed(selectedItems());
	is_select_counterparts_locked = false;
}

//******************************************************************************
void StructureScene::select_counterparts(QList<QGraphicsItem*> foreign_items) {
	if(!is_select_counterparts_locked) {
		clearSelection();
		for(auto const* foreign_item : foreign_items) {
			auto const* entity = DataKeys::get_entity(foreign_item->data(DataKeys::ENTITY));
			if(index.contains(entity)) {
				index.at(entity)->setSelected(true);
			}
		}
	}
}

} // namespace ui::qt
