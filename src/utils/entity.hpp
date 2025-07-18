///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <cstddef>
//#include <type_traits>

//******************************************************************************
class Entity {
public:
	std::size_t const id;

protected:
	Entity();
	explicit Entity(std::size_t id);
	virtual ~Entity() = default;
};

//******************************************************************************
template<typename T, typename Visitor>
class Visitable {
public:
//	static_assert(std::is_base_of<Entity, T>::value);
	virtual void accept(Visitor& visitor) final {
		visitor.visit(dynamic_cast<T&>(*this));
	}

	virtual ~Visitable() = default;
};
