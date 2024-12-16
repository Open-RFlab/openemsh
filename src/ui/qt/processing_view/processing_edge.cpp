///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include "domain/geometrics/edge.hpp"
#include "ui/qt/data_keys.hpp"
#include "ui/qt/utils/nodegraph/text.hpp"
#include "infra/utils/to_string.hpp"

#include "processing_edge.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingEdge::ProcessingEdge(domain::Edge const* edge, QGraphicsItem* parent)
: nodegraph::Node("Edge", parent)
, edge(edge)
{
	setData(DataKeys::TYPE, "Edge");
	setData(DataKeys::ID, (qulonglong) edge->id);
	setData(DataKeys::ENTITY, QVariant::fromValue(static_cast<void const*>(edge)));

	QGraphicsLinearLayout* h_box = new QGraphicsLinearLayout(Qt::Horizontal, layout());
	QGraphicsLinearLayout* v_box1 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	QGraphicsLinearLayout* v_box2 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	QGraphicsLinearLayout* v_box3 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	layout()->addItem(h_box);
	v_box1->addStretch();
	v_box1->addItem(add_input_port());
	v_box1->addStretch();
	h_box->addItem(v_box1);
	h_box->addItem(v_box2);
	h_box->addItem(v_box3);
	nodegraph::Port* output_port = add_output_port();
	v_box3->addStretch();
	v_box3->addItem(output_port);
	v_box3->addStretch();
	v_box3->setAlignment(output_port, Qt::AlignRight | Qt::AlignVCenter);

	QString normal("Normal: ");
	QString to_mesh("To mesh: ");
	if(edge) {
		normal += to_string(edge->normal);
		to_mesh += (edge->to_mesh ? "true" : "false");
	}

	{
		nodegraph::Text* text = new nodegraph::Text(normal, this);
		text->setBrush(Qt::white);
		v_box2->addItem(text);
	}
	{
		nodegraph::Text* text = new nodegraph::Text(to_mesh, this);
		if(edge) {
			if(edge->to_mesh)
				text->setBrush(Qt::darkGreen);
			else
				text->setBrush(Qt::darkRed);
		} else {
			text->setBrush(Qt::white);
		}
		v_box2->addItem(text);
	}
}

//******************************************************************************
ProcessingEdge::~ProcessingEdge() = default;

//******************************************************************************
int ProcessingEdge::type() const {
	return Type;
}

//******************************************************************************
void ProcessingEdge::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget) {
	nodegraph::Node::paint(painter, option, widget);

#ifdef OEMSH_NODEGRAPH_DEBUG
	painter->setBrush(Qt::NoBrush);
	painter->setPen(QPen(Qt::red));
	painter->drawRect(boundingRect());
#endif // OEMSH_NODEGRAPH_DEBUG
}

} // namespace ui::qt
