///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QGraphicsLinearLayout>

#include "domain/conflicts/conflict_colinear_edges.hpp"
#include "infra/utils/to_string.hpp"

#include "processing_conflict_colinear_edges.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingConflictColinearEdges::ProcessingConflictColinearEdges(domain::ConflictColinearEdges const* conflict, QGraphicsItem* parent)
: nodegraph::Node("ConflictColinearEdges", parent)
, conflict(conflict)
{
	QGraphicsLinearLayout* h_box = new QGraphicsLinearLayout(Qt::Horizontal, layout());
	QGraphicsLinearLayout* v_box1 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	QGraphicsLinearLayout* v_box2 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	layout()->addItem(h_box);
	h_box->addItem(v_box1);
	h_box->addItem(v_box2);

	for(domain::Edge* edge : conflict->edges) {
		nodegraph::Port* port = add_input_port("Normal: " + QString::fromStdString(to_string(edge->normal)) + (edge->to_mesh ? " enabled" : " disabled"));
		v_box1->addItem(port);
		port_index[edge] = port;
	}

	nodegraph::Port* out = add_output_port();
	v_box2->addStretch();
	v_box2->addItem(out);
	v_box2->addStretch();
	v_box2->setAlignment(out, Qt::AlignRight | Qt::AlignVCenter);
}

//******************************************************************************
ProcessingConflictColinearEdges::~ProcessingConflictColinearEdges() = default;

//******************************************************************************
int ProcessingConflictColinearEdges::type() const {
	return Type;
}

//******************************************************************************
void ProcessingConflictColinearEdges::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget) {
	nodegraph::Node::paint(painter, option, widget);

#ifdef OEMSH_NODEGRAPH_DEBUG
	painter->setBrush(Qt::NoBrush);
	painter->setPen(QPen(Qt::red));
	painter->drawRect(boundingRect());
#endif // OEMSH_NODEGRAPH_DEBUG
//	painter->drawRect(inner_space->mapToParent(inner_space->childrenBoundingRect()).boundingRect());
}

} // namespace ui::qt
