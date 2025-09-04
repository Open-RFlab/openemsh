///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <map>
#include <iostream>

#include <pugixml.hpp>

#include "domain/geometrics/bounding.hpp"
#include "domain/geometrics/polygon.hpp"
#include "domain/geometrics/space.hpp"
#include "domain/board.hpp"
#include "csxcad_layer/point_3d.hpp"

#include "parser_from_csx.hpp"
#include "utils/unreachable.hpp"
#include "utils/vector_utils.hpp"

using namespace domain;
using namespace std;

//******************************************************************************
constexpr optional<Axis> to_axis(size_t const normdir) {
	switch(normdir) {
	case 0: return X;
	case 1: return Y;
	case 2: return Z;
	default: return nullopt;
	}
}

//******************************************************************************
constexpr optional<Plane> to_plane(size_t const normdir) {
	switch(normdir) {
	case 0: return YZ;
	case 1: return ZX;
	case 2: return XY;
	default: return nullopt;
	}
}

//******************************************************************************
class ParserFromCsx::Pimpl {
public:
	ParserFromCsx::Params const& params;
	std::map<pugi::xml_node const, size_t> primitives_ids;
	Board::Builder board;

	Pimpl(ParserFromCsx::Params const& params);

	void parse_property(pugi::xml_node const& node);

	void parse_primitive(pugi::xml_node const& node);
	void parse_primitive_box(pugi::xml_node const& node, std::string name);
	void parse_primitive_linpoly(pugi::xml_node const& node, std::string name);

private:
	optional<Polygon::Type> which_material_is(pugi::xml_node const& primitive_node);
};

//******************************************************************************
ParserFromCsx::Pimpl::Pimpl(ParserFromCsx::Params const& params)
: params(params)
{}

//******************************************************************************
void ParserFromCsx::Pimpl::parse_property(pugi::xml_node const& node) {
	cerr << node.name() << endl;

	// TODO try to guess kind TRACK GROUND SUBSTRATE to disable EIP detection
	string name(node.attribute("Name").as_string());

	if(node.name() == "Material"s) {
		pugi::xml_node property = node.child("Property");
		double epsilon = node.attribute("Epsilon").as_double(); // TODO optional
		double mue = node.attribute("Mue").as_double(); // TODO optional
		double kappa = node.attribute("Kappa").as_double(); // TODO optional
		double sigma = node.attribute("Sigma").as_double(); // TODO optional
	} else if(node.name() == "Metal"s) {
	} else if(node.name() == "ConductingSheet"s) {
		double conductivity = node.attribute("Conductivity").as_double();
		double thickness = node.attribute("Thickness").as_double();
	} else if(node.name() == "LumpedElement"s) {
	} else if(node.name() == "Excitation"s) {
	} else if(node.name() == "ProbeBox"s) {
	} else if(node.name() == "DumpBox"s) {
	} else if(node.name() == "DebyeMaterial"s) {
	} else if(node.name() == "LorentzMaterial"s) {
	} else if(node.name() == "ResBox"s) {
	} else if(node.name() == "Unknown"s) {
	} else if(node.name() == "DiscMaterial"s) {
	}
}

//******************************************************************************
void ParserFromCsx::Pimpl::parse_primitive(pugi::xml_node const& node) {
	cerr << node.name() << endl;

	string property_name(node.parent().parent().attribute("Name").as_string());
	string name(property_name + "::" + to_string(primitives_ids.at(node)));

	using pugi::char_t;
	if(node.name() == "Box"s)
		parse_primitive_box(node, name);
	else if(node.name() == "LinPoly"s)
		parse_primitive_linpoly(node, name);
	else if(node.name() == "Polyhedron"s) {}
	else if(node.name() == "Polygon"s) {}
	else if(node.name() == "RotPoly"s) {}
	else if(node.name() == "Sphere"s) {}
	else if(node.name() == "Cylinder"s) {}
	else if(node.name() == "Wire"s) {}
	else if(node.name() == "CylindricalShell"s) {}
	else if(node.name() == "User-Defined"s) {}
	else if(node.name() == "Curve"s) {}

}

//******************************************************************************
optional<Polygon::Type> ParserFromCsx::Pimpl::which_material_is(pugi::xml_node const& primitive_node) {
	string property(primitive_node.parent().parent().name());
	string property_name(primitive_node.parent().parent().attribute("Name").as_string());

	if(contains(params.grounds, property_name)) {
		return Polygon::Type::GROUND;
	} else {
		if(property == "Material") {
			return Polygon::Type::SUBSTRATE;
		} else if(property == "Metal"
			|| property == "ConductingSheet") {
			return Polygon::Type::SHAPE;
		} else if(property == "LumpedElement") {
		} else if(property == "Excitation") {
		} else if(property == "ProbeBox") {
		} else if(property == "DumpBox") {
		} else if(property == "DebyeMaterial") {
		} else if(property == "LorentzMaterial") {
		} else if(property == "ResBox") {
		} else if(property == "Unknown") {
		} else if(property == "DiscMaterial") {
		}
	}

	return nullopt;
}

//******************************************************************************
void ParserFromCsx::Pimpl::parse_primitive_box(pugi::xml_node const& node, string name) {
	auto const type = which_material_is(node);
//	if(!type.has_value() || type.value() != Polygon::Type::SHAPE) // TODO
	if(!type.has_value() || type.value() == Polygon::Type::SUBSTRATE) // TODO
		return;

	pugi::xml_node node_p1 = node.child("P1");
	pugi::xml_node node_p2 = node.child("P2");
	Point3D p1(
		node_p1.attribute("X").as_double(),
		node_p1.attribute("Y").as_double(),
		node_p1.attribute("Z").as_double());
	Point3D p2(
		node_p2.attribute("X").as_double(),
		node_p2.attribute("Y").as_double(),
		node_p2.attribute("Z").as_double());
	if(params.with_yz)
		board.add_polygon_from_box(YZ, type.value(), name, { p1.x, p2.x }, { p1.y, p1.z }, { p2.y, p2.z });
	if(params.with_zx)
		board.add_polygon_from_box(ZX, type.value(), name, { p1.y, p2.y }, { p1.z, p1.x }, { p2.z, p2.x });
	if(params.with_xy)
		board.add_polygon_from_box(XY, type.value(), name, { p1.z, p2.z }, { p1.x, p1.y }, { p2.x, p2.y });
	// TODO if property == ConductingSheet : add_fixed_meshline_policy()
}

//******************************************************************************
void ParserFromCsx::Pimpl::parse_primitive_linpoly(pugi::xml_node const& node, string name) {
	auto const type = which_material_is(node);
//	if(!type.has_value() || type.value() != Polygon::Type::SHAPE) // TODO
	if(!type.has_value() || type.value() == Polygon::Type::SUBSTRATE) // TODO
		return;

	double elevation = node.attribute("Elevation").as_double(); // offset in normdir
	double length = node.attribute("Length").as_double(); // height in normdir
	size_t normdir = node.attribute("NormDir").as_uint(); // (0->x, 1->y, 2->z)
	optional<Plane> plane = to_plane(normdir);
	optional<Axis> normal = to_axis(normdir);
	if(!plane || !normal)
		return;

	vector<unique_ptr<Point const>> points;
	for(auto const& vertex : node.children("Vertex"))
		points.push_back(make_unique<Point const>(
			vertex.attribute("X1").as_double(),
			vertex.attribute("X2").as_double()));

	Bounding2D bounding(detect_bounding(points));

	board.add_polygon(plane.value(), type.value(), name, { elevation, elevation + length }, std::move(points));
	if(length == 0) {
		board.add_fixed_meshline_policy(normal.value(), elevation);
//		switch(plane.value()) {
//		case YZ:
//			board.add_fixed_meshline_policy(Y, elevation);
//			board.add_fixed_meshline_policy(Z, elevation);
//			break;
//		case ZX:
//			board.add_fixed_meshline_policy(Z, elevation);
//			board.add_fixed_meshline_policy(X, elevation);
//			break;
//		case XY:
//			board.add_fixed_meshline_policy(X, elevation);
//			board.add_fixed_meshline_policy(Y, elevation);
//			break;
//		default:
//			unreachable();
//		}
	} else {
/*
		switch(plane.value()) {
		case YZ:
			if(params.with_zx)
				board.add_polygon_from_box(ZX, name,
					{ bounding[YMIN], elevation },
					{ bounding[YMAX], elevation + length });
			if(params.with_xy)
				board.add_polygon_from_box(XY, name,
					{ elevation, bounding[XMIN] },
					{ elevation + length, bounding[XMAX] });
			break;
		case ZX:
			if(params.with_xy)
				board.add_polygon_from_box(XY, name,
					{ bounding[YMIN], elevation },
					{ bounding[YMAX], elevation + length });
			if(params.with_yz)
				board.add_polygon_from_box(YZ, name,
					{ elevation, bounding[XMIN] },
					{ elevation + length, bounding[XMAX] });
			break;
		case XY:
			if(params.with_yz)
				board.add_polygon_from_box(YZ, name,
					{ bounding[YMIN], elevation },
					{ bounding[YMAX], elevation + length });
			if(params.with_zx)
				board.add_polygon_from_box(ZX, name,
					{ elevation, bounding[XMIN] },
					{ elevation + length, bounding[XMAX] });
			break;
		default:
			unreachable();
		}
*/
	}
}

//******************************************************************************
shared_ptr<Board> ParserFromCsx::run(std::filesystem::path const& input) {
	return ParserFromCsx::run(input, {});
}

//******************************************************************************
shared_ptr<Board> ParserFromCsx::run(std::filesystem::path const& input, Params params) {
	ParserFromCsx parser(input, std::move(params));
	parser.parse();
	return parser.output();
}

//******************************************************************************
shared_ptr<Board> ParserFromCsx::run(std::filesystem::path const& input, Params params, std::function<void (domain::Params&)> const& override_domain_params) {
	ParserFromCsx parser(input, std::move(params));
	parser.parse();
	override_domain_params(parser.domain_params);
	return parser.output();
}

//******************************************************************************
ParserFromCsx::ParserFromCsx(filesystem::path const& input)
: input(input)
, pimpl(make_unique<Pimpl>(parser_params))
{}

//******************************************************************************
ParserFromCsx::ParserFromCsx(filesystem::path const& input, Params params)
: input(input)
, parser_params(std::move(params))
, pimpl(make_unique<Pimpl>(parser_params))
{}

//******************************************************************************
ParserFromCsx::~ParserFromCsx() = default;

//******************************************************************************
void ParserFromCsx::parse() {
	pugi::xml_document doc;
	pugi::xml_parse_result res = doc.load_file(input.native().c_str());

	if(res.status != pugi::status_ok) {
		cerr << res.description() << endl;
		return;
	}

	pugi::xpath_node fdtd = doc.select_node("/openEMS/FDTD");

	{
		// Primitives' IDs grow disregarding properties.
		size_t id = 0;
		pugi::xpath_node_set primitives = doc.select_nodes("/openEMS/ContinuousStructure/Properties/*/Primitives");
		for(auto const& primitive : primitives)
			for(auto const& node : primitive.node().children()) {
				pimpl->primitives_ids.emplace(node, id++);
				cerr << node.name() << endl;
			}
	}

	pugi::xpath_node properties = doc.select_node("/openEMS/ContinuousStructure/Properties");
	for(auto const& node : properties.node().children()) {
		pimpl->parse_property(node);

		pugi::xml_node primitives = node.child("Primitives");
		for(auto const& node : primitives.children())
			pimpl->parse_primitive(node);

	}
}

//******************************************************************************
shared_ptr<Board> ParserFromCsx::output() {
	pimpl->primitives_ids.clear();
	return pimpl->board.build(std::move(domain_params));
}
