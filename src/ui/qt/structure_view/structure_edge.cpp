///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "domain/geometrics/edge.hpp"
#include "ui/qt/data_keys.hpp"

#include "structure_edge.hpp"

namespace ui::qt {

//******************************************************************************
StructureEdge::StructureEdge(domain::Edge const* edge, QGraphicsItem* parent)
: QGraphicsLineItem(edge->p0().x.value(), edge->p0().y.value(), edge->p1().x.value(), edge->p1().y.value(), parent)
, edge(edge)
{
	setFlags(ItemIsSelectable);
	setAcceptHoverEvents(true);
	setAcceptTouchEvents(true);
	setData(DataKeys::TYPE, "Edge");
	setData(DataKeys::ID, (qulonglong) edge->id);
//	setData(DataKeys::NAME, polygon->name); // TODO What should be edge name? <polygon>-<edge-n> ?
}

//******************************************************************************
int StructureEdge::type() const {
	return Type;
}

//******************************************************************************
void StructureEdge::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	if(option->state & QStyle::State_MouseOver) {
		painter->setPen(QPen(Qt::red, 0.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
//		QBrush brush = painter->brush();
//		brush.setColor(brush.color().lighter());
////		brush.setColor(brush.color().darker());
//		painter->setBrush(brush);
////		painter->setBrush(painter->brush(painter->brush().color().lighter());
		painter->drawLine(line());
	}
	if(option->state & QStyle::State_Selected) {
		painter->setPen(QPen(Qt::red, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		painter->drawLine(line());
	}
}

} // namespace ui::qt
