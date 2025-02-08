///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsLineItem>
#include <QPen>

#include <array>
#include <functional>

#include "domain/geometrics/space.hpp"
#include "ui/qt/user_types.hpp"

namespace domain {
class MeshlinePolicy;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class StructureMeshlinePolicy : public QGraphicsLineItem {
public:
	enum { Type = UserTypes::STRUCTURE_MESHLINE_POLICY };

	struct Params final {
		QPen main_line_regular = Qt::NoPen;
		QPen main_line_selected = QPen(Qt::red, 0.1, Qt::DashLine, Qt::FlatCap, Qt::MiterJoin);
		QPen main_line_regular_hovered = main_line_regular;
		QPen main_line_selected_hovered = main_line_selected;
		QPen policy_lines_regular = Qt::NoPen;
		QPen policy_lines_selected = QPen(Qt::red, 0.1, Qt::DashLine, Qt::FlatCap, Qt::MiterJoin);
		QPen policy_lines_regular_hovered = policy_lines_regular;
		QPen policy_lines_selected_hovered = policy_lines_selected;
	};

	std::function<Params const& ()> locate_structure_meshline_policy_params;

	StructureMeshlinePolicy(domain::ViewAxis axis, domain::MeshlinePolicy const* meshline_policy, QRectF scene_rect, QGraphicsItem* parent = nullptr);

	int type() const override;
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

	domain::ViewAxis const axis;

private:
	domain::MeshlinePolicy const* const meshline_policy;
	std::array<QLineF, 2> const policy_lines;
};

} // namespace ui::qt
