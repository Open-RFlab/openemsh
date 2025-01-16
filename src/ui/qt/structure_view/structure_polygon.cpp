///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include "domain/geometrics/point.hpp"
#include "domain/geometrics/polygon.hpp"
#include "utils/unreachable.hpp"
#include "ui/qt/data_keys.hpp"

#include "structure_polygon.hpp"

namespace ui::qt {

//******************************************************************************
QPolygonF convert(domain::Polygon const* polygon) {
	QPolygonF q_polygon;
	for(auto const& point : polygon->points) {
		q_polygon.append(QPointF(point->x.value(), point->y.value()));
	}
	q_polygon.append(q_polygon.first()); // Close the polygon.
	return q_polygon;
}

//******************************************************************************
StructurePolygon::StructurePolygon(domain::Polygon const* polygon, QGraphicsItem* parent)
: QGraphicsPolygonItem(convert(polygon), parent)
, polygon(polygon)
{
	setFlags(ItemIsSelectable);
	setAcceptHoverEvents(true);
	setAcceptTouchEvents(true);
	setData(DataKeys::TYPE, "Polygon");
	setData(DataKeys::ID, (qulonglong) polygon->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(polygon));
	setData(DataKeys::NAME, polygon->name.c_str());
}

//******************************************************************************
int StructurePolygon::type() const {
	return Type;
}

//******************************************************************************
void StructurePolygon::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	painter->setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	switch(polygon->type) {
	case domain::Polygon::Type::SHAPE:
		painter->setBrush(Qt::cyan);
		break;
	case domain::Polygon::Type::PORT:
		painter->setBrush(Qt::red);
		break;
	case domain::Polygon::Type::GROUND:
		painter->setBrush(Qt::darkCyan);
		break;
	case domain::Polygon::Type::SUBSTRATE:
		painter->setBrush(Qt::green);
		break;
	default:
		unreachable();
	}

	if(option->state & QStyle::State_MouseOver) {
		QBrush brush = painter->brush();
		brush.setColor(brush.color().lighter());
//		brush.setColor(brush.color().darker());
		painter->setBrush(brush);
	}
	if(option->state & QStyle::State_Selected) {
		QBrush brush = painter->brush();
		brush.setColor(Qt::red);
		painter->setBrush(brush);
	}

	painter->drawPolygon(QGraphicsPolygonItem::polygon());
}

} // namespace ui::qt
