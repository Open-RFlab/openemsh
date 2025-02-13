///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QGraphicsLinearLayout>

#include "domain/conflicts/conflict_colinear_edges.hpp"
#include "infra/utils/to_string.hpp"
#include "ui/qt/data_keys.hpp"

#include "processing_conflict_colinear_edges.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingConflictColinearEdges::ProcessingConflictColinearEdges(domain::ConflictColinearEdges const* conflict, QGraphicsItem* parent)
: nodegraph::Node("ConflictColinearEdges", parent)
, locate_processing_conflict_ce_params(default_locator<Params>)
, conflict(conflict)
{
	QList<QVariant> to_wire;

	locate_node_params = [&]() -> auto& {
		return locate_processing_conflict_ce_params().node;
	};

	title->locate_text_params = [&]() -> auto& {
		return locate_processing_conflict_ce_params().title;
	};

	QGraphicsLinearLayout* h_box = new QGraphicsLinearLayout(Qt::Horizontal, layout());
	QGraphicsLinearLayout* v_box1 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	QGraphicsLinearLayout* v_box2 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	layout()->addItem(h_box);
	h_box->addItem(v_box1);
	h_box->addItem(v_box2);

	for(domain::Edge* edge : conflict->edges) {
		nodegraph::Port* port = add_input_port("Normal: " + QString::fromStdString(to_string(edge->normal)) + (edge->to_mesh ? " enabled" : " disabled"));
		port->setFlag(QGraphicsItem::ItemIsSelectable);
		port->setAcceptedMouseButtons(Qt::NoButton);
		if(edge->to_mesh)
			port->locate_port_params = [&]() -> auto& {
				return locate_processing_conflict_ce_params().port_enabled;
			};
		else
			port->locate_port_params = [&]() -> auto& {
				return locate_processing_conflict_ce_params().port_disabled;
			};
		v_box1->addItem(port);
		to_wire.emplace_back(DataKeys::set_to_wire(edge, port));
	}

	nodegraph::Port* out = add_output_port();
	out->setFlag(QGraphicsItem::ItemIsSelectable);
	out->setAcceptedMouseButtons(Qt::NoButton);
	out->locate_port_params = [&]() -> auto& {
		return locate_processing_conflict_ce_params().port_normal;
	};

	v_box2->addStretch();
	v_box2->addItem(out);
	v_box2->addStretch();
	v_box2->setAlignment(out, Qt::AlignRight | Qt::AlignVCenter);

	setData(DataKeys::TYPE, "ConflictColinearEdges");
	setData(DataKeys::ID, (qulonglong) conflict->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(conflict));
	setData(DataKeys::TO_WIRE, std::move(to_wire));
	retrieve_highlightable_children();
}

//******************************************************************************
ProcessingConflictColinearEdges::~ProcessingConflictColinearEdges() = default;

//******************************************************************************
int ProcessingConflictColinearEdges::type() const {
	return Type;
}

} // namespace ui::qt
