///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <memory>
#include <vector>
#include <iostream>
#include <fstream>

#include "domain/board.hpp"
#include "infra/parsers/parser_from_csx.hpp"
#include "infra/serializers/serializer_to_csx.hpp"
#include "infra/serializers/serializer_to_plantuml.hpp"
#include "infra/serializers/serializer_to_prettyprint.hpp"
#include "ui/cli.hpp"

using namespace domain;
using namespace std;
using namespace ui;

//******************************************************************************
void print(vector<unique_ptr<Meshline>> const& mesh_x, vector<unique_ptr<Meshline>> const& mesh_y) {
	cout << "\nH :" << endl;
	cout << "      <YLines>";
	for(size_t i = 0; i < mesh_y.size(); ++i) {
		cout << mesh_y[i]->coord.value();
		if(i != mesh_y.size()-1)
			cout << ",";
	}
	cout << "</YLines>" << endl;;
	cout << "\nV :" << endl;
	cout << "      <XLines>";
	for(size_t i = 0; i < mesh_x.size(); ++i) {
		cout << mesh_x[i]->coord.value();
		if(i != mesh_x.size()-1)
			cout << ",";
	}
	cout << "</XLines>" << endl;
}

//******************************************************************************
int main(int argc, char* argv[]) {
	optional<CliParams> const params = cli(argc, argv);
	if(!params)
		return 1;

	if(argc != 3)
		return 1;

	if(string(argv[1]) == "lpf") {
		unique_ptr<Board> lpf = [] {
			using Type = domain::Polygon::Type;

			Board::Builder builder;
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS1", { 16.1, -26.5 }, { 20.6, -26 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS2", { 20.6, -36.5 }, { 22.1, -16 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS3", { 22.1, -26.5 }, { 40.1, -26 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS4", { 40.1, -36.5 }, { 42.6, -16 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS5", { 42.6, -26.5 }, { 60.6, -26 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS6", { 60.6, -36.5 }, { 62.1, -16 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS7", { 62.1, -26.5 }, { 66.6, -26 });

			return builder.build();
		} ();

		lpf->params.lmin = 1;
		lpf->params.proximity_limit = 0;
//		lpf->params.dmax = 2;

		cout << "\nlpf :" << endl;
		lpf->auto_mesh();

		if(string(argv[2]) == "mesh") {
			auto const& lpf_mesh_y = lpf->get_meshlines(Y);
			auto const& lpf_mesh_x = lpf->get_meshlines(X);
			print(lpf_mesh_x, lpf_mesh_y);
		} else if(string(argv[2]) == "policy") {
			auto const lpf_mesh_y = lpf->get_meshline_policies_meshlines(Y);
			auto const lpf_mesh_x = lpf->get_meshline_policies_meshlines(X);
			print(lpf_mesh_x, lpf_mesh_y);
		}

		ofstream fout("dump.puml");
		fout << SerializerToPlantuml::run(*lpf);
		cerr << SerializerToPrettyprint::run(*lpf);

	} else if(string(argv[1]) == "stub") {

		unique_ptr<Board> stub = [] {
			using Type = domain::Polygon::Type;

			Board::Builder builder;
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS1", { 31.1, -44.8024 }, { 51.3539, -42.0164 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS2", { 120.029, -44.8024 }, { 140.283, -42.0164 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS3", { 51.6077, -42.0164 }, { 51.3539, -31 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS4", { 120.024, -42.0164 }, { 119.78, -31 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS5", { 109.208, -43.9276 }, { 119.775, -42.8913 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS6", { 51.6077, -43.9276 }, { 62.1753, -42.8913 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS7", { 70.3673, -42.8674 }, { 62.1753, -33.2952 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS8", { 109.208, -42.8674 }, { 101.016, -33.2952 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS9", { 90.96, -42.8674 }, { 80.4229, -33.1334 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS10", { 70.3673, -43.9514 }, { 80.4229, -42.8674 });
			builder.add_polygon_from_box(XY, Type::SHAPE, "MS11", { 90.96, -43.9514 }, { 101.016, -42.8674 });
			builder.add_polygon(XY, Type::SHAPE, "MS12", {
				{ 51.6077, -42.0164 },
				{ 51.3539, -42.0164 },
				{ 51.3539, -44.8024 },
				{ 51.4808, -44.8024 },
				{ 51.4808, -43.9276 },
				{ 51.6077, -43.9276 }});
			builder.add_polygon(XY, Type::SHAPE, "MS13", {
				{ 70.3673, -42.8674 },
				{ 62.1753, -42.8674 },
				{ 62.1753, -43.9276 },
				{ 66.2713, -43.9276 },
				{ 66.2713, -43.9514 },
				{ 70.3673, -43.9514 }});
			builder.add_polygon(XY, Type::SHAPE, "MS14", {
				{ 90.96, -42.8674 },
				{ 80.4229, -42.8674 },
				{ 80.4229, -43.9514 },
				{ 85.6915, -43.9514 },
				{ 85.6915, -43.9514 },
				{ 90.96, -43.9514 }});
			builder.add_polygon(XY, Type::SHAPE, "MS15", {
				{ 109.208, -42.8674 },
				{ 101.016, -42.8674 },
				{ 101.016, -43.9514 },
				{ 105.112, -43.9514 },
				{ 105.112, -43.9276 },
				{ 109.208, -43.9276 }});
			builder.add_polygon(XY, Type::SHAPE, "MS16", {
				{ 120.029, -42.0164 },
				{ 119.775, -42.0164 },
				{ 119.775, -43.9276 },
				{ 119.902, -43.9276 },
				{ 119.902, -44.8024 },
				{ 120.029, -44.8024 }});

			return builder.build();
		} ();

		stub->params.lmin = 0;
		stub->params.proximity_limit = 0.1;
//		stub->params.dmax = 2;

		cout << "\nstub :" << endl;
		stub->auto_mesh();

		if(string(argv[2]) == "mesh") {
			auto const& stub_mesh_y = stub->get_meshlines(Y);
			auto const& stub_mesh_x = stub->get_meshlines(X);
			print(stub_mesh_x, stub_mesh_y);
		} else if(string(argv[2]) == "policy") {
			auto const stub_mesh_y = stub->get_meshline_policies_meshlines(Y);
			auto const stub_mesh_x = stub->get_meshline_policies_meshlines(X);
			print(stub_mesh_x, stub_mesh_y);
		}

		ofstream fout("dump.puml");
		fout << SerializerToPlantuml::run(*stub);
		cerr << SerializerToPrettyprint::run(*stub);
	}

	return 0;
}
