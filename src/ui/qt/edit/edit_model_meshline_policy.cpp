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
using Normal = domain::MeshlinePolicy::Normal;
using Policy = domain::MeshlinePolicy::Policy;

//******************************************************************************
EditModelMeshlinePolicy::EditModelMeshlinePolicy(domain::MeshlinePolicy* meshline_policy, QObject* parent)
: EditModel(parent)
, meshline_policy(meshline_policy)
{
	auto const& state = meshline_policy->get_current_state();
	setRowCount(4);

	make_row(0, "Policy", state.policy,
		"<b>ONELINE:</b> One meshline at the policy position.<br/>"
		"<b>HALFS:</b> Two lines centered around the policy position.<br/>"
		"<b>THIRDS:</b> Two lines placed around (2d/3 in Normal direction, d/3 the other side) the policy position.");
	make_row(1, "Normal", state.normal,
		"Direction associated with Policy.");
	make_row(2, "Enabled", state.is_enabled,
		"Take into account in the meshing process.");
	make_row(3, "d", QString::number(state.d),
		"Desired distance between policy lines (HALFS|THIRDS) or "
		"between policy line and adjacent lines (ONELINE).<br/>"
		"Can be decreased by the meshing algorithm.");
}

//******************************************************************************
void EditModelMeshlinePolicy::commit() {
	auto state = meshline_policy->get_current_state();

	auto const are_policy_and_normal_compatible = [&state]() {
		switch(state.policy) {
		case Policy::ONELINE: [[fallthrough]];
		case Policy::HALFS:
			switch(state.normal) {
			case Normal::MIN: [[fallthrough]];
			case Normal::MAX: return false;
			case Normal::NONE: return true;
			default: unreachable();
			}
		case Policy::THIRDS:
			switch(state.normal) {
			case Normal::MIN: [[fallthrough]];
			case Normal::MAX: return true;
			case Normal::NONE: return false;
			default: unreachable();
			}
		default:unreachable();
		}
	};

	state.policy = item(0, V)->data().value<Policy>();
	state.normal = item(1, V)->data().value<Normal>();

	std::array does_succeed = {
		are_policy_and_normal_compatible(),
		try_to_bool(item(2, V)->checkState(), state.is_enabled),
		try_to_double(item(3, V)->text(), state.d)
	};

	if(std::ranges::all_of(does_succeed, is_true)) {
		emit edit_from(app::Step::DETECT_INTERVALS, [&]() {
			meshline_policy->set_next_state(state);
		});
	}
}

} // namespace ui::qt
