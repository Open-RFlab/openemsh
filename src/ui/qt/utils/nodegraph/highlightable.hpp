///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QSet>

class QGraphicsItem;

namespace ui::qt::nodegraph {

//******************************************************************************
class Highlightable {
public:
	virtual ~Highlightable() = 0;

	bool is_highlighted() const;
	virtual void set_highlighted(bool is_highlighted, QGraphicsItem const* by_item = nullptr);

private:
	bool _is_highlighted = false;
	QSet<QGraphicsItem const*> by_items;
};

} // namespace ui::qt::nodegraph
