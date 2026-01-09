///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "domain/geometrics/angle.hpp"
#include "ui/qt/data_keys.hpp"
#include "utils/default_locator.hpp"

#include "structure_angle.hpp"

namespace ui::qt {

static qreal diameter = 5;

//******************************************************************************
static QRectF convert(domain::Angle const* angle) {
	return QRectF(angle->p.x.value() - diameter/2, angle->p.y.value() - diameter/2, diameter, diameter);
}

//******************************************************************************
static domain::ViewAxisSpace<QLineF> convert_cross_lines(domain::Angle const* angle) {
	return {
		QLineF( // V line -> H axis
			angle->p.x.value(), angle->p.y.value() + diameter/2,
			angle->p.x.value(), angle->p.y.value() - diameter/2),
		QLineF( // H line -> V axis
			angle->p.x.value() + diameter/2, angle->p.y.value(),
			angle->p.x.value() - diameter/2, angle->p.y.value())
	};
}

//******************************************************************************
StructureAngle::StructureAngle(domain::Angle const* angle, QGraphicsItem* parent)
: QGraphicsEllipseItem(convert(angle), parent)
, locate_structure_angle_params(default_locator<Params>)
, angle(angle)
, cross_lines(convert_cross_lines(angle))
{
	setFlags(ItemIsSelectable);
	setAcceptHoverEvents(true);
	setAcceptTouchEvents(true);
	setData(DataKeys::TYPE, "Angle");
	setData(DataKeys::ID, (qulonglong) angle->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(angle));
}

//******************************************************************************
int StructureAngle::type() const {
	return Type;
}

//******************************************************************************
void StructureAngle::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	Params const& params = locate_structure_angle_params();

	if(option->state & QStyle::State_Selected) {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.h_line_selected_hovered);
		else
			painter->setPen(params.h_line_selected);
	} else {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.h_line_regular_hovered);
		else
			painter->setPen(params.h_line_regular);
	}

	painter->drawLine(cross_lines[domain::H]);

	if(option->state & QStyle::State_Selected) {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.v_line_selected_hovered);
		else
			painter->setPen(params.v_line_selected);
	} else {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.v_line_regular_hovered);
		else
			painter->setPen(params.v_line_regular);
	}

	painter->drawLine(cross_lines[domain::V]);


	if(option->state & QStyle::State_Selected) {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.circle_selected_hovered);
		else
			painter->setPen(params.circle_selected);
	} else {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.circle_regular_hovered);
		else
			painter->setPen(params.circle_regular);
	}

	painter->drawEllipse(rect());
}

} // namespace ui::qt
