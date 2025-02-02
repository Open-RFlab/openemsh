///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QString>

#include <optional>
#include <vector>

#include "processing_view/processing_style.hpp"
#include "structure_view/structure_style.hpp"

namespace ui::qt {

//******************************************************************************
struct Style {
	QString name;
	ProcessingStyle processing;
	StructureStyle structure;

	static std::vector<Style> const available_styles;
	static std::optional<Style> find_style(QString const& name);
};

} // namespace ui::qt
