///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "edit_model.hpp"

namespace domain {
class Edge;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class EditModelEdge : public EditModel {
public:
	explicit EditModelEdge(domain::Edge* edge, QObject* parent = nullptr);
	void commit() override;

private:
	domain::Edge* edge;
};

} // namespace ui::qt
