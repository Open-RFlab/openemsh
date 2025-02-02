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
#include "utils/default_locator.hpp"
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
, locate_structure_polygon_params(default_locator<Params>)
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
	Params const& params = locate_structure_polygon_params();

	if(option->state & QStyle::State_Selected) {
		if(option->state & QStyle::State_MouseOver) {
			painter->setBrush(params.fill_selected_hovered);
			painter->setPen(params.contour_selected_hovered);
		} else {
			painter->setBrush(params.fill_selected);
			painter->setPen(params.contour_selected);
		}
	} else {
		if(option->state & QStyle::State_MouseOver) {
			painter->setBrush(params.fill_regular_hovered);
			painter->setPen(params.contour_regular_hovered);
		} else {
			painter->setBrush(params.fill_regular);
			painter->setPen(params.contour_regular);
		}
	}

	painter->drawPolygon(QGraphicsPolygonItem::polygon());
}

} // namespace ui::qt
