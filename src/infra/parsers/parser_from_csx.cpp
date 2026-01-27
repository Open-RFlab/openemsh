///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <exception>
#include <format>
#include <map>
#include <optional>
#include <ranges>
#include <set>
#include <string_view>

#include <pugixml.hpp>

#include "domain/material.hpp"
#include "domain/geometrics/bounding.hpp"
#include "domain/geometrics/polygon.hpp"
#include "domain/geometrics/space.hpp"
#include "domain/board.hpp"
#include "csxcad_layer/point_3d.hpp"

#include "parser_from_csx.hpp"
#include "utils/expected_utils.hpp"
#include "utils/logger.hpp"
#include "utils/progress.hpp"
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
	domain::Params domain_params;

	set<string> warning_unsupported_primitives_types;
	set<string> warning_unsupported_primitives_names;

	Pimpl(ParserFromCsx::Params const& params);

	expected<void, string> parse_oemsh(pugi::xml_node const& node);
	expected<void, string> parse_grid(pugi::xml_node const& node);

	shared_ptr<Material> parse_property(pugi::xml_node const& node);

	bool parse_primitive(pugi::xml_node const& node, shared_ptr<Material> const& material);
	void parse_primitive_box(pugi::xml_node const& node, shared_ptr<Material> const& material, std::string name);
	void parse_primitive_linpoly(pugi::xml_node const& node, shared_ptr<Material> const& material, std::string name);
	void parse_primitive_polygon(pugi::xml_node const& node, shared_ptr<Material> const& material, std::string name);
	void parse_primitive_shpere(pugi::xml_node const& node, shared_ptr<Material> const& material, std::string name);
	void parse_primitive_cylinder(pugi::xml_node const& node, shared_ptr<Material> const& material, std::string name);
	void parse_primitive_point(pugi::xml_node const& node, shared_ptr<Material> const& material, std::string name);

private:
	void warn_unsupported(string const& primitive_type, string const& primitive_name);
};

//******************************************************************************
ParserFromCsx::Pimpl::Pimpl(ParserFromCsx::Params const& params)
: params(params)
{}

//******************************************************************************
void ParserFromCsx::Pimpl::warn_unsupported(string const& primitive_type, string const& primitive_name) {
	warning_unsupported_primitives_types.emplace(primitive_type);
	warning_unsupported_primitives_names.emplace(primitive_name);
}

//******************************************************************************
expected<void, string> ParserFromCsx::Pimpl::parse_oemsh(pugi::xml_node const& node) {
	pugi::xml_node global_params = node.child("GlobalParams");
	if(auto a = global_params.attribute("ProximityLimit"); a) domain_params.proximity_limit = a.as_double();
	if(auto a = global_params.attribute("Smoothness"); a) domain_params.smoothness = a.as_double();
	if(auto a = global_params.attribute("dmax"); a) domain_params.dmax = a.as_double();
	if(auto a = global_params.attribute("lmin"); a) domain_params.lmin = a.as_uint();

	pugi::xml_node fixed_meshlines = node.child("FixedMeshlines");
	size_t delta_unit = fixed_meshlines.attribute("DeltaUnit").as_uint(1);
	AxisSpace<string_view> lines = {
		fixed_meshlines.child_value("XLines"),
		fixed_meshlines.child_value("YLines"),
		fixed_meshlines.child_value("ZLines")
	};
	for(Axis axis : AllAxis) {
		for(auto const part : views::split(lines[axis], ',')) {
			try {
				board.add_fixed_meshline_policy(axis, delta_unit * stod(string(string_view(part))));
			} catch(exception const& e) {
				return unexpected("Invalid meshline value"s + e.what());
			}
		}
	}
	return {};
}

//******************************************************************************
expected<void, string> ParserFromCsx::Pimpl::parse_grid(pugi::xml_node const& node) {
	std::size_t coord_system = node.attribute("CoordSystem").as_uint();
	std::size_t delta_unit = node.attribute("DeltaUnit").as_uint(1);

	if(coord_system == 0) {
		// First step : into bool has_grid_already
		pugi::xml_node grid = node.child("RectilinearGrid");
		AxisSpace<string_view> lines = {
			grid.child_value("XLines"),
			grid.child_value("YLines"),
			grid.child_value("ZLines")
		};
		domain_params.has_grid_already = ranges::any_of(lines, [](auto const& str){ return !str.empty(); });
		// TODO Second step : into fixed MLP
		if(params.keep_old_mesh) {
			for(Axis axis : AllAxis) {
				for(auto const part : views::split(lines[axis], ',')) {
					try {
						board.add_fixed_meshline_policy(axis, delta_unit * stod(string(string_view(part))));
					} catch(exception const& e) {
						return unexpected("Invalid meshline value"s + e.what());
					}
				}
			}
		}
		// TODO Third step : into vizualisable set of meshlines for comparison
//	} else if(coord_system == 1) {
	} else {
		return unexpected("Unsupported CoordSystem");
	}
	return {};
}

//******************************************************************************
shared_ptr<Material> ParserFromCsx::Pimpl::parse_property(pugi::xml_node const& node) {
	string name(node.attribute("Name").as_string());

	auto const parse_color = [](pugi::xml_node const& node) -> optional<Material::Color> {
		if(node)
			return Material::Color {
				static_cast<unsigned char>(node.attribute("R").as_uint()),
				static_cast<unsigned char>(node.attribute("G").as_uint()),
				static_cast<unsigned char>(node.attribute("B").as_uint()),
				static_cast<unsigned char>(node.attribute("a").as_uint())
			};
		else
			return nullopt;
	};

	auto const parse_material_property = [](pugi::xml_node const& node) -> array<double, 4> {
		return {
			node.attribute("Epsilon").as_double(Material::default_epsilon),
			node.attribute("Mue").as_double(Material::default_mue),
			node.attribute("Kappa").as_double(Material::default_kappa),
			node.attribute("Sigma").as_double(Material::default_sigma)
		};
	};

	if(node.name() == "BackgroundMaterial"s) {
		auto [epsilon, mue, kappa, sigma] = parse_material_property(node);
		return make_shared<Material>(Material::deduce_type(epsilon, mue, kappa), "BackgroundMaterial");
	} else {
		auto fill = parse_color(node.child("FillColor"));
		auto edge = parse_color(node.child("EdgeColor"));

		if(node.name() == "Material"s) {
			// https://github.com/thliebig/openEMS-Project/discussions/347
			// Currently do not take care of Isotropy=false
			// as_double() selects the first term and ditch the part after
			bool isotropy = node.attribute("Isotropy").as_bool();
			auto [epsilon, mue, kappa, sigma] = parse_material_property(node.child("Property"));
			return make_shared<Material>(Material::deduce_type(epsilon, mue, kappa), name, fill, edge);
		} else if(node.name() == "Metal"s) {
			return make_shared<Material>(Material::Type::CONDUCTOR, name, fill, edge);
		} else if(node.name() == "ConductingSheet"s) {
			double conductivity = node.attribute("Conductivity").as_double();
			double thickness = node.attribute("Thickness").as_double();
			return make_shared<Material>(Material::Type::CONDUCTOR, name, fill, edge);
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

	return shared_ptr<Material>();
}

//******************************************************************************
bool ParserFromCsx::Pimpl::parse_primitive(pugi::xml_node const& node, shared_ptr<Material> const& material) {

	string property_name(node.parent().parent().attribute("Name").as_string());
	string name(property_name + "::" + to_string(primitives_ids.at(node)));

	if(node.child("Transformation")) {
		warn_unsupported(format("Transformed {}", node.name()), name);
		return false;
	}

	using pugi::char_t;
	if(node.name() == "Box"s) {
		parse_primitive_box(node, material, name);
		return true;
	} else if(node.name() == "LinPoly"s) {
		parse_primitive_linpoly(node, material, name);
		return true;
	} else if(node.name() == "Polygon"s) {
		parse_primitive_polygon(node, material, name);
		return true;
	} else if(node.name() == "Sphere"s) {
		parse_primitive_shpere(node, material, name);
		return true;
	} else if(node.name() == "Cylinder"s) {
		parse_primitive_cylinder(node, material, name);
		return true;
	} else if(node.name() == "Point"s) {
		parse_primitive_point(node, material, name);
		return true;
	} else if(node.name() == "Polyhedron"s
	       || node.name() == "PolyhedronReader"s
	       || node.name() == "RotPoly"s
	       || node.name() == "SphericalShell"s
	       || node.name() == "CylindricalShell"s
	       || node.name() == "Curve"s
	       || node.name() == "Wire"s
	       || node.name() == "MultiBox"s
	       || node.name() == "User-Defined"s) {
		warn_unsupported(node.name(), name);
	}
	return false;
}

//******************************************************************************
void ParserFromCsx::Pimpl::parse_primitive_box(pugi::xml_node const& node, shared_ptr<Material> const& material, string name) {
	size_t priority = node.attribute("Priority").as_uint();
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
		board.add_polygon_from_box(YZ, material, name, priority, { p1.x, p2.x }, { p1.y, p1.z }, { p2.y, p2.z });
	if(params.with_zx)
		board.add_polygon_from_box(ZX, material, name, priority, { p1.y, p2.y }, { p1.z, p1.x }, { p2.z, p2.x });
	if(params.with_xy)
		board.add_polygon_from_box(XY, material, name, priority, { p1.z, p2.z }, { p1.x, p1.y }, { p2.x, p2.y });
	// TODO if property == ConductingSheet : add_fixed_meshline_policy()
}

//******************************************************************************
void ParserFromCsx::Pimpl::parse_primitive_linpoly(pugi::xml_node const& node, shared_ptr<Material> const& material, string name) {
	size_t priority = node.attribute("Priority").as_uint();
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

	board.add_polygon(plane.value(), material, name, priority, { elevation, elevation + length }, std::move(points));
	if(length == 0) {
//		board.add_fixed_meshline_policy(normal.value(), elevation);
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
//			::unreachable();
//		}
	} else {
		switch(plane.value()) {
		case YZ:
			if(params.with_zx)
				board.add_polygon_from_box(ZX, material, name, priority,
					{ bounding[XMIN], bounding[XMAX] },
					{ bounding[YMIN], elevation },
					{ bounding[YMAX], elevation + length });
			if(params.with_xy)
				board.add_polygon_from_box(XY, material, name, priority,
					{ bounding[YMIN], bounding[YMAX] },
					{ elevation, bounding[XMIN] },
					{ elevation + length, bounding[XMAX] });
			break;
		case ZX:
			if(params.with_xy)
				board.add_polygon_from_box(XY, material, name, priority,
					{ bounding[XMIN], bounding[XMAX] },
					{ bounding[YMIN], elevation },
					{ bounding[YMAX], elevation + length });
			if(params.with_yz)
				board.add_polygon_from_box(YZ, material, name, priority,
					{ bounding[YMIN], bounding[YMAX] },
					{ elevation, bounding[XMIN] },
					{ elevation + length, bounding[XMAX] });
			break;
		case XY:
			if(params.with_yz)
				board.add_polygon_from_box(YZ, material, name, priority,
					{ bounding[XMIN], bounding[XMAX] },
					{ bounding[YMIN], elevation },
					{ bounding[YMAX], elevation + length });
			if(params.with_zx)
				board.add_polygon_from_box(ZX, material, name, priority,
					{ bounding[YMIN], bounding[YMAX] },
					{ elevation, bounding[XMIN] },
					{ elevation + length, bounding[XMAX] });
			break;
		default:
			::unreachable();
		}
	}
}

//******************************************************************************
void ParserFromCsx::Pimpl::parse_primitive_polygon(pugi::xml_node const& node, shared_ptr<Material> const& material, string name) {
	size_t priority = node.attribute("Priority").as_uint();
	double elevation = node.attribute("Elevation").as_double(); // offset in normdir
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

	board.add_polygon(plane.value(), material, name, priority, { elevation, elevation }, std::move(points));
	board.add_fixed_meshline_policy(normal.value(), elevation);
}

//******************************************************************************
void ParserFromCsx::Pimpl::parse_primitive_shpere(pugi::xml_node const& node, shared_ptr<Material> const& material, std::string name) {
	size_t priority = node.attribute("Priority").as_uint();
	double radius = node.attribute("Radius").as_double();
	pugi::xml_node node_center = node.child("Center");
	Point3D center(
		node_center.attribute("X").as_double(),
		node_center.attribute("Y").as_double(),
		node_center.attribute("Z").as_double());

	// TODO Z placement here is the bounding box
	if(params.with_yz)
		board.add_polygon(YZ, material, name, priority, { center.x - radius, center.x + radius }, circle_to_points({ center.y, center.z }, radius));
	if(params.with_zx)
		board.add_polygon(ZX, material, name, priority, { center.y - radius, center.y + radius }, circle_to_points({ center.z, center.x }, radius));
	if(params.with_xy)
		board.add_polygon(XY, material, name, priority, { center.z - radius, center.z + radius }, circle_to_points({ center.x, center.y }, radius));
}

//******************************************************************************
void ParserFromCsx::Pimpl::parse_primitive_cylinder(pugi::xml_node const& node, shared_ptr<Material> const& material, std::string name) {
	size_t priority = node.attribute("Priority").as_uint();
	double radius = node.attribute("Radius").as_double();
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

	// If orthogonal circle + polygons.
	if(p1.y == p2.y && p1.z == p2.z) {
		if(params.with_yz)
			board.add_polygon(YZ, material, name, priority, { p1.x, p2.x }, circle_to_points({ p1.y, p1.z }, radius));
		if(params.with_zx)
			// TODO Z placement here is the bounding box
			board.add_polygon_from_box(ZX, material, name, priority,
				{ p1.y, p2.y },
				{ p1.z - radius/2, p1.x },
				{ p2.z + radius/2, p2.x });
		if(params.with_xy)
			// TODO Z placement here is the bounding box
			board.add_polygon_from_box(XY, material, name, priority,
				{ p1.z, p2.z },
				{ p1.x, p1.y - radius/2 },
				{ p2.x, p2.y + radius/2 });
	} else if(p1.z == p2.z && p1.x == p2.x) {
		if(params.with_zx)
			board.add_polygon(ZX, material, name, priority, { p1.y, p2.y }, circle_to_points({ p1.z, p1.x }, radius));
		if(params.with_xy)
			// TODO Z placement here is the bounding box
			board.add_polygon_from_box(XY, material, name, priority,
				{ p1.z, p2.z },
				{ p1.x - radius/2, p1.y },
				{ p2.x + radius/2, p2.y });
		if(params.with_yz)
			// TODO Z placement here is the bounding box
			board.add_polygon_from_box(YZ, material, name, priority,
				{ p1.x, p2.x },
				{ p1.y, p1.z - radius/2 },
				{ p2.y, p2.z + radius/2 });
	} else if(p1.x == p2.x && p1.y == p2.y) {
		if(params.with_xy)
			board.add_polygon(XY, material, name, priority, { p1.z, p2.z }, circle_to_points({ p1.x, p1.y }, radius));
		if(params.with_yz)
			// TODO Z placement here is the bounding box
			board.add_polygon_from_box(YZ, material, name, priority,
				{ p1.x, p2.x },
				{ p1.y - radius/2, p1.z },
				{ p2.y + radius/2, p2.z });
		if(params.with_zx)
			// TODO Z placement here is the bounding box
			board.add_polygon_from_box(ZX, material, name, priority,
				{ p1.y, p2.y },
				{ p1.z, p1.x - radius/2 },
				{ p2.z, p2.x + radius/2 });
	} else {
		warn_unsupported(format("{} (with diagonal axis)", node.name()), name);
	}
}

//******************************************************************************
void ParserFromCsx::Pimpl::parse_primitive_point(pugi::xml_node const& node, shared_ptr<Material> const& material, std::string name) {
//	size_t priority = node.attribute("Priority").as_uint();
	Point3D p(
		node.attribute("X").as_double(),
		node.attribute("Y").as_double(),
		node.attribute("Z").as_double());

	// TODO board.add_point()
	if(params.with_yz) {
//		board.add_point(YZ, material, name, /*priority,*/ { p.y, p.z });
		board.add_fixed_meshline_policy(Y, p.y);
		board.add_fixed_meshline_policy(Z, p.z);
	}
	if(params.with_zx) {
//		board.add_point(ZX, material, name, /*priority,*/ { p.z, p.x });
		board.add_fixed_meshline_policy(Z, p.z);
		board.add_fixed_meshline_policy(X, p.x);
	}
	if(params.with_xy) {
//		board.add_point(XY, material, name, /*priority,*/ { p.x, p.y });
		board.add_fixed_meshline_policy(X, p.x);
		board.add_fixed_meshline_policy(Y, p.y);
	}
}

//******************************************************************************
expected<shared_ptr<Board>, string> ParserFromCsx::run(std::filesystem::path const& input) {
	return ParserFromCsx::run(input, {});
}

//******************************************************************************
expected<shared_ptr<Board>, string> ParserFromCsx::run(std::filesystem::path const& input, Params params) {
	ParserFromCsx parser(input, std::move(params));
	TRY(parser.parse());
	return parser.output();
}

//******************************************************************************
expected<shared_ptr<Board>, string> ParserFromCsx::run(std::filesystem::path const& input, Params params, std::function<void (domain::Params&)> const& override_domain_params) {
	ParserFromCsx parser(input, std::move(params));
	TRY(parser.parse());
	override_domain_params(parser.domain_params);
	for(auto& [axis, coord] : parser.domain_params.input_fixed_meshlines)
		parser.pimpl->board.add_fixed_meshline_policy(axis, coord);
	parser.domain_params.input_fixed_meshlines.clear();
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
expected<void, string> ParserFromCsx::parse() {
	pugi::xml_document doc;

	if(auto res = doc.load_file(input.native().c_str())
	; res.status != pugi::status_ok) {
		return unexpected(res.description());
	}

	if(parser_params.read_oemsh_params) {
		pugi::xpath_node oemsh = doc.select_node("/OpenEMSH");
		TRY(pimpl->parse_oemsh(oemsh.node()));
	}

	if(!doc.select_node("/openEMS"))
		return unexpected("No \"/openEMS\" path in CSX XML file");

	pugi::xpath_node fdtd = doc.select_node("/openEMS/FDTD");

	pugi::xpath_node csx = doc.select_node("/openEMS/ContinuousStructure");
	TRY(pimpl->parse_grid(csx.node()));

	pimpl->board.set_background_material(pimpl->parse_property(csx.node().child("BackgroundMaterial")));

	{
		// Primitives' IDs grow disregarding properties.
		size_t id = 0;
		pugi::xpath_node_set primitives = doc.select_nodes("/openEMS/ContinuousStructure/Properties/*/Primitives");
		for(auto const& primitive : primitives)
			for(auto const& node : primitive.node().children()) {
				pimpl->primitives_ids.emplace(node, id++);
//				cerr << node.name() << endl;
			}
	}

	auto [bar, found, i] = Progress::Bar::build(
		pimpl->primitives_ids.size(),
		"Parsing primitives ");

	pugi::xpath_node properties = doc.select_node("/openEMS/ContinuousStructure/Properties");
	for(auto const& node : properties.node().children()) {
		auto material = pimpl->parse_property(node);

		pugi::xml_node primitives = node.child("Primitives");
		for(auto const& node : primitives.children()) {
			if(material && pimpl->parse_primitive(node, material))
				++found;
			bar.tick(found, ++i);
		}

	}
	bar.complete();
	domain_params = std::move(pimpl->domain_params);

	if(!pimpl->warning_unsupported_primitives_names.empty()
	&& !pimpl->warning_unsupported_primitives_types.empty())
		log({
			.level = Logger::Level::WARNING,
			.user_actions = { Logger::UserAction::OK },
			.message = "Unsupported CSXCAD Primitives:",
			.informative = pimpl->warning_unsupported_primitives_types | views::join_with(", "s) | ranges::to<string>(),
			.details = pimpl->warning_unsupported_primitives_names | views::join_with(", "s) | ranges::to<string>()
			});

	return {};
}

//******************************************************************************
shared_ptr<Board> ParserFromCsx::output() {
	pimpl->primitives_ids.clear();
	return pimpl->board.build(std::move(domain_params));
}
