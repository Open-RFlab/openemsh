///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QAction>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPainter>

#include "domain/geometrics/polygon.hpp"
#include "domain/geometrics/edge.hpp"
#include "domain/mesh/meshline.hpp"
#include "utils/unreachable.hpp"
#include "ui/qt/data_keys.hpp"
#include "ui/qt/user_types.hpp"
#include "structure_edge.hpp"
#include "structure_meshline.hpp"
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
, meshlines{{ new StructureGroup(), new StructureGroup() }}
{
	addItem(edges);
	addItem(polygons);
	for(auto* group : meshlines)
		addItem(group);

	polygons->stackBefore(edges);
	for(auto* group : meshlines)
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
StructureEdge* StructureScene::add(domain::Edge* edge) {
	auto* item = new StructureEdge(edge, edges);
	index[edge] = item;
	item->locate_structure_edge_params = [&]() -> auto& {
		return style_selector.get_edge();
	};
	return item;
}

//******************************************************************************
StructurePolygon* StructureScene::add(domain::Polygon* polygon) {
	auto* item = new StructurePolygon(polygon, polygons);
	index[polygon] = item;
	switch(polygon->type) {
	case domain::Polygon::Type::SHAPE:
		item->locate_structure_polygon_params = [&]() -> auto& {
			return style_selector.get_polygon_shape();
		};
		break;
	case domain::Polygon::Type::PORT:
		item->locate_structure_polygon_params = [&]() -> auto& {
			return style_selector.get_polygon_port();
		};
		break;
	case domain::Polygon::Type::GROUND:
		item->locate_structure_polygon_params = [&]() -> auto& {
			return style_selector.get_polygon_ground();
		};
		break;
	case domain::Polygon::Type::SUBSTRATE:
		item->locate_structure_polygon_params = [&]() -> auto& {
			return style_selector.get_polygon_substrate();
		};
		break;
	default:
		unreachable();
	}
	return item;
}

//******************************************************************************
StructureMeshline* StructureScene::add(domain::Meshline* meshline, domain::ViewAxis view_axis, QRectF const& scene_rect) {
	auto const meshline_axis = reverse(view_axis);
	auto* item = new StructureMeshline(meshline_axis, meshline, scene_rect, meshlines[meshline_axis]);
	index[meshline] = item;
	item->locate_structure_meshline_params = [&]() ->auto& {
		return style_selector.get_meshline();
	};
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
	for(auto* group : meshlines)
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
void StructureScene::clear() {
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

	QList<QGraphicsItem*> items = QGraphicsScene::items(event->scenePos());
	items.removeIf([](auto const it) { return it->type() <= QGraphicsItem::UserType; });

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

			QAction* action = new QAction(title, &menu);
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
		for(auto* foreign_item : foreign_items) {
			auto const* entity = DataKeys::get_entity(foreign_item->data(DataKeys::ENTITY));
			if(index.contains(entity)) {
				index.at(entity)->setSelected(true);
			}
		}
	}
}

} // namespace ui::qt
