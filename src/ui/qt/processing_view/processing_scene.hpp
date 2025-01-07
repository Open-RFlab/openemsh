///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsScene>
#include <QObject>
#include <QList>

#include <map>

#include "ui/qt/utils/nodegraph/wire.hpp"
#include "processing_style.hpp"

class Entity;

namespace ui::qt {

class ProcessingAxis;
class ProcessingPlane;
class ProcessingPolygon;
class ProcessingEdge;
class ProcessingConflictColinearEdges;

//******************************************************************************
class ProcessingScene : public QGraphicsScene {
	Q_OBJECT
public:

	explicit ProcessingScene(QObject* parent = nullptr);
	~ProcessingScene();

	void set_wire_style(nodegraph::Wire::Style style);
	void fit_containers();

	ProcessingStyleSelector style_selector;

//private: //TODO
	QList<nodegraph::Wire*> wires;
	QList<ProcessingPlane*> planes;
	QList<ProcessingAxis*> axes;
	QList<ProcessingPolygon*> polygons;
	QList<ProcessingEdge*> edges;
	QList<QGraphicsItem*> meshlines;
	QList<QGraphicsItem*> intervals;
	QList<QGraphicsItem*> conflict_edge_in_polygons;
	QList<ProcessingConflictColinearEdges*> conflict_colinear_edges;
	QList<QGraphicsItem*> conflict_too_close_meshline_policies;
	QList<QGraphicsItem*> policies;

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
};

} // namespace ui::qt
