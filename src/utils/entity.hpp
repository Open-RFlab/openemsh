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
	virtual void visit(Board& board) = 0;
	virtual void visit(Edge& edge) = 0;
	virtual void visit(Polygon& polygon) = 0;
	virtual void visit(ConflictColinearEdges& conflict) = 0;
	virtual void visit(ConflictEdgeInPolygon& conflict) = 0;
	virtual void visit(ConflictTooCloseMeshlinePolicies& conflict) = 0;
	virtual void visit(MeshlinePolicy& policy) = 0;
	virtual void visit(Interval& interval) = 0;
};

//******************************************************************************
template<typename T, typename Visitor>
class Visitable {
public:
//	static_assert(std::is_base_of<Entity, T>::value);
	virtual void accept(Visitor& visitor) final {
		visitor.visit(dynamic_cast<T&>(*this));
	}
};
