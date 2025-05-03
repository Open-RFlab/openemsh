///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "domain/conflicts/conflict_too_close_meshline_policies.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "utils/default_locator.hpp"
#include "utils/unreachable.hpp"
#include "ui/qt/data_keys.hpp"

#include "structure_conflict_too_close_meshline_policies.hpp"

namespace ui::qt {

//******************************************************************************
static QLineF convert(domain::ViewAxis axis, domain::MeshlinePolicy const* meshline_policy, QRectF scene_rect) {
	qreal side = qMax(scene_rect.width(), scene_rect.height());
	scene_rect += QMarginsF(side, side, side, side) * 2;

	switch(axis) {
	case domain::ViewAxis::H:
		return QLineF(
			scene_rect.left(), meshline_policy->coord.value(),
			scene_rect.right(), meshline_policy->coord.value());
	case domain::ViewAxis::V:
		return QLineF(
			meshline_policy->coord.value(), scene_rect.bottom(),
			meshline_policy->coord.value(), scene_rect.top());
	default:
		unreachable();
	}
}

//******************************************************************************
StructureConflictTooCloseMeshlinePolicies::StructureConflictTooCloseMeshlinePolicies(domain::ViewAxis axis, domain::ConflictTooCloseMeshlinePolicies const* conflict, QRectF const& scene_rect, QGraphicsItem* parent)
: QGraphicsLineItem(convert(axis, dynamic_cast<domain::MeshlinePolicy*>(conflict->get_current_state().solution), scene_rect), parent)
, locate_structure_conflict_tcmlp_params(default_locator<Params>)
, axis(axis)
, conflict(conflict)
, origin_lines{{
	convert(axis, conflict->meshline_policies[0], scene_rect),
	convert(axis, conflict->meshline_policies[1], scene_rect) }}
{
	setFlags(ItemIsSelectable);

	setData(DataKeys::TYPE, "ConflictTooCloseMeshlinePolicies");
	setData(DataKeys::ID, (qulonglong) conflict->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(conflict));
//	setData(DataKeys::NAME, conflict->name.c_str());
}

//******************************************************************************
int StructureConflictTooCloseMeshlinePolicies::type() const {
	return Type;
}

//******************************************************************************
void StructureConflictTooCloseMeshlinePolicies::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	Params const& params = locate_structure_conflict_tcmlp_params();

	if(option->state & QStyle::State_Selected) {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.solution_line_selected_hovered);
		else
			painter->setPen(params.solution_line_selected);
	} else {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.solution_line_regular_hovered);
		else
			painter->setPen(params.solution_line_regular);
	}

	painter->drawLine(line());

	if(option->state & QStyle::State_Selected) {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.origin_lines_selected_hovered);
		else
			painter->setPen(params.origin_lines_selected);
	} else {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.origin_lines_regular_hovered);
		else
			painter->setPen(params.origin_lines_regular);
	}

	for(auto& line : origin_lines)
		painter->drawLine(line);
}

} // namespace ui::qt
