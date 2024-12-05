///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "text.hpp"

#include "node.hpp"

namespace ui::qt::nodegraph {

//******************************************************************************
Node::Node(QString title, QGraphicsItem* parent)
: QGraphicsWidget(parent)
, title(new Text(title, this))
, linear_layout(new QGraphicsLinearLayout(Qt::Vertical, this))
{
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	setAcceptHoverEvents(true);
	setAcceptTouchEvents(true);
	setLayout(linear_layout);
	this->title->setBrush(Qt::white);
	linear_layout->addItem(this->title);
}


//******************************************************************************
Node::Node(QGraphicsItem* parent)
: QGraphicsWidget(parent)
, title(nullptr)
, linear_layout(new QGraphicsLinearLayout(Qt::Vertical, this))
{
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	setAcceptHoverEvents(true);
	setAcceptTouchEvents(true);
	setLayout(linear_layout);
}

//******************************************************************************
Node::~Node() = default;

//******************************************************************************
QVariant Node::itemChange(GraphicsItemChange change, QVariant const& value) {
	if(change == ItemPositionChange && parentItem() != nullptr) {
		QRectF const rect = parentItem()->boundingRect()
		                 -= QMarginsF(0, 0, boundingRect().width(), boundingRect().height());
		QPointF newPos = value.toPointF();
		if(!rect.contains(newPos)) {
			// Keep the item inside the scene rect.
			newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
			newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
			return newPos;
		}
	}
	return QGraphicsItem::itemChange(change, value);
}

//******************************************************************************
QGraphicsLinearLayout* Node::layout() {
	return linear_layout;
}

//******************************************************************************
void Node::paint(QPainter* painter, QStyleOptionGraphicsItem const* /*option*/, QWidget* /*widget*/) {
	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor(27, 27, 27));

	// TODO radius = title height / 2
	painter->drawRoundedRect(boundingRect(), 10, 10);

	if(title) {
		painter->setBrush(QColor(33, 33, 33));
		QRectF titlebar = boundingRect();
		titlebar.setBottom(title->mapToParent(title->boundingRect()).boundingRect().bottom());
		painter->drawRoundedRect(titlebar, 10, 10);
		titlebar.setTop(titlebar.center().y());
		painter->drawRect(titlebar);
	}
}

//******************************************************************************
Port* Node::add_input_port(QString const& text, Port::AnchorPoint anchor) {
	Port* port = new Port(text, anchor, this);
	input_ports.append(port);
	return port;
}

//******************************************************************************
Port* Node::add_output_port(QString const& text, Port::AnchorPoint anchor) {
	Port* port = new Port(text, anchor, this);
	output_ports.append(port);
	return port;
}

} // namespace ui::qt
