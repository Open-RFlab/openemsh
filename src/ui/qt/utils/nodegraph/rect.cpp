///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "rect.hpp"

namespace ui::qt::nodegraph {

//******************************************************************************
Rect::Rect(QGraphicsItem* parent)
: QGraphicsRectItem(parent)
, QGraphicsLayoutItem()
{
	setGraphicsItem(this);
}

//******************************************************************************
Rect::Rect(qreal x, qreal y, qreal width, qreal height, QGraphicsItem* parent)
: QGraphicsRectItem(x, y, width, height, parent)
, QGraphicsLayoutItem()
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

} // namespace ui:qt::nodegraph
