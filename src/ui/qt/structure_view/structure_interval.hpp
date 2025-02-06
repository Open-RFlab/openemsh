///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsRectItem>
#include <QBrush>

#include <functional>

#include "domain/geometrics/space.hpp"
#include "ui/qt/user_types.hpp"

namespace domain {
class Interval;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class StructureInterval : public QGraphicsRectItem {
public:
	enum { Type = UserTypes::STRUCTURE_INTERVAL };

	struct Params final {
		QBrush regular = Qt::transparent;
		QBrush selected = QColor(0, 255, 0, 100);
		QBrush regular_hovered = regular;
		QBrush selected_hovered = selected;
	};

	std::function<Params const& ()> locate_structure_interval_params;

	StructureInterval(domain::ViewAxis axis, domain::Interval const* interval, QRectF scene_rect, QGraphicsItem* parent = nullptr);

	int type() const override;

protected:
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;
	QVariant itemChange(GraphicsItemChange change, QVariant const& value) override;

	domain::ViewAxis const axis;
private:
	domain::Interval const* const interval;
};

} // namespace ui::qt
