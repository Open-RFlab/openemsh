///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsItem>

namespace ui::qt {

// Scoped enum is not used but imitated because implicit conversion is wanted.
//******************************************************************************
namespace UserTypes {
enum UserTypes {
	STRUCTURE_EDGE = QGraphicsItem::UserType + 1,
	STRUCTURE_POLYGON,
	STRUCTURE_INTERVAL,
	STRUCTURE_MESHLINE,
	STRUCTURE_MESHLINE_POLICY,
	STRUCTURE_CONFLICT_CE,
	STRUCTURE_CONFLICT_TCMLP,
	PROCESSING_EDGE,
	PROCESSING_POLYGON,
	PROCESSING_INTERVAL,
	PROCESSING_MESHLINE,
	PROCESSING_MESHLINE_POLICY,
	PROCESSING_CONFLICT_CE,
	PROCESSING_CONFLICT_EIP,
	PROCESSING_CONFLICT_TCMLP,
	PROCESSING_AXIS,
	PROCESSING_PLANE
};
} // namespace UserTypes

} // namespace ui::qt
