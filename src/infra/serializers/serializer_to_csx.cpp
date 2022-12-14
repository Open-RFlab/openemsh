///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

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
	default: unreachable();
	}
}

//******************************************************************************
void SerializerToCsx::run(
		Board& board,
		filesystem::path const& input,
		filesystem::path const& output) {

	SerializerToCsx::run(board, input, output, {});
}

//******************************************************************************
void SerializerToCsx::run(
		Board& board,
		filesystem::path const& input,
		filesystem::path const& output,
		Params params) {

	SerializerToCsx serializer(input, output, std::move(params));
	board.accept(serializer);
}

//******************************************************************************
SerializerToCsx::SerializerToCsx(filesystem::path const& input, filesystem::path const& output, Params params)
: params(std::move(params))
, input(input)
, output(output)
{}

//******************************************************************************
void SerializerToCsx::visit(Board& board) {
	pugi::xml_document doc;
	pugi::xml_parse_result const res = doc.load_file(input.c_str());

	if(res.status != pugi::status_ok) {
		cerr << res.description() << endl;
		return;
	}

	pugi::xpath_node const grid = doc.select_node("/openEMS/ContinuousStructure/RectilinearGrid");

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

		cerr << "lines: " << out << endl << endl;
		grid.node().select_node(to_xml_node(axis).c_str())
		    .node().text().set(out.c_str());
	};

	if(params.with_axis_x)
		add_meshlines_to_xml_doc(X);
	if(params.with_axis_y)
		add_meshlines_to_xml_doc(Y);
	if(params.with_axis_z)
		add_meshlines_to_xml_doc(Z);

	cerr << "output: " << output << endl;
	doc.save_file(output.c_str());
}
