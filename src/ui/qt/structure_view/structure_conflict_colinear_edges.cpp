///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "domain/conflicts/conflict_colinear_edges.hpp"
#include "domain/geometrics/edge.hpp"
#include "utils/default_locator.hpp"
#include "utils/unreachable.hpp"
#include "ui/qt/data_keys.hpp"

#include "structure_conflict_colinear_edges.hpp"

namespace ui::qt {

//******************************************************************************
static QLineF convert(domain::ViewAxis axis, domain::ConflictColinearEdges const* conflict, QRectF scene_rect) {
	qreal side = qMax(scene_rect.width(), scene_rect.height());
	scene_rect += QMarginsF(side, side, side, side) * 2;

	switch(axis) {
	case domain::ViewAxis::H:
		return QLineF(
			scene_rect.left(), conflict->edges[0]->p0().y.value(),
			scene_rect.right(), conflict->edges[0]->p0().y.value());
	case domain::ViewAxis::V:
		return QLineF(
			conflict->edges[0]->p0().x.value(), scene_rect.bottom(),
			conflict->edges[0]->p0().x.value(), scene_rect.top());
	default:
		unreachable();
	}
}

//******************************************************************************
StructureConflictColinearEdges::StructureConflictColinearEdges(domain::ViewAxis axis, domain::ConflictColinearEdges const* conflict, QRectF const& scene_rect, QGraphicsItem* parent)
: QGraphicsLineItem(convert(axis, conflict, scene_rect), parent)
, locate_structure_conflict_ce_params(default_locator<Params>)
, axis(axis)
, conflict(conflict)
{
	setFlags(ItemIsSelectable);

	setData(DataKeys::TYPE, "ConflictColinearEdges");
	setData(DataKeys::ID, (qulonglong) conflict->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(conflict));
//	setData(DataKeys::NAME, conflict->name.c_str());
}

//******************************************************************************
int StructureConflictColinearEdges::type() const {
	return Type;
}

//******************************************************************************
void StructureConflictColinearEdges::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	Params const& params = locate_structure_conflict_ce_params();

	if(option->state & QStyle::State_Selected) {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.selected_hovered);
		else
			painter->setPen(params.selected);
	} else {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.regular_hovered);
		else
			painter->setPen(params.regular);
	}

	painter->drawLine(line());
}

} // namespace ui::qt
