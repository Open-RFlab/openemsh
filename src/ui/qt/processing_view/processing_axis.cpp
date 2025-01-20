///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "infra/utils/to_string.hpp"
#include "ui/qt/data_keys.hpp"
#include "processing_meshline_policy.hpp"

#include "processing_axis.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingAxis::ProcessingAxis(domain::Axis axis, QGraphicsItem* parent)
: nodegraph::Container("Axis " + QString::fromStdString(to_string(axis)), QSizeF(50, 10), parent)
, locate_processing_axis_params(default_locator<Params>)
, axis(axis)
{
	get_column = [](QGraphicsItem const* item) {
		if(!item)
			return 0;
		switch(item->type()) {
		case UserTypes::PROCESSING_CONFLICT_CE: return 0;
		case UserTypes::PROCESSING_MESHLINE_POLICY:
			if(auto* policy = dynamic_cast<ProcessingMeshlinePolicy const*>(item)
			; policy && policy->has_conflict_tcmlp_origin())
				return 3;
			else
				return 1;
		case UserTypes::PROCESSING_CONFLICT_TCMLP: return 2;
		case UserTypes::PROCESSING_INTERVAL: return 4;
		case UserTypes::PROCESSING_MESHLINE: return 5;
		default: return 0;
		}
	};

	locate_node_params = [&]() -> auto& {
		return locate_processing_axis_params().node;
	};

	title->locate_text_params = [&]() -> auto& {
		return locate_processing_axis_params().title;
	};

	nested_zone->locate_rect_params = [&]() -> auto& {
		return locate_processing_axis_params().nested_zone;
	};

	setData(DataKeys::TYPE, "Axis");
	setData(DataKeys::NAME, QString::fromStdString(to_string(axis)));
}

//******************************************************************************
ProcessingAxis::~ProcessingAxis() = default;

//******************************************************************************
int ProcessingAxis::type() const {
	return Type;
}

} // namespace ui::qt
