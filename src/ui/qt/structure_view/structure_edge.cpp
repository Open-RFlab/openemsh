///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "domain/geometrics/edge.hpp"
#include "ui/qt/data_keys.hpp"
#include "utils/default_locator.hpp"

#include "structure_edge.hpp"

namespace ui::qt {

//******************************************************************************
StructureEdge::StructureEdge(domain::Edge const* edge, QGraphicsItem* parent)
: QGraphicsLineItem(edge->p0().x.value(), edge->p0().y.value(), edge->p1().x.value(), edge->p1().y.value(), parent)
, locate_structure_edge_params(default_locator<Params>)
, edge(edge)
{
	setFlags(ItemIsSelectable);
	setAcceptHoverEvents(true);
	setAcceptTouchEvents(true);
	setData(DataKeys::TYPE, "Edge");
	setData(DataKeys::ID, (qulonglong) edge->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(edge));
//	setData(DataKeys::NAME, polygon->name); // TODO What should be edge name? <polygon>-<edge-n> ?
}

//******************************************************************************
int StructureEdge::type() const {
	return Type;
}

//******************************************************************************
void StructureEdge::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	Params const& params = locate_structure_edge_params();

	if(option->state & QStyle::State_Selected) {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.selected_hovered);
		else
			painter->setPen(params.selected);
	} else {
		if(option->state & QStyle::State_MouseOver)
			painter->setPen(params.regular_hovered);
		else
			painter->setPen(params.regular);
	}

	painter->drawLine(line());
}

} // namespace ui::qt
