///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <string>

#include "domain/utils/entity_visitor.hpp"

//******************************************************************************
class SerializerToPrettyprint final : public domain::EntityVisitor {
public:
	static std::string run(domain::Board& board);

private:
	friend class domain::Board;
	friend class domain::Edge;
	friend class domain::Polygon;
	friend class domain::ConflictColinearEdges;
	friend class domain::ConflictEdgeInPolygon;

	void visit(domain::Board& board) override;
	void visit(domain::Edge& edge) override;
	void visit(domain::Polygon& polygon) override;
	void visit(domain::ConflictColinearEdges& conflict) override;
	void visit(domain::ConflictEdgeInPolygon& conflict) override;
	std::string dump();

	std::string out;
	size_t indent = 0;
};
