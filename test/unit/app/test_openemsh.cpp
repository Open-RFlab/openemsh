///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "app/openemsh.hpp"

/// @test optional<Step> next(Step step)
///*****************************************************************************

using namespace app;

//******************************************************************************
SCENARIO("optional<Step> next(Step step)", "[app][openemsh]") {
	WHEN("Running for DETECT_CONFLICT_EIP") {
		THEN("Should return DETECT_CONFLICT_CE") {
			std::optional<Step> a = next(Step::DETECT_CONFLICT_EIP);
			REQUIRE(a.has_value());
			REQUIRE(a.value() == Step::DETECT_CONFLICT_CE);
		}
	}
	WHEN("Running for DETECT_CONFLICT_CE") {
		THEN("Should return DETECT_NON_CONFLICTING_EDGES") {
			std::optional<Step> a = next(Step::DETECT_CONFLICT_CE);
			REQUIRE(a.has_value());
			REQUIRE(a.value() == Step::DETECT_NON_CONFLICTING_EDGES);
		}
	}
	WHEN("Running for DETECT_NON_CONFLICTING_EDGES") {
		THEN("Should return ADD_FIXED_MLP") {
			std::optional<Step> a = next(Step::DETECT_NON_CONFLICTING_EDGES);
			REQUIRE(a.has_value());
			REQUIRE(a.value() == Step::ADD_FIXED_MLP);
		}
	}
	WHEN("Running for ADD_FIXED_MLP") {
		THEN("Should return SOLVE_ALL_EIP") {
			std::optional<Step> a = next(Step::ADD_FIXED_MLP);
			REQUIRE(a.has_value());
			REQUIRE(a.value() == Step::SOLVE_ALL_EIP);
		}
	}
	WHEN("Running for SOLVE_ALL_EIP") {
		THEN("Should return SOLVE_ALL_CE") {
			std::optional<Step> a = next(Step::SOLVE_ALL_EIP);
			REQUIRE(a.has_value());
			REQUIRE(a.value() == Step::SOLVE_ALL_CE);
		}
	}
	WHEN("Running for SOLVE_ALL_CE") {
		THEN("Should return DETECT_AND_SOLVE_TCMLP") {
			std::optional<Step> a = next(Step::SOLVE_ALL_CE);
			REQUIRE(a.has_value());
			REQUIRE(a.value() == Step::DETECT_AND_SOLVE_TCMLP);
		}
	}
	WHEN("Running for DETECT_AND_SOLVE_TCMLP") {
		THEN("Should return DETECT_INTERVALS") {
			std::optional<Step> a = next(Step::DETECT_AND_SOLVE_TCMLP);
			REQUIRE(a.has_value());
			REQUIRE(a.value() == Step::DETECT_INTERVALS);
		}
	}
	WHEN("Running for DETECT_INTERVALS") {
		THEN("Should return MESH") {
			std::optional<Step> a = next(Step::DETECT_INTERVALS);
			REQUIRE(a.has_value());
			REQUIRE(a.value() == Step::MESH);
		}
	}
	WHEN("Running for MESH") {
		THEN("Should return nullopt") {
			std::optional<Step> a = next(Step::MESH);
			REQUIRE_FALSE(a.has_value());
		}
	}
}
