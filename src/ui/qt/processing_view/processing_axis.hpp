///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "domain/geometrics/space.hpp"
#include "ui/qt/user_types.hpp"
#include "ui/qt/utils/nodegraph/container.hpp"

namespace ui::qt {

//******************************************************************************
class ProcessingAxis : public nodegraph::Container {
public:
	enum { Type = UserTypes::PROCESSING_AXIS };

	explicit ProcessingAxis(domain::Axis axis, QGraphicsItem* parent = nullptr);
	~ProcessingAxis();

	int type() const override;

	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

private:
	domain::Axis const axis;
};

} // namespace ui::qt
