///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "domain/geometrics/space.hpp"
#include "ui/qt/user_types.hpp"
#include "ui/qt/utils/nodegraph/container.hpp"
#include "ui/qt/utils/nodegraph/rect.hpp"
#include "ui/qt/utils/nodegraph/text.hpp"
#include "utils/default_locator.hpp"

namespace ui::qt {

//******************************************************************************
class ProcessingAxis : public nodegraph::Container {
public:
	enum { Type = UserTypes::PROCESSING_AXIS };

	struct Params final {
		nodegraph::Node::Params const& node = default_locator<nodegraph::Node::Params>();
		nodegraph::Text::Params const& title = default_locator<nodegraph::Text::Params>();
		nodegraph::Rect::Params const& nested_zone = default_locator<nodegraph::Rect::Params>();
	};

	std::function<Params const& ()> locate_processing_axis_params;

	explicit ProcessingAxis(domain::Axis axis, QGraphicsItem* parent = nullptr);

	int type() const override;

private:
	domain::Axis const axis;
};

} // namespace ui::qt
