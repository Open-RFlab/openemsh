///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "highlightable.hpp"

namespace ui::qt::nodegraph {

//******************************************************************************
Highlightable::~Highlightable() = default;

//******************************************************************************
bool Highlightable::is_highlighted() const {
	return _is_highlighted;
}

//******************************************************************************
void Highlightable::set_highlighted(bool is_highlighted, QGraphicsItem const* by_item) {
	if(is_highlighted) {
		_is_highlighted = true;
		if(by_item)
			by_items.insert(by_item);
	} else {
		if(by_item)
			by_items.remove(by_item);
		else
			by_items.clear();
		if(by_items.empty())
			_is_highlighted = false;
	}
}

} // namespace ui::qt::nodegraph
