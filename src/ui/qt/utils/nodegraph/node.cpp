///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "utils/default_locator.hpp"
#include "text.hpp"

#include "node.hpp"

namespace ui::qt::nodegraph {

//******************************************************************************
Node::Node(QString title, QGraphicsItem* parent)
: QGraphicsWidget(parent)
, locate_node_params(default_locator<Params>)
, title(new Text(title, this))
, linear_layout(new QGraphicsLinearLayout(Qt::Vertical, this))
{
	setFlag(QGraphicsItem::ItemContainsChildrenInShape);
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	setAcceptHoverEvents(true);
	setAcceptTouchEvents(true);
	setLayout(linear_layout);
	this->title->setFlag(QGraphicsItem::ItemIsSelectable);
	this->title->setAcceptedMouseButtons(Qt::NoButton);
	linear_layout->addItem(this->title);
}

//******************************************************************************
Node::Node(QGraphicsItem* parent)
: QGraphicsWidget(parent)
, locate_node_params(default_locator<Params>)
, title(nullptr)
, linear_layout(new QGraphicsLinearLayout(Qt::Vertical, this))
{
	setFlag(QGraphicsItem::ItemContainsChildrenInShape);
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
	} else if(change == ItemSelectedChange) {
		for(auto* item : childItems()) {
			item->setSelected(value.toBool());
		}
		return value;
	}
	return QGraphicsItem::itemChange(change, value);
}

//******************************************************************************
void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
	QGraphicsItem::mouseReleaseEvent(event);
	// Propagate selection to children as QGraphicsItem::mouseReleaseEvent() may
	// perform QGraphicsScene::clearSelection().
	if(isSelected()) {
		for(auto* item : childItems()) {
			item->setSelected(true);
		}
	}
}

//******************************************************************************
QGraphicsLinearLayout* Node::layout() {
	return linear_layout;
}

//******************************************************************************
void Node::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	Params const& params = locate_node_params();

	if(option->state & QStyle::State_MouseOver
	&& option->state & QStyle::State_Selected) {
		painter->setBrush(params.background_selected_highlighted);
	} else if(option->state & QStyle::State_MouseOver) {
		painter->setBrush(params.background_highlighted);
	} else if(option->state & QStyle::State_Selected) {
		painter->setBrush(params.background_selected);
	} else {
		painter->setBrush(params.background_regular);
	}

	painter->setPen(Qt::NoPen);
	painter->drawRoundedRect(boundingRect(), params.radius, params.radius);

	if(title) {
		if(option->state & QStyle::State_MouseOver
		&& option->state & QStyle::State_Selected) {
			painter->setBrush(params.title_background_selected_highlighted);
		} else if(option->state & QStyle::State_MouseOver) {
			painter->setBrush(params.title_background_highlighted);
		} else if(option->state & QStyle::State_Selected) {
			painter->setBrush(params.title_background_selected);
		} else {
			painter->setBrush(params.title_background_regular);
		}
		QRectF titlebar = boundingRect();
		titlebar.setBottom(title->mapToParent(title->boundingRect()).boundingRect().bottom());
		painter->drawRoundedRect(titlebar, params.radius, params.radius);
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
