///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "utils/default_locator.hpp"

#include "rect.hpp"

namespace ui::qt::nodegraph {

//******************************************************************************
Rect::Rect(QGraphicsItem* parent)
: QGraphicsRectItem(parent)
, QGraphicsLayoutItem()
, locate_rect_params(default_locator<Params>)
{
	setGraphicsItem(this);
}

//******************************************************************************
Rect::Rect(qreal x, qreal y, qreal width, qreal height, QGraphicsItem* parent)
: QGraphicsRectItem(x, y, width, height, parent)
, QGraphicsLayoutItem()
, locate_rect_params(default_locator<Params>)
{
	setGraphicsItem(this);
}

//******************************************************************************
Rect::~Rect() = default;

//******************************************************************************
void Rect::setGeometry(QRectF const& geom) {
	prepareGeometryChange();
	QGraphicsLayoutItem::setGeometry(geom);
	setPos(geom.topLeft());
}

//******************************************************************************
QSizeF Rect::sizeHint(Qt::SizeHint /*which*/, QSizeF const& /*constraint*/) const {
	return boundingRect().size();
}

//******************************************************************************
void Rect::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	Params const& params = locate_rect_params();

	if(option->state & QStyle::State_Selected) {
		if(option->state & QStyle::State_MouseOver) {
			painter->setBrush(params.fill_selected_hovered);
			painter->setPen(params.contour_selected_hovered);
			painter->setOpacity(params.opacity_selected_hovered);
		} else {
			painter->setBrush(params.fill_selected);
			painter->setPen(params.contour_selected);
			painter->setOpacity(params.opacity_selected);
		}
	} else if(is_highlighted()) {
		if(option->state & QStyle::State_MouseOver) {
			painter->setBrush(params.fill_highlighted_hovered);
			painter->setPen(params.contour_highlighted_hovered);
			painter->setOpacity(params.opacity_highlighted_hovered);
		} else {
			painter->setBrush(params.fill_highlighted);
			painter->setPen(params.contour_highlighted);
			painter->setOpacity(params.opacity_highlighted);
		}
	} else {
		if(option->state & QStyle::State_MouseOver) {
			painter->setBrush(params.fill_regular_hovered);
			painter->setPen(params.contour_regular_hovered);
			painter->setOpacity(params.opacity_regular_hovered);
		} else {
			painter->setBrush(params.fill_regular);
			painter->setPen(params.contour_regular);
			painter->setOpacity(params.opacity_regular);
		}
	}

	painter->drawRect(rect());
}

} // namespace ui:qt::nodegraph
