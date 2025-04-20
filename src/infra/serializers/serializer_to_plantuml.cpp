///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "domain/conflicts/conflict_colinear_edges.hpp"
#include "domain/conflicts/conflict_edge_in_polygon.hpp"
#include "domain/conflicts/conflict_too_close_meshline_policies.hpp"
#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/polygon.hpp"
#include "domain/geometrics/space.hpp"
#include "domain/mesh/meshline.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "domain/mesh/interval.hpp"
#include "domain/board.hpp"
#include "infra/utils/to_string.hpp"
#include "utils/vector_utils.hpp"

#include "serializer_to_plantuml.hpp"

using namespace domain;
using namespace std;

//******************************************************************************
string SerializerToPlantuml::run(Board& board) {
	return SerializerToPlantuml::run(board, {});
}

//******************************************************************************
string SerializerToPlantuml::run(Board& board, Params params) {
	SerializerToPlantuml serializer(std::move(params));
	board.accept(serializer);
	return serializer.dump();
}

//******************************************************************************
SerializerToPlantuml::SerializerToPlantuml(Params params)
: params(std::move(params))
{}

//******************************************************************************
void SerializerToPlantuml::visit(Board& board) {
	out += "@startuml\n"
//		"skinparam linetype ortho\n"
//		"!theme plain\n"
//		"hide empty description\n"
		;

	for(auto const plane : AllPlane) {
		out += "state \"Plane " + to_string(plane) + "\" as " + to_string(plane) + " {\n";

		if(params.with_geometric_part)
			for(auto const& polygon : board.get_polygons(plane))
				polygon->accept(*this);

		if(params.with_conflict_edge_in_polygon)
			for(auto const& conflict : board.get_conflicts_edge_in_polygons(plane))
				conflict->accept(*this);

		out += "}\n";
		}

	if(params.with_axis_part)
		for(auto const axis : AllAxis) {
			out += "state \"Axis " + to_string(axis) + "\" as " + to_string(axis) + " {\n";

			auto policies = board.get_meshline_policies(axis);

			ranges::sort(policies,
				[](auto const& a, auto const& b) {
					return a->coord < b->coord;
				});

//			for(auto const& policy : board.get_meshline_policies(axis))
			for(auto const& policy : policies)
				policy->accept(*this);

			for(auto const& interval : board.get_intervals(axis))
				interval->accept(*this);

			for(auto const& meshline : board.get_meshlines(axis))
				meshline->accept(*this);

			if(params.with_conflict_colinear_edges)
				for(auto const& conflict : board.get_conflicts_colinear_edges(axis))
					conflict->accept(*this);

			if(params.with_conflict_too_close_meshline_policies)
				for(auto const& conflict : board.get_conflicts_too_close_meshline_policies(axis))
					conflict->accept(*this);

			out += "}\n";
		}

	out += "@enduml\n";
}

//******************************************************************************
void SerializerToPlantuml::visit(Edge& edge) {
	auto id = to_string(edge.id);

	out +=
		"state \"Edge\" as " + id + (edge.get_current_state().to_mesh ? (edge.get_current_state().conflicts.empty() ? " #chartreuse" : " #green") : " #red") + " {\n" +
		id + " : Normal = " + to_string(edge.normal) + "\n" +
		id + " : To mesh = " + (edge.get_current_state().to_mesh ? "true" : "false") + "\n"
		"}\n";
}

//******************************************************************************
void SerializerToPlantuml::visit(Polygon& polygon) {
	auto id = to_string(polygon.id);

	out +=
		"state \"Polygon " + polygon.name + "\" as " + id + " {\n"
		"state \" \" as " + id + "_out <<outputPin>>\n" +
		id + " : rotation = " + to_string(polygon.rotation) + "\n";

	for(auto const& edge : polygon.edges)
		edge->accept(*this);

	out += "}\n";
}

//******************************************************************************
void SerializerToPlantuml::visit(ConflictColinearEdges& conflict) {
	auto id = to_string(conflict.id);
	auto solution = dynamic_cast<MeshlinePolicy*>(conflict.get_current_state().solution);

	out +=
		"state \"Conflict ColinearEdges\" as " + id + " {\n"
		"state \" \" as " + id + "_in <<inputPin>>\n"
		"state \" \" as " + id + "_out <<outputPin>>\n";

	for(auto const& edge : conflict.get_current_state().edges) {
		out +=
			id + " : Normal = " + to_string(edge->normal) + (edge->get_current_state().to_mesh ? " enabled" : " disabled") + "\n" +
			to_string(edge->id) + " ------> " + id + "_in \n";
	}

	if(solution) {
		out +=
			id + " : Solution.policy = " + to_string(solution->policy) + "\n" +
			id + " : Solution.normal = " + to_string(solution->normal) + "\n" +
			id + "_out ------> " + to_string(solution->id) + "_in\n";
	}

	out += "}\n";
}

//******************************************************************************
void SerializerToPlantuml::visit(ConflictEdgeInPolygon& conflict) {
	auto id = to_string(conflict.id);

	out +=
		"state \"Conflict EdgeInPolygon\" as " + id + (conflict.edge->get_current_state().to_mesh ? " #green" : " #red") + " {\n"
		"state \" \" as " + id + "_out <<outputPin>>\n" +
		to_string(conflict.edge->id) + " ------> " + id + "\n" +
		id + "_out -----[bold," + (conflict.edge->get_current_state().to_mesh ? "#green" : "#red") + "]-> " + to_string(conflict.edge->id) + "\n";

	size_t i = 0;
	for(auto const& [polygon, range, overlapping_edge] : conflict.get_current_state().overlaps) {
		out += "state \" \" as " + id + "_in" + to_string(i) + " <<inputPin>>\n" +
			to_string(polygon->id) + "_out ------> " + id + "_in" + to_string(i) + "\n";
		if(overlapping_edge)
			out += to_string(overlapping_edge.value()->id) + " ------> " + id + "_in" + to_string(i) + "\n";
		++i;
	}

	out += "}\n";
}

//******************************************************************************
void SerializerToPlantuml::visit(ConflictTooCloseMeshlinePolicies& conflict) {
	auto id = to_string(conflict.id);
	auto solution = dynamic_cast<MeshlinePolicy*>(conflict.get_current_state().solution);

	out +=
		"state \"Conflict TooCloseMeshlinePolicies\" as " + id + " {\n"
		"state \" \" as " + id + "_in <<inputPin>>\n"
		"state \" \" as " + id + "_out <<outputPin>>\n";

	for(auto const& policy : conflict.meshline_policies)
		out += to_string(policy->id) + "_out -----[#red]-> " + id + "_in \n";

	if(solution)
		out += id + "_out -----[#green]-> " + to_string(solution->id) + "_in\n";

	out += "}\n";
}

//******************************************************************************
void SerializerToPlantuml::visit(MeshlinePolicy& policy) {
	auto id = to_string(policy.id);
	auto origin = policy.get_current_state().origins.size() == 1
	            ? dynamic_cast<Edge*>(policy.get_current_state().origins.front())
	            : nullptr;

	out +=
		"state \"MeshlinePolicy\" as " + id + (policy.get_current_state().is_enabled ? " #green" : " #red") + " {\n"
		"state \" \" as " + id + "_in <<inputPin>>\n"
		"state \" \" as " + id + "_out <<outputPin>>\n" +
		id + " : Normal = " + to_string(policy.normal) + "\n" +
		id + " : Is enabled = " + (policy.get_current_state().is_enabled ? "true" : "false") + "\n" +
		id + " : Policy = " + to_string(policy.policy) + "\n" +
		id + " : d = " + to_string(policy.get_current_state().d) + "\n";

	if(origin)
		out += to_string(origin->id) + " -----[#chartreuse]-> " + id + "_in\n";

	out += "}\n";
}

//******************************************************************************
void SerializerToPlantuml::visit(Interval& interval) {
	auto id = to_string(interval.id);

	out +=
		"state \"Interval\" as " + id + " {\n"
		"state \" \" as " + id + "_in1 <<inputPin>>\n"
		"state \" \" as " + id + "_in2 <<inputPin>>\n"
		"state \" \" as " + id + "_out <<outputPin>>\n" +
		id + " : dmax = " + to_string(interval.get_current_state().dmax) + "\n" +
		id + " : before.lmin = " + to_string(interval.get_current_state().before.lmin) + "\n" +
		id + " : before.lambda = " + to_string(interval.get_current_state().before.lambda) + "\n" +
		id + " : after.lmin = " + to_string(interval.get_current_state().after.lmin) + "\n" +
		id + " : after.lambda = " + to_string(interval.get_current_state().after.lambda) + "\n" +
		to_string(interval.get_current_state().before.meshline_policy->id) + "_out ------> " + id + "_in1 \n" +
		to_string(interval.get_current_state().after.meshline_policy->id) + "_out ------> " + id + "_in2 \n"
		"}\n";
}

//******************************************************************************
void SerializerToPlantuml::visit(Meshline& meshline) {
	auto id = to_string(meshline.id);

	out +=
		"state \"Meshline\" as " + id + " {\n" +
		id + " : Coord = " + to_string(meshline.coord.value()) + "\n";

	if(meshline.interval)
		out += to_string(meshline.interval->id) + "_out ------> " + id + "\n";
	if(meshline.policy)
		out += to_string(meshline.policy->id) + "_out ------> " + id + "\n";

	out += "}\n";
}

//******************************************************************************
string SerializerToPlantuml::dump() {
	return std::move(out);
}
