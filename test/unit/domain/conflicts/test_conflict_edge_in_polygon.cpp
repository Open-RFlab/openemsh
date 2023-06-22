///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/point.hpp"
#include "domain/geometrics/polygon.hpp"
#include "domain/meshline_policy_manager.hpp"

#include "domain/conflicts/conflict_edge_in_polygon.hpp"

/// @test void sort_overlaps_by_p0_by_vector_orientation(vector<Overlap>& overlaps, Point const& vector)
/// @test void ConflictEdgeInPolygon::auto_solve(MeshlinePolicyManager&)
///*****************************************************************************

using namespace domain;

//******************************************************************************
SCENARIO("void sort_overlaps_by_p0_by_vector_orientation(vector<Overlap>& overlaps, Point const& vector)", "[conflict_edge_in_polygon]") {
	GIVEN("Unordered colinear overlaps in (+, 0) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, Range({ 4, 1 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 2, 1 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 3, 1 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 1, 1 }, { 0, 0 }), std::nullopt);
		Point v(std::get<RANGE>(overlaps[0]).p0() - std::get<RANGE>(overlaps[3]).p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(std::get<RANGE>(overlaps[0]) == Range({ 1, 1 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[1]) == Range({ 2, 1 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[2]) == Range({ 3, 1 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[3]) == Range({ 4, 1 }, { 0, 0 }));
		}
	}

	GIVEN("Unordered colinear overlaps in (0, +) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, Range({ 1, 4 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 1, 2 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 1, 3 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 1, 1 }, { 0, 0 }), std::nullopt);
		Point v(std::get<RANGE>(overlaps[0]).p0() - std::get<RANGE>(overlaps[3]).p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(std::get<RANGE>(overlaps[0]) == Range({ 1, 1 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[1]) == Range({ 1, 2 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[2]) == Range({ 1, 3 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[3]) == Range({ 1, 4 }, { 0, 0 }));
		}
	}

	GIVEN("Unordered colinear overlaps in (-, 0) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, Range({ 4, 1 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 2, 1 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 3, 1 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 1, 1 }, { 0, 0 }), std::nullopt);
		Point v(std::get<RANGE>(overlaps[3]).p0() - std::get<RANGE>(overlaps[0]).p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(std::get<RANGE>(overlaps[0]) == Range({ 4, 1 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[1]) == Range({ 3, 1 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[2]) == Range({ 2, 1 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[3]) == Range({ 1, 1 }, { 0, 0 }));
		}
	}

	GIVEN("Unordered colinear overlaps in (0, -) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, Range({ 1, 4 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 1, 2 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 1, 3 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 1, 1 }, { 0, 0 }), std::nullopt);
		Point v(std::get<RANGE>(overlaps[3]).p0() - std::get<RANGE>(overlaps[0]).p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(std::get<RANGE>(overlaps[0]) == Range({ 1, 4 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[1]) == Range({ 1, 3 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[2]) == Range({ 1, 2 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[3]) == Range({ 1, 1 }, { 0, 0 }));
		}
	}

	GIVEN("Unordered colinear overlaps in (+, +) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, Range({ 4, 4 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 2, 2 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 3, 3 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 1, 1 }, { 0, 0 }), std::nullopt);
		Point v(std::get<RANGE>(overlaps[0]).p0() - std::get<RANGE>(overlaps[3]).p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(std::get<RANGE>(overlaps[0]) == Range({ 1, 1 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[1]) == Range({ 2, 2 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[2]) == Range({ 3, 3 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[3]) == Range({ 4, 4 }, { 0, 0 }));
		}
	}

	GIVEN("Unordered colinear overlaps in (-, +) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, Range({ 1, 4 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 3, 2 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 2, 3 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 4, 1 }, { 0, 0 }), std::nullopt);
		Point v(std::get<RANGE>(overlaps[0]).p0() - std::get<RANGE>(overlaps[3]).p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(std::get<RANGE>(overlaps[0]) == Range({ 4, 1 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[1]) == Range({ 3, 2 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[2]) == Range({ 2, 3 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[3]) == Range({ 1, 4 }, { 0, 0 }));
		}
	}

	GIVEN("Unordered colinear overlaps in (-, -) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, Range({ 4, 4 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 2, 2 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 3, 3 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 1, 1 }, { 0, 0 }), std::nullopt);
		Point v(std::get<RANGE>(overlaps[3]).p0() - std::get<RANGE>(overlaps[0]).p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(std::get<RANGE>(overlaps[0]) == Range({ 4, 4 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[1]) == Range({ 3, 3 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[2]) == Range({ 2, 2 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[3]) == Range({ 1, 1 }, { 0, 0 }));
		}
	}

	GIVEN("Unordered colinear overlaps in (+, -) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, Range({ 1, 4 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 3, 2 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 2, 3 }, { 0, 0 }), std::nullopt);
		overlaps.emplace_back(nullptr, Range({ 4, 1 }, { 0, 0 }), std::nullopt);
		Point v(std::get<RANGE>(overlaps[3]).p0() - std::get<RANGE>(overlaps[0]).p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(std::get<RANGE>(overlaps[0]) == Range({ 1, 4 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[1]) == Range({ 2, 3 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[2]) == Range({ 3, 2 }, { 0, 0 }));
			REQUIRE(std::get<RANGE>(overlaps[3]) == Range({ 4, 1 }, { 0, 0 }));
		}
	}
}

//******************************************************************************
SCENARIO("void ConflictEdgeInPolygon::auto_solve(MeshlinePolicyManager&)", "[conflict_edge_in_polygon]") {
	Params params;
	MeshlinePolicyManager mpm(params, nullptr);
	GIVEN("A conflict about a vertical edge partially covered by some polygons (with a gap)") {
		Point e0(1, 1), e1(8, 1);
		Edge e(XY, &e0, &e1);
		e.normal = Normal::NONE;
		ConflictEdgeInPolygon ceip(XY, &e, nullptr, Range({ 1, 1 }, { 4, 1 }), std::nullopt);
		ceip.append(nullptr, Range({ 5, 1 }, { 8, 1 }), std::nullopt);
		ceip.auto_solve(mpm);
		THEN("The edge should be eligible to mesh") {
			REQUIRE(ceip.is_solved);
			REQUIRE(ceip.solution == &e);
			REQUIRE(e.to_mesh);
		}
	}

	GIVEN("A conflict about a vertical edge totally covered by some polygons") {
		Point e0(1, 1), e1(8, 1);
		Edge e(XY, &e0, &e1);
		e.normal = Normal::NONE;
		ConflictEdgeInPolygon ceip(XY, &e, nullptr, Range({ 1, 1 }, { 4, 1 }), std::nullopt);
		ceip.append(nullptr, Range({ 3, 1 }, { 6, 1 }), std::nullopt);
		ceip.append(nullptr, Range({ 2, 1 }, { 7, 1 }), std::nullopt);
		ceip.append(nullptr, Range({ 5, 1 }, { 8, 1 }), std::nullopt);
		ceip.auto_solve(mpm);
		THEN("The edge should not be eligible to mesh") {
			REQUIRE(ceip.is_solved);
			REQUIRE(ceip.solution == &e);
			REQUIRE_FALSE(e.to_mesh);
		}
	}

	GIVEN("A conflict about a vertical edge totally covered by some polygons that share an edge with opposed normal vector") {
		Point e0(1, 1), e1(8, 1);
		Point a0(0, 0), a1(0, 0);
		Point b0(0, 0), b1(0, 0);
		Edge e(XY, &e0, &e1), a(XY, &a0, &a1), b(XY, &b0, &b1);
		e.normal = Normal::XMIN;
		a.normal = Normal::XMAX;
		b.normal = Normal::XMAX;
		ConflictEdgeInPolygon ceip(XY, &e, nullptr, Range({ 1, 1 }, { 4, 1 }), std::nullopt);
		ceip.append(nullptr, Range({ 3, 1 }, { 6, 1 }), std::nullopt);
		ceip.append(nullptr, Range({ 2, 1 }, { 7, 1 }), &a);
		ceip.append(nullptr, Range({ 5, 1 }, { 8, 1 }), &b);
		ceip.auto_solve(mpm);
		THEN("The edge should not be eligible to mesh") {
			REQUIRE(ceip.is_solved);
			REQUIRE(ceip.solution == &e);
			REQUIRE_FALSE(e.to_mesh);
		}
	}

	GIVEN("A conflict about a vertical edge totally covered by some polygons that share an edge with the same normal vector") {
		Point e0(1, 1), e1(8, 1);
		Point a0(0, 0), a1(0, 0);
		Point b0(0, 0), b1(0, 0);
		Edge e(XY, &e0, &e1), a(XY, &a0, &a1), b(XY, &b0, &b1);
		e.normal = Normal::XMIN;
		a.normal = Normal::XMAX;
		b.normal = Normal::XMIN;
		ConflictEdgeInPolygon ceip(XY, &e, nullptr, Range({ 1, 1 }, { 4, 1 }), std::nullopt);
		ceip.append(nullptr, Range({ 3, 1 }, { 6, 1 }), std::nullopt);
		ceip.append(nullptr, Range({ 2, 1 }, { 7, 1 }), &a);
		ceip.append(nullptr, Range({ 5, 1 }, { 8, 1 }), &b);
		ceip.auto_solve(mpm);
		THEN("The edge should be eligible to mesh") {
			REQUIRE(ceip.is_solved);
			REQUIRE(ceip.solution == &e);
			REQUIRE(e.to_mesh);
		}
	}

	GIVEN("A conflict about an horizontal edge partially covered by some polygons (with a gap)") {
		Point e0(1, 1), e1(1, 8);
		Edge e(XY, &e0, &e1);
		e.normal = Normal::NONE;
		ConflictEdgeInPolygon ceip(XY, &e, nullptr, Range({ 1, 1 }, { 1, 4 }), std::nullopt);
		ceip.append(nullptr, Range({ 1, 5 }, { 1, 8 }), std::nullopt);
		ceip.auto_solve(mpm);
		THEN("The edge should be eligible to mesh") {
			REQUIRE(ceip.is_solved);
			REQUIRE(ceip.solution == &e);
			REQUIRE(e.to_mesh);
		}
	}

	GIVEN("A conflict about an horizontal edge totally covered by some polygons") {
		Point e0(1, 1), e1(1, 8);
		Edge e(XY, &e0, &e1);
		e.normal = Normal::NONE;
		ConflictEdgeInPolygon ceip(XY, &e, nullptr, Range({ 1, 1 }, { 1, 4 }), std::nullopt);
		ceip.append(nullptr, Range({ 1, 3 }, { 1, 6 }), std::nullopt);
		ceip.append(nullptr, Range({ 1, 2 }, { 1, 7 }), std::nullopt);
		ceip.append(nullptr, Range({ 1, 5 }, { 1, 8 }), std::nullopt);
		ceip.auto_solve(mpm);
		THEN("The edge should not be eligible to mesh") {
			REQUIRE(ceip.is_solved);
			REQUIRE(ceip.solution == &e);
			REQUIRE_FALSE(e.to_mesh);
		}
	}

	GIVEN("A conflict about an horizontal edge totally covered by some polygons that share an edge with opposed normal vector") {
		Point e0(1, 1), e1(1, 8);
		Point a0(0, 0), a1(0, 0);
		Point b0(0, 0), b1(0, 0);
		Edge e(XY, &e0, &e1), a(XY, &a0, &a1), b(XY, &b0, &b1);
		e.normal = Normal::XMIN;
		a.normal = Normal::XMAX;
		b.normal = Normal::XMAX;
		ConflictEdgeInPolygon ceip(XY, &e, nullptr, Range({ 1, 1 }, { 1, 4 }), std::nullopt);
		ceip.append(nullptr, Range({ 1, 3 }, { 1, 6 }), std::nullopt);
		ceip.append(nullptr, Range({ 1, 2 }, { 1, 7 }), &a);
		ceip.append(nullptr, Range({ 1, 5 }, { 1, 8 }), &b);
		ceip.auto_solve(mpm);
		THEN("The edge should not be eligible to mesh") {
			REQUIRE(ceip.is_solved);
			REQUIRE(ceip.solution == &e);
			REQUIRE_FALSE(e.to_mesh);
		}
	}

	GIVEN("A conflict about an horizontal edge totally covered by some polygons that share an edge with the same normal vector") {
		Point e0(1, 1), e1(1, 8);
		Point a0(0, 0), a1(0, 0);
		Point b0(0, 0), b1(0, 0);
		Edge e(XY, &e0, &e1), a(XY, &a0, &a1), b(XY, &b0, &b1);
		e.normal = Normal::XMIN;
		a.normal = Normal::XMAX;
		b.normal = Normal::XMIN;
		ConflictEdgeInPolygon ceip(XY, &e, nullptr, Range({ 1, 1 }, { 1, 4 }), std::nullopt);
		ceip.append(nullptr, Range({ 1, 3 }, { 1, 6 }), std::nullopt);
		ceip.append(nullptr, Range({ 1, 2 }, { 1, 7 }), &a);
		ceip.append(nullptr, Range({ 1, 5 }, { 1, 8 }), &b);
		ceip.auto_solve(mpm);
		THEN("The edge should be eligible to mesh") {
			REQUIRE(ceip.is_solved);
			REQUIRE(ceip.solution == &e);
			REQUIRE(e.to_mesh);
		}
	}

	GIVEN("A conflict about a diagonal edge partially covered by some polygons (with a gap)") {
		Point e0(1, 1), e1(8, 8);
		Edge e(XY, &e0, &e1);
		e.normal = Normal::NONE;
		ConflictEdgeInPolygon ceip(XY, &e, nullptr, Range({ 1, 1 }, { 4, 4 }), std::nullopt);
		ceip.append(nullptr, Range({ 5, 5 }, { 8, 8 }), std::nullopt);
		ceip.auto_solve(mpm);
		THEN("The edge should be eligible to mesh") {
			REQUIRE(ceip.is_solved);
			REQUIRE(ceip.solution == &e);
			REQUIRE(e.to_mesh);
		}
	}

	GIVEN("A conflict about a diagonal edge totally covered by some polygons") {
		Point e0(1, 1), e1(8, 8);
		Edge e(XY, &e0, &e1);
		e.normal = Normal::NONE;
		ConflictEdgeInPolygon ceip(XY, &e, nullptr, Range({ 1, 1 }, { 4, 4 }), std::nullopt);
		ceip.append(nullptr, Range({ 3, 3 }, { 6, 6 }), std::nullopt);
		ceip.append(nullptr, Range({ 2, 2 }, { 7, 7 }), std::nullopt);
		ceip.append(nullptr, Range({ 5, 5 }, { 8, 8 }), std::nullopt);
		ceip.auto_solve(mpm);
		THEN("The edge should not be eligible to mesh") {
			REQUIRE(ceip.is_solved);
			REQUIRE(ceip.solution == &e);
			REQUIRE_FALSE(e.to_mesh);
		}
	}

	GIVEN("A conflict about a diagonal edge totally covered by some polygons that share an edge with opposed normal vector") {
		Point e0(1, 1), e1(8, 8);
		Point a0(0, 0), a1(0, 0);
		Point b0(0, 0), b1(0, 0);
		Edge e(XY, &e0, &e1), a(XY, &a0, &a1), b(XY, &b0, &b1);
		e.normal = Normal::XMIN;
		a.normal = Normal::XMAX;
		b.normal = Normal::XMAX;
		ConflictEdgeInPolygon ceip(XY, &e, nullptr, Range({ 1, 1 }, { 4, 4 }), std::nullopt);
		ceip.append(nullptr, Range({ 3, 3 }, { 6, 6 }), std::nullopt);
		ceip.append(nullptr, Range({ 2, 2 }, { 7, 7 }), &a);
		ceip.append(nullptr, Range({ 5, 5 }, { 8, 8 }), &b);
		ceip.auto_solve(mpm);
		THEN("The edge should not be eligible to mesh") {
			REQUIRE(ceip.is_solved);
			REQUIRE(ceip.solution == &e);
			REQUIRE_FALSE(e.to_mesh);
		}
	}

	GIVEN("A conflict about a diagonal edge totally covered by some polygons that share an edge with the same normal vector") {
		Point e0(1, 1), e1(8, 8);
		Point a0(0, 0), a1(0, 0);
		Point b0(0, 0), b1(0, 0);
		Edge e(XY, &e0, &e1), a(XY, &a0, &a1), b(XY, &b0, &b1);
		e.normal = Normal::XMIN;
		a.normal = Normal::XMAX;
		b.normal = Normal::XMIN;
		ConflictEdgeInPolygon ceip(XY, &e, nullptr, Range({ 1, 1 }, { 4, 4 }), std::nullopt);
		ceip.append(nullptr, Range({ 3, 3 }, { 6, 6 }), std::nullopt);
		ceip.append(nullptr, Range({ 2, 2 }, { 7, 7 }), &a);
		ceip.append(nullptr, Range({ 5, 5 }, { 8, 8 }), &b);
		ceip.auto_solve(mpm);
		THEN("The edge should be eligible to mesh") {
			REQUIRE(ceip.is_solved);
			REQUIRE(ceip.solution == &e);
			REQUIRE(e.to_mesh);
		}
	}
}
