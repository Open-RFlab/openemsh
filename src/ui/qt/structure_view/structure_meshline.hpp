///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsLineItem>

#include "domain/geometrics/space.hpp"
#include "ui/qt/user_types.hpp"

namespace domain {
class Meshline;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class StructureMeshline : public QGraphicsLineItem {
public:
	enum { Type = UserType + UserTypes::STRUCTURE_MESHLINE };

	StructureMeshline(domain::ViewAxis axis, domain::Meshline const* meshline, QRectF scene_rect, QGraphicsItem* parent = nullptr);

	int type() const override;
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

	domain::ViewAxis const axis;
private:
	domain::Meshline const* const meshline;
};

} // namespace ui::qt
