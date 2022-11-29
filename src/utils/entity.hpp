///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <cstddef>
#include <type_traits>

class Board;
class Edge;
class Polygon;
class ConflictColinearEdges;
class ConflictEdgeInPolygon;
class ConflictTooCloseMeshlinePolicies;
class MeshlinePolicy;
class Interval;
class Meshline;

//******************************************************************************
inline size_t generate_id() {
	static size_t i = 0;
	return i++;
}

//******************************************************************************
class Entity {
public:
	size_t const id;

protected:
	explicit Entity(size_t id) : id(id) {}
	Entity() : id(generate_id()) {}
	virtual ~Entity() = default;
};

//******************************************************************************
class EntityVisitor {
public:
	virtual void visit([[maybe_unused]] Board& board) {};
	virtual void visit([[maybe_unused]] Edge& edge) {};
	virtual void visit([[maybe_unused]] Polygon& polygon) {};
	virtual void visit([[maybe_unused]] ConflictColinearEdges& conflict) {};
	virtual void visit([[maybe_unused]] ConflictEdgeInPolygon& conflict) {};
	virtual void visit([[maybe_unused]] ConflictTooCloseMeshlinePolicies& conflict) {};
	virtual void visit([[maybe_unused]] MeshlinePolicy& policy) {};
	virtual void visit([[maybe_unused]] Meshline& meshline) {};
	virtual void visit([[maybe_unused]] Interval& interval) {};
	virtual ~EntityVisitor() = 0;
};

//******************************************************************************
inline EntityVisitor::~EntityVisitor() = default;

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
