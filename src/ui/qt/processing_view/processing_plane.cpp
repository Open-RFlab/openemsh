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
, plane(plane)
{}

//******************************************************************************
ProcessingPlane::~ProcessingPlane() = default;

//******************************************************************************
int ProcessingPlane::type() const {
	return Type;
}

//******************************************************************************
void ProcessingPlane::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget) {
	nodegraph::Container::paint(painter, option, widget);
}

} // namespace ui::qt
