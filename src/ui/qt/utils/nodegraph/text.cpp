///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "text.hpp"

namespace ui::qt::nodegraph {

//******************************************************************************
Text::Text(QString const& text, QGraphicsItem* parent)
: QGraphicsSimpleTextItem(text, parent)
, QGraphicsLayoutItem()
{
	setGraphicsItem(this);
}

//******************************************************************************
Text::~Text() = default;

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
void Text::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget) {
	// TODO centered ?
	QGraphicsSimpleTextItem::paint(painter, option, widget);
	// TODO check QGraphicsSimpleTextItem::paint code to understand why painter->drawText
	// does not use object pen / font / brush.
//	painter->setFont(font());   // ???
//	painter->setPen(pen());     // ???
//	painter->setBrush(brush()); // ???
//	painter->drawText(boundingRect(), Qt::AlignHCenter | Qt::AlignVCenter, text());
}

} // namespace ui::qt::nodegraph
