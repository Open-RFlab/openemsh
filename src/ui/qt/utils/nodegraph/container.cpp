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

#include <map>

#include "rect.hpp"
#include "text.hpp"

#include "container.hpp"

namespace ui::qt::nodegraph {

//******************************************************************************
Container::Container(QSizeF margins, QGraphicsItem* parent)
: Node(parent)
, get_column([](QGraphicsItem const*) { return 0; })
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
, get_column([](QGraphicsItem const*) { return 0; })
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

		// TODO if active only

		std::map<std::size_t, qreal> column_w; // without margins
		std::map<std::size_t, qreal> column_h; // with margins
		std::map<QGraphicsItem const*, qreal> item_y;

		// Calculate columns sizes.
		for(QGraphicsItem* item : nested_zone->childItems()) {
			std::size_t const col = get_column(item);
			if(!column_w.contains(col))
				column_w[col] = 0;
			if(!column_h.contains(col))
				column_h[col] = margins.height();

			item_y[item] = column_h[col];
			column_h[col] += item->boundingRect().height() + margins.height();
			column_w[col] = qMax(column_w[col], item->boundingRect().width());
		}

		// Calculate column positions.
		std::map<std::size_t, qreal> column_x = column_w; // Copy for the key part.
		std::begin(column_x)->second = margins.width();
		for(auto it = std::next(std::begin(column_x)); it != std::end(column_x); ++it) {
			it->second = prev(it)->second + column_w[prev(it)->first] + margins.width();
		}

		// Calculate container size.
		qreal w = std::rbegin(column_x)->second + std::rbegin(column_w)->second + margins.width();
		qreal h = 0;
		for(auto& it : column_h)
			h = qMax(h, it.second);

		// Update the Container size.
		nested_zone->setMinimumSize(w, h); // <-----------------
		nested_zone->setRect(QRectF(0, 0, w, h));
		layout()->updateGeometry();
		updateGeometry();

		// Place items for real.
		for(QGraphicsItem* item : nested_zone->childItems()) {
			item->setPos(column_x[get_column(item)], item_y[item]);
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
