///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <algorithm>
#include <array>

#include "app/steps.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "infra/utils/to_string.hpp"
#include "utils/unreachable.hpp"

#include "edit_model_meshline_policy.hpp"

namespace ui::qt {

//******************************************************************************
EditModelMeshlinePolicy::EditModelMeshlinePolicy(domain::MeshlinePolicy* meshline_policy, QObject* parent)
: EditModel(parent)
, meshline_policy(meshline_policy)
{
	setRowCount(5);
	for(std::size_t i = 0; auto& str : {
		"policy",
		"normal",
		"is_enabled",
		"res_factor",
		"d"
	}) {
		setItem(i++, 0, make_property_item(str));
	}

	auto const& state = meshline_policy->get_current_state();
	setItem(0, 1, new QStandardItem(QString::fromStdString(to_string(state.policy))));
	setItem(1, 1, new QStandardItem(QString::fromStdString(to_string(state.normal))));
	setItem(2, 1, make_bool_item(state.is_enabled));
	setItem(3, 1, new QStandardItem(QString::number(state.res_factor)));
	setItem(4, 1, new QStandardItem(QString::number(state.d)));
}

//******************************************************************************
void EditModelMeshlinePolicy::commit() {
	auto state = meshline_policy->get_current_state();

	auto const are_policy_and_normal_compatible = [&state]() {
		switch(state.policy) {
		case domain::MeshlinePolicy::Policy::ONELINE: [[fallthrough]];
		case domain::MeshlinePolicy::Policy::HALFS:
			switch(state.normal) {
			case domain::MeshlinePolicy::Normal::MIN: [[fallthrough]];
			case domain::MeshlinePolicy::Normal::MAX: return false;
			case domain::MeshlinePolicy::Normal::NONE: return true;
			default: unreachable();
			}
		case domain::MeshlinePolicy::Policy::THIRDS:
			switch(state.normal) {
			case domain::MeshlinePolicy::Normal::MIN: [[fallthrough]];
			case domain::MeshlinePolicy::Normal::MAX: return true;
			case domain::MeshlinePolicy::Normal::NONE: return false;
			default: unreachable();
			}
		default:unreachable();
		}
	};

	std::array does_succeed = {
		try_from_map({
				{ "ONELINE", domain::MeshlinePolicy::Policy::ONELINE },
				{ "HALFS", domain::MeshlinePolicy::Policy::HALFS },
				{ "THIRDS", domain::MeshlinePolicy::Policy::THIRDS }
			}, item(0, 1)->text(), state.policy),
		try_from_map({
				{ "MIN", domain::MeshlinePolicy::Normal::MIN },
				{ "MAX", domain::MeshlinePolicy::Normal::MAX },
				{ "NONE", domain::MeshlinePolicy::Normal::NONE }
			}, item(1, 1)->text(), state.normal),
		are_policy_and_normal_compatible(),
		try_to_bool(item(2, 1)->checkState(), state.is_enabled),
		try_to_double(item(3, 1)->text(), state.res_factor),
		try_to_double(item(4, 1)->text(), state.d)
	};

	if(std::ranges::all_of(does_succeed, is_true)) {
		emit request_to_go_before(app::Step::DETECT_INTERVALS);
		meshline_policy->set_next_state(state);
		emit edited(app::Step::DETECT_INTERVALS);
	}
}

} // namespace ui::qt
