///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "infra/utils/to_string.hpp"

#include "processing_plane.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingPlane::ProcessingPlane(domain::Plane plane, QGraphicsItem* parent)
: nodegraph::Container("Plane " + QString::fromStdString(to_string(plane)), QSizeF(50, 10), parent)
, locate_processing_plane_params(default_locator<Params>)
, plane(plane)
{
	locate_node_params = [&]() -> auto& {
		return locate_processing_plane_params().node;
	};

	title->locate_text_params = [&]() -> auto& {
		return locate_processing_plane_params().title;
	};

	nested_zone->locate_rect_params = [&]() -> auto& {
		return locate_processing_plane_params().nested_zone;
	};
}

//******************************************************************************
ProcessingPlane::~ProcessingPlane() = default;

//******************************************************************************
int ProcessingPlane::type() const {
	return Type;
}

} // namespace ui::qt
