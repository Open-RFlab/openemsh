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
#include "wire.hpp"

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
		if(value.toBool())
			set_highlighted(false, this);
		for(auto* item : childItems()) {
			item->setSelected(value.toBool());
		}
		for(auto* node : get_chain()) {
			node->set_highlighted(value.toBool(), this);
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
		for(auto* node : get_chain()) {
			node->set_highlighted(true, this);
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

	if(option->state & QStyle::State_Selected) {
		if(option->state & QStyle::State_MouseOver)
			painter->setBrush(params.background_selected_hovered);
		else
			painter->setBrush(params.background_selected);
	} else if(is_highlighted()) {
		if(option->state & QStyle::State_MouseOver)
			painter->setBrush(params.background_highlighted_hovered);
		else
			painter->setBrush(params.background_highlighted);
	} else {
		if(option->state & QStyle::State_MouseOver)
			painter->setBrush(params.background_regular_hovered);
		else
			painter->setBrush(params.background_regular);
	}

	painter->setPen(Qt::NoPen);
	painter->drawRoundedRect(boundingRect(), params.radius, params.radius);

	if(title) {
		if(option->state & QStyle::State_Selected) {
			if(option->state & QStyle::State_MouseOver)
				painter->setBrush(params.title_background_selected_hovered);
			else
				painter->setBrush(params.title_background_selected);
		} else if(is_highlighted()) {
			if(option->state & QStyle::State_MouseOver)
				painter->setBrush(params.title_background_highlighted_hovered);
			else
				painter->setBrush(params.title_background_highlighted);
		} else {
			if(option->state & QStyle::State_MouseOver)
				painter->setBrush(params.title_background_regular_hovered);
			else
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

//******************************************************************************
void Node::traverse_up(QSet<Node*>& out, Node const* node) {
	for(auto const* port : node->input_ports) {
		for(auto const* wire : port->get_wires()) {
			if(wire) {
				if(auto const* forward_port = wire->traverse(port)
				; forward_port) {
					if(auto* forward_node = forward_port->get_node()
					; forward_node && !out.contains(forward_node)) {
						out.insert(forward_node);
						traverse_up(out, forward_node);
					}
				}
			}
		}
	}
}

//******************************************************************************
void Node::traverse_down(QSet<Node*>& out, Node const* node) {
	for(auto const* port : node->output_ports) {
		for(auto const* wire : port->get_wires()) {
			if(wire) {
				if(auto const* forward_port = wire->traverse(port)
				; forward_port) {
					if(auto* forward_node = forward_port->get_node()
					; forward_node && !out.contains(forward_node)) {
						out.insert(forward_node);
						traverse_down(out, forward_node);
					}
				}
			}
		}
	}
}

//******************************************************************************
QList<Node*> Node::get_chain() const {
	QSet<Node*> up, down;
	traverse_up(up, this);
	traverse_down(down, this);
	return (std::move(up) + std::move(down)).values();
}

//******************************************************************************
void Node::set_highlighted(bool is_highlighted, QGraphicsItem const* by_item) {
	Highlightable::set_highlighted(is_highlighted, by_item);
	if(title)
		title->set_highlighted(is_highlighted);
	for(auto& list : { input_ports, output_ports }) {
		for(auto* port : list) {
			if(port)
				port->set_highlighted(is_highlighted);
		}
	}
}

} // namespace ui::qt
