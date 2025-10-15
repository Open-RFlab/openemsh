///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

namespace ui::qt {

//******************************************************************************
struct Settings {
	static Settings& singleton() noexcept;

	bool does_use_material_color;
};

//******************************************************************************
inline Settings& Settings::singleton() noexcept {
	static Settings s;
	return s;
}

} // namespace ui::qt
