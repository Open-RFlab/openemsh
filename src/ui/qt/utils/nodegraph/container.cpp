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

#include "utils/unreachable.hpp"
#include "rect.hpp"
#include "text.hpp"

#include "container.hpp"

namespace ui::qt::nodegraph {

//******************************************************************************
Container::Container(QSizeF margins, QGraphicsItem* parent)
: Node(parent)
, spacement(Spacement::UNPACKED_REGULAR)
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
, spacement(Spacement::UNPACKED_REGULAR)
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
	if(isVisible()) {
		prepareGeometryChange();

		struct Column {
			qreal w = 0;
			qreal h = 0;
			qreal x = 0;
			qreal n_items = 0;
			qreal shift_y = 0;
			qreal margin_y = 0;
		};

		auto const children = visible_contained_items();

		std::map<std::size_t, Column> columns;
		std::map<QGraphicsItem const*, qreal> item_y;

		// Calculate columns sizes.
		for(QGraphicsItem const* item : children) {
			std::size_t const i = get_column(item);
			if(!columns.contains(i))
				columns[i].h = margins.height();

			auto& col = columns[i];
			item_y[item] = col.h;
			col.h += item->boundingRect().height() + margins.height();
			col.w = qMax(col.w, item->boundingRect().width());
			++col.n_items;
		}

		// Calculate column positions.
		if(!columns.empty()) {
			std::begin(columns)->second.x = margins.width();
			if(columns.size() > 1) {
				for(auto it = std::next(std::begin(columns)); it != std::end(columns); ++it) {
					auto& current_col = it->second;
					auto& prev_col = prev(it)->second;
					current_col.x = prev_col.x + prev_col.w + margins.width();
				}
			}
		}

		// Calculate container size.
		qreal w = columns.empty()
		        ? 0
		        : std::rbegin(columns)->second.x + std::rbegin(columns)->second.w + margins.width();
		qreal h = 0;
		for(auto& it : columns)
			h = qMax(h, it.second.h);

		// Update the Container size.
		nested_zone->setMinimumSize(w, h); // <-----------------
		nested_zone->setRect(QRectF(0, 0, w, h));
		layout()->updateGeometry();
		updateGeometry();

		// Take care of spacement policy and place items.
		switch(spacement) {
		case Spacement::PACKED_UP: {
			for(QGraphicsItem* item : children) {
				item->setPos(columns[get_column(item)].x, item_y[item]);
			}
		} break;
		case Spacement::PACKED_DOWN: {
			for(auto& [i, col] : columns) {
				col.shift_y = h - col.h;
			}
			for(QGraphicsItem* item : children) {
				std::size_t const col = get_column(item);
				item->setPos(columns[col].x, item_y[item] + columns[col].shift_y);
			}
		} break;
		case Spacement::UNPACKED_MAX: {
			for(auto& [i, col] : columns) {
				qreal const items_summed_size = col.h - (col.n_items + 1) * margins.height();
				col.margin_y = (h - items_summed_size - 2 * margins.height()) / (col.n_items - 1);
				col.h = margins.height(); // Reset.
			}
			for(QGraphicsItem* item : children) {
				std::size_t const col = get_column(item);
				item->setPos(columns[col].x, columns[col].h);
				columns[col].h += item->boundingRect().height() + columns[col].margin_y;
			}
		} break;
		case Spacement::UNPACKED_REGULAR: {
			for(auto& [i, col] : columns) {
				qreal const items_summed_size = col.h - (col.n_items + 1) * margins.height();
				col.margin_y = (h - items_summed_size) / (col.n_items + 1);
				col.h = col.margin_y; // Reset.
			}
			for(QGraphicsItem* item : children) {
				std::size_t const col = get_column(item);
				item->setPos(columns[col].x, columns[col].h);
				columns[col].h += item->boundingRect().height() + columns[col].margin_y;
			}
		} break;
		default:
			unreachable();
		}
	}
}

//******************************************************************************
void Container::add(QGraphicsItem* item) {
	item->setParentItem(nested_zone);
}

//******************************************************************************
bool Container::does_contains_visible_items() const {
	return std::ranges::any_of(nested_zone->childItems(), [](QGraphicsItem const* item) {
		return item && item->isVisible();
	});
}

//******************************************************************************
QList<QGraphicsItem*> Container::visible_contained_items() const {
	auto children = nested_zone->childItems();
	children.removeIf([](QGraphicsItem const* item) {
		return item == nullptr || !item->isVisible();
	});
	return children;
}

//******************************************************************************
void Container::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	Node::Params const& params = locate_node_params();

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

	QPainterPath path;
	path.addRoundedRect(boundingRect(), params.radius, params.radius);
	if(does_contains_visible_items()) {
		QPainterPath nested;
		nested.addRect(nested_zone->mapToParent(nested_zone->boundingRect()).boundingRect());
		path -= nested;
	}
	painter->setPen(Qt::NoPen);
	painter->drawPath(path);

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

#ifdef OEMSH_NODEGRAPH_DEBUG
	painter->setBrush(Qt::NoBrush);
	painter->setPen(QPen(Qt::red));
	painter->drawRect(boundingRect());
	painter->drawRect(nested_zone->mapToParent(nested_zone->childrenBoundingRect()).boundingRect());
#endif // OEMSH_NODEGRAPH_DEBUG
}

} // namespace qt::ui::nodegraph
