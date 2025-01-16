///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QPair>
#include <QVariant>

class Entity;

namespace ui::qt {

namespace nodegraph {
class Port;
} // namespace nodegraph

// Scoped enum is not used but imitated because implicit conversion is wanted.
//******************************************************************************
namespace DataKeys {

enum DataKeys : int {
	TYPE,
	ID,
	ENTITY,
	NAME,
	TO_WIRE
};


//******************************************************************************
constexpr Entity const* get_entity(QVariant const& variant) {
	return static_cast<Entity const*>(variant.value<void const*>());
}

//******************************************************************************
constexpr QVariant set_entity(Entity const* entity) {
	return QVariant::fromValue(static_cast<void const*>(entity));
}

//******************************************************************************
using ToWire = QPair<Entity const*, nodegraph::Port*>;

} // namespace DataKeys

} // namespace ui::qt
