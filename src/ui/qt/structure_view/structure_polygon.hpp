///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsPolygonItem>

#include "ui/qt/user_types.hpp"

namespace domain {
class Polygon;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class StructurePolygon : public QGraphicsPolygonItem {
public:
	enum { Type = UserType + UserTypes::STRUCTURE_POLYGON };

	explicit StructurePolygon(domain::Polygon const* polygon, QGraphicsItem* parent = nullptr);

	int type() const override;
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

private:
	domain::Polygon const* const polygon;
};

} // namespace ui::qt
