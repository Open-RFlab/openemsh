///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QPair>
#include <QVariant>

#include "utils/nodegraph/port.hpp"

class Entity;

namespace ui::qt {

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
inline Entity const* get_entity(QVariant const& variant) {
	return variant.value<Entity const*>();
}

//******************************************************************************
inline QVariant set_entity(Entity const* entity) {
	return QVariant::fromValue(entity);
}

//******************************************************************************
using ToWire = QPair<Entity const*, nodegraph::Port*>;

//******************************************************************************
inline ToWire get_to_wire(QVariant const& variant) {
	return variant.value<ToWire>();
}

//******************************************************************************
inline QVariant set_to_wire(Entity const* entity, nodegraph::Port* port) {
	return QVariant::fromValue(ToWire(entity, port));
}

} // namespace DataKeys

} // namespace ui::qt
