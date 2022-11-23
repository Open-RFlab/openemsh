///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <string>

#include "utils/entity.hpp"

//******************************************************************************
class SerializerToPrettyprint : public EntityVisitor {
public:
	static std::string run(Board& board);

private:
	friend class Board;
	friend class Edge;
	friend class Polygon;
	friend class ConflictColinearEdges;
	friend class ConflictEdgeInPolygon;
	friend class ConflictTooCloseMeshlinePolicies;
	friend class MeshlinePolicy;
	friend class Interval;
	friend class Meshline;

	void visit(Board& board) override;
	void visit(Edge& edge) override;
	void visit(Polygon& polygon) override;
	void visit(ConflictColinearEdges& conflict) override;
	void visit(ConflictEdgeInPolygon& conflict) override;
	void visit(ConflictTooCloseMeshlinePolicies& conflict) override;
	void visit(MeshlinePolicy& policy) override;
	void visit(Interval& interval) override;
	void visit(Meshline& meshline) override {}
	std::string dump();

	std::string out;
	size_t indent = 0;
};
