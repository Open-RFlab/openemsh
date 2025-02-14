///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "ui/qt/user_types.hpp"
#include "ui/qt/utils/nodegraph/node.hpp"
#include "ui/qt/utils/nodegraph/text.hpp"
#include "utils/default_locator.hpp"

namespace domain {
class Meshline;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class ProcessingMeshline : public nodegraph::Node {
public:
	enum { Type = UserTypes::PROCESSING_MESHLINE };

	struct Params final {
		nodegraph::Node::Params const& node = default_locator<nodegraph::Node::Params>();
		nodegraph::Port::Params const& port = default_locator<nodegraph::Port::Params>();
		nodegraph::Text::Params const& title = default_locator<nodegraph::Text::Params>();
		nodegraph::Text::Params const& main = default_locator<nodegraph::Text::Params>();
	};

	std::function<Params const& ()> locate_processing_meshline_params;

	explicit ProcessingMeshline(domain::Meshline const* meshline, QGraphicsItem* parent = nullptr);

	int type() const override;

private:
	domain::Meshline const* const meshline;
};

} // namespace ui::qt
