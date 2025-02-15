///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <limits>

#include "infra/utils/to_string.hpp"
#include "ui/qt/data_keys.hpp"
#include "processing_conflict_too_close_meshline_policies.hpp"
#include "processing_meshline_policy.hpp"

#include "processing_axis.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingAxis::ProcessingAxis(domain::Axis axis, QGraphicsItem* parent)
: nodegraph::Container("Axis " + QString::fromStdString(to_string(axis)), QSizeF(50, 10), parent)
, locate_processing_axis_params(default_locator<Params>)
, axis(axis)
{
	get_column = [](QGraphicsItem const* item) -> std::size_t {
		if(!item)
			return 0;
		switch(item->type()) {
		case UserTypes::PROCESSING_CONFLICT_CE: return 0;
		// Can be optimized by splitting count() -> compute()/get()
		case UserTypes::PROCESSING_MESHLINE_POLICY: return 1 + static_cast<ProcessingMeshlinePolicy const*>(item)->count_mlp_tcmlp_deepness();
		case UserTypes::PROCESSING_CONFLICT_TCMLP: return 2 + static_cast<ProcessingConflictTooCloseMeshlinePolicies const*>(item)->count_tcmlp_mlp_deepness();
		case UserTypes::PROCESSING_INTERVAL: return std::numeric_limits<std::size_t>::max() - 1;
		case UserTypes::PROCESSING_MESHLINE: return std::numeric_limits<std::size_t>::max();
		default: return 0;
		}
	};

	locate_node_params = [this]() -> auto& {
		return locate_processing_axis_params().node;
	};

	title->locate_text_params = [this]() -> auto& {
		return locate_processing_axis_params().title;
	};

	nested_zone->locate_rect_params = [this]() -> auto& {
		return locate_processing_axis_params().nested_zone;
	};

	setData(DataKeys::TYPE, "Axis");
	setData(DataKeys::NAME, QString::fromStdString(to_string(axis)));
}

//******************************************************************************
int ProcessingAxis::type() const {
	return Type;
}

} // namespace ui::qt
