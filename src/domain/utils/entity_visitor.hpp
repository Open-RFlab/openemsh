///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

namespace domain {

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

} // namespace domain
