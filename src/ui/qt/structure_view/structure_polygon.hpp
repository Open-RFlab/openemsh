///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QBrush>
#include <QGraphicsPolygonItem>
#include <QPen>

#include <functional>

#include "ui/qt/user_types.hpp"

namespace domain {
class Polygon;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class StructurePolygon : public QGraphicsPolygonItem {
public:
	enum { Type = UserTypes::STRUCTURE_POLYGON };

	struct Params final {
		QPen contour_regular = QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		QPen contour_selected = contour_regular;
		QPen contour_regular_hovered = contour_regular;
		QPen contour_selected_hovered = contour_selected;
		QBrush fill_regular = QBrush(Qt::yellow);
		QBrush fill_selected = QBrush(Qt::red);
		QBrush fill_regular_hovered = fill_regular;
		QBrush fill_selected_hovered = fill_selected;
	};

	std::function<Params const& ()> locate_structure_polygon_params;

	explicit StructurePolygon(domain::Polygon const* polygon, QGraphicsItem* parent = nullptr);

	int type() const override;
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

private:
	domain::Polygon const* const polygon;
};

} // namespace ui::qt
