///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "domain/conflicts/conflict_colinear_edges.hpp"
#include "domain/conflicts/conflict_edge_in_polygon.hpp"
#include "domain/conflicts/conflict_too_close_meshline_policies.hpp"
#include "domain/geometrics/coord.hpp"
#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/point.hpp"
#include "domain/geometrics/polygon.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "domain/mesh/interval.hpp"
#include "domain/board.hpp"
#include "infra/utils/to_string.hpp"
#include "utils/vector_utils.hpp"
#include "utils/color.hpp"

#include "serializer_to_prettyprint.hpp"

using namespace domain;
using namespace std;

//******************************************************************************
string indent(size_t indent=0) {
	string out;
	for(size_t i = 0; i < indent; ++i)
		out += "\t";

	return out;
}

//******************************************************************************
string to_string(Coord const& coord) {
	return to_string(coord.value());
}

//******************************************************************************
string to_string(Point const& point) {
	return "{ " + to_string(point.x) + ", " + to_string(point.y) + " }";
}

//******************************************************************************
string to_string(Range const& range) {
	return to_string(range.p0()) + " -> " + to_string(range.p1()) + " " + to_string(range.axis);
}

//******************************************************************************
string SerializerToPrettyprint::run(Board& board) {
	SerializerToPrettyprint serializer;
	board.accept(serializer);
	return serializer.dump();
}

//******************************************************************************
void SerializerToPrettyprint::visit(Board& board) {

	out += F_ORANGE + S_BOLD + "\nCONFLICT: EDGE IN POLYGONS\n\n" + S_RESET;
	for(auto const plane : AllPlane)
		for(auto const& conflict : board.get_conflicts_edge_in_polygons(plane))
			conflict->accept(*this);

	for(auto const axis : AllAxis) {
		auto policies = board.get_meshline_policies(axis);

		ranges::sort(policies,
			[](auto const& a, auto const& b) {
				return a->coord < b->coord;
			});

		for(auto const& policy : policies)
			policy->accept(*this);

		for(auto const& interval : board.get_intervals(axis))
			interval->accept(*this);
	}

	out += F_ORANGE + S_BOLD + "\nCONFLICT: COLINEAR EDGES\n\n" + S_RESET;
	for(auto const axis : AllAxis)
		for(auto const& conflict : board.get_conflicts_colinear_edges(axis))
			conflict->accept(*this);

	out += F_ORANGE + S_BOLD + "\nCONFLICT: TOO CLOSE MESHLINE POLICIES\n\n" + S_RESET;
	for(auto const axis : AllAxis)
		for(auto const& conflict : board.get_conflicts_too_close_meshline_policies(axis))
			conflict->accept(*this);
}

//******************************************************************************
void SerializerToPrettyprint::visit(Edge& edge) {
	out += to_string(edge.p0()) + " -> " + to_string(edge.p1()) + " " + to_string(edge.axis) + " " + to_string(edge.direction) + " " + to_string(edge.normal) + (edge.get_current_state().to_mesh ? " O" : " X") + "\n";
}

//******************************************************************************
void SerializerToPrettyprint::visit(Polygon& polygon) {
	out += F_CYAN + polygon.name + S_RESET + " " + to_string(polygon.rotation) + /*" " + to_string(polygon.plane) +*/ " {\n";

	for(auto const& edge : polygon.edges) {
		out += ::indent(indent);
		edge->accept(*this);
	}

	out += ::indent(indent) + "}\n";
}

//******************************************************************************
void SerializerToPrettyprint::visit(ConflictColinearEdges& conflict) {
	auto solution = dynamic_cast<MeshlinePolicy*>(conflict.get_current_state().solution);

	out +=
		F_ORANGE + "Conflict ColinearEdges between:\n" + S_RESET +
		"\tAxis: " + to_string(conflict.axis) + "\n";

	for(auto const& edge : conflict.get_current_state().edges) {
		out += "\tEdge: ";
		edge->accept(*this);
	}

	if(solution)
		out += "\tSolution: " + F_D_GREEN + to_string(solution->policy) + S_RESET;

	out += "\n";
}

//******************************************************************************
void SerializerToPrettyprint::visit(ConflictEdgeInPolygon& conflict) {
	out +=
		F_ORANGE + "Conflict EdgeInPolygon between:\n" + S_RESET +
		"\tPlane: " + to_string(conflict.plane) + "\n"
		"\tEdge: ";

	conflict.edge->accept(*this);

	indent += 2;
	for(auto& [polygon, range, overlapping_edge] : conflict.get_current_state().overlaps) {
		out += "\tPolygon: ";
		const_cast<Polygon*>(polygon)->accept(*this);

		if(overlapping_edge) {
			out += ::indent(indent) + "by edge: ";
			const_cast<Edge*>(overlapping_edge.value())->accept(*this);
		}

		out += ::indent(indent) + "on range: " + to_string(range) + "\n";
	}
	indent -= 2;

	if(conflict.get_current_state().solution)
		out += "\tSolution: " + (conflict.edge->get_current_state().to_mesh ? F_D_GREEN + "to mesh" : F_D_RED + "don't mesh") + S_RESET;

	out += "\n";
}

//******************************************************************************
string SerializerToPrettyprint::dump() {
	return std::move(out);
}
