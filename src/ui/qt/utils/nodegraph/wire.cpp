///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include <cmath>

#include "utils/default_locator.hpp"
#include "utils/unreachable.hpp"
#include "port.hpp"

#include "wire.hpp"

namespace ui::qt::nodegraph {

//******************************************************************************
Wire::Wire(Port* begin, Port* end, QGraphicsItem* parent)
: QGraphicsPathItem(parent)
, style(Style::CURVED)
, locate_wire_params(default_locator<Params>)
, begin(begin)
, end(end)
{
	setAcceptHoverEvents(true);
//	setZValue(-1);

	if(begin) {
		begin->wires.append(this);
	}
	if(end) {
		end->wires.append(this);
	}
	update_path();
}

//******************************************************************************
Wire::~Wire() = default;

// https://github.com/jchanvfx/NodeGraphQt/blob/a8fa9b394d35bf4fcad3c241ec1bd7a98b40bb1c/NodeGraphQt/qgraphics/pipe.py#L205-L366
// https://github.com/nwaniek/qt5-node-editor/blob/master/src/graphicsbezieredge.cpp#L126-L159
// https://github.com/paceholder/nodeeditor/blob/master/src/ConnectionGraphicsObject.cpp#L318-L378
//******************************************************************************
void Wire::update_path() {
	if(isVisible() && begin && end) {
		prepareGeometryChange();
		QPointF p1 = mapFromScene(begin->mapToScene(begin->attach_pos()));
		QPointF p2 = mapFromScene(end->mapToScene(end->attach_pos()));
		QPainterPath path(p1);

		switch(style) {
		case Style::CURVED: {
			QPointF c1 = p1;
			QPointF c2 = p2;

			//check that using anchor points as nothing ensure which of p1/p2 is in/out.
//			qreal fx = (p1.x() > p2.x()) ? 1 : 0.5;
//			qreal fy = (p1.y() > p2.y()) ? 1 : 0.5;
			qreal constexpr dmin = 50;
//			qreal d = qSqrt(QPointF::dotProduct(p1, p2));
			qreal const dx = std::max(dmin, std::abs(p1.x() - p2.x()) * /*fx*/ 0.5);
			qreal const dy = std::max(dmin, std::abs(p1.y() - p2.y()) * /*fy*/ 0.5);

			switch(begin->anchor) {
			case Port::AnchorPoint::TOP: c1.ry() -= dy; break;
			case Port::AnchorPoint::BOTTOM: c1.ry() += dy; break;
			case Port::AnchorPoint::LEFT: c1.rx() -= dx; break;
			case Port::AnchorPoint::RIGHT: c1.rx() += dx; break;
			default: break;
			}
			switch(end->anchor) {
			case Port::AnchorPoint::TOP: c2.ry() -= dy; break;
			case Port::AnchorPoint::BOTTOM: c2.ry() += dy; break;
			case Port::AnchorPoint::LEFT: c2.rx() -= dx; break;
			case Port::AnchorPoint::RIGHT: c2.rx() += dx; break;
			default: break;
			}

			path.cubicTo(c1, c2, p2);

#ifdef OEMSH_NODEGRAPH_DEBUG
			path.addEllipse(c1, 2, 2);
			path.addEllipse(c2, 3, 3);
#endif // OEMSH_NODEGRAPH_DEBUG
		} break;
		case Style::DIRECT: {
			path.lineTo(p2);
		} break;
		default: unreachable();
		}

		setPath(path);
	}
}

/// Ownership is transfered to the caller.
///*****************************************************************************
Wire* Wire::unwire(Port const* called_from_dtor_of) {
	if(scene())
		scene()->removeItem(this);
	if(begin && called_from_dtor_of != begin)
		begin->wires.removeAll(this);
	if(end && called_from_dtor_of != end)
		end->wires.removeAll(this);
	return this;
}

//******************************************************************************
Port* Wire::traverse(Port const* port) const {
	if(port == begin)
		return end;
	else if(port == end)
		return begin;
	else
		return nullptr;
}

//******************************************************************************
QVariant Wire::itemChange(GraphicsItemChange change, QVariant const& value) {
	if(change == ItemVisibleHasChanged) {
		if(value.toBool())
			update_path();
	}
	return QGraphicsItem::itemChange(change, value);
}

//******************************************************************************
void Wire::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	if(begin && begin->isVisible() && end && end->isVisible()) {
		Params const& params = locate_wire_params();

		if(option->state & QStyle::State_Selected) {
			if(option->state & QStyle::State_MouseOver)
				painter->setPen(params.selected_hovered);
			else
				painter->setPen(params.selected);
		} else if((begin->is_highlighted() || begin->isSelected()) && (end->is_highlighted() || end->isSelected())) {
			if(option->state & QStyle::State_MouseOver)
				painter->setPen(params.highlighted_hovered);
			else
				painter->setPen(params.highlighted);
		} else {
			if(option->state & QStyle::State_MouseOver)
				painter->setPen(params.regular_hovered);
			else
				painter->setPen(params.regular);
		}

		painter->drawPath(path());
	}
}

} // namespace ui::qt::nodegraph
