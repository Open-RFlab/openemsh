///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include "domain/mesh/interval.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "ui/qt/data_keys.hpp"
#include "ui/qt/utils/nodegraph/text.hpp"

#include "processing_interval.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingInterval::ProcessingInterval(domain::Interval const* interval, QGraphicsItem* parent)
: nodegraph::Node("Interval", parent)
, locate_processing_interval_params(default_locator<Params>)
, interval(interval)
{
	QList<QVariant> to_wire;

	locate_node_params = [this]() -> auto& {
		return locate_processing_interval_params().node;
	};

	title->locate_text_params = [this]() -> auto& {
		return locate_processing_interval_params().title;
	};

	auto* h_box = new QGraphicsLinearLayout(Qt::Horizontal, layout());
	auto* v_box1 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	auto* v_box2 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	auto* v_box3 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	layout()->addItem(h_box);
	h_box->addItem(v_box1);
	h_box->addItem(v_box2);
	h_box->addItem(v_box3);
	v_box1->addStretch();

	nodegraph::Port* before_port = add_input_port(" ");
	before_port->setFlag(QGraphicsItem::ItemIsSelectable);
	before_port->setAcceptedMouseButtons(Qt::NoButton);
	before_port->locate_port_params = [this]() -> auto& {
		return locate_processing_interval_params().port;
	};
	v_box1->addItem(before_port);
	v_box1->addStretch();

	nodegraph::Port* after_port = add_input_port(" ");
	after_port->setFlag(QGraphicsItem::ItemIsSelectable);
	after_port->setAcceptedMouseButtons(Qt::NoButton);
	after_port->locate_port_params = [this]() -> auto& {
		return locate_processing_interval_params().port;
	};
	v_box1->addItem(after_port);
	v_box1->addStretch();

	nodegraph::Port* output_port = add_output_port();
	output_port->setFlag(QGraphicsItem::ItemIsSelectable);
	output_port->setAcceptedMouseButtons(Qt::NoButton);
	output_port->locate_port_params = [this]() -> auto& {
		return locate_processing_interval_params().port;
	};

	QString dmax("dmax: ");
	QString before_lmin ("Before.lmin: ");
	QString before_lambda ("Before.Smoothness: ");
	QString after_lmin("After.lmin: ");
	QString after_lambda("After.Smoothness: ");
	if(interval) {
		auto const& state = interval->get_current_state();
		dmax += QString::number(state.dmax);
		before_lmin += QString::number(state.before.lmin);
		before_lambda += QString::number(state.before.lambda);
		after_lmin += QString::number(state.after.lmin);
		after_lambda += QString::number(state.after.lambda);
		if(state.before.meshline_policy)
			to_wire.emplace_back(DataKeys::set_to_wire(state.before.meshline_policy, before_port));
		if(state.after.meshline_policy)
			to_wire.emplace_back(DataKeys::set_to_wire(state.after.meshline_policy, after_port));
	}

	auto* text_dmax = new nodegraph::Text(dmax, this);
	text_dmax->setFlag(QGraphicsItem::ItemIsSelectable);
	text_dmax->setAcceptedMouseButtons(Qt::NoButton);
	text_dmax->locate_text_params = [this]() -> auto& {
		return locate_processing_interval_params().main;
	};

	auto* text_before_lmin = new nodegraph::Text(before_lmin, this);
	text_before_lmin->setFlag(QGraphicsItem::ItemIsSelectable);
	text_before_lmin->setAcceptedMouseButtons(Qt::NoButton);
	text_before_lmin->locate_text_params = [this]() -> auto& {
		return locate_processing_interval_params().main;
	};

	auto* text_before_lambda = new nodegraph::Text(before_lambda, this);
	text_before_lambda->setFlag(QGraphicsItem::ItemIsSelectable);
	text_before_lambda->setAcceptedMouseButtons(Qt::NoButton);
	text_before_lambda->locate_text_params = [this]() -> auto& {
		return locate_processing_interval_params().main;
	};

	auto* text_after_lmin = new nodegraph::Text(after_lmin, this);
	text_after_lmin->setFlag(QGraphicsItem::ItemIsSelectable);
	text_after_lmin->setAcceptedMouseButtons(Qt::NoButton);
	text_after_lmin->locate_text_params = [this]() -> auto& {
		return locate_processing_interval_params().main;
	};

	auto* text_after_lambda = new nodegraph::Text(after_lambda, this);
	text_after_lambda->setFlag(QGraphicsItem::ItemIsSelectable);
	text_after_lambda->setAcceptedMouseButtons(Qt::NoButton);
	text_after_lambda->locate_text_params = [this]() -> auto& {
		return locate_processing_interval_params().main;
	};

	v_box2->addItem(text_dmax);
	v_box2->addItem(text_before_lmin);
	v_box2->addItem(text_before_lambda);
	v_box2->addItem(text_after_lmin);
	v_box2->addItem(text_after_lambda);
	v_box3->addStretch();
	v_box3->addItem(output_port);
	v_box3->addStretch();
	v_box3->setAlignment(output_port, Qt::AlignRight | Qt::AlignVCenter);

	setData(DataKeys::TYPE, "Interval");
	setData(DataKeys::ID, (qulonglong) interval->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(interval));
	setData(DataKeys::TO_WIRE, to_wire);
	retrieve_highlightable_children();
}

//******************************************************************************
int ProcessingInterval::type() const {
	return Type;
}

} // namespace ui::qt
