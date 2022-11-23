///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <string>

#include "utils/entity.hpp"

//******************************************************************************
class SerializerToPlantuml : public EntityVisitor {
public:
	struct Params {
		bool with_conflict_edge_in_polygon = true;
		bool with_conflict_colinear_edges = true;
		bool with_conflict_too_close_meshline_policies = true;
		bool with_axis_part = true;
		bool with_geometric_part = true;
	};

	static std::string run(Board& board);
	static std::string run(Board& board, Params params);

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
	void visit(Meshline& meshline) override;

	SerializerToPlantuml(Params params);
	std::string dump();

	std::string out;
	Params const params;
};
