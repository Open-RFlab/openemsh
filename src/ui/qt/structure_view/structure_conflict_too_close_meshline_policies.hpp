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
class ConflictTooCloseMeshlinePolicies;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class StructureConflictTooCloseMeshlinePolicies : public QGraphicsLineItem {
public:
	enum { Type = UserTypes::STRUCTURE_CONFLICT_TCMLP };

	struct Params final {
		QPen solution_line_regular = Qt::NoPen;
		QPen solution_line_selected = QPen(Qt::green, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		QPen solution_line_regular_hovered = solution_line_regular;
		QPen solution_line_selected_hovered = solution_line_selected;
		QPen origin_lines_regular = Qt::NoPen;
		QPen origin_lines_selected = QPen(Qt::red, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		QPen origin_lines_regular_hovered = origin_lines_regular;
		QPen origin_lines_selected_hovered = origin_lines_selected;
	};

	std::function<Params const& ()> locate_structure_conflict_tcmlp_params;

	StructureConflictTooCloseMeshlinePolicies(domain::ViewAxis axis, domain::ConflictTooCloseMeshlinePolicies const* conflict, QRectF scene_rect, QGraphicsItem* parent = nullptr);

	int type() const override;
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

	domain::ViewAxis const axis;

private:
	domain::ConflictTooCloseMeshlinePolicies const* const conflict;
	std::array<QLineF, 2> const origin_lines;
};

} // namespace ui::qt
