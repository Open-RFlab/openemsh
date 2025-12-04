///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <pugixml.hpp>

#include "domain/mesh/meshline.hpp"
#include "domain/board.hpp"
#include "utils/unreachable.hpp"

#include "serializer_to_csx.hpp"

using namespace domain;
using namespace std;

//******************************************************************************
string to_xml_node(Axis const axis) noexcept {
	switch(axis) {
	case X: return "XLines";
	case Y: return "YLines";
	case Z: return "ZLines";
	default: ::unreachable();
	}
}

//******************************************************************************
expected<void, string> SerializerToCsx::run(
		Board& board,
		filesystem::path const& input,
		filesystem::path const& output) {

	return SerializerToCsx::run(board, input, output, {});
}

//******************************************************************************
expected<void, string> SerializerToCsx::run(
		Board& board,
		filesystem::path const& input,
		filesystem::path const& output,
		Params params) {

	SerializerToCsx serializer(input, output, std::move(params));
	board.accept(serializer);

	if(serializer.error)
		return unexpected(serializer.error.value());
	return {};
}

//******************************************************************************
SerializerToCsx::SerializerToCsx(filesystem::path const& input, filesystem::path const& output, Params params)
: params(std::move(params))
, input(input)
, output(output)
{}

//******************************************************************************
pugi::xml_node find_or_prepend_child(pugi::xml_node& node, char const* child) {
	if(pugi::xml_node c = node.child(child); c.empty())
		return node.prepend_child(child);
	else
		return c;
}

//******************************************************************************
pugi::xml_node find_or_append_child(pugi::xml_node& node, char const* child) {
	if(pugi::xml_node c = node.child(child); c.empty())
		return node.append_child(child);
	else
		return c;
}

//******************************************************************************
void SerializerToCsx::visit(Board& board) {
	pugi::xml_document doc;
	pugi::xml_parse_result const res = doc.load_file(input.native().c_str());

	if(res.status != pugi::status_ok) {
		error = res.description();
		return;
	}

	pugi::xml_node oems = find_or_append_child(doc, "openEMS");
	pugi::xml_node csx = find_or_append_child(oems, "ContinuousStructure");
	pugi::xml_node grid = find_or_append_child(csx, "RectilinearGrid");
	grid.remove_children();

	auto const add_meshlines_to_xml_doc = [this, &grid, &board](Axis const axis) {
		string out;
		if(params.with_meshlines)
			for(auto const& meshline : board.get_meshlines(axis))
				out += to_string(meshline->coord.value()) + ",";
		if(params.with_meshline_policies)
			for(auto const& meshline : board.get_meshline_policies_meshlines(axis))
				out += to_string(meshline->coord.value()) + ",";

		if(!out.empty())
			out.pop_back();

		grid.append_child(to_xml_node(axis).c_str()).text().set(out.c_str());
	};

	if(params.with_axis_x)
		add_meshlines_to_xml_doc(X);
	if(params.with_axis_y)
		add_meshlines_to_xml_doc(Y);
	if(params.with_axis_z)
		add_meshlines_to_xml_doc(Z);

	if(params.with_oemsh_params) {
		auto const& p = board.global_params->get_current_state();
		pugi::xml_node oemsh = find_or_prepend_child(doc, "OpenEMSH");
		pugi::xml_node global_params = find_or_append_child(oemsh, "GlobalParams");
		global_params.remove_attributes();
		global_params.append_attribute("ProximityLimit").set_value(p.proximity_limit);
		global_params.append_attribute("Smoothness").set_value(p.smoothness);
		global_params.append_attribute("dmax").set_value(p.dmax);
		global_params.append_attribute("lmin").set_value(p.lmin);
	} else {
		doc.remove_child("OpenEMSH");
	}

	doc.save_file(output.native().c_str(), "  ");
}
