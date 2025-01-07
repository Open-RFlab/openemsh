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

#include <functional>

namespace ui::qt {

//******************************************************************************
class ProcessingPlane : public nodegraph::Container {
public:
	enum { Type = UserTypes::PROCESSING_PLANE };

	struct Params final {
		nodegraph::Node::Params const& node = default_locator<nodegraph::Node::Params>();
		nodegraph::Text::Params const& title = default_locator<nodegraph::Text::Params>();
		nodegraph::Rect::Params const& nested_zone = default_locator<nodegraph::Rect::Params>();
	};

	std::function<Params const& ()> locate_processing_plane_params;

	explicit ProcessingPlane(domain::Plane plane, QGraphicsItem* parent = nullptr);
	~ProcessingPlane();

	int type() const override;

private:
	domain::Plane const plane;
};

} // namespace ui::qt
