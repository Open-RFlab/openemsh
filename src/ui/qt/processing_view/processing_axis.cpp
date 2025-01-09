///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "infra/utils/to_string.hpp"

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
		case UserTypes::PROCESSING_MESHLINE_POLICY: return 1;
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
}

//******************************************************************************
ProcessingAxis::~ProcessingAxis() = default;

//******************************************************************************
int ProcessingAxis::type() const {
	return Type;
}

} // namespace ui::qt
