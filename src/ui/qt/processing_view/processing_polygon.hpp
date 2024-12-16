///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QList>

#include "ui/qt/user_types.hpp"
#include "ui/qt/utils/nodegraph/container.hpp"

namespace domain {
class Polygon;
} // namespace domain

namespace ui::qt {

class ProcessingEdge;

//******************************************************************************
class ProcessingPolygon : public nodegraph::Container {
public:
	enum { Type = UserTypes::PROCESSING_POLYGON };

	explicit ProcessingPolygon(domain::Polygon const* polygon, QGraphicsItem* parent = nullptr);
	~ProcessingPolygon();

	int type() const override;

	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

	QList<ProcessingEdge*> edges();

private:
	domain::Polygon const* const polygon;
};

} // namespace ui::qt
