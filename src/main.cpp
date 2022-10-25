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
#include "infra/serializers/serializer_to_plantuml.hpp"
#include "infra/serializers/serializer_to_prettyprint.hpp"

using namespace std;

//******************************************************************************
Polygon from_box(string name, Point p1, Point p3) {
	return Polygon(name, {
		{ p1.x, p1.y },
		{ p1.x, p3.y },
		{ p3.x, p3.y },
		{ p3.x, p1.y }
	});
}
/*
initializer_list<Point> const& from_box(Point p1, Point p3) {
	return {
		{ p1.x, p1.y },
		{ p1.x, p3.y },
		{ p3.x, p3.y },
		{ p3.x, p1.y }
	};
}
*/

//******************************************************************************
void print(vector<unique_ptr<Meshline>> const& mesh_h, vector<unique_ptr<Meshline>> const& mesh_v) {
	cout << "\nH :" << endl;
	cout << "      <YLines>";
	for(size_t i = 0; i < mesh_h.size(); ++i) {
		cout << mesh_h[i]->coord.value();
		if(i != mesh_h.size()-1)
			cout << ",";
	}
	cout << "</YLines>" << endl;;
	cout << "\nV :" << endl;
	cout << "      <XLines>";
	for(size_t i = 0; i < mesh_v.size(); ++i) {
		cout << mesh_v[i]->coord.value();
		if(i != mesh_v.size()-1)
			cout << ",";
	}
	cout << "</XLines>" << endl;
}

//******************************************************************************
int main(int argc, char* argv[]) {
	if(argc != 3)
		return 1;

	if(string(argv[1]) == "lpf") {
		unique_ptr<Board> lpf = [] {
			vector<unique_ptr<Polygon>> tmp;
			tmp.push_back(make_unique<Polygon>(from_box("MS1", { 16.1, -26.5 }, { 20.6, -26 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS2", { 20.6, -36.5 }, { 22.1, -16 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS3", { 22.1, -26.5 }, { 40.1, -26 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS4", { 40.1, -36.5 }, { 42.6, -16 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS5", { 42.6, -26.5 }, { 60.6, -26 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS6", { 60.6, -36.5 }, { 62.1, -16 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS7", { 62.1, -26.5 }, { 66.6, -26 })));

			return make_unique<Board>(tmp);
		} ();

		lpf->params.lmin = 1;
		lpf->params.proximity_limit = 0;
//		lpf->params.dmax = 2;

		cout << "\nlpf :" << endl;
		lpf->auto_mesh();

		if(string(argv[2]) == "mesh") {
			auto const& lpf_mesh_h = lpf->get_meshlines(H);
			auto const& lpf_mesh_v = lpf->get_meshlines(V);
			print(lpf_mesh_h, lpf_mesh_v);
		} else if(string(argv[2]) == "policy") {
			auto const lpf_mesh_h = lpf->get_meshline_policies_meshlines(H);
			auto const lpf_mesh_v = lpf->get_meshline_policies_meshlines(V);
			print(lpf_mesh_h, lpf_mesh_v);
		}

		ofstream fout("dump.puml");
		fout << SerializerToPlantuml::run(*lpf);
		cerr << SerializerToPrettyprint::run(*lpf);

	} else if(string(argv[1]) == "stub") {

		unique_ptr<Board> stub = [] {
			vector<unique_ptr<Polygon>> tmp;
			tmp.push_back(make_unique<Polygon>(from_box("MS1", { 31.1, -44.8024 }, { 51.3539, -42.0164 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS2", { 120.029, -44.8024 }, { 140.283, -42.0164 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS3", { 51.6077, -42.0164 }, { 51.3539, -31 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS4", { 120.024, -42.0164 }, { 119.78, -31 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS5", { 109.208, -43.9276 }, { 119.775, -42.8913 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS6", { 51.6077, -43.9276 }, { 62.1753, -42.8913 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS7", { 70.3673, -42.8674 }, { 62.1753, -33.2952 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS8", { 109.208, -42.8674 }, { 101.016, -33.2952 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS9", { 90.96, -42.8674 }, { 80.4229, -33.1334 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS10", { 70.3673, -43.9514 }, { 80.4229, -42.8674 })));
			tmp.push_back(make_unique<Polygon>(from_box("MS11", { 90.96, -43.9514 }, { 101.016, -42.8674 })));
			tmp.push_back(make_unique<Polygon>(Polygon("MS12", {
				{ 51.6077, -42.0164 },
				{ 51.3539, -42.0164 },
				{ 51.3539, -44.8024 },
				{ 51.4808, -44.8024 },
				{ 51.4808, -43.9276 },
				{ 51.6077, -43.9276 }})));
			tmp.push_back(make_unique<Polygon>(Polygon("MS13", {
				{ 70.3673, -42.8674 },
				{ 62.1753, -42.8674 },
				{ 62.1753, -43.9276 },
				{ 66.2713, -43.9276 },
				{ 66.2713, -43.9514 },
				{ 70.3673, -43.9514 }})));
			tmp.push_back(make_unique<Polygon>(Polygon("MS14", {
				{ 90.96, -42.8674 },
				{ 80.4229, -42.8674 },
				{ 80.4229, -43.9514 },
				{ 85.6915, -43.9514 },
				{ 85.6915, -43.9514 },
				{ 90.96, -43.9514 }})));
			tmp.push_back(make_unique<Polygon>(Polygon("MS15", {
				{ 109.208, -42.8674 },
				{ 101.016, -42.8674 },
				{ 101.016, -43.9514 },
				{ 105.112, -43.9514 },
				{ 105.112, -43.9276 },
				{ 109.208, -43.9276 }})));
			tmp.push_back(make_unique<Polygon>(Polygon("MS16", {
				{ 120.029, -42.0164 },
				{ 119.775, -42.0164 },
				{ 119.775, -43.9276 },
				{ 119.902, -43.9276 },
				{ 119.902, -44.8024 },
				{ 120.029, -44.8024 }})));

			return make_unique<Board>(tmp);
		} ();

		stub->params.lmin = 0;
		stub->params.proximity_limit = 0.1;
//		stub->params.dmax = 2;

		cout << "\nstub :" << endl;
		stub->auto_mesh();

		if(string(argv[2]) == "mesh") {
			auto const& stub_mesh_h = stub->get_meshlines(H);
			auto const& stub_mesh_v = stub->get_meshlines(V);
			print(stub_mesh_h, stub_mesh_v);
		} else if(string(argv[2]) == "policy") {
			auto const stub_mesh_h = stub->get_meshline_policies_meshlines(H);
			auto const stub_mesh_v = stub->get_meshline_policies_meshlines(V);
			print(stub_mesh_h, stub_mesh_v);
		}

		ofstream fout("dump.puml");
		fout << SerializerToPlantuml::run(*stub);
		cerr << SerializerToPrettyprint::run(*stub);
	}

	return 0;
}
