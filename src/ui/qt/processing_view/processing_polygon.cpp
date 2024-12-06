///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "domain/geometrics/polygon.hpp"
#include "ui/qt/utils/nodegraph/rect.hpp"
#include "processing_edge.hpp"

#include "processing_polygon.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingPolygon::ProcessingPolygon(domain::Polygon const* polygon, QGraphicsItem* parent)
: nodegraph::Container("Polygon " + (polygon ? QString::fromStdString(polygon->name) : QString()), QSizeF(50, 10), parent)
, polygon(polygon)
{}

//******************************************************************************
ProcessingPolygon::~ProcessingPolygon() = default;

//******************************************************************************
int ProcessingPolygon::type() const {
	return Type;
}

//******************************************************************************
void ProcessingPolygon::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget) {
	nodegraph::Container::paint(painter, option, widget);
}

//******************************************************************************
QList<ProcessingEdge*> ProcessingPolygon::edges() {
	// TODO better
	QList<QGraphicsItem*> const children = nested_zone->childItems();
	QList<ProcessingEdge*> edges(children.size());
	for(qsizetype i = 0; i < children.size(); ++i)
		edges[i] = dynamic_cast<ProcessingEdge*>(children[i]);
	return edges;
}

} // namespace ui::qt
