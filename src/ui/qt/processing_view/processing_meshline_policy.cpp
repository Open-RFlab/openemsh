///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include "domain/geometrics/edge.hpp"
#include "domain/conflicts/conflict_colinear_edges.hpp"
#include "domain/conflicts/conflict_too_close_meshline_policies.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "ui/qt/data_keys.hpp"
#include "ui/qt/utils/nodegraph/text.hpp"
#include "utils/down_up_cast.hpp"
#include "infra/utils/to_string.hpp"

#include "processing_meshline_policy.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingMeshlinePolicy::ProcessingMeshlinePolicy(domain::MeshlinePolicy const* meshline_policy, QGraphicsItem* parent)
: nodegraph::Node("MeshlinePolicy", parent)
, locate_processing_meshline_policy_params(default_locator<Params>)
, meshline_policy(meshline_policy)
{
	QList<QVariant> to_wire;

	locate_node_params = [&]() -> auto& {
		return locate_processing_meshline_policy_params().node;
	};

	title->locate_text_params = [&]() -> auto& {
		return locate_processing_meshline_policy_params().title;
	};

	QGraphicsLinearLayout* h_box = new QGraphicsLinearLayout(Qt::Horizontal, layout());
	QGraphicsLinearLayout* v_box1 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	QGraphicsLinearLayout* v_box2 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	QGraphicsLinearLayout* v_box3 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	layout()->addItem(h_box);
	h_box->addItem(v_box1);
	h_box->addItem(v_box2);
	h_box->addItem(v_box3);
	v_box1->addStretch();

	for(auto* origin : meshline_policy->origins) {
		nodegraph::Port* port = add_input_port();
		port->setFlag(QGraphicsItem::ItemIsSelectable);
		port->setAcceptedMouseButtons(Qt::NoButton);
		port->locate_port_params = [&]() -> auto& {
			return locate_processing_meshline_policy_params().port;
		};
		v_box1->addItem(port);
		// Downcast from IMeshLineOrigin is mandatory to upcast to Entity.
		if(auto* entity = down_up_cast<Entity,
			domain::Edge,
			domain::ConflictColinearEdges,
			domain::ConflictTooCloseMeshlinePolicies>(origin)
		; entity)
			to_wire.emplace_back(std::in_place_type<DataKeys::ToWire>, entity, port);
	}

	v_box1->addStretch();

	nodegraph::Port* output_port = add_output_port();
	output_port->setFlag(QGraphicsItem::ItemIsSelectable);
	output_port->setAcceptedMouseButtons(Qt::NoButton);
	output_port->locate_port_params = [&]() -> auto& {
		return locate_processing_meshline_policy_params().port;
	};

	QString normal("Normal: ");
	QString is_enabled("Is enabled: ");
	QString policy("Policy: ");
	QString d("d: ");
	if(meshline_policy) {
		normal += to_string(meshline_policy->normal);
		is_enabled += (meshline_policy->is_enabled ? "true" : "false");
		policy += to_string(meshline_policy->policy);
		d += QString::number(meshline_policy->d);
	}

	nodegraph::Text* text_normal = new nodegraph::Text(normal, this);
	text_normal->setFlag(QGraphicsItem::ItemIsSelectable);
	text_normal->setAcceptedMouseButtons(Qt::NoButton);
	text_normal->locate_text_params = [&]() -> auto& {
		return locate_processing_meshline_policy_params().main;
	};

	nodegraph::Text* text_is_enabled = new nodegraph::Text(is_enabled, this);
	text_is_enabled->setFlag(QGraphicsItem::ItemIsSelectable);
	text_is_enabled->setAcceptedMouseButtons(Qt::NoButton);
	if(meshline_policy) {
		if(meshline_policy->is_enabled)
			text_is_enabled->locate_text_params = [&]() -> auto& {
				return locate_processing_meshline_policy_params().enabled;
			};
		else
			text_is_enabled->locate_text_params = [&]() -> auto& {
				return locate_processing_meshline_policy_params().disabled;
			};
	} else {
		text_is_enabled->locate_text_params = [&]() -> auto& {
			return locate_processing_meshline_policy_params().main;
		};
	}

	nodegraph::Text* text_policy = new nodegraph::Text(policy, this);
	text_policy->setFlag(QGraphicsItem::ItemIsSelectable);
	text_policy->setAcceptedMouseButtons(Qt::NoButton);
	text_policy->locate_text_params = [&]() -> auto& {
		return locate_processing_meshline_policy_params().main;
	};

	nodegraph::Text* text_d = new nodegraph::Text(d, this);
	text_d->setFlag(QGraphicsItem::ItemIsSelectable);
	text_d->setAcceptedMouseButtons(Qt::NoButton);
	text_d->locate_text_params = [&]() -> auto& {
		return locate_processing_meshline_policy_params().main;
	};

	v_box2->addItem(text_normal);
	v_box2->addItem(text_is_enabled);
	v_box2->addItem(text_policy);
	v_box2->addItem(text_d);
	v_box3->addStretch();
	v_box3->addItem(output_port);
	v_box3->addStretch();
	v_box3->setAlignment(output_port, Qt::AlignRight | Qt::AlignVCenter);

	setData(DataKeys::TYPE, "MeshlinePolicy");
	setData(DataKeys::ID, (qulonglong) meshline_policy->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(meshline_policy));
	setData(DataKeys::TO_WIRE, std::move(to_wire));
	retrieve_highlightable_children();
}

//******************************************************************************
ProcessingMeshlinePolicy::~ProcessingMeshlinePolicy() = default;

//******************************************************************************
int ProcessingMeshlinePolicy::type() const {
	return Type;
}

//******************************************************************************
bool ProcessingMeshlinePolicy::has_conflict_tcmlp_origin() const {
	bool has_conflict_tcmlp = false;

	if(meshline_policy) {
		for(auto const* origin : meshline_policy->origins) {
			if(dynamic_cast<domain::ConflictTooCloseMeshlinePolicies const*>(origin)) {
				has_conflict_tcmlp = true;
				break;
			}
		}
	}

	return has_conflict_tcmlp;
}

} // namespace ui::qt
