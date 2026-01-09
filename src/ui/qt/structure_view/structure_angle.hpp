///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsEllipseItem>
#include <QPen>

#include <functional>

#include "domain/geometrics/space.hpp"
#include "ui/qt/user_types.hpp"

namespace domain {
class Angle;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class StructureAngle : public QGraphicsEllipseItem {
public:
	enum { Type = UserTypes::STRUCTURE_ANGLE };

	struct Params final {
//		qreal radius = 2; // TODO
		QPen circle_regular = QPen(Qt::NoPen);
		QPen circle_selected = QPen(Qt::blue, 0.1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		QPen circle_regular_hovered = circle_regular;
		QPen circle_selected_hovered = circle_selected;
		QPen h_line_regular = QPen(Qt::NoPen);
		QPen h_line_selected = QPen(Qt::blue, 0.1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		QPen h_line_regular_hovered = h_line_regular;
		QPen h_line_selected_hovered = h_line_selected;
		QPen v_line_regular = QPen(Qt::NoPen);
		QPen v_line_selected = QPen(Qt::blue, 0.1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		QPen v_line_regular_hovered = v_line_regular;
		QPen v_line_selected_hovered = v_line_selected;
	};

	std::function<Params const& ()> locate_structure_angle_params;

	StructureAngle(domain::Angle const* angle, QGraphicsItem* parent = nullptr);

	int type() const override;
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

private:
	domain::Angle const* const angle;
	domain::ViewAxisSpace<QLineF> const cross_lines;
};

} // namespace ui::qt
