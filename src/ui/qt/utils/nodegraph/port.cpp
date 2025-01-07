///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "utils/default_locator.hpp"
#include "utils/unreachable.hpp"
#include "wire.hpp"

#include "port.hpp"

namespace ui::qt::nodegraph {

//******************************************************************************
Port::Port(QString const& text, AnchorPoint anchor, QGraphicsItem* parent)
: QGraphicsSimpleTextItem(text, parent)
, QGraphicsLayoutItem()
, locate_port_params(default_locator<Params>)
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
void Port::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	Params const& params = locate_port_params();

	if(option->state & QStyle::State_MouseOver
	&& option->state & QStyle::State_Selected) {
		painter->setPen(params.text_selected_highlighted);
		painter->drawText(boundingRect(), Qt::AlignHCenter | Qt::AlignVCenter, text());
		painter->setPen(params.contour_selected_highlighted);
		painter->setBrush(params.fill_selected_highlighted);
		painter->drawEllipse(attach_pos(), params.radius, params.radius);
	} else if(option->state & QStyle::State_MouseOver) {
		painter->setPen(params.text_highlighted);
		painter->drawText(boundingRect(), Qt::AlignHCenter | Qt::AlignVCenter, text());
		painter->setPen(params.contour_highlighted);
		painter->setBrush(params.fill_highlighted);
		painter->drawEllipse(attach_pos(), params.radius, params.radius);
	} else if(option->state & QStyle::State_Selected) {
		painter->setPen(params.text_selected);
		painter->drawText(boundingRect(), Qt::AlignHCenter | Qt::AlignVCenter, text());
		painter->setPen(params.contour_selected);
		painter->setBrush(params.fill_selected);
		painter->drawEllipse(attach_pos(), params.radius, params.radius);
	} else {
		painter->setPen(params.text_regular);
		painter->drawText(boundingRect(), Qt::AlignHCenter | Qt::AlignVCenter, text());
		painter->setPen(params.contour_regular);
		painter->setBrush(params.fill_regular);
		painter->drawEllipse(attach_pos(), params.radius, params.radius);
	}

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
	QRectF const rect = boundingRect();
	QGraphicsItem const* const parent = parentItem();
	QRectF const parent_rect = parent
	                         ? mapFromParent(parent->boundingRect()).boundingRect()
	                         : rect;

	switch(anchor) {
	case AnchorPoint::TOP:    return QPointF(rect.center().x(), parent_rect.top());
	case AnchorPoint::BOTTOM: return QPointF(rect.center().x(), parent_rect.bottom());
	case AnchorPoint::LEFT:   return QPointF(parent_rect.left(), rect.center().y());
	case AnchorPoint::RIGHT:  return QPointF(parent_rect.right(), rect.center().y());
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
