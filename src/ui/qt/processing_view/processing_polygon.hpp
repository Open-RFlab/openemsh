///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QList>

#include "ui/qt/user_types.hpp"
#include "ui/qt/utils/nodegraph/container.hpp"
#include "ui/qt/utils/nodegraph/port.hpp"
#include "ui/qt/utils/nodegraph/rect.hpp"
#include "ui/qt/utils/nodegraph/text.hpp"
#include "utils/default_locator.hpp"

#include <functional>

namespace domain {
class Polygon;
} // namespace domain

namespace ui::qt {

class ProcessingEdge;

//******************************************************************************
class ProcessingPolygon : public nodegraph::Container {
public:
	enum { Type = UserTypes::PROCESSING_POLYGON };

	struct Params final {
		nodegraph::Node::Params const& node = default_locator<nodegraph::Node::Params>();
		nodegraph::Port::Params const& port = default_locator<nodegraph::Port::Params>();
		nodegraph::Text::Params const& title = default_locator<nodegraph::Text::Params>();
		nodegraph::Text::Params const& main = default_locator<nodegraph::Text::Params>();
		nodegraph::Rect::Params const& nested_zone = default_locator<nodegraph::Rect::Params>();
	};

	std::function<Params const& ()> locate_processing_polygon_params;

	explicit ProcessingPolygon(domain::Polygon const* polygon, QGraphicsItem* parent = nullptr);

	int type() const override;

	QList<ProcessingEdge*> edges() const;

private:
	domain::Polygon const* const polygon;
};

} // namespace ui::qt
