///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QGraphicsLinearLayout>
#include <QPainter>

#include "utils/unreachable.hpp"
#include "wire.hpp"

#include "port.hpp"

namespace ui::qt::nodegraph {

//******************************************************************************
Port::Port(QString const& text, AnchorPoint anchor, QGraphicsItem* parent)
: QGraphicsSimpleTextItem(text, parent)
, QGraphicsLayoutItem()
, anchor(anchor)
, wire(nullptr)
{
	setGraphicsItem(this);
//	setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

//******************************************************************************
Port::~Port() {
	if(wire)
		delete wire->unwire();
}

//******************************************************************************
void Port::setGeometry(QRectF const& geom) {
	prepareGeometryChange();
	QGraphicsLayoutItem::setGeometry(geom);
	setPos(geom.topLeft());
}

//******************************************************************************
void Port::paint(QPainter* painter, QStyleOptionGraphicsItem const* /*option*/, QWidget* /*widget*/) {
	painter->setPen(QPen(Qt::white));
//	QGraphicsSimpleTextItem::paint(painter, option, widget);
	painter->drawText(boundingRect(), Qt::AlignHCenter | Qt::AlignVCenter, text());

	painter->setPen(QPen(Qt::black));
	painter->setBrush(QColor(255, 119, 0));
	painter->drawEllipse(attach_pos(), radius, radius);
//	painter->drawEllipse(QPointF(boundingRect().left(), boundingRect().center().y()), 5, 5);

#ifdef OEMSH_NODEGRAPH_DEBUG
	painter->setBrush(Qt::NoBrush);
	painter->setPen(QPen(Qt::red));
	painter->drawRect(boundingRect());
//	painter->setPen(QPen(Qt::blue));
//	painter->drawRect(x(), y(), preferredWidth(), preferredHeight());
//	painter->setPen(QPen(Qt::green));
//	painter->drawRect(x(), y(), maximumWidth(), maximumHeight());
#endif // OEMSH_NODEGRAPH_DEBUG
}

//******************************************************************************
QPointF Port::attach_pos() const {
	//TODO if hidden return parent(aka node)->parent(aka container)->in/out port
	QRectF rect = boundingRect();
	switch(anchor) {
	case AnchorPoint::TOP:    return QPointF(rect.center().x(), rect.top()-radius*1.8);
	case AnchorPoint::BOTTOM: return QPointF(rect.center().x(), rect.bottom()+radius*1.8);
	case AnchorPoint::LEFT:   return QPointF(rect.left()-radius*1.8, rect.center().y());
	case AnchorPoint::RIGHT:  return QPointF(rect.right()+radius*1.8, rect.center().y());
	default: unreachable();
	}
}

//******************************************************************************
QVariant Port::itemChange(GraphicsItemChange change, QVariant const& value) {
	if(change == ItemScenePositionHasChanged && wire != nullptr) {
		wire->update_path();
	}
	return QGraphicsItem::itemChange(change, value);
}

//******************************************************************************
QSizeF Port::sizeHint(Qt::SizeHint /*which*/, QSizeF const& /*constraint*/) const {
//	QGraphicsRectItem::sizeHint(which, constraint);
	return boundingRect().size();
//	switch(which) {
//	case Qt::MinimumSize:
//	case Qt::PreferredSize:
//		// Do not allow a size smaller than the pixmap with two frames around it.
//		return m_pix.size() + QSize(12, 12);
//	case Qt::MaximumSize:
//		return QSizeF(1000,1000);
//	default:
//		break;
//	}
//	return constraint;
}

} // namespace ui:qt::nodegraph
