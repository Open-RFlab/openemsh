///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsScene>
#include <QObject>

#include <map>

#include "domain/geometrics/space.hpp"
#include "structure_style.hpp"

class Entity;

namespace domain {
class Edge;
class Interval;
class Meshline;
class MeshlinePolicy;
class Polygon;
} // namespace domain

namespace ui::qt {

class StructureEdge;
class StructureInterval;
class StructureMeshline;
class StructureMeshlinePolicy;
class StructurePolygon;

//******************************************************************************
class StructureGroup : public QGraphicsItem {
public:
	explicit StructureGroup(QGraphicsItem* parent = nullptr);
	QRectF boundingRect() const override;
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;
};

//******************************************************************************
class StructureScene : public QGraphicsScene {
	Q_OBJECT
public:

	enum class MeshVisibility {
		NONE,
		VERTICAL,
		HORIZONTAL,
		FULL
	};

	explicit StructureScene(StructureStyleSelector& style_selector, QObject* parent = nullptr);
	~StructureScene();

	StructureEdge* add(domain::Edge* edge);
	StructureInterval* add(domain::Interval* interval, domain::ViewAxis view_axis, QRectF const& scene_rect);
	StructureMeshline* add(domain::Meshline* meshline, domain::ViewAxis view_axis, QRectF const& scene_rect);
	StructureMeshlinePolicy* add(domain::MeshlinePolicy* policy, domain::ViewAxis view_axis, QRectF const& scene_rect);
	StructurePolygon* add(domain::Polygon* polygon);

	void clear_edges();
	void clear_polygons();
	void clear_meshlines();
	void clear();

	void set_mesh_visibility(MeshVisibility mesh_visibility);

	StructureStyleSelector& style_selector;
//private: //TODO
	// TODO structure item layer + z value
	StructureGroup* edges;
	StructureGroup* polygons;
	domain::ViewAxisSpace<StructureGroup*> intervals;
	domain::ViewAxisSpace<StructureGroup*> meshlines;
	domain::ViewAxisSpace<StructureGroup*> meshline_policies;

//	std::map<std::size_t, QGraphicsItem*> index;
	std::map<Entity const*, QGraphicsItem*> index;

private slots:
	void on_selectionChanged();
signals:
	void selection_changed(QList<QGraphicsItem*> items);
public slots:
	void select_counterparts(QList<QGraphicsItem*> foreign_items);
private:
	bool is_select_counterparts_locked = false;

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

} // namespace ui::qt
