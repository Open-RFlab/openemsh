///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <algorithm>
#include <array>

#include "app/steps.hpp"
#include "domain/geometrics/edge.hpp"

#include "edit_model_edge.hpp"

namespace ui::qt {

//******************************************************************************
EditModelEdge::EditModelEdge(domain::Edge* edge, QObject* parent)
: EditModel(parent)
, edge(edge)
{
	auto const& state = edge->get_current_state();
	setRowCount(2);

	make_row(0, "To mesh", state.to_mesh, "Take into account in the meshing process.");
	make_row(1, "To reverse", state.to_mesh, "Swap in-side and out-side.");
}

//******************************************************************************
void EditModelEdge::commit() {
	auto state = edge->get_current_state();

	std::array does_succeed = {
		try_to_bool(item(0, V)->checkState(), state.to_mesh),
		try_to_bool(item(1, V)->checkState(), state.to_reverse)
	};

	if(std::ranges::all_of(does_succeed, is_true)) {
		emit edit_from(app::Step::DETECT_CONFLICT_EIP, [&]() {
			edge->set_next_state(state);
		});
	}
}

} // namespace ui::qt
