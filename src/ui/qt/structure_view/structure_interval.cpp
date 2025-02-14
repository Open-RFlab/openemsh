///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "domain/mesh/interval.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "utils/default_locator.hpp"
#include "utils/unreachable.hpp"
#include "ui/qt/data_keys.hpp"

#include "structure_interval.hpp"

namespace ui::qt {

//******************************************************************************
static QRectF convert(domain::ViewAxis axis, domain::Interval const* interval, QRectF scene_rect) {
	qreal side = qMax(scene_rect.width(), scene_rect.height());
	scene_rect += QMarginsF(side, side, side, side) * 2;

	switch(axis) {
	case domain::ViewAxis::H:
		return QRectF(
			QPointF(scene_rect.left(), interval->before.meshline_policy->coord.value()),
			QPointF(scene_rect.right(), interval->after.meshline_policy->coord.value()));
//		return QLineF(
//			scene_rect.left(), interval->coord.value(),
//			scene_rect.right(), interval->coord.value());
	case domain::ViewAxis::V:
		return QRectF(
			QPointF(interval->before.meshline_policy->coord.value(), scene_rect.bottom()),
			QPointF(interval->after.meshline_policy->coord.value(), scene_rect.top()));
//		return QLineF(
//			interval->coord.value(), scene_rect.bottom(),
//			interval->coord.value(), scene_rect.top());
	default:
		unreachable();
	}
}

//******************************************************************************
StructureInterval::StructureInterval(domain::ViewAxis axis, domain::Interval const* interval, QRectF const& scene_rect, QGraphicsItem* parent)
: QGraphicsRectItem(convert(axis, interval, scene_rect), parent)
, locate_structure_interval_params(default_locator<Params>)
, axis(axis)
, interval(interval)
{
	setFlags(ItemIsSelectable);

	setData(DataKeys::TYPE, "Interval");
	setData(DataKeys::ID, (qulonglong) interval->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(interval));
//	setData(DataKeys::NAME, interval->name.c_str());
}

//******************************************************************************
int StructureInterval::type() const {
	return Type;
}

//******************************************************************************
QVariant StructureInterval::itemChange(GraphicsItemChange change, QVariant const& value) {
	if(change == ItemSelectedChange) {
		// StructureInterval is almost always zoomed enough to disable repaint trigger.
		prepareGeometryChange();
		update();
	}
	return QGraphicsItem::itemChange(change, value);
}

//******************************************************************************
void StructureInterval::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	Params const& params = locate_structure_interval_params();

	if(option->state & QStyle::State_Selected) {
		if(option->state & QStyle::State_MouseOver)
			painter->setBrush(params.selected_hovered);
		else
			painter->setBrush(params.selected);
	} else {
		if(option->state & QStyle::State_MouseOver)
			painter->setBrush(params.regular_hovered);
		else
			painter->setBrush(params.regular);
	}

	painter->setPen(Qt::NoPen);
	painter->drawRect(rect());
}

} // namespace ui::qt
