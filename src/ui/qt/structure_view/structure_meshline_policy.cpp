///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "domain/mesh/meshline_policy.hpp"
#include "utils/default_locator.hpp"
#include "utils/unreachable.hpp"
#include "ui/qt/data_keys.hpp"

#include "structure_meshline_policy.hpp"

namespace ui::qt {

//******************************************************************************
static QLineF convert_main(domain::ViewAxis axis, domain::MeshlinePolicy const* meshline_policy, QRectF scene_rect) {
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

// TODO Refactor Interval::Side to combine with this.
//******************************************************************************
static std::array<QLineF, 2> convert_policy_lines(domain::ViewAxis axis, domain::MeshlinePolicy const* meshline_policy, QRectF scene_rect) {
	qreal side = qMax(scene_rect.width(), scene_rect.height());
	scene_rect += QMarginsF(side, side, side, side) * 2;

	auto const offset = [&]() -> std::array<double, 2> {
		auto const d = meshline_policy->d;
		switch(meshline_policy->policy) {
		case domain::MeshlinePolicy::Policy::ONELINE: return { 0.0, 0.0 };
		case domain::MeshlinePolicy::Policy::HALFS: return { d / 2.0, -d / 2.0 };
		case domain::MeshlinePolicy::Policy::THIRDS:
			switch(meshline_policy->normal) {
			case domain::MeshlinePolicy::Normal::MAX:
				return { 2.0 / 3.0 * d, -1.0 / 3.0 * d };
			case domain::MeshlinePolicy::Normal::MIN:
				return { 1.0 / 3.0 * d, -2.0 / 3.0 * d };
			default:
				return { 0.0, 0.0 };
			}
		default: unreachable();
		}
	} ();

	switch(axis) {
	case domain::ViewAxis::H:
		return {
			QLineF(
				scene_rect.left(), meshline_policy->coord.value() + offset[0],
				scene_rect.right(), meshline_policy->coord.value() + offset[0]),
			QLineF(
				scene_rect.left(), meshline_policy->coord.value() + offset[1],
				scene_rect.right(), meshline_policy->coord.value() + offset[1])
		};
	case domain::ViewAxis::V:
		return {
			QLineF(
				meshline_policy->coord.value() + offset[0], scene_rect.bottom(),
				meshline_policy->coord.value() + offset[0], scene_rect.top()),
			QLineF(
				meshline_policy->coord.value() + offset[1], scene_rect.bottom(),
				meshline_policy->coord.value() + offset[1], scene_rect.top())
		};
	default:
		unreachable();
	}
}

//******************************************************************************
StructureMeshlinePolicy::StructureMeshlinePolicy(domain::ViewAxis axis, domain::MeshlinePolicy const* meshline_policy, QRectF scene_rect, QGraphicsItem* parent)
: QGraphicsLineItem(convert_main(axis, meshline_policy, scene_rect), parent)
, locate_structure_meshline_policy_params(default_locator<Params>)
, axis(axis)
, meshline_policy(meshline_policy)
, policy_lines(convert_policy_lines(axis, meshline_policy, scene_rect))
{
	setFlags(ItemIsSelectable);

	setData(DataKeys::TYPE, "MeshlinePolicy");
	setData(DataKeys::ID, (qulonglong) meshline_policy->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(meshline_policy));
//	setData(DataKeys::NAME, meshline_policy->name.c_str());
}

//******************************************************************************
int StructureMeshlinePolicy::type() const {
	return Type;
}

//******************************************************************************
void StructureMeshlinePolicy::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	Params const& params = locate_structure_meshline_policy_params();

	if(option->state & QStyle::State_Selected) {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.main_line_selected_hovered);
		else
			painter->setPen(params.main_line_selected);
	} else {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.main_line_regular_hovered);
		else
			painter->setPen(params.main_line_regular);
	}

	painter->drawLine(line());

	if(option->state & QStyle::State_Selected) {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.policy_lines_selected_hovered);
		else
			painter->setPen(params.policy_lines_selected);
	} else {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.policy_lines_regular_hovered);
		else
			painter->setPen(params.policy_lines_regular);
	}

	for(auto& line : policy_lines)
		painter->drawLine(line);
}

} // namespace ui::qt
