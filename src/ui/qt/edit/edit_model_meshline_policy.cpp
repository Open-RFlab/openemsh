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

Q_DECLARE_METATYPE(domain::MeshlinePolicy::Policy)
Q_DECLARE_METATYPE(domain::MeshlinePolicy::Normal)

namespace ui::qt {

//******************************************************************************
EditModelMeshlinePolicy::EditModelMeshlinePolicy(domain::MeshlinePolicy* meshline_policy, QObject* parent)
: EditModel(parent)
, meshline_policy(meshline_policy)
{
	auto const& state = meshline_policy->get_current_state();
	setRowCount(5);

	make_row(0, "policy", state.policy, "");
	make_row(1, "normal", state.normal, "");
	make_row(2, "is_enabled", state.is_enabled, "");
	make_row(3, "res_factor", QString::number(state.res_factor), "");
	make_row(4, "d", QString::number(state.d), "");
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

	state.policy = item(0, V)->data().value<domain::MeshlinePolicy::Policy>();
	state.normal = item(1, V)->data().value<domain::MeshlinePolicy::Normal>();

	std::array does_succeed = {
		are_policy_and_normal_compatible(),
		try_to_bool(item(2, V)->checkState(), state.is_enabled),
		try_to_double(item(3, V)->text(), state.res_factor),
		try_to_double(item(4, V)->text(), state.d)
	};

	if(std::ranges::all_of(does_succeed, is_true)) {
		emit edit_from(app::Step::DETECT_INTERVALS, [&]() {
			meshline_policy->set_next_state(state);
		});
	}
}

} // namespace ui::qt
