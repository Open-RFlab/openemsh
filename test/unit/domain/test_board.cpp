///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include <array>
#include <utility>

#include "domain/conflicts/conflict_colinear_edges.hpp"
#include "domain/conflicts/conflict_edge_in_polygon.hpp"
#include "domain/material.hpp"
#include "utils/vector_utils.hpp"

#include "domain/board.hpp"

/// @test void sort_points_by_vector_orientation(std::vector<Point>& points, Point const& vector)
/// @test void Board::Builder::add_polygon(Plane plane, std::string const& name, Polygon::RangeZ const& z_placement, std::initializer_list<Point> points)
/// @test void Board::Builder::add_polygon(Plane plane, std::string const& name, Polygon::RangeZ const& z_placement, std::vector<std::unique_ptr<Point const>>&& points)
/// @test void Board::Builder::add_polygon_from_box(Plane plane, std::string const& name, Polygon::RangeZ const& z_placement, Point const p1, Point const p3)
/// @test std::unique_ptr<Board> Board::Builder::build()
/// @test std::pair<std::shared_ptr<Material>, std::remove_const_t<decltype(Polygon::priority)>> Board::find_ambient_material(Plane plane, Segment const& segment, std::shared_ptr<Polygon> const& current_polygon) const
/// @test void Board::adjust_edges_to_materials()
/// @test void Board::detect_edges_in_polygons()
/// @test void Board::detect_colinear_edges()
/// @test void Board::detect_individual_edges()
///*****************************************************************************

using namespace domain;

//******************************************************************************
SCENARIO("void sort_points_by_vector_orientation(std::vector<Point>& points, Point const& vector)", "[board]") {
	GIVEN("Unordered colinear points in (+, 0) direction") {
		std::vector<Point> points({{ 4, 1 }, { 2, 1 }, { 3, 1 }, { 1, 1 }});
		Point v(points[0] - points[3]);
		sort_points_by_vector_orientation(points, v);
		THEN("Points should be ordered") {
			REQUIRE(points.size() == 4);
			REQUIRE(points[0] == Point(1, 1));
			REQUIRE(points[1] == Point(2, 1));
			REQUIRE(points[2] == Point(3, 1));
			REQUIRE(points[3] == Point(4, 1));
		}
	}

	GIVEN("Unordered colinear points in (0, +) direction") {
		std::vector<Point> points({{ 1, 4 }, { 1, 2 }, { 1, 3 }, { 1, 1 }});
		Point v(points[0] - points[3]);
		sort_points_by_vector_orientation(points, v);
		THEN("Points should be ordered") {
			REQUIRE(points.size() == 4);
			REQUIRE(points[0] == Point(1, 1));
			REQUIRE(points[1] == Point(1, 2));
			REQUIRE(points[2] == Point(1, 3));
			REQUIRE(points[3] == Point(1, 4));
		}
	}

	GIVEN("Unordered colinear points in (-, 0) direction") {
		std::vector<Point> points({{ 4, 1 }, { 2, 1 }, { 3, 1 }, { 1, 1 }});
		Point v(points[3] - points[0]);
		sort_points_by_vector_orientation(points, v);
		THEN("Points should be ordered") {
			REQUIRE(points.size() == 4);
			REQUIRE(points[0] == Point(4, 1));
			REQUIRE(points[1] == Point(3, 1));
			REQUIRE(points[2] == Point(2, 1));
			REQUIRE(points[3] == Point(1, 1));
		}
	}

	GIVEN("Unordered colinear points in (0, -) direction") {
		std::vector<Point> points({{ 1, 4 }, { 1, 2 }, { 1, 3 }, { 1, 1 }});
		Point v(points[3] - points[0]);
		sort_points_by_vector_orientation(points, v);
		THEN("Points should be ordered") {
			REQUIRE(points.size() == 4);
			REQUIRE(points[0] == Point(1, 4));
			REQUIRE(points[1] == Point(1, 3));
			REQUIRE(points[2] == Point(1, 2));
			REQUIRE(points[3] == Point(1, 1));
		}
	}

	GIVEN("Unordered colinear points in (+, +) direction") {
		std::vector<Point> points({{ 4, 4 }, { 2, 2 }, { 3, 3 }, { 1, 1 }});
		Point v(points[0] - points[3]);
		sort_points_by_vector_orientation(points, v);
		THEN("Points should be ordered") {
			REQUIRE(points.size() == 4);
			REQUIRE(points[0] == Point(1, 1));
			REQUIRE(points[1] == Point(2, 2));
			REQUIRE(points[2] == Point(3, 3));
			REQUIRE(points[3] == Point(4, 4));
		}
	}

	GIVEN("Unordered colinear points in (-, +) direction") {
		std::vector<Point> points({{ 1, 4 }, { 3, 2 }, { 2, 3 }, { 4, 1 }});
		Point v(points[0] - points[3]);
		sort_points_by_vector_orientation(points, v);
		THEN("Points should be ordered") {
			REQUIRE(points.size() == 4);
			REQUIRE(points[0] == Point(4, 1));
			REQUIRE(points[1] == Point(3, 2));
			REQUIRE(points[2] == Point(2, 3));
			REQUIRE(points[3] == Point(1, 4));
		}
	}

	GIVEN("Unordered colinear points in (-, -) direction") {
		std::vector<Point> points({{ 4, 4 }, { 2, 2 }, { 3, 3 }, { 1, 1 }});
		Point v(points[3] - points[0]);
		sort_points_by_vector_orientation(points, v);
		THEN("Points should be ordered") {
			REQUIRE(points.size() == 4);
			REQUIRE(points[0] == Point(4, 4));
			REQUIRE(points[1] == Point(3, 3));
			REQUIRE(points[2] == Point(2, 2));
			REQUIRE(points[3] == Point(1, 1));
		}
	}

	GIVEN("Unordered colinear points in (+, -) direction") {
		std::vector<Point> points({{ 1, 4 }, { 3, 2 }, { 2, 3 }, { 4, 1 }});
		Point v(points[3] - points[0]);
		sort_points_by_vector_orientation(points, v);
		THEN("Points should be ordered") {
			REQUIRE(points.size() == 4);
			REQUIRE(points[0] == Point(1, 4));
			REQUIRE(points[1] == Point(2, 3));
			REQUIRE(points[2] == Point(3, 2));
			REQUIRE(points[3] == Point(4, 1));
		}
	}
}


//******************************************************************************
SCENARIO("void Board::Builder::add_polygon(Plane plane, std::string const& name, Polygon::RangeZ const& z_placement, std::initializer_list<Point> points)", "[board]") {
	GIVEN("A Board Builder") {
		Board::Builder b;
		auto material = std::make_shared<Material>(Material::Type::CONDUCTOR, "");
		REQUIRE(b.polygons[XY].empty());
		WHEN("Adding a polygon as an initializer_list of Points") {
			b.add_polygon(XY, material, "MS13", 0, { 0, 0 }, {
				{ 70.3673, -42.8674 },
				{ 62.1753, -42.8674 },
				{ 62.1753, -43.9276 },
				{ 66.2713, -43.9276 },
				{ 66.2713, -43.9514 },
				{ 70.3673, -43.9514 }});
			THEN("Should add a Polygon in the inner vector") {
				REQUIRE(b.polygons[XY].size() == 1);
				REQUIRE(b.polygons[XY][0]->name == "MS13");
				REQUIRE(*(b.polygons[XY][0]->points[0]) == Point(70.3673, -42.8674));
				REQUIRE(*(b.polygons[XY][0]->points[1]) == Point(62.1753, -42.8674));
				REQUIRE(*(b.polygons[XY][0]->points[2]) == Point(62.1753, -43.9276));
				REQUIRE(*(b.polygons[XY][0]->points[3]) == Point(66.2713, -43.9276));
				REQUIRE(*(b.polygons[XY][0]->points[4]) == Point(66.2713, -43.9514));
				REQUIRE(*(b.polygons[XY][0]->points[5]) == Point(70.3673, -43.9514));
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Board::Builder::add_polygon(Plane plane, std::string const& name, Polygon::RangeZ const& z_placement, std::vector<std::unique_ptr<Point const>>&& points)", "[board]") {
	GIVEN("A Board Builder") {
		Board::Builder b;
		auto material = std::make_shared<Material>(Material::Type::CONDUCTOR, "");
		REQUIRE(b.polygons[XY].empty());
		WHEN("Adding a polygon as a vector of Points") {
			std::vector<std::unique_ptr<Point const>> points(from_init_list<Point>({
				{ 70.3673, -42.8674 },
				{ 62.1753, -42.8674 },
				{ 62.1753, -43.9276 },
				{ 66.2713, -43.9276 },
				{ 66.2713, -43.9514 },
				{ 70.3673, -43.9514 }}));
			b.add_polygon(XY, material, "MS13", 0, { 0, 0 }, std::move(points));
			b.add_polygon(XY, material, "MS15", 0, { 0, 0 }, from_init_list<Point>({
				{ 70.3673, -42.8674 },
				{ 62.1753, -42.8674 },
				{ 62.1753, -43.9276 },
				{ 66.2713, -43.9276 },
				{ 66.2713, -43.9514 },
				{ 70.3673, -43.9514 }}));
			THEN("Should add a Polygon in the inner vector") {
				REQUIRE(b.polygons[XY].size() == 2);
				REQUIRE(b.polygons[XY][0]->name == "MS13");
				REQUIRE(*(b.polygons[XY][0]->points[0]) == Point(70.3673, -42.8674));
				REQUIRE(*(b.polygons[XY][0]->points[1]) == Point(62.1753, -42.8674));
				REQUIRE(*(b.polygons[XY][0]->points[2]) == Point(62.1753, -43.9276));
				REQUIRE(*(b.polygons[XY][0]->points[3]) == Point(66.2713, -43.9276));
				REQUIRE(*(b.polygons[XY][0]->points[4]) == Point(66.2713, -43.9514));
				REQUIRE(*(b.polygons[XY][0]->points[5]) == Point(70.3673, -43.9514));
				REQUIRE(b.polygons[XY][1]->name == "MS15");
				REQUIRE(*(b.polygons[XY][1]->points[0]) == Point(70.3673, -42.8674));
				REQUIRE(*(b.polygons[XY][1]->points[1]) == Point(62.1753, -42.8674));
				REQUIRE(*(b.polygons[XY][1]->points[2]) == Point(62.1753, -43.9276));
				REQUIRE(*(b.polygons[XY][1]->points[3]) == Point(66.2713, -43.9276));
				REQUIRE(*(b.polygons[XY][1]->points[4]) == Point(66.2713, -43.9514));
				REQUIRE(*(b.polygons[XY][1]->points[5]) == Point(70.3673, -43.9514));
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Board::Builder::add_polygon_from_box(Plane plane, std::string const& name, Polygon::RangeZ const& z_placement, Point const p1, Point const p3)", "[board]") {
	GIVEN("A Board Builder") {
		Board::Builder b;
		auto material = std::make_shared<Material>(Material::Type::CONDUCTOR, "");
		REQUIRE(b.polygons[XY].empty());
		WHEN("Adding a rectangle polygon as a box of opposite Points") {
			b.add_polygon_from_box(XY, material, "MS1", 0, { 0, 0 }, { 16.1, -26.5 }, { 20.6, -26 });
			THEN("Should add a Polygon in the inner vector") {
				REQUIRE(b.polygons[XY].size() == 1);
				REQUIRE(b.polygons[XY][0]->name == "MS1");
				REQUIRE(*(b.polygons[XY][0]->points[0]) == Point(16.1, -26.5));
				REQUIRE(*(b.polygons[XY][0]->points[1]) == Point(16.1, -26));
				REQUIRE(*(b.polygons[XY][0]->points[2]) == Point(20.6, -26));
				REQUIRE(*(b.polygons[XY][0]->points[3]) == Point(20.6, -26.5));
			}
		}
	}
}

//******************************************************************************
SCENARIO("std::unique_ptr<Board> Board::Builder::build()", "[board]") {
	GIVEN("A Board Builder previously fed of polygons") {
		Board::Builder b;
		auto material = std::make_shared<Material>(Material::Type::CONDUCTOR, "");
		REQUIRE(b.polygons[XY].empty());
		b.add_polygon_from_box(XY, material, "MS1", 0, { 0, 0 }, { 16.1, -26.5 }, { 20.6, -26 });
		b.add_polygon(XY, material, "MS15", 0, { 0, 0 }, from_init_list<Point>({
			{ 70.3673, -42.8674 },
			{ 62.1753, -42.8674 },
			{ 62.1753, -43.9276 },
			{ 66.2713, -43.9276 },
			{ 66.2713, -43.9514 },
			{ 70.3673, -43.9514 }}));
		b.add_polygon(XY, material, "MS13", 0, { 0, 0 }, {
			{ 70.3673, -42.8674 },
			{ 62.1753, -42.8674 },
			{ 62.1753, -43.9276 },
			{ 66.2713, -43.9276 },
			{ 66.2713, -43.9514 },
			{ 70.3673, -43.9514 }});
		b.set_background_material(material);
		REQUIRE(b.polygons[XY].size() == 3);
		WHEN("Calling build()") {
			std::shared_ptr<Board> a = b.build();
			THEN("Should output a Board containing the polygons") {
				REQUIRE(a->material == material);
				REQUIRE(a->get_current_state().polygons[XY].size() == 3);
				REQUIRE(a->get_current_state().polygons[XY][0]->name == "MS1");
				REQUIRE(*(a->get_current_state().polygons[XY][0]->points[0]) == Point(16.1, -26.5));
				REQUIRE(*(a->get_current_state().polygons[XY][0]->points[1]) == Point(16.1, -26));
				REQUIRE(*(a->get_current_state().polygons[XY][0]->points[2]) == Point(20.6, -26));
				REQUIRE(*(a->get_current_state().polygons[XY][0]->points[3]) == Point(20.6, -26.5));
				REQUIRE(a->get_current_state().polygons[XY][1]->name == "MS15");
				REQUIRE(*(a->get_current_state().polygons[XY][1]->points[0]) == Point(70.3673, -42.8674));
				REQUIRE(*(a->get_current_state().polygons[XY][1]->points[1]) == Point(62.1753, -42.8674));
				REQUIRE(*(a->get_current_state().polygons[XY][1]->points[2]) == Point(62.1753, -43.9276));
				REQUIRE(*(a->get_current_state().polygons[XY][1]->points[3]) == Point(66.2713, -43.9276));
				REQUIRE(*(a->get_current_state().polygons[XY][1]->points[4]) == Point(66.2713, -43.9514));
				REQUIRE(*(a->get_current_state().polygons[XY][1]->points[5]) == Point(70.3673, -43.9514));
				REQUIRE(a->get_current_state().polygons[XY][2]->name == "MS13");
				REQUIRE(*(a->get_current_state().polygons[XY][2]->points[0]) == Point(70.3673, -42.8674));
				REQUIRE(*(a->get_current_state().polygons[XY][2]->points[1]) == Point(62.1753, -42.8674));
				REQUIRE(*(a->get_current_state().polygons[XY][2]->points[2]) == Point(62.1753, -43.9276));
				REQUIRE(*(a->get_current_state().polygons[XY][2]->points[3]) == Point(66.2713, -43.9276));
				REQUIRE(*(a->get_current_state().polygons[XY][2]->points[4]) == Point(66.2713, -43.9514));
				REQUIRE(*(a->get_current_state().polygons[XY][2]->points[5]) == Point(70.3673, -43.9514));
			}

			THEN("The Builder should be empty") {
				REQUIRE(b.polygons[XY].empty());
			}
		}
	}
}

//******************************************************************************
SCENARIO("std::pair<std::shared_ptr<Material>, std::remove_const_t<decltype(Polygon::priority)>> Board::find_ambient_material(Plane plane, Segment const& segment, std::shared_ptr<Polygon> const& current_polygon) const", "[board]") {
	Timepoint* t = Caretaker::singleton().get_history_root();
	std::unique_ptr<Board> b;
	GIVEN("A board holding some polygons of each Material type, for some prioriries, all overlapping in plane and normal axis") {
		Polygon::RangeZ z = { 0, 3 };
		auto a0x = std::make_shared<Material>(Material::Type::AIR, "");
		auto c0 = std::make_shared<Material>(Material::Type::CONDUCTOR, "");
		auto d0 = std::make_shared<Material>(Material::Type::DIELECTRIC, "");
		auto a0 = std::make_shared<Material>(Material::Type::AIR, "");
		auto c1 = std::make_shared<Material>(Material::Type::CONDUCTOR, "");
		auto d1 = std::make_shared<Material>(Material::Type::DIELECTRIC, "");
		auto a1 = std::make_shared<Material>(Material::Type::AIR, "");
		auto c2 = std::make_shared<Material>(Material::Type::CONDUCTOR, "");
		auto d2 = std::make_shared<Material>(Material::Type::DIELECTRIC, "");
		auto a2 = std::make_shared<Material>(Material::Type::AIR, "");
		auto px = std::make_shared<Polygon>(XY, a0x, "", 0, z, from_init_list<Point>({{ 2, 2 }, { 2, 5 }, { 5, 5 }, { 5, 2 }}), t);
		auto pc0 = std::make_shared<Polygon>(XY, c0, "", 0, z, from_init_list<Point>({{ 1, 1 }, { 6, 1 }, { 6, 6 }, { 1, 6 }}), t);
		auto pd0 = std::make_shared<Polygon>(XY, d0, "", 0, z, from_init_list<Point>({{ 1, 1 }, { 6, 1 }, { 6, 6 }, { 1, 6 }}), t);
		auto pa0 = std::make_shared<Polygon>(XY, a0, "", 0, z, from_init_list<Point>({{ 1, 1 }, { 6, 1 }, { 6, 6 }, { 1, 6 }}), t);
		auto pc1 = std::make_shared<Polygon>(XY, c1, "", 1, z, from_init_list<Point>({{ 1, 1 }, { 6, 1 }, { 6, 6 }, { 1, 6 }}), t);
		auto pd1 = std::make_shared<Polygon>(XY, d1, "", 1, z, from_init_list<Point>({{ 1, 1 }, { 6, 1 }, { 6, 6 }, { 1, 6 }}), t);
		auto pa1 = std::make_shared<Polygon>(XY, a1, "", 1, z, from_init_list<Point>({{ 1, 1 }, { 6, 1 }, { 6, 6 }, { 1, 6 }}), t);
		auto pc2 = std::make_shared<Polygon>(XY, c2, "", 2, z, from_init_list<Point>({{ 1, 1 }, { 6, 1 }, { 6, 6 }, { 1, 6 }}), t);
		auto pd2 = std::make_shared<Polygon>(XY, d2, "", 2, z, from_init_list<Point>({{ 1, 1 }, { 6, 1 }, { 6, 6 }, { 1, 6 }}), t);
		auto pa2 = std::make_shared<Polygon>(XY, a2, "", 2, z, from_init_list<Point>({{ 1, 1 }, { 6, 1 }, { 6, 6 }, { 1, 6 }}), t);
		{
			PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
			tmp[XY].push_back(px);
			tmp[XY].push_back(pc0);
			tmp[XY].push_back(pd0);
			tmp[XY].push_back(pa0);
			tmp[XY].push_back(pc1);
			tmp[XY].push_back(pd1);
			tmp[XY].push_back(pa1);
			tmp[XY].push_back(pc2);
			tmp[XY].push_back(pd2);
			tmp[XY].push_back(pa2);
			b = std::make_unique<Board>(std::move(tmp), Params(), t);
		}
		WHEN("Looking for ambient Material outside of any Polygon, relative to a Polygon") {
			AND_WHEN("The board has a background Material") {
				auto background = std::make_shared<Material>(Material::Type::DIELECTRIC, "");
				b->material = background;
				auto [material, priority] = b->find_ambient_material(XY, Range({ 10, 10 }, { 11, 11 }), px);
				THEN("Should return the board background Material and the lowest possible priority") {
					REQUIRE(material);
					REQUIRE(material == background);
					REQUIRE(material->type == Material::Type::DIELECTRIC);
					REQUIRE(priority == std::numeric_limits<std::size_t>::min());
				}
			}
			AND_WHEN("The board has no background Material") {
				auto [material, priority] = b->find_ambient_material(XY, Range({ 10, 10 }, { 11, 11 }), px);
				THEN("Should not return any Material and the lowest possible priority") {
					REQUIRE_FALSE(material);
					REQUIRE(priority == std::numeric_limits<std::size_t>::min());
				}
			}
		}
		WHEN("Looking for ambient Material inside the overlap of all Polygons relative to one of AIR Material with the lower priority") {
			auto [material, priority] = b->find_ambient_material(XY, Range({ 3.5, 3.2 }, { 3.5, 3.8 }), px);
			THEN("Should the CONDUCTOR Material of the Polygon with the highest priority") {
				REQUIRE(material);
				REQUIRE(material->type == Material::Type::CONDUCTOR);
				REQUIRE(material.get() == c2.get());
				REQUIRE(priority == pc2->priority);
			}
		}
	}

	GIVEN("A board holding some polygons of each Material type, all overlapping in plane, some overlapping in normal axis") {
		auto conductor = std::make_shared<Material>(Material::Type::CONDUCTOR, "");
		auto dielectric = std::make_shared<Material>(Material::Type::DIELECTRIC, "");
		auto air = std::make_shared<Material>(Material::Type::AIR, "");
		auto x0 = std::make_shared<Polygon>(XY, conductor, "", 0, Polygon::RangeZ { 0, 3 }, from_init_list<Point>({{ 1, 1 }, { 6, 1 }, { 6, 6 }, { 1, 6 }}), t);
		auto y0 = std::make_shared<Polygon>(XY, dielectric, "", 0, Polygon::RangeZ { 2, 4 }, from_init_list<Point>({{ 2, 2 }, { 2, 5 }, { 5, 5 }, { 5, 2 }}), t);
		auto z0 = std::make_shared<Polygon>(XY, air, "", 0, Polygon::RangeZ { 5, 6 }, from_init_list<Point>({{ 3, 3 }, { 3, 4 }, { 4, 4 }, { 4, 3 }}), t);
		auto x1 = std::make_shared<Polygon>(XY, conductor, "", 1, Polygon::RangeZ { 0, 3 }, from_init_list<Point>({{ 1, 1 }, { 6, 1 }, { 6, 6 }, { 1, 6 }}), t);
		auto y1 = std::make_shared<Polygon>(XY, dielectric, "", 1, Polygon::RangeZ { 2, 4 }, from_init_list<Point>({{ 2, 2 }, { 2, 5 }, { 5, 5 }, { 5, 2 }}), t);
		auto z1 = std::make_shared<Polygon>(XY, air, "", 1, Polygon::RangeZ { 5, 6 }, from_init_list<Point>({{ 3, 3 }, { 3, 4 }, { 4, 4 }, { 4, 3 }}), t);
		WHEN("Looking for absolute ambient Material outside of any Polygon") {
			{
				PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
				tmp[XY].push_back(x0);
				tmp[XY].push_back(y0);
				tmp[XY].push_back(z0);
				b = std::make_unique<Board>(std::move(tmp), Params(), t);
			}
			AND_WHEN("The board has a background Material") {
				auto background = std::make_shared<Material>(Material::Type::DIELECTRIC, "");
				b->material = background;
				auto material = b->find_ambient_material(XY, Range({ 10, 10 }, { 11, 11 }));
				THEN("Should return the board background Material") {
					REQUIRE(material);
					REQUIRE(material == background);
					REQUIRE(material->type == Material::Type::DIELECTRIC);
				}
			}
			AND_WHEN("The board has no background Material") {
				THEN("Should not return any Material") {
					REQUIRE_FALSE(b->find_ambient_material(XY, Range({ 10, 10 }, { 11, 11 })));
				}
			}
		}
		WHEN("Looking for absolute ambient Material inside the overlap of all Polygons") {
			{
				PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
				tmp[XY].push_back(x0);
				tmp[XY].push_back(y0);
				tmp[XY].push_back(z0);
				b = std::make_unique<Board>(std::move(tmp), Params(), t);
			}
			auto material = b->find_ambient_material(XY, Range({ 3.5, 3.2 }, { 3.5, 3.8 }));
			THEN("Should return CONDUCTOR Material") {
				REQUIRE(material);
				REQUIRE(material->type == Material::Type::CONDUCTOR);
			}
		}
		WHEN("Looking for ambient Material inside the overlap of all Polygons") {
			AND_WHEN("Relative to CONDUCTOR Material Polygon") {
				WHEN("Relative to a Polygon with inferior priority than others") {
					{
						PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
						tmp[XY].push_back(x0);
						tmp[XY].push_back(y1);
						tmp[XY].push_back(z1);
						b = std::make_unique<Board>(std::move(tmp), Params(), t);
					}
					auto [material, priority] = b->find_ambient_material(XY, Range({ 3.5, 3.2 }, { 3.5, 3.8 }), x0);
					THEN("Should return DIELECTRIC Material") {
						REQUIRE(material);
						REQUIRE(material->type == Material::Type::DIELECTRIC);
						REQUIRE(priority == 1);
					}
				}
				WHEN("Relative to a Polygon with the same priority than others") {
					{
						PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
						tmp[XY].push_back(x0);
						tmp[XY].push_back(y0);
						tmp[XY].push_back(z0);
						b = std::make_unique<Board>(std::move(tmp), Params(), t);
					}
					auto [material, priority] = b->find_ambient_material(XY, Range({ 3.5, 3.2 }, { 3.5, 3.8 }), x0);
					THEN("Should return DIELECTRIC Material") {
						REQUIRE(material);
						REQUIRE(material->type == Material::Type::DIELECTRIC);
						REQUIRE(priority == 0);
					}
				}
				WHEN("Relative to a Polygon with superior priority than others") {
					{
						PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
						tmp[XY].push_back(x1);
						tmp[XY].push_back(y0);
						tmp[XY].push_back(z0);
						b = std::make_unique<Board>(std::move(tmp), Params(), t);
					}
					auto [material, priority] = b->find_ambient_material(XY, Range({ 3.5, 3.2 }, { 3.5, 3.8 }), x1);
					THEN("Should return DIELECTRIC Material") {
						REQUIRE(material);
						REQUIRE(material->type == Material::Type::DIELECTRIC);
						REQUIRE(priority == 0);
					}
				}
			}
			AND_WHEN("Relative to DIELECTRIC Material Polygon") {
				WHEN("Relative to a Polygon with inferior priority than others") {
					{
						PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
						tmp[XY].push_back(x1);
						tmp[XY].push_back(y0);
						tmp[XY].push_back(z1);
						b = std::make_unique<Board>(std::move(tmp), Params(), t);
					}
					auto [material, priority] = b->find_ambient_material(XY, Range({ 3.5, 3.2 }, { 3.5, 3.8 }), y0);
					THEN("Should return CONDUCTOR Material") {
						REQUIRE(material);
						REQUIRE(material->type == Material::Type::CONDUCTOR);
						REQUIRE(priority == 1);
					}
				}
				WHEN("Relative to a Polygon with the same priority than others") {
					{
						PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
						tmp[XY].push_back(x0);
						tmp[XY].push_back(y0);
						tmp[XY].push_back(z0);
						b = std::make_unique<Board>(std::move(tmp), Params(), t);
					}
					auto [material, priority] = b->find_ambient_material(XY, Range({ 3.5, 3.2 }, { 3.5, 3.8 }), y0);
					THEN("Should return CONDUCTOR Material") {
						REQUIRE(material);
						REQUIRE(material->type == Material::Type::CONDUCTOR);
						REQUIRE(priority == 0);
					}
				}
				WHEN("Relative to a Polygon with superior priority than others") {
					{
						PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
						tmp[XY].push_back(x0);
						tmp[XY].push_back(y1);
						tmp[XY].push_back(z0);
						b = std::make_unique<Board>(std::move(tmp), Params(), t);
					}
					auto [material, priority] = b->find_ambient_material(XY, Range({ 3.5, 3.2 }, { 3.5, 3.8 }), y1);
					THEN("Should return CONDUCTOR Material") {
						REQUIRE(material);
						REQUIRE(material->type == Material::Type::CONDUCTOR);
						REQUIRE(priority == 0);
					}
				}
			}
			AND_WHEN("Relative to AIR Material Polygon") {
				{
					PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
					tmp[XY].push_back(x0);
					tmp[XY].push_back(y0);
					tmp[XY].push_back(z0);
					b = std::make_unique<Board>(std::move(tmp), Params(), t);
				}
				THEN("Should not return any Material because no overlap in normal axis") {
					REQUIRE_FALSE(b->find_ambient_material(XY, Range({ 3.5, 3.2 }, { 3.5, 3.8 }), z0).first);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Board::adjust_edges_to_materials()", "[board]") {
	Timepoint* t = Caretaker::singleton().get_history_root();
	auto conductor = std::make_shared<Material>(Material::Type::CONDUCTOR, "");
	auto dielectric = std::make_shared<Material>(Material::Type::DIELECTRIC, "");
	auto air = std::make_shared<Material>(Material::Type::AIR, "");
	std::unique_ptr<Board> b;

	auto const make_polygons = [&b, &t](auto& x_m, std::size_t x_p, auto&& x_z, auto&& x, auto& y_m, std::size_t y_p, auto&& y_z, auto&& y) {
		PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
		tmp[XY].push_back(std::make_shared<Polygon>(XY, x_m, "", x_p, x_z, std::move(x), t));
		tmp[XY].push_back(std::make_shared<Polygon>(XY, y_m, "", y_p, y_z, std::move(y), t));
		b = std::make_unique<Board>(std::move(tmp), Params(), t);
	};

	GIVEN("A board holding two Polygons that does not overlap") {
		make_polygons(
			conductor, 0, Polygon::RangeZ { 0, 3 }, from_init_list<Point>({{ 1, 1 }, { 2, 1 }, { 2, 2 }, { 1, 2 }}),
			air, 0, Polygon::RangeZ { 2, 4 }, from_init_list<Point>({{ 3, 3 }, { 4, 3 }, { 4, 4 }, { 3, 4 }}));
		b->adjust_edges_to_materials();
		THEN("Should not adjust any edge") {
			for(auto const& edge : b->get_current_state().edges[XY]) {
				REQUIRE_FALSE(edge->get_current_state().to_reverse);
			}
		}
	}
	GIVEN("A board holding two Polygons that overlap partially both in plane and normal axis") {
		make_polygons(
			conductor, 0, Polygon::RangeZ { 0, 3 }, from_init_list<Point>({{ 1, 1 }, { 3, 1 }, { 3, 3 }, { 1, 3 }}),
			air, 0, Polygon::RangeZ { 2, 4 }, from_init_list<Point>({{ 2, 2 }, { 4, 2 }, { 4, 4 }, { 2, 4 }}));
		b->adjust_edges_to_materials();
		THEN("The 2 overlapping edges of the AIR Polygon should be marked for Normal reverse") {
			REQUIRE_FALSE(b->get_current_state().polygons[XY][0]->edges[0]->get_current_state().to_reverse);
			REQUIRE_FALSE(b->get_current_state().polygons[XY][0]->edges[1]->get_current_state().to_reverse);
			REQUIRE_FALSE(b->get_current_state().polygons[XY][0]->edges[2]->get_current_state().to_reverse);
			REQUIRE_FALSE(b->get_current_state().polygons[XY][0]->edges[3]->get_current_state().to_reverse);
			REQUIRE(b->get_current_state().polygons[XY][1]->edges[0]->get_current_state().to_reverse);
			REQUIRE(b->get_current_state().polygons[XY][1]->edges[1]->get_current_state().to_reverse);
			REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().to_reverse);
			REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[3]->get_current_state().to_reverse);
		}
	}
	GIVEN("A board holding a corner-bent Polygon completly inside another Polygon") {
		auto inner = from_init_list<Point>({{ 2, 2 }, { 5, 2 }, { 5, 4 }, { 4, 5 }, { 2, 5 }});
		auto outer = from_init_list<Point>({{ 1, 1 }, { 6, 1 }, { 6, 6 }, { 1, 6 }});
		WHEN("Polygons overlap in normal axis") {
			Polygon::RangeZ i_z = { 0, 3 };
			Polygon::RangeZ o_z = { 2, 4 };
			WHEN("Outer Polygon is of CONDUCTOR Material") {
				auto& o_m = conductor;
				AND_WHEN("Inner Polygon is of CONDUCTOR Material") {
					auto& i_m = conductor;
					make_polygons(i_m, 0, i_z, inner, o_m, 0, o_z, outer);
					b->adjust_edges_to_materials();
					THEN("Should not adjust any edge") {
						for(auto const& edge : b->get_current_state().edges[XY]) {
							REQUIRE_FALSE(edge->get_current_state().to_reverse);
						}
					}
				}
				AND_WHEN("Inner Polygon is of DIELECTRIC Material") {
					auto& i_m = dielectric;
					make_polygons(i_m, 0, i_z, inner, o_m, 0, o_z, outer);
					b->adjust_edges_to_materials();
					THEN("Inner Polygon orthogonal edges should be marked for Normal reverse") {
						REQUIRE(b->get_current_state().polygons[XY][0]->edges[0]->get_current_state().to_reverse);
						REQUIRE(b->get_current_state().polygons[XY][0]->edges[1]->get_current_state().to_reverse);
						REQUIRE(b->get_current_state().polygons[XY][0]->edges[2]->get_current_state().to_reverse);
						REQUIRE(b->get_current_state().polygons[XY][0]->edges[3]->direction == Edge::Direction::NONE);
						REQUIRE_FALSE(b->get_current_state().polygons[XY][0]->edges[3]->get_current_state().to_reverse);
						REQUIRE(b->get_current_state().polygons[XY][0]->edges[4]->get_current_state().to_reverse);
						REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[0]->get_current_state().to_reverse);
						REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[1]->get_current_state().to_reverse);
						REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().to_reverse);
						REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[3]->get_current_state().to_reverse);
					}
				}
				AND_WHEN("Inner Polygon is of AIR Material") {
					auto& i_m = air;
					WHEN("Inner Polygon is of superior priority than outer") {
						make_polygons(i_m, 3, i_z, inner, o_m, 2, o_z, outer);
						b->adjust_edges_to_materials();
						THEN("Inner Polygon orthogonal edges should be marked for Normal reverse") {
							REQUIRE(b->get_current_state().polygons[XY][0]->edges[0]->get_current_state().to_reverse);
							REQUIRE(b->get_current_state().polygons[XY][0]->edges[1]->get_current_state().to_reverse);
							REQUIRE(b->get_current_state().polygons[XY][0]->edges[2]->get_current_state().to_reverse);
							REQUIRE(b->get_current_state().polygons[XY][0]->edges[3]->direction == Edge::Direction::NONE);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][0]->edges[3]->get_current_state().to_reverse);
							REQUIRE(b->get_current_state().polygons[XY][0]->edges[4]->get_current_state().to_reverse);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[0]->get_current_state().to_reverse);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[1]->get_current_state().to_reverse);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().to_reverse);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[3]->get_current_state().to_reverse);
						}
					}
					WHEN("Both Polygons are of the same priority") {
						make_polygons(i_m, 0, i_z, inner, o_m, 0, o_z, outer);
						b->adjust_edges_to_materials();
						THEN("Inner Polygon orthogonal edges should be marked for Normal reverse") {
							REQUIRE(b->get_current_state().polygons[XY][0]->edges[0]->get_current_state().to_reverse);
							REQUIRE(b->get_current_state().polygons[XY][0]->edges[1]->get_current_state().to_reverse);
							REQUIRE(b->get_current_state().polygons[XY][0]->edges[2]->get_current_state().to_reverse);
							REQUIRE(b->get_current_state().polygons[XY][0]->edges[3]->direction == Edge::Direction::NONE);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][0]->edges[3]->get_current_state().to_reverse);
							REQUIRE(b->get_current_state().polygons[XY][0]->edges[4]->get_current_state().to_reverse);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[0]->get_current_state().to_reverse);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[1]->get_current_state().to_reverse);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().to_reverse);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[3]->get_current_state().to_reverse);
						}
					}
					WHEN("Inner Polygon is of inferior priority than outer") {
						make_polygons(i_m, 2, i_z, inner, o_m, 3, o_z, outer);
						b->adjust_edges_to_materials();
						THEN("Inner Polygon orthogonal edges should not be marked for Normal reverse") {
							REQUIRE_FALSE(b->get_current_state().polygons[XY][0]->edges[0]->get_current_state().to_reverse);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][0]->edges[1]->get_current_state().to_reverse);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][0]->edges[2]->get_current_state().to_reverse);
							REQUIRE(b->get_current_state().polygons[XY][0]->edges[3]->direction == Edge::Direction::NONE);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][0]->edges[3]->get_current_state().to_reverse);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][0]->edges[4]->get_current_state().to_reverse);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[0]->get_current_state().to_reverse);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[1]->get_current_state().to_reverse);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().to_reverse);
							REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[3]->get_current_state().to_reverse);
						}
					}
				}
			}
			WHEN("Outer Polygon is of DIELECTRIC Material") {
				auto& o_m = dielectric;
				AND_WHEN("Inner Polygon is of CONDUCTOR Material") {
					auto& i_m = conductor;
					make_polygons(i_m, 0, i_z, inner, o_m, 0, o_z, outer);
					b->adjust_edges_to_materials();
					THEN("Should not adjust any edge") {
						for(auto const& edge : b->get_current_state().edges[XY]) {
							REQUIRE_FALSE(edge->get_current_state().to_reverse);
						}
					}
				}
				AND_WHEN("Inner Polygon is of DIELECTRIC Material") {
					auto& i_m = dielectric;
					make_polygons(i_m, 0, i_z, inner, o_m, 0, o_z, outer);
					b->adjust_edges_to_materials();
					THEN("Should not adjust any edge") {
						for(auto const& edge : b->get_current_state().edges[XY]) {
							REQUIRE_FALSE(edge->get_current_state().to_reverse);
						}
					}
				}
				AND_WHEN("Inner Polygon is of AIR Material") {
					auto& i_m = air;
					make_polygons(i_m, 0, i_z, inner, o_m, 0, o_z, outer);
					b->adjust_edges_to_materials();
					THEN("Inner Polygon orthogonal edges should be marked for Normal reverse") {
						REQUIRE(b->get_current_state().polygons[XY][0]->edges[0]->get_current_state().to_reverse);
						REQUIRE(b->get_current_state().polygons[XY][0]->edges[1]->get_current_state().to_reverse);
						REQUIRE(b->get_current_state().polygons[XY][0]->edges[2]->get_current_state().to_reverse);
						REQUIRE(b->get_current_state().polygons[XY][0]->edges[3]->direction == Edge::Direction::NONE);
						REQUIRE_FALSE(b->get_current_state().polygons[XY][0]->edges[3]->get_current_state().to_reverse);
						REQUIRE(b->get_current_state().polygons[XY][0]->edges[4]->get_current_state().to_reverse);
						REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[0]->get_current_state().to_reverse);
						REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[1]->get_current_state().to_reverse);
						REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().to_reverse);
						REQUIRE_FALSE(b->get_current_state().polygons[XY][1]->edges[3]->get_current_state().to_reverse);
					}
				}
			}
			WHEN("Outer Polygon is of AIR Material") {
				auto& o_m = air;
				AND_WHEN("Inner Polygon is of CONDUCTOR Material") {
					auto& i_m = conductor;
					make_polygons(i_m, 0, i_z, inner, o_m, 0, o_z, outer);
					b->adjust_edges_to_materials();
					THEN("Should not adjust any edge") {
						for(auto const& edge : b->get_current_state().edges[XY]) {
							REQUIRE_FALSE(edge->get_current_state().to_reverse);
						}
					}
				}
				AND_WHEN("Inner Polygon is of DIELECTRIC Material") {
					auto& i_m = dielectric;
					make_polygons(i_m, 0, i_z, inner, o_m, 0, o_z, outer);
					b->adjust_edges_to_materials();
					THEN("Should not adjust any edge") {
						for(auto const& edge : b->get_current_state().edges[XY]) {
							REQUIRE_FALSE(edge->get_current_state().to_reverse);
						}
					}
				}
				AND_WHEN("Inner Polygon is of AIR Material") {
					auto& i_m = air;
					make_polygons(i_m, 0, i_z, inner, o_m, 0, o_z, outer);
					b->adjust_edges_to_materials();
					THEN("Should not adjust any edge") {
						for(auto const& edge : b->get_current_state().edges[XY]) {
							REQUIRE_FALSE(edge->get_current_state().to_reverse);
						}
					}
				}
			}
		}
		WHEN("Polygons do not overlap in normal axis") {
			Polygon::RangeZ i_z = { 0, 2 };
			Polygon::RangeZ o_z = { 3, 4 };
			make_polygons(air, 0, i_z, inner, conductor, 0, o_z, outer);
			b->adjust_edges_to_materials();
			THEN("Should not adjust any edge") {
				for(auto const& edge : b->get_current_state().edges[XY]) {
					REQUIRE_FALSE(edge->get_current_state().to_reverse);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Board::detect_edges_in_polygons()", "[board]") {
	Timepoint* t = Caretaker::singleton().get_history_root();
	auto material = std::make_shared<Material>(Material::Type::CONDUCTOR, "");
	GIVEN("A board holding two simple polygons (orthogonal squares)") {
		WHEN("A polygon is totally inside the other") {
			std::unique_ptr<Board> b;
			{
				PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 1, 1 }, { 4, 1 }, { 4, 4 }, { 1, 4 }}), t));
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 2, 2 }, { 2, 3 }, { 3, 3 }, { 3, 2 }}), t));
				b = std::make_unique<Board>(std::move(tmp), Params(), t);
			}
			b->detect_edges_in_polygons();
			THEN("4 EDGE_IN_POLYGON conflicts should be registered") {
				REQUIRE(b->conflict_manager->get_current_state().all_edge_in_polygons[XY].size() == 4);
				for(std::shared_ptr<ConflictEdgeInPolygon> const& conflict : b->conflict_manager->get_current_state().all_edge_in_polygons[XY])
					REQUIRE(conflict->kind == Conflict::Kind::EDGE_IN_POLYGON);
				AND_THEN("The registered conflicts should be between the right edges and polygons") {
					bool is_first_registered = false;
					bool is_second_registered = false;
					bool is_third_registered = false;
					bool is_fourth_registered = false;
					for(std::shared_ptr<ConflictEdgeInPolygon> const& c : b->conflict_manager->get_current_state().all_edge_in_polygons[XY]) {
						if(c->edge == b->get_current_state().polygons[XY][1]->edges[0].get()) {
							is_first_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 3, 2 }, { 2, 2 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[1].get()) {
							is_second_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 2, 2 }, { 2, 3 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[2].get()) {
							is_third_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 2, 3 }, { 3, 3 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[3].get()) {
							is_fourth_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 3, 3 }, { 3, 2 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						}
					}
					REQUIRE(is_first_registered);
					REQUIRE(is_second_registered);
					REQUIRE(is_third_registered);
					REQUIRE(is_fourth_registered);
				}
				AND_THEN("The edges and polygons should register the conflicts") {
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[0]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[1]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[3]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][0]->get_current_state().conflicts.size() == 4);

					auto p0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->get_current_state().conflicts[0]);
					auto p0c1 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->get_current_state().conflicts[1]);
					auto p0c2 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->get_current_state().conflicts[2]);
					auto p0c3 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->get_current_state().conflicts[3]);
					auto p1e0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->edges[0]->get_current_state().conflicts[0]);
					auto p1e1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->edges[1]->get_current_state().conflicts[0]);
					auto p1e2c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().conflicts[0]);
					auto p1e3c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->edges[3]->get_current_state().conflicts[0]);
					REQUIRE(std::get<POLYGON>(p0c0->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
					REQUIRE(std::get<POLYGON>(p0c1->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
					REQUIRE(std::get<POLYGON>(p0c2->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
					REQUIRE(std::get<POLYGON>(p0c3->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
					REQUIRE(p1e0c0->edge == b->get_current_state().polygons[XY][1]->edges[0].get());
					REQUIRE(p1e1c0->edge == b->get_current_state().polygons[XY][1]->edges[1].get());
					REQUIRE(p1e2c0->edge == b->get_current_state().polygons[XY][1]->edges[2].get());
					REQUIRE(p1e3c0->edge == b->get_current_state().polygons[XY][1]->edges[3].get());
				}
			}
		}

		WHEN("Polygons overlap by a corner") {
			std::unique_ptr<Board> b;
			{
				PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 1, 2 }, { 3, 2 }, { 3, 4 }, { 1, 4 }}), t));
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 2, 1 }, { 4, 1 }, { 4, 3 }, { 2, 3 }}), t));
				b = std::make_unique<Board>(std::move(tmp), Params(), t);
			}
			b->detect_edges_in_polygons();
			THEN("4 EDGE_IN_POLYGON conflicts should be registered") {
				REQUIRE(b->conflict_manager->get_current_state().all_edge_in_polygons[XY].size() == 4);
				for(std::shared_ptr<ConflictEdgeInPolygon> const& c : b->conflict_manager->get_current_state().all_edge_in_polygons[XY])
					REQUIRE(c->kind == Conflict::Kind::EDGE_IN_POLYGON);
				AND_THEN("The registered conflicts should be between the right edges and polygons") {
					bool is_first_registered = false;
					bool is_second_registered = false;
					bool is_third_registered = false;
					bool is_fourth_registered = false;
					for(std::shared_ptr<ConflictEdgeInPolygon> const& c : b->conflict_manager->get_current_state().all_edge_in_polygons[XY]) {
						if(c->edge == b->get_current_state().polygons[XY][0]->edges[1].get()) {
							is_first_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][1].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 2, 2 }, { 3, 2 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][0]->edges[2].get()) {
							is_second_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][1].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 3, 2 }, { 3, 3 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[0].get()) {
							is_third_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 2, 3 }, { 2, 2 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[3].get()) {
							is_fourth_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 3, 3 }, { 2, 3 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						}
					}
					REQUIRE(is_first_registered);
					REQUIRE(is_second_registered);
					REQUIRE(is_third_registered);
					REQUIRE(is_fourth_registered);
				}
				AND_THEN("The edges and polygons should register the conflicts") {
					REQUIRE(b->get_current_state().polygons[XY][0]->edges[1]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][0]->edges[2]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[0]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[3]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][0]->get_current_state().conflicts.size() == 2);
					REQUIRE(b->get_current_state().polygons[XY][1]->get_current_state().conflicts.size() == 2);

					auto p0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->get_current_state().conflicts[0]);
					auto p0c1 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->get_current_state().conflicts[1]);
					auto p1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->get_current_state().conflicts[0]);
					auto p1c1 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->get_current_state().conflicts[1]);
					auto p0e1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->edges[1]->get_current_state().conflicts[0]);
					auto p0e2c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->edges[2]->get_current_state().conflicts[0]);
					auto p1e0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->edges[0]->get_current_state().conflicts[0]);
					auto p1e3c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->edges[3]->get_current_state().conflicts[0]);
					REQUIRE(std::get<POLYGON>(p0c0->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
					REQUIRE(std::get<POLYGON>(p0c1->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
					REQUIRE(std::get<POLYGON>(p1c0->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][1].get());
					REQUIRE(std::get<POLYGON>(p1c1->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][1].get());
					REQUIRE(p0e1c0->edge == b->get_current_state().polygons[XY][0]->edges[1].get());
					REQUIRE(p0e2c0->edge == b->get_current_state().polygons[XY][0]->edges[2].get());
					REQUIRE(p1e0c0->edge == b->get_current_state().polygons[XY][1]->edges[0].get());
					REQUIRE(p1e3c0->edge == b->get_current_state().polygons[XY][1]->edges[3].get());
				}
			}
		}

		WHEN("One half of a polygon is totally inside the other polygon") {
			std::unique_ptr<Board> b;
			{
				PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 1, 2 }, { 4, 2 }, { 4, 4 }, { 1, 4 }}), t));
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 2, 1 }, { 3, 1 }, { 3, 3 }, { 2, 3 }}), t));
				b = std::make_unique<Board>(std::move(tmp), Params(), t);
			}
			b->detect_edges_in_polygons();
			THEN("4 EDGE_IN_POLYGON conflicts should be registered") {
				REQUIRE(b->conflict_manager->get_current_state().all_edge_in_polygons[XY].size() == 4);
				for(std::shared_ptr<ConflictEdgeInPolygon> const& c : b->conflict_manager->get_current_state().all_edge_in_polygons[XY])
					REQUIRE(c->kind == Conflict::Kind::EDGE_IN_POLYGON);
				AND_THEN("The registered conflicts should be between the right edges and polygons") {
					bool is_first_registered = false;
					bool is_second_registered = false;
					bool is_third_registered = false;
					bool is_fourth_registered = false;
					for(std::shared_ptr<ConflictEdgeInPolygon> const& c : b->conflict_manager->get_current_state().all_edge_in_polygons[XY]) {
						if(c->edge == b->get_current_state().polygons[XY][0]->edges[1].get()) {
							is_first_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][1].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 2, 2 }, { 3, 2 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[0].get()) {
							is_second_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 2, 3 }, { 2, 2 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[2].get()) {
							is_third_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 3, 2 }, { 3, 3 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[3].get()) {
							is_fourth_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 3, 3 }, { 2, 3 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						}
					}
					REQUIRE(is_first_registered);
					REQUIRE(is_second_registered);
					REQUIRE(is_third_registered);
					REQUIRE(is_fourth_registered);
				}
				AND_THEN("The edges and polygons should register the conflicts") {
					REQUIRE(b->get_current_state().polygons[XY][0]->edges[1]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[0]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[3]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][0]->get_current_state().conflicts.size() == 3);
					REQUIRE(b->get_current_state().polygons[XY][1]->get_current_state().conflicts.size() == 1);

					auto p0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->get_current_state().conflicts[0]);
					auto p0c1 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->get_current_state().conflicts[1]);
					auto p0c2 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->get_current_state().conflicts[2]);
					auto p1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->get_current_state().conflicts[0]);
					auto p0e1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->edges[1]->get_current_state().conflicts[0]);
					auto p1e0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->edges[0]->get_current_state().conflicts[0]);
					auto p1e2c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().conflicts[0]);
					auto p1e3c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->edges[3]->get_current_state().conflicts[0]);
					REQUIRE(std::get<POLYGON>(p0c0->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
					REQUIRE(std::get<POLYGON>(p0c1->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
					REQUIRE(std::get<POLYGON>(p0c2->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
					REQUIRE(std::get<POLYGON>(p1c0->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][1].get());
					REQUIRE(p0e1c0->edge == b->get_current_state().polygons[XY][0]->edges[1].get());
					REQUIRE(p1e0c0->edge == b->get_current_state().polygons[XY][1]->edges[0].get());
					REQUIRE(p1e2c0->edge == b->get_current_state().polygons[XY][1]->edges[2].get());
					REQUIRE(p1e3c0->edge == b->get_current_state().polygons[XY][1]->edges[3].get());
				}
			}
		}

		WHEN("One half of a diagonal polygon is totally inside the other polygon") {
			std::unique_ptr<Board> b;
			{
				PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 1, 1 }, { 4, 1 }, { 4, 4 }, { 1, 4 }}), t));
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 2, 3 }, { 3, 2 }, { 5, 4 }, { 4, 5 }}), t));
				b = std::make_unique<Board>(std::move(tmp), Params(), t);
			}
			b->detect_edges_in_polygons();
			THEN("5 EDGE_IN_POLYGON conflicts should be registered") {
				REQUIRE(b->conflict_manager->get_current_state().all_edge_in_polygons[XY].size() == 5);
				for(std::shared_ptr<ConflictEdgeInPolygon> const& c : b->conflict_manager->get_current_state().all_edge_in_polygons[XY])
					REQUIRE(c->kind == Conflict::Kind::EDGE_IN_POLYGON);
				AND_THEN("The registered conflicts should be between the right edges and polygons") {
					bool is_first_registered = false;
					bool is_second_registered = false;
					bool is_third_registered = false;
					bool is_fourth_registered = false;
					bool is_fifth_registered = false;
					for(std::shared_ptr<ConflictEdgeInPolygon> const& c : b->conflict_manager->get_current_state().all_edge_in_polygons[XY]) {
						if(c->edge == b->get_current_state().polygons[XY][0]->edges[2].get()) {
							is_first_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][1].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 4, 3 }, { 4, 4 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][0]->edges[3].get()) {
							is_second_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][1].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 4, 4 }, { 3, 4 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[0].get()) {
							is_third_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 3, 4 }, { 2, 3 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[1].get()) {
							is_fourth_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 2, 3 }, { 3, 2 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[2].get()) {
							is_fifth_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 3, 2 }, { 4, 3 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						}
					}
					REQUIRE(is_first_registered);
					REQUIRE(is_second_registered);
					REQUIRE(is_third_registered);
					REQUIRE(is_fourth_registered);
					REQUIRE(is_fifth_registered);
				}
				AND_THEN("The edges and polygons should register the conflicts") {
					REQUIRE(b->get_current_state().polygons[XY][0]->edges[2]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][0]->edges[3]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[0]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[1]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][0]->get_current_state().conflicts.size() == 3);
					REQUIRE(b->get_current_state().polygons[XY][1]->get_current_state().conflicts.size() == 2);

					auto p0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->get_current_state().conflicts[0]);
					auto p0c1 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->get_current_state().conflicts[1]);
					auto p0c2 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->get_current_state().conflicts[2]);
					auto p1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->get_current_state().conflicts[0]);
					auto p1c1 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->get_current_state().conflicts[1]);
					auto p0e2c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->edges[2]->get_current_state().conflicts[0]);
					auto p0e3c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->edges[3]->get_current_state().conflicts[0]);
					auto p1e0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->edges[0]->get_current_state().conflicts[0]);
					auto p1e1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->edges[1]->get_current_state().conflicts[0]);
					auto p1e2c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().conflicts[0]);
					REQUIRE(std::get<POLYGON>(p0c0->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
					REQUIRE(std::get<POLYGON>(p0c1->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
					REQUIRE(std::get<POLYGON>(p0c2->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
					REQUIRE(std::get<POLYGON>(p1c0->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][1].get());
					REQUIRE(std::get<POLYGON>(p1c1->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][1].get());
					REQUIRE(p0e2c0->edge == b->get_current_state().polygons[XY][0]->edges[2].get());
					REQUIRE(p0e3c0->edge == b->get_current_state().polygons[XY][0]->edges[3].get());
					REQUIRE(p1e0c0->edge == b->get_current_state().polygons[XY][1]->edges[0].get());
					REQUIRE(p1e1c0->edge == b->get_current_state().polygons[XY][1]->edges[1].get());
					REQUIRE(p1e2c0->edge == b->get_current_state().polygons[XY][1]->edges[2].get());
				}
			}
		}

		WHEN("Polygons touch each other by an edge") {
			std::unique_ptr<Board> b;
			{
				PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 1, 2 }, { 4, 2 }, { 4, 4 }, { 1, 4 }}), t));
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 2, 1 }, { 3, 1 }, { 3, 2 }, { 2, 2 }}), t));
				b = std::make_unique<Board>(std::move(tmp), Params(), t);
			}
			b->detect_edges_in_polygons();
			THEN("2 EDGE_IN_POLYGON conflicts should be registered") {
				REQUIRE(b->conflict_manager->get_current_state().all_edge_in_polygons[XY].size() == 2);
				for(std::shared_ptr<ConflictEdgeInPolygon> const& c : b->conflict_manager->get_current_state().all_edge_in_polygons[XY])
					REQUIRE(c->kind == Conflict::Kind::EDGE_IN_POLYGON);
				AND_THEN("The registered conflicts should be between the right edges and polygons") {
					bool is_first_registered = false;
					bool is_second_registered = false;
					for(std::shared_ptr<ConflictEdgeInPolygon> const& c : b->conflict_manager->get_current_state().all_edge_in_polygons[XY]) {
						if(c->edge == b->get_current_state().polygons[XY][0]->edges[1].get()) {
							is_first_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][1].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 2, 2 }, { 3, 2 }));
							REQUIRE(*std::get<EDGE>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][1]->edges[3].get());
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[3].get()) {
							is_second_registered = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 3, 2 }, { 2, 2 }));
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 2, 2 }, { 3, 2 }));
							REQUIRE(*std::get<EDGE>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0]->edges[1].get());
						}
					}
					REQUIRE(is_first_registered);
					REQUIRE(is_second_registered);
				}
				AND_THEN("The edges and polygons should register the conflicts") {
					REQUIRE(b->get_current_state().polygons[XY][0]->edges[1]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[3]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][0]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][1]->get_current_state().conflicts.size() == 1);

					auto p0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->get_current_state().conflicts[0]);
					auto p1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->get_current_state().conflicts[0]);
					auto p0e1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][0]->edges[1]->get_current_state().conflicts[0]);
					auto p1e3c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->get_current_state().polygons[XY][1]->edges[3]->get_current_state().conflicts[0]);
					REQUIRE(std::get<POLYGON>(p0c0->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
					REQUIRE(std::get<POLYGON>(p1c0->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][1].get());
					REQUIRE(p0e1c0->edge == b->get_current_state().polygons[XY][0]->edges[1].get());
					REQUIRE(p1e3c0->edge == b->get_current_state().polygons[XY][1]->edges[3].get());
				}
			}
		}

		WHEN("Polygons touch each other by a corner") {
			std::unique_ptr<Board> b;
			{
				PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 1, 2 }, { 2, 2 }, { 2, 3 }, { 1, 3 }}), t));
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 2, 1 }, { 3, 1 }, { 3, 2 }, { 2, 2 }}), t));
				b = std::make_unique<Board>(std::move(tmp), Params(), t);
			}
			b->detect_edges_in_polygons();
			THEN("There should not be any conflict registered") {
				REQUIRE_FALSE(b->conflict_manager->get_current_state().all_edge_in_polygons[XY].size());
			}
		}

		WHEN("Polygons are apart") {
			std::unique_ptr<Board> b;
			{
				PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 1, 1 }, { 2, 1 }, { 2, 2 }, { 2, 1 }}), t));
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 3, 3 }, { 3, 4 }, { 4, 4 }, { 4, 3 }}), t));
				b = std::make_unique<Board>(std::move(tmp), Params(), t);
			}
			b->detect_edges_in_polygons();
			THEN("There should not be any conflict registered") {
				REQUIRE_FALSE(b->conflict_manager->get_current_state().all_edge_in_polygons[XY].size());
			}
		}
	}

	GIVEN("A board holding two complex polygons") {
		WHEN("Polygons overlap by many segments but not by Z placement") {
			std::unique_ptr<Board> b;
			{
				PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 1 }, from_init_list<Point>({
					{ 1, 1 }, { 10, 1 }, { 10, 10 }, { 1, 10 }}), t));
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 2, 3 }, from_init_list<Point>({
					{ 11, 4 }, { 9, 3 }, { 13, 2 }, { 5, 2 },
					{ 5, 11 }, { 4, 11.3 }, { 3, 10 }, { 2, 10 },
					{ 2.3, 12 }, { 6, 11.3 }, { 6, 9 }, { 10, 10 },
					{ 11, 9 }, { 9, 7 }, { 10, 6.3 }, { 10, 5 }}), t));
				b = std::make_unique<Board>(std::move(tmp), Params(), t);
			}
			b->detect_edges_in_polygons();
			THEN("There should not be any conflict registered") {
				REQUIRE_FALSE(b->conflict_manager->get_current_state().all_edge_in_polygons[XY].size());
			}
		}

		WHEN("Polygons overlap by many segments, by Z placement and are of different priority") {
			THEN("In one case") {
				std::unique_ptr<Board> b;
				{
					PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
					tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 1, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({
						{ 1, 1 }, { 10, 1 }, { 10, 10 }, { 1, 10 }}), t));
					tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 2, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({
						{ 11, 4 }, { 9, 3 }, { 13, 2 }, { 5, 2 },
						{ 5, 11 }, { 4, 11.3 }, { 3, 10 }, { 2, 10 },
						{ 2.3, 12 }, { 6, 11.3 }, { 6, 9 }, { 10, 10 },
						{ 11, 9 }, { 9, 7 }, { 10, 6.3 }, { 10, 5 }}), t));
					b = std::make_unique<Board>(std::move(tmp), Params(), t);
				}
				b->detect_edges_in_polygons();
				THEN("2 EDGE_IN_POLYGON conflicts should be registered") {
					REQUIRE(b->conflict_manager->get_current_state().all_edge_in_polygons[XY].size() == 2);
					for(std::shared_ptr<ConflictEdgeInPolygon> const& c : b->conflict_manager->get_current_state().all_edge_in_polygons[XY])
						REQUIRE(c->kind == Conflict::Kind::EDGE_IN_POLYGON);
					AND_THEN("The registered conflicts should be between the right edges and polygons") {
						std::array<bool, 2> are_conflicts_registered;
						are_conflicts_registered.fill(false);
						for(std::shared_ptr<ConflictEdgeInPolygon> const& c : b->conflict_manager->get_current_state().all_edge_in_polygons[XY]) {
							if(c->edge == b->get_current_state().polygons[XY][0]->edges[2].get()) {
								are_conflicts_registered[0] = true;
								REQUIRE(c->get_current_state().overlaps.size() == 4);
								std::array<bool, 4> are_overlaps_registered;
								are_overlaps_registered.fill(false);
								for(auto const& overlap : c->get_current_state().overlaps) {
									if(std::get<POLYGON>(overlap) == b->get_current_state().polygons[XY][1].get()
									&& std::get<RANGE>(overlap) == Range({ 10, 2 }, { 10, 2.75 })
									&& std::get<EDGE>(overlap) == std::nullopt)
										are_overlaps_registered[0] = true;
									else if(std::get<POLYGON>(overlap) == b->get_current_state().polygons[XY][1].get()
									     && std::get<RANGE>(overlap) == Range({ 10, 3.5 }, { 10, 5 })
									     && std::get<EDGE>(overlap) == std::nullopt)
										are_overlaps_registered[1] = true;
									else if(std::get<POLYGON>(overlap) == b->get_current_state().polygons[XY][1].get()
									     && std::get<RANGE>(overlap) == Range({ 10, 5 }, { 10, 6.3 })
									     && std::get<EDGE>(overlap) == b->get_current_state().polygons[XY][1]->edges[15].get())
										are_overlaps_registered[2] = true;
									else if(std::get<POLYGON>(overlap) == b->get_current_state().polygons[XY][1].get()
									     && std::get<RANGE>(overlap) == Range({ 10, 8 }, { 10, 10 })
									     && std::get<EDGE>(overlap) == std::nullopt)
										are_overlaps_registered[3] = true;
								}
								for(bool is_overlap_registered : are_overlaps_registered)
									REQUIRE(is_overlap_registered);
							} else if(c->edge == b->get_current_state().polygons[XY][0]->edges[3].get()) {
								are_conflicts_registered[1] = true;
								REQUIRE(c->get_current_state().overlaps.size() == 2);
								std::array<bool, 2> are_overlaps_registered;
								are_overlaps_registered.fill(false);
								for(auto const& overlap : c->get_current_state().overlaps) {
									if(std::get<POLYGON>(overlap) == b->get_current_state().polygons[XY][1].get()
									&& std::get<RANGE>(overlap) == Range({ 5, 10 }, { 6, 10 })
									&& std::get<EDGE>(overlap) == std::nullopt)
										are_overlaps_registered[0] = true;
									else if(std::get<POLYGON>(overlap) == b->get_current_state().polygons[XY][1].get()
									     && std::get<RANGE>(overlap) == Range({ 3, 10 }, { 2, 10 })
									     && std::get<EDGE>(overlap) == b->get_current_state().polygons[XY][1]->edges[7].get())
										are_overlaps_registered[1] = true;
								}
								for(bool is_overlap_registered : are_overlaps_registered)
									REQUIRE(is_overlap_registered);
							}
						}
						for(bool is_conflict_registered : are_conflicts_registered)
							REQUIRE(is_conflict_registered);
					}
				}
			}
			THEN("In the other case") {
				std::unique_ptr<Board> b;
				{
					PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
					tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 2, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({
						{ 1, 1 }, { 10, 1 }, { 10, 10 }, { 1, 10 }}), t));
					tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 1, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({
						{ 11, 4 }, { 9, 3 }, { 13, 2 }, { 5, 2 },
						{ 5, 11 }, { 4, 11.3 }, { 3, 10 }, { 2, 10 },
						{ 2.3, 12 }, { 6, 11.3 }, { 6, 9 }, { 10, 10 },
						{ 11, 9 }, { 9, 7 }, { 10, 6.3 }, { 10, 5 }}), t));
					b = std::make_unique<Board>(std::move(tmp), Params(), t);
				}
				b->detect_edges_in_polygons();
				THEN("10 EDGE_IN_POLYGON conflicts should be registered") {
					REQUIRE(b->conflict_manager->get_current_state().all_edge_in_polygons[XY].size() == 10);
					for(std::shared_ptr<ConflictEdgeInPolygon> const& c : b->conflict_manager->get_current_state().all_edge_in_polygons[XY])
						REQUIRE(c->kind == Conflict::Kind::EDGE_IN_POLYGON);
					AND_THEN("The registered conflicts should be between the right edges and polygons") {
						std::array<bool, 10> are_conflicts_registered;
						are_conflicts_registered.fill(false);
						for(std::shared_ptr<ConflictEdgeInPolygon> const& c : b->conflict_manager->get_current_state().all_edge_in_polygons[XY]) {
							if(c->edge == b->get_current_state().polygons[XY][1]->edges[1].get()) {
								are_conflicts_registered[0] = true;
								REQUIRE(c->get_current_state().overlaps.size() == 1);
								REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
								REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 10, 3.5 }, { 9, 3 }));
								REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
							} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[2].get()) {
								are_conflicts_registered[1] = true;
								REQUIRE(c->get_current_state().overlaps.size() == 1);
								REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
								REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 9, 3 }, { 10, 2.75 }));
								REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
							} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[3].get()) {
								are_conflicts_registered[2] = true;
								REQUIRE(c->get_current_state().overlaps.size() == 1);
								REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
								REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 10, 2 }, { 5, 2 }));
								REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
							} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[4].get()) {
								are_conflicts_registered[3] = true;
								REQUIRE(c->get_current_state().overlaps.size() == 1);
								REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
								REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 5, 2 }, { 5, 10 }));
								REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
							} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[7].get()) {
								are_conflicts_registered[4] = true;
								REQUIRE(c->get_current_state().overlaps.size() == 1);
								REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
								REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 3, 10 }, { 2, 10 }));
								REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0]->edges[3].get());
							} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[10].get()) {
								are_conflicts_registered[5] = true;
								REQUIRE(c->get_current_state().overlaps.size() == 1);
								REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
								REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 6, 10 }, { 6, 9 }));
								REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
							} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[11].get()) {
								are_conflicts_registered[6] = true;
								REQUIRE(c->get_current_state().overlaps.size() == 1);
								REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
								REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 6, 9 }, { 10, 10 }));
								REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
							} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[13].get()) {
								are_conflicts_registered[7] = true;
								REQUIRE(c->get_current_state().overlaps.size() == 1);
								REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
								REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 10, 8 }, { 9, 7 }));
								REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
							} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[14].get()) {
								are_conflicts_registered[8] = true;
								REQUIRE(c->get_current_state().overlaps.size() == 1);
								REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
								REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 9, 7 }, { 10, 6.3 }));
								REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
							} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[15].get()) {
								are_conflicts_registered[9] = true;
								REQUIRE(c->get_current_state().overlaps.size() == 1);
								REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
								REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 10, 6.3 }, { 10, 5 }));
								REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0]->edges[2].get());
							}
						}
						for(bool is_conflict_registered : are_conflicts_registered)
							REQUIRE(is_conflict_registered);
					}
				}
			}
		}

		WHEN("Polygons overlap by many segments, by Z placement and are of same priority") {
			std::unique_ptr<Board> b;
			{
				PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({
					{ 1, 1 }, { 10, 1 }, { 10, 10 }, { 1, 10 }}), t));
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({
					{ 11, 4 }, { 9, 3 }, { 13, 2 }, { 5, 2 },
					{ 5, 11 }, { 4, 11.3 }, { 3, 10 }, { 2, 10 },
					{ 2.3, 12 }, { 6, 11.3 }, { 6, 9 }, { 10, 10 },
					{ 11, 9 }, { 9, 7 }, { 10, 6.3 }, { 10, 5 }}), t));
				b = std::make_unique<Board>(std::move(tmp), Params(), t);
			}
			b->detect_edges_in_polygons();
			THEN("12 EDGE_IN_POLYGON conflicts should be registered") {
				REQUIRE(b->conflict_manager->get_current_state().all_edge_in_polygons[XY].size() == 12);
				for(std::shared_ptr<ConflictEdgeInPolygon> const& c : b->conflict_manager->get_current_state().all_edge_in_polygons[XY])
					REQUIRE(c->kind == Conflict::Kind::EDGE_IN_POLYGON);
				AND_THEN("The registered conflicts should be between the right edges and polygons") {
					std::array<bool, 12> are_conflicts_registered;
					are_conflicts_registered.fill(false);
					for(std::shared_ptr<ConflictEdgeInPolygon> const& c : b->conflict_manager->get_current_state().all_edge_in_polygons[XY]) {
						if(c->edge == b->get_current_state().polygons[XY][0]->edges[2].get()) {
							are_conflicts_registered[0] = true;
							REQUIRE(c->get_current_state().overlaps.size() == 4);
							std::array<bool, 4> are_overlaps_registered;
							are_overlaps_registered.fill(false);
							for(auto const& overlap : c->get_current_state().overlaps) {
								if(std::get<POLYGON>(overlap) == b->get_current_state().polygons[XY][1].get()
								&& std::get<RANGE>(overlap) == Range({ 10, 2 }, { 10, 2.75 })
								&& std::get<EDGE>(overlap) == std::nullopt)
									are_overlaps_registered[0] = true;
								else if(std::get<POLYGON>(overlap) == b->get_current_state().polygons[XY][1].get()
								     && std::get<RANGE>(overlap) == Range({ 10, 3.5 }, { 10, 5 })
								     && std::get<EDGE>(overlap) == std::nullopt)
									are_overlaps_registered[1] = true;
								else if(std::get<POLYGON>(overlap) == b->get_current_state().polygons[XY][1].get()
								     && std::get<RANGE>(overlap) == Range({ 10, 5 }, { 10, 6.3 })
								     && std::get<EDGE>(overlap) == b->get_current_state().polygons[XY][1]->edges[15].get())
									are_overlaps_registered[2] = true;
								else if(std::get<POLYGON>(overlap) == b->get_current_state().polygons[XY][1].get()
								     && std::get<RANGE>(overlap) == Range({ 10, 8 }, { 10, 10 })
								     && std::get<EDGE>(overlap) == std::nullopt)
									are_overlaps_registered[3] = true;
							}
							for(bool is_overlap_registered : are_overlaps_registered)
								REQUIRE(is_overlap_registered);
						} else if(c->edge == b->get_current_state().polygons[XY][0]->edges[3].get()) {
							are_conflicts_registered[1] = true;
							REQUIRE(c->get_current_state().overlaps.size() == 2);
							std::array<bool, 2> are_overlaps_registered;
							are_overlaps_registered.fill(false);
							for(auto const& overlap : c->get_current_state().overlaps) {
								if(std::get<POLYGON>(overlap) == b->get_current_state().polygons[XY][1].get()
								&& std::get<RANGE>(overlap) == Range({ 5, 10 }, { 6, 10 })
								&& std::get<EDGE>(overlap) == std::nullopt)
									are_overlaps_registered[0] = true;
								else if(std::get<POLYGON>(overlap) == b->get_current_state().polygons[XY][1].get()
								     && std::get<RANGE>(overlap) == Range({ 3, 10 }, { 2, 10 })
								     && std::get<EDGE>(overlap) == b->get_current_state().polygons[XY][1]->edges[7].get())
									are_overlaps_registered[1] = true;
							}
							for(bool is_overlap_registered : are_overlaps_registered)
								REQUIRE(is_overlap_registered);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[1].get()) {
							are_conflicts_registered[2] = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 10, 3.5 }, { 9, 3 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[2].get()) {
							are_conflicts_registered[3] = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 9, 3 }, { 10, 2.75 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[3].get()) {
							are_conflicts_registered[4] = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 10, 2 }, { 5, 2 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[4].get()) {
							are_conflicts_registered[5] = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 5, 2 }, { 5, 10 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[7].get()) {
							are_conflicts_registered[6] = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 3, 10 }, { 2, 10 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0]->edges[3].get());
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[10].get()) {
							are_conflicts_registered[7] = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 6, 10 }, { 6, 9 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[11].get()) {
							are_conflicts_registered[8] = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 6, 9 }, { 10, 10 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[13].get()) {
							are_conflicts_registered[9] = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 10, 8 }, { 9, 7 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[14].get()) {
							are_conflicts_registered[10] = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 9, 7 }, { 10, 6.3 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == std::nullopt);
						} else if(c->edge == b->get_current_state().polygons[XY][1]->edges[15].get()) {
							are_conflicts_registered[11] = true;
							REQUIRE(c->get_current_state().overlaps.size() == 1);
							REQUIRE(std::get<POLYGON>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0].get());
							REQUIRE(std::get<RANGE>(c->get_current_state().overlaps[0]) == Range({ 10, 6.3 }, { 10, 5 }));
							REQUIRE(std::get<EDGE>(c->get_current_state().overlaps[0]) == b->get_current_state().polygons[XY][0]->edges[2].get());
						}
					}
					for(bool is_conflict_registered : are_conflicts_registered)
						REQUIRE(is_conflict_registered);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Board::detect_colinear_edges()", "[board]") {
	Timepoint* t = Caretaker::singleton().get_history_root();
	auto material = std::make_shared<Material>(Material::Type::CONDUCTOR, "");
	GIVEN("A board holding three polygons") {
		WHEN("Three polygons share a colinear vertical edge") {
			std::unique_ptr<Board> b;
			{
				PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 1, 1 }, { 2, 1 }, { 2, 2 }, { 1, 2 }}), t));
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 0.5, 3 }, { 2, 3 }, { 2, 4 }, { 0.5, 4 }}), t));
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 3, 5 }, { 2, 5 }, { 2, 6 }, { 3, 6 }}), t));
				b = std::make_unique<Board>(std::move(tmp), Params(), t);
			}
			b->detect_colinear_edges();
			THEN("A COLINEAR_EDGES conflict should be registered") {
				REQUIRE(b->conflict_manager->get_current_state().all_colinear_edges[X].size() == 1);
				REQUIRE(b->conflict_manager->get_current_state().all_colinear_edges[X].back()->kind == Conflict::Kind::COLINEAR_EDGES);
				AND_THEN("The registered conflict should be between tree edges") {
					ConflictColinearEdges* c = b->conflict_manager->get_current_state().all_colinear_edges[X].back().get();
					REQUIRE(c->get_current_state().edges.size() == 3);
					bool is_first_registered = false;
					bool is_second_registered = false;
					bool is_third_registered = false;
					for(Edge* edge : c->get_current_state().edges) {
						if(edge == b->get_current_state().polygons[XY][0]->edges[2].get())
							is_first_registered = true;
						if(edge == b->get_current_state().polygons[XY][1]->edges[2].get())
							is_second_registered = true;
						if(edge == b->get_current_state().polygons[XY][2]->edges[2].get())
							is_third_registered = true;
					}
					REQUIRE(is_first_registered);
					REQUIRE(is_second_registered);
					REQUIRE(is_third_registered);
				}
				AND_THEN("The three edges should register the conflict") {
					ConflictColinearEdges* c = b->conflict_manager->get_current_state().all_colinear_edges[X].back().get();
					REQUIRE(b->get_current_state().polygons[XY][0]->edges[2]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][2]->edges[2]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][0]->edges[2]->get_current_state().conflicts[0] == c);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().conflicts[0] == c);
					REQUIRE(b->get_current_state().polygons[XY][2]->edges[2]->get_current_state().conflicts[0] == c);
				}
			}
		}

		WHEN("Three polygons share a colinear horizontal edge") {
			std::unique_ptr<Board> b;
			{
				PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 1, 1 }, { 2, 1 }, { 2, 2 }, { 1, 2 }}), t));
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 3, 0.5 }, { 3, 2 }, { 4, 2 }, { 4, 0.5 }}), t));
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 5, 3 }, { 5, 2 }, { 6, 2 }, { 6, 3 }}), t));
				b = std::make_unique<Board>(std::move(tmp), Params(), t);
			}
			b->detect_colinear_edges();
			THEN("A COLINEAR_EDGES conflict should be registered") {
				REQUIRE(b->conflict_manager->get_current_state().all_colinear_edges[Y].size() == 1);
				REQUIRE(b->conflict_manager->get_current_state().all_colinear_edges[Y].back()->kind == Conflict::Kind::COLINEAR_EDGES);
				AND_THEN("The registered conflict should be between tree edges") {
					ConflictColinearEdges* c = b->conflict_manager->get_current_state().all_colinear_edges[Y].back().get();
					REQUIRE(c->get_current_state().edges.size() == 3);
					bool is_first_registered = false;
					bool is_second_registered = false;
					bool is_third_registered = false;
					for(Edge* edge : c->get_current_state().edges) {
						if(edge == b->get_current_state().polygons[XY][0]->edges[3].get())
							is_first_registered = true;
						if(edge == b->get_current_state().polygons[XY][1]->edges[2].get())
							is_second_registered = true;
						if(edge == b->get_current_state().polygons[XY][2]->edges[2].get())
							is_third_registered = true;
					}
					REQUIRE(is_first_registered);
					REQUIRE(is_second_registered);
					REQUIRE(is_third_registered);
				}
				AND_THEN("The three edges should register the conflict") {
					ConflictColinearEdges* c = b->conflict_manager->get_current_state().all_colinear_edges[Y].back().get();
					REQUIRE(b->get_current_state().polygons[XY][0]->edges[3]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][2]->edges[2]->get_current_state().conflicts.size() == 1);
					REQUIRE(b->get_current_state().polygons[XY][0]->edges[3]->get_current_state().conflicts[0] == c);
					REQUIRE(b->get_current_state().polygons[XY][1]->edges[2]->get_current_state().conflicts[0] == c);
					REQUIRE(b->get_current_state().polygons[XY][2]->edges[2]->get_current_state().conflicts[0] == c);
				}
			}
		}

		WHEN("Three polygons share a colinear diagonal edge") {
			std::unique_ptr<Board> b;
			{
				PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 1, 1 }, { 1, 2 }, { 2, 2 }}), t));
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 3, 3 }, { 3, 4 }, { 4, 4 }}), t));
				tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 5, 5 }, { 5, 6 }, { 6, 6 }}), t));
				b = std::make_unique<Board>(std::move(tmp), Params(), t);
			}
			b->detect_colinear_edges();
			THEN("There should not be any conflict registered") {
				REQUIRE(b->conflict_manager->get_current_state().all_colinear_edges[X].empty());
				REQUIRE(b->conflict_manager->get_current_state().all_colinear_edges[Y].empty());
				REQUIRE(b->conflict_manager->get_current_state().all_colinear_edges[Z].empty());
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Board::detect_individual_edges()", "[board]") {
	Timepoint* t = Caretaker::singleton().get_history_root();
	auto material = std::make_shared<Material>(Material::Type::CONDUCTOR, "");
	ConflictColinearEdges cce(X, nullptr, nullptr, t);
	ConflictEdgeInPolygon ceip(XY, nullptr, nullptr, {{ 0, 0 }, { 0, 0 }}, std::nullopt, t);
	GIVEN("Some conflicting edges and some non conflicting edges, with some to revert") {
		std::unique_ptr<Board> b;
		{
			PlaneSpace<std::vector<std::shared_ptr<Polygon>>> tmp;
			tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 1, 1 }, { 1, 2 }, { 2, 2 }}), t));
			tmp[XY].push_back(std::make_shared<Polygon>(XY, material, "", 0, Polygon::RangeZ { 0, 0 }, from_init_list<Point>({{ 3, 3 }, { 3, 4 }, { 4, 4 }}), t));
			b = std::make_unique<Board>(std::move(tmp), Params(), t);
		}
		REQUIRE(b->get_current_state().edges[XY].size() == 6);
		auto state_e0 = b->get_current_state().edges[XY][0]->get_current_state();
		auto state_e1 = b->get_current_state().edges[XY][1]->get_current_state();
		auto state_e4 = b->get_current_state().edges[XY][4]->get_current_state();
		auto state_e5 = b->get_current_state().edges[XY][5]->get_current_state();
		state_e0.conflicts.push_back(nullptr);
		state_e4.conflicts.push_back(&cce);
		state_e5.conflicts.push_back(&ceip);
		state_e1.to_reverse = true;
		b->get_current_state().edges[XY][0]->set_next_state(state_e0);
		b->get_current_state().edges[XY][1]->set_next_state(state_e1);
		b->get_current_state().edges[XY][4]->set_next_state(state_e4);
		b->get_current_state().edges[XY][5]->set_next_state(state_e5);
		b->detect_individual_edges();
		THEN("Should add a thirds meshline policy in the meshline policy manager for each orthogonal and non colinear edge") {
			REQUIRE(b->get_current_state().edges[XY][0]->get_current_state().conflicts.size() == 1);
			REQUIRE(b->get_current_state().edges[XY][1]->get_current_state().conflicts.size() == 0);
			REQUIRE(b->get_current_state().edges[XY][2]->get_current_state().conflicts.size() == 0);
			REQUIRE(b->get_current_state().edges[XY][3]->get_current_state().conflicts.size() == 0);
			REQUIRE(b->get_current_state().edges[XY][4]->get_current_state().conflicts.size() == 1);
			REQUIRE(b->get_current_state().edges[XY][5]->get_current_state().conflicts.size() == 1);
			REQUIRE_FALSE(b->get_current_state().edges[XY][0]->get_current_state().meshline_policy);
			REQUIRE(b->get_current_state().edges[XY][1]->get_current_state().meshline_policy);
			REQUIRE(b->get_current_state().edges[XY][2]->get_current_state().meshline_policy);
			REQUIRE_FALSE(b->get_current_state().edges[XY][3]->get_current_state().meshline_policy);
			REQUIRE_FALSE(b->get_current_state().edges[XY][4]->get_current_state().meshline_policy);
			REQUIRE(b->get_current_state().edges[XY][5]->get_current_state().meshline_policy);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[X].size() == 1);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[X][0].get() == b->get_current_state().edges[XY][1]->get_current_state().meshline_policy);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[X][0]->get_current_state().origins.size() == 1);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[X][0]->get_current_state().origins[0] == b->get_current_state().edges[XY][1]);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[X][0]->get_current_state().policy == MeshlinePolicy::Policy::THIRDS);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[X][0]->get_current_state().normal == MeshlinePolicy::Normal::MAX);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[Y].size() == 2);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[Y][0].get() == b->get_current_state().edges[XY][2]->get_current_state().meshline_policy);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[Y][0]->get_current_state().origins.size() == 1);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[Y][0]->get_current_state().origins[0] == b->get_current_state().edges[XY][2]);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[Y][0]->get_current_state().policy == MeshlinePolicy::Policy::THIRDS);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[Y][0]->get_current_state().normal == MeshlinePolicy::Normal::MAX);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[Y][1].get() == b->get_current_state().edges[XY][5]->get_current_state().meshline_policy);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[Y][1]->get_current_state().origins.size() == 1);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[Y][1]->get_current_state().origins[0] == b->get_current_state().edges[XY][5]);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[Y][1]->get_current_state().policy == MeshlinePolicy::Policy::THIRDS);
			REQUIRE(b->line_policy_manager->get_current_state().line_policies[Y][1]->get_current_state().normal == MeshlinePolicy::Normal::MAX);
		}
	}
}
