///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "domain/geometrics/angle.hpp"
#include "domain/conflicts/conflict_diagonal_or_circular_zone.hpp"
#include "utils/default_locator.hpp"
#include "utils/unreachable.hpp"
#include "ui/qt/data_keys.hpp"

#include "structure_conflict_diagonal_or_circular_zone.hpp"

namespace ui::qt {

//******************************************************************************
static QRectF convert(domain::ViewAxis axis, domain::ConflictDiagonalOrCircularZone const* conflict, QRectF scene_rect) {
	qreal side = qMax(scene_rect.width(), scene_rect.height());
	scene_rect += QMarginsF(side, side, side, side) * 2;
	auto bounding = conflict->bounding();

	switch(axis) {
	case domain::ViewAxis::H:
		return QRectF(
			QPointF(scene_rect.left(), bounding[domain::XMIN].value()),
			QPointF(scene_rect.right(), bounding[domain::XMAX].value()));
	case domain::ViewAxis::V:
		return QRectF(
			QPointF(bounding[domain::XMIN].value(), scene_rect.bottom()),
			QPointF(bounding[domain::XMAX].value(), scene_rect.top()));
	default:
		unreachable();
	}
}

//******************************************************************************
StructureConflictDiagonalOrCircularZone::StructureConflictDiagonalOrCircularZone(domain::ViewAxis axis, domain::ConflictDiagonalOrCircularZone const* conflict, QRectF const& scene_rect, QGraphicsItem* parent)
: QGraphicsRectItem(convert(axis, conflict, scene_rect), parent)
, locate_structure_conflict_docz_params(default_locator<Params>)
, axis(axis)
, conflict(conflict)
{
	setFlags(ItemIsSelectable);

	setData(DataKeys::TYPE, "ConflictDiagonalOrCircularZone");
	setData(DataKeys::ID, (qulonglong) conflict->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(conflict));
//	setData(DataKeys::NAME, QString::fromStdString(conflict->name));
}

//******************************************************************************
int StructureConflictDiagonalOrCircularZone::type() const {
	return Type;
}

//******************************************************************************
QVariant StructureConflictDiagonalOrCircularZone::itemChange(GraphicsItemChange change, QVariant const& value) {
	if(change == ItemSelectedChange) {
		// StructureConflictDiagonalOrCircularZone is almost always zoomed enough to disable repaint trigger.
		prepareGeometryChange();
		update();
	}
	return QGraphicsItem::itemChange(change, value);
}

//******************************************************************************
void StructureConflictDiagonalOrCircularZone::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	Params const& params = locate_structure_conflict_docz_params();

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
