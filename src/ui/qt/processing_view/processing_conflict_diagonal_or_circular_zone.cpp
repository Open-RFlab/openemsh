///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QGraphicsLinearLayout>

#include "domain/geometrics/angle.hpp"
#include "domain/conflicts/conflict_diagonal_or_circular_zone.hpp"
#include "ui/qt/data_keys.hpp"

#include "processing_conflict_diagonal_or_circular_zone.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingConflictDiagonalOrCircularZone::ProcessingConflictDiagonalOrCircularZone(domain::ConflictDiagonalOrCircularZone const* conflict, QGraphicsItem* parent)
: nodegraph::Node("ConflictDiagonalOrCircularZone - " + (conflict ? QString::number(conflict->id) : QString()), parent)
, locate_processing_conflict_docz_params(default_locator<Params>)
, conflict(conflict)
{
	QList<QVariant> to_wire;

	locate_node_params = [this]() -> auto& {
		return locate_processing_conflict_docz_params().node;
	};

	title->locate_text_params = [this]() -> auto& {
		return locate_processing_conflict_docz_params().title;
	};

	auto* h_box = new QGraphicsLinearLayout(Qt::Horizontal, layout());
	auto* v_box1 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	auto* v_box2 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	auto* v_box3 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	layout()->addItem(h_box);
	h_box->addItem(v_box1);
	h_box->addItem(v_box2);
	h_box->addItem(v_box3);

	nodegraph::Port* in = add_input_port(" ");
	in->setFlag(QGraphicsItem::ItemIsSelectable);
	in->setAcceptedMouseButtons(Qt::NoButton);
	in->locate_port_params = [this]() -> auto& {
		return locate_processing_conflict_docz_params().port;
	};
	v_box1->addItem(in);

	nodegraph::Port* out = add_output_port();
	out->setFlag(QGraphicsItem::ItemIsSelectable);
	out->setAcceptedMouseButtons(Qt::NoButton);
	out->locate_port_params = [this]() -> auto& {
		return locate_processing_conflict_docz_params().port;
	};

	QString dmax("dmax: ");
	QString lmin("lmin: ");
	QString minimal_angle ("Minimal angle (°): ");
	if(conflict) {
		auto const& state = conflict->get_current_state();
		dmax += QString::number(state.dmax);
		lmin += QString::number(state.lmin);
		minimal_angle += QString::number(state.minimal_angle);
		for(auto const* angle : state.angles)
			to_wire.emplace_back(DataKeys::set_to_wire(angle, in));
	}

	auto* text_dmax = new nodegraph::Text(dmax, this);
	text_dmax->setFlag(QGraphicsItem::ItemIsSelectable);
	text_dmax->setAcceptedMouseButtons(Qt::NoButton);
	text_dmax->locate_text_params = [this]() -> auto& {
		return locate_processing_conflict_docz_params().main;
	};

	auto* text_lmin = new nodegraph::Text(lmin, this);
	text_lmin->setFlag(QGraphicsItem::ItemIsSelectable);
	text_lmin->setAcceptedMouseButtons(Qt::NoButton);
	text_lmin->locate_text_params = [this]() -> auto& {
		return locate_processing_conflict_docz_params().main;
	};

	auto* text_minimal_angle = new nodegraph::Text(minimal_angle, this);
	text_minimal_angle->setFlag(QGraphicsItem::ItemIsSelectable);
	text_minimal_angle->setAcceptedMouseButtons(Qt::NoButton);
	text_minimal_angle->locate_text_params = [this]() -> auto& {
		return locate_processing_conflict_docz_params().main;
	};

	v_box2->addItem(text_dmax);
	v_box2->addItem(text_lmin);
	v_box2->addItem(text_minimal_angle);
	v_box3->addStretch();
	v_box3->addItem(out);
	v_box3->addStretch();
	v_box3->setAlignment(out, Qt::AlignRight | Qt::AlignVCenter);

	setData(DataKeys::TYPE, "ConflictDiagonalOrCircularZone");
	setData(DataKeys::ID, (qulonglong) conflict->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(conflict));
	setData(DataKeys::TO_WIRE, to_wire);
	retrieve_highlightable_children();
}

//******************************************************************************
int ProcessingConflictDiagonalOrCircularZone::type() const {
	return Type;
}

} // namespace ui::qt
