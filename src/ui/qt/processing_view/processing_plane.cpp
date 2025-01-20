///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "infra/utils/to_string.hpp"
#include "ui/qt/data_keys.hpp"

#include "processing_plane.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingPlane::ProcessingPlane(domain::Plane plane, QGraphicsItem* parent)
: nodegraph::Container("Plane " + QString::fromStdString(to_string(plane)), QSizeF(50, 10), parent)
, locate_processing_plane_params(default_locator<Params>)
, plane(plane)
{
	get_column = [](QGraphicsItem const* item) {
		if(!item)
			return 0;
		switch(item->type()) {
		case UserTypes::PROCESSING_CONFLICT_EIP: return 0;
		case UserTypes::PROCESSING_POLYGON: return 1;
		default: return 0;
		}
	};

	locate_node_params = [&]() -> auto& {
		return locate_processing_plane_params().node;
	};

	title->locate_text_params = [&]() -> auto& {
		return locate_processing_plane_params().title;
	};

	nested_zone->locate_rect_params = [&]() -> auto& {
		return locate_processing_plane_params().nested_zone;
	};

	setData(DataKeys::TYPE, "Plane");
	setData(DataKeys::NAME, QString::fromStdString(to_string(plane)));
}

//******************************************************************************
ProcessingPlane::~ProcessingPlane() = default;

//******************************************************************************
int ProcessingPlane::type() const {
	return Type;
}

} // namespace ui::qt
