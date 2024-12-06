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
, axis(axis)
{}

//******************************************************************************
ProcessingAxis::~ProcessingAxis() = default;

//******************************************************************************
int ProcessingAxis::type() const {
	return Type;
}

//******************************************************************************
void ProcessingAxis::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget) {
	nodegraph::Container::paint(painter, option, widget);
}

} // namespace ui::qt
