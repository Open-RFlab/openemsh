///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

namespace ui::qt {

// Scoped enum is not used but imitated because implicit conversion is wanted.
//******************************************************************************
namespace DataKeys {
enum DataKeys : int {
	TYPE,
	ID,
	NAME
};
} // namespace DataKeys

} // namespace ui::qt
