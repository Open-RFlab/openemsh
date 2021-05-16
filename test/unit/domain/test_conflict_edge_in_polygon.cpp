///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/edge.hpp"
#include "domain/meshline_policy_manager.hpp"
#include "domain/point.hpp"

#include "domain/conflict_edge_in_polygon.hpp"

/// @test void sort_overlaps_by_p0_by_vector_orientation(vector<Overlap>& overlaps, Point const& vector)
///*****************************************************************************

//******************************************************************************
SCENARIO("void sort_overlaps_by_p0_by_vector_orientation(vector<Overlap>& overlaps, Point const& vector)", "[conflict_edge_in_polygon]") {
	GIVEN("Unordered colinear overlaps in (+, 0) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 4, 1 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 2, 1 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 3, 1 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 1, 1 }, { 0, 0 })), std::nullopt);
		Point v(std::get<RANGE>(overlaps[0])->p0() - std::get<RANGE>(overlaps[3])->p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(*std::get<RANGE>(overlaps[0]) == Range({ 1, 1 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[1]) == Range({ 2, 1 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[2]) == Range({ 3, 1 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[3]) == Range({ 4, 1 }, { 0, 0 }));
		}
	}

	GIVEN("Unordered colinear overlaps in (0, +) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 1, 4 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 1, 2 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 1, 3 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 1, 1 }, { 0, 0 })), std::nullopt);
		Point v(std::get<RANGE>(overlaps[0])->p0() - std::get<RANGE>(overlaps[3])->p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(*std::get<RANGE>(overlaps[0]) == Range({ 1, 1 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[1]) == Range({ 1, 2 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[2]) == Range({ 1, 3 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[3]) == Range({ 1, 4 }, { 0, 0 }));
		}
	}

	GIVEN("Unordered colinear overlaps in (-, 0) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 4, 1 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 2, 1 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 3, 1 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 1, 1 }, { 0, 0 })), std::nullopt);
		Point v(std::get<RANGE>(overlaps[3])->p0() - std::get<RANGE>(overlaps[0])->p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(*std::get<RANGE>(overlaps[0]) == Range({ 4, 1 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[1]) == Range({ 3, 1 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[2]) == Range({ 2, 1 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[3]) == Range({ 1, 1 }, { 0, 0 }));
		}
	}

	GIVEN("Unordered colinear overlaps in (0, -) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 1, 4 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 1, 2 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 1, 3 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 1, 1 }, { 0, 0 })), std::nullopt);
		Point v(std::get<RANGE>(overlaps[3])->p0() - std::get<RANGE>(overlaps[0])->p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(*std::get<RANGE>(overlaps[0]) == Range({ 1, 4 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[1]) == Range({ 1, 3 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[2]) == Range({ 1, 2 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[3]) == Range({ 1, 1 }, { 0, 0 }));
		}
	}

	GIVEN("Unordered colinear overlaps in (+, +) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 4, 4 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 2, 2 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 3, 3 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 1, 1 }, { 0, 0 })), std::nullopt);
		Point v(std::get<RANGE>(overlaps[0])->p0() - std::get<RANGE>(overlaps[3])->p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(*std::get<RANGE>(overlaps[0]) == Range({ 1, 1 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[1]) == Range({ 2, 2 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[2]) == Range({ 3, 3 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[3]) == Range({ 4, 4 }, { 0, 0 }));
		}
	}

	GIVEN("Unordered colinear overlaps in (-, +) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 1, 4 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 3, 2 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 2, 3 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 4, 1 }, { 0, 0 })), std::nullopt);
		Point v(std::get<RANGE>(overlaps[0])->p0() - std::get<RANGE>(overlaps[3])->p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(*std::get<RANGE>(overlaps[0]) == Range({ 4, 1 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[1]) == Range({ 3, 2 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[2]) == Range({ 2, 3 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[3]) == Range({ 1, 4 }, { 0, 0 }));
		}
	}

	GIVEN("Unordered colinear overlaps in (-, -) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 4, 4 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 2, 2 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 3, 3 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 1, 1 }, { 0, 0 })), std::nullopt);
		Point v(std::get<RANGE>(overlaps[3])->p0() - std::get<RANGE>(overlaps[0])->p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(*std::get<RANGE>(overlaps[0]) == Range({ 4, 4 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[1]) == Range({ 3, 3 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[2]) == Range({ 2, 2 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[3]) == Range({ 1, 1 }, { 0, 0 }));
		}
	}

	GIVEN("Unordered colinear overlaps in (+, -) direction") {
		std::vector<Overlap> overlaps;
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 1, 4 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 3, 2 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 2, 3 }, { 0, 0 })), std::nullopt);
		overlaps.emplace_back(nullptr, std::make_unique<Range>(Range({ 4, 1 }, { 0, 0 })), std::nullopt);
		Point v(std::get<RANGE>(overlaps[3])->p0() - std::get<RANGE>(overlaps[0])->p0());
		sort_overlaps_by_p0_by_vector_orientation(overlaps, v);
		THEN("Overlaps should be ordered") {
			REQUIRE(overlaps.size() == 4);
			REQUIRE(*std::get<RANGE>(overlaps[0]) == Range({ 1, 4 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[1]) == Range({ 2, 3 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[2]) == Range({ 3, 2 }, { 0, 0 }));
			REQUIRE(*std::get<RANGE>(overlaps[3]) == Range({ 4, 1 }, { 0, 0 }));
		}
	}
}
