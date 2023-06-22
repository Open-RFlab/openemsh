///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <string>

#include "domain/utils/entity_visitor.hpp"

//******************************************************************************
class SerializerToPlantuml final : public domain::EntityVisitor {
public:
	struct Params {
		bool with_conflict_edge_in_polygon = true;
		bool with_conflict_colinear_edges = true;
		bool with_conflict_too_close_meshline_policies = true;
		bool with_axis_part = true;
		bool with_geometric_part = true;
	};

	static std::string run(domain::Board& board);
	static std::string run(domain::Board& board, Params params);

private:
	friend class domain::Board;
	friend class domain::Edge;
	friend class domain::Polygon;
	friend class domain::ConflictColinearEdges;
	friend class domain::ConflictEdgeInPolygon;
	friend class domain::ConflictTooCloseMeshlinePolicies;
	friend class domain::MeshlinePolicy;
	friend class domain::Interval;
	friend class domain::Meshline;

	void visit(domain::Board& board) override;
	void visit(domain::Edge& edge) override;
	void visit(domain::Polygon& polygon) override;
	void visit(domain::ConflictColinearEdges& conflict) override;
	void visit(domain::ConflictEdgeInPolygon& conflict) override;
	void visit(domain::ConflictTooCloseMeshlinePolicies& conflict) override;
	void visit(domain::MeshlinePolicy& policy) override;
	void visit(domain::Interval& interval) override;
	void visit(domain::Meshline& meshline) override;

	SerializerToPlantuml(Params params);
	std::string dump();

	std::string out;
	Params const params;
};
