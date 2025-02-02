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

#include "domain/geometrics/space.hpp"
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
, vertical_meshlines(new StructureGroup())
, horizontal_meshlines(new StructureGroup())
{
	addItem(edges);
	addItem(polygons);
	addItem(vertical_meshlines);
	addItem(horizontal_meshlines);
	polygons->stackBefore(edges);
	edges->stackBefore(vertical_meshlines);
	edges->stackBefore(horizontal_meshlines);

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
void StructureScene::add(StructureEdge* edge) {
	edge->setParentItem(edges);
	edge->locate_structure_edge_params = [&]() ->auto& {
		return style_selector.get_edge();
	};
}

//******************************************************************************
void StructureScene::add(StructurePolygon* polygon) {
	polygon->setParentItem(polygons);
	polygon->locate_structure_polygon_params = [&]() ->auto& {
		return style_selector.get_polygon_shape();
	};
}

//******************************************************************************
void StructureScene::add(StructureMeshline* meshline) {
	switch(meshline->axis) {
	case domain::ViewAxis::H: meshline->setParentItem(horizontal_meshlines); break;
	case domain::ViewAxis::V: meshline->setParentItem(vertical_meshlines); break;
	default: unreachable();
	}
	meshline->locate_structure_meshline_params = [&]() ->auto& {
		return style_selector.get_meshline();
	};
}

//******************************************************************************
void StructureScene::clear_edges() {
	removeItem(edges);
	delete edges;
	edges = new StructureGroup();
	addItem(edges);

	polygons->stackBefore(edges);
	edges->stackBefore(vertical_meshlines);
	edges->stackBefore(horizontal_meshlines);
}

//******************************************************************************
void StructureScene::clear_polygons() {
	removeItem(polygons);
	delete polygons;
	polygons = new StructureGroup();
	addItem(polygons);

	polygons->stackBefore(edges);
}

//******************************************************************************
void StructureScene::clear_meshlines() {
	removeItem(vertical_meshlines);
	removeItem(horizontal_meshlines);
	delete vertical_meshlines;
	delete horizontal_meshlines;
	vertical_meshlines = new StructureGroup();
	horizontal_meshlines = new StructureGroup();
	addItem(vertical_meshlines);
	addItem(horizontal_meshlines);

	edges->stackBefore(vertical_meshlines);
	edges->stackBefore(horizontal_meshlines);
}

//******************************************************************************
void StructureScene::clear() {
	clear_edges();
	clear_polygons();
	clear_meshlines();
}

//******************************************************************************
void StructureScene::set_mesh_visibility(MeshVisibility mesh_visibility) {
	switch(mesh_visibility) {
	case MeshVisibility::NONE:
		horizontal_meshlines->setVisible(false);
		vertical_meshlines->setVisible(false);
		break;
	case MeshVisibility::VERTICAL:
		horizontal_meshlines->setVisible(false);
		vertical_meshlines->setVisible(true);
		break;
	case MeshVisibility::HORIZONTAL:
		horizontal_meshlines->setVisible(true);
		vertical_meshlines->setVisible(false);
		break;
	case MeshVisibility::FULL:
		horizontal_meshlines->setVisible(true);
		vertical_meshlines->setVisible(true);
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
