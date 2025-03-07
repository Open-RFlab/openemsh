///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "utils/default_locator.hpp"

#include "text.hpp"

namespace ui::qt::nodegraph {

//******************************************************************************
Text::Text(QString const& text, QGraphicsItem* parent)
: QGraphicsSimpleTextItem(text, parent)
, QGraphicsLayoutItem()
, locate_text_params(default_locator<Params>)
{
	setGraphicsItem(this);
}

//******************************************************************************
void Text::setGeometry(QRectF const& geom) {
	prepareGeometryChange();
	QGraphicsLayoutItem::setGeometry(geom);
	setPos(geom.topLeft());
}

//******************************************************************************
QSizeF Text::sizeHint(Qt::SizeHint /*which*/, QSizeF const& /*constraint*/) const {
	return boundingRect().size();
}

//******************************************************************************
void Text::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	Params const& params = locate_text_params();

	if(option->state & QStyle::State_Selected) {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.selected_hovered);
		else
			painter->setPen(params.selected);
	} else if(is_highlighted()) {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.highlighted_hovered);
		else
			painter->setPen(params.highlighted);
	} else {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.regular_hovered);
		else
			painter->setPen(params.regular);
	}

	painter->drawText(boundingRect(), Qt::AlignHCenter | Qt::AlignVCenter, text());
}

} // namespace ui::qt::nodegraph
