///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPen>

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
	nested_zone->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren);
	nested_zone->setOpacity(0.5);
	nested_zone->setPen(QPen(QColor(57, 57, 57)));
	nested_zone->setBrush(QColor(57, 57, 57));
	layout()->addItem(nested_zone);
}

//******************************************************************************
Container::Container(QString title, QSizeF margins, QGraphicsItem* parent)
: Node(std::move(title), parent)
, nested_zone(new Rect(this))
, margins(std::move(margins))
{
	nested_zone->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren);
	nested_zone->setOpacity(0.5);
	nested_zone->setPen(QPen(QColor(57, 57, 57)));
	nested_zone->setBrush(QColor(57, 57, 57));
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
void Container::paint(QPainter* painter, QStyleOptionGraphicsItem const* /*option*/, QWidget* /*widget*/) {
//	Node::paint(painter, option, widget);

	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor(27, 27, 27));
//	painter->setBrush(Qt::white);

	// TODO radius = title height / 2
//	painter->drawRoundedRect(boundingRect(), 10, 10);
	QPainterPath path;
	QPainterPath nested;
	path.addRoundedRect(boundingRect(), 10, 10);
	nested.addRect(nested_zone->mapToParent(nested_zone->boundingRect()).boundingRect());
	painter->drawPath(path - nested);


	if(title) {
		painter->setBrush(QColor(33, 33, 33));
		QRectF titlebar = boundingRect();
		titlebar.setBottom(title->mapToParent(title->boundingRect()).boundingRect().bottom());
//		painter->drawRoundedRect(title->boundingRect(), 10, 10);
		painter->drawRoundedRect(titlebar, 10, 10);
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
