///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QStyleOptionGraphicsItem>

#include "rect.hpp"
#include "text.hpp"

#include "container.hpp"

namespace ui::qt::nodegraph {

//******************************************************************************
Container::Container(QSizeF margins, QGraphicsItem* parent)
: Node(parent)
, nested_zone(new Rect(this))
, margins(std::move(margins))
{
	nested_zone->setFlag(QGraphicsItem::ItemContainsChildrenInShape);
	nested_zone->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren);
	nested_zone->setFiltersChildEvents(false);
	layout()->addItem(nested_zone);
}

//******************************************************************************
Container::Container(QString title, QSizeF margins, QGraphicsItem* parent)
: Node(std::move(title), parent)
, nested_zone(new Rect(this))
, margins(std::move(margins))
{
	nested_zone->setFlag(QGraphicsItem::ItemContainsChildrenInShape);
	nested_zone->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren);
	nested_zone->setFiltersChildEvents(false);
	layout()->addItem(nested_zone);
}

//******************************************************************************
Container::~Container() = default;

//******************************************************************************
void Container::expand() {
	nested_zone->show();
}

//******************************************************************************
void Container::collapse() {
	nested_zone->hide();
}

//******************************************************************************
void Container::fit() {
	if(!nested_zone->childItems().isEmpty()) {
		prepareGeometryChange();

// TODO if active only + add columns & type/column associations

		qreal x = 0;
		qreal y = margins.height();
		for(QGraphicsItem* item : nested_zone->childItems()) {
			x = qMax(x, item->boundingRect().width());
			y += item->boundingRect().height() + margins.height();
		}
		x += margins.width() * 2;
		nested_zone->setMinimumSize(x, y); // <-----------------
		nested_zone->setRect(QRectF(0, 0, x, y));
		layout()->updateGeometry();
		updateGeometry();

		y = margins.height();
		for(QGraphicsItem* item : nested_zone->childItems()) {
//			item->moveBy(margins.width(), y);
			item->setPos(margins.width(), y);

			y += item->boundingRect().height() + margins.height();
		}
	}
}

//******************************************************************************
void Container::add(QGraphicsItem* item) {
	item->setParentItem(nested_zone);
}

//******************************************************************************
void Container::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	Node::Params const& params = locate_node_params();

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

	QPainterPath path;
	QPainterPath nested;
	path.addRoundedRect(boundingRect(), params.radius, params.radius);
	nested.addRect(nested_zone->mapToParent(nested_zone->boundingRect()).boundingRect());
	painter->setPen(Qt::NoPen);
	painter->drawPath(path - nested);

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

#ifdef OEMSH_NODEGRAPH_DEBUG
	painter->setBrush(Qt::NoBrush);
	painter->setPen(QPen(Qt::red));
	painter->drawRect(boundingRect());
	painter->drawRect(nested_zone->mapToParent(nested_zone->childrenBoundingRect()).boundingRect());
#endif // OEMSH_NODEGRAPH_DEBUG
}

} // namespace qt::ui::nodegraph
