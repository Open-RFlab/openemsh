///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "app/openemsh.hpp"

/// @test optional<Step> next(Step step)
/// @test set<Step> that_and_after(Step step)
///*****************************************************************************

using namespace app;

//******************************************************************************
SCENARIO("optional<Step> next(Step step)", "[app][openemsh]") {
	WHEN("Running for ADJUST_EDGE_TO_MATERIAL") {
		THEN("Should return DETECT_DIAG_ANGLES") {
			std::optional<Step> a = next(Step::ADJUST_EDGE_TO_MATERIAL);
			REQUIRE(a.has_value());
			REQUIRE(a.value() == Step::DETECT_DIAG_ANGLES);
		}
	}
	WHEN("Running for DETECT_DIAG_ANGLES") {
		THEN("Should return DETECT_DIAG_ZONES") {
			std::optional<Step> a = next(Step::DETECT_DIAG_ANGLES);
			REQUIRE(a.has_value());
			REQUIRE(a.value() == Step::DETECT_DIAG_ZONES);
		}
	}

	WHEN("Running for DETECT_DIAG_ZONES") {
		THEN("Should return SOLVE_DIAG_ZONES_ANGLES") {
			std::optional<Step> a = next(Step::DETECT_DIAG_ZONES);
			REQUIRE(a.has_value());
			REQUIRE(a.value() == Step::SOLVE_DIAG_ZONES_ANGLES);
		}
	}

	WHEN("Running for SOLVE_DIAG_ZONES_ANGLES") {
		THEN("Should return DETECT_CONFLICT_EIP") {
			std::optional<Step> a = next(Step::SOLVE_DIAG_ZONES_ANGLES);
			REQUIRE(a.has_value());
			REQUIRE(a.value() == Step::DETECT_CONFLICT_EIP);
		}
	}
	WHEN("Running for DETECT_CONFLICT_EIP") {
		THEN("Should return DETECT_CONFLICT_CE") {
			std::optional<Step> a = next(Step::DETECT_CONFLICT_EIP);
			REQUIRE(a.has_value());
			REQUIRE(a.value() == Step::DETECT_CONFLICT_CE);
		}
	}
	WHEN("Running for DETECT_CONFLICT_CE") {
			std::optional<Step> a = next(Step::DETECT_CONFLICT_CE);
		THEN("Should return ADD_FIXED_MLP") {
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
		THEN("Should return DETECT_INDIVIDUAL_EDGES") {
			std::optional<Step> a = next(Step::SOLVE_ALL_CE);
			REQUIRE(a.has_value());
			REQUIRE(a.value() == Step::DETECT_INDIVIDUAL_EDGES);
		}
	}
	WHEN("Running for DETECT_INDIVIDUAL_EDGES") {
		THEN("Should return DETECT_AND_SOLVE_TCMLP") {
			std::optional<Step> a = next(Step::DETECT_INDIVIDUAL_EDGES);
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
			REQUIRE(a.value() == Step::DETECT_INTERVALS_PER_DIAG_ZONES);
		}
	}
	WHEN("Running for DETECT_INTERVALS_PER_DIAG_ZONES") {
		THEN("Should return SOLVE_DIAG_ZONES_INTERVALS") {
			std::optional<Step> a = next(Step::DETECT_INTERVALS_PER_DIAG_ZONES);
			REQUIRE(a.has_value());
			REQUIRE(a.value() == Step::SOLVE_DIAG_ZONES_INTERVALS);
		}
	}
	WHEN("Running for SOLVE_DIAG_ZONES_INTERVALS") {
		THEN("Should return MESH") {
			std::optional<Step> a = next(Step::SOLVE_DIAG_ZONES_INTERVALS);
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

//******************************************************************************
SCENARIO("set<Step> that_and_after(Step step)", "[app][openemsh]") {
	WHEN("Running for ADJUST_EDGE_TO_MATERIAL") {
		THEN("Should return all Steps except those coming before ADJUST_EDGE_TO_MATERIAL") {
			REQUIRE(that_and_after(Step::ADJUST_EDGE_TO_MATERIAL) == std::set<Step> {
				Step::ADJUST_EDGE_TO_MATERIAL,
				Step::DETECT_DIAG_ANGLES,
				Step::DETECT_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_ANGLES,
				Step::DETECT_CONFLICT_EIP,
				Step::DETECT_CONFLICT_CE,
				Step::ADD_FIXED_MLP,
				Step::SOLVE_ALL_EIP,
				Step::SOLVE_ALL_CE,
				Step::DETECT_INDIVIDUAL_EDGES,
				Step::DETECT_AND_SOLVE_TCMLP,
				Step::DETECT_INTERVALS,
				Step::DETECT_INTERVALS_PER_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_INTERVALS,
				Step::MESH
			});
		}
	}
	WHEN("Running for DETECT_DIAG_ANGLES") {
		THEN("Should return all Steps except those coming before DETECT_DIAG_ANGLES") {
			REQUIRE(that_and_after(Step::DETECT_DIAG_ANGLES) == std::set<Step> {
				Step::DETECT_DIAG_ANGLES,
				Step::DETECT_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_ANGLES,
				Step::DETECT_CONFLICT_EIP,
				Step::DETECT_CONFLICT_CE,
				Step::ADD_FIXED_MLP,
				Step::SOLVE_ALL_EIP,
				Step::SOLVE_ALL_CE,
				Step::DETECT_INDIVIDUAL_EDGES,
				Step::DETECT_AND_SOLVE_TCMLP,
				Step::DETECT_INTERVALS,
				Step::DETECT_INTERVALS_PER_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_INTERVALS,
				Step::MESH
			});
		}
	}
	WHEN("Running for DETECT_DIAG_ZONES") {
		THEN("Should return all Steps except those coming before DETECT_DIAG_ZONES") {
			REQUIRE(that_and_after(Step::DETECT_DIAG_ZONES) == std::set<Step> {
				Step::DETECT_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_ANGLES,
				Step::DETECT_CONFLICT_EIP,
				Step::DETECT_CONFLICT_CE,
				Step::ADD_FIXED_MLP,
				Step::SOLVE_ALL_EIP,
				Step::SOLVE_ALL_CE,
				Step::DETECT_INDIVIDUAL_EDGES,
				Step::DETECT_AND_SOLVE_TCMLP,
				Step::DETECT_INTERVALS,
				Step::DETECT_INTERVALS_PER_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_INTERVALS,
				Step::MESH
			});
		}
	}
	WHEN("Running for SOLVE_DIAG_ZONES_ANGLES") {
		THEN("Should return all Steps except those coming before SOLVE_DIAG_ZONES_ANGLES") {
			REQUIRE(that_and_after(Step::SOLVE_DIAG_ZONES_ANGLES) == std::set<Step> {
				Step::SOLVE_DIAG_ZONES_ANGLES,
				Step::DETECT_CONFLICT_EIP,
				Step::DETECT_CONFLICT_CE,
				Step::ADD_FIXED_MLP,
				Step::SOLVE_ALL_EIP,
				Step::SOLVE_ALL_CE,
				Step::DETECT_INDIVIDUAL_EDGES,
				Step::DETECT_AND_SOLVE_TCMLP,
				Step::DETECT_INTERVALS,
				Step::DETECT_INTERVALS_PER_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_INTERVALS,
				Step::MESH
			});
		}
	}
	WHEN("Running for DETECT_CONFLICT_EIP") {
		THEN("Should return all Steps except those coming before DETECT_CONFLICT_EIP") {
			REQUIRE(that_and_after(Step::DETECT_CONFLICT_EIP) == std::set<Step> {
				Step::DETECT_CONFLICT_EIP,
				Step::DETECT_CONFLICT_CE,
				Step::ADD_FIXED_MLP,
				Step::SOLVE_ALL_EIP,
				Step::SOLVE_ALL_CE,
				Step::DETECT_INDIVIDUAL_EDGES,
				Step::DETECT_AND_SOLVE_TCMLP,
				Step::DETECT_INTERVALS,
				Step::DETECT_INTERVALS_PER_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_INTERVALS,
				Step::MESH
			});
		}
	}
	WHEN("Running for DETECT_CONFLICT_CE") {
		THEN("Should return all Steps except those coming before DETECT_CONFLICT_CE") {
			REQUIRE(that_and_after(Step::DETECT_CONFLICT_CE) == std::set<Step> {
				Step::DETECT_CONFLICT_CE,
				Step::ADD_FIXED_MLP,
				Step::SOLVE_ALL_EIP,
				Step::SOLVE_ALL_CE,
				Step::DETECT_INDIVIDUAL_EDGES,
				Step::DETECT_AND_SOLVE_TCMLP,
				Step::DETECT_INTERVALS,
				Step::DETECT_INTERVALS_PER_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_INTERVALS,
				Step::MESH
			});
		}
	}
	WHEN("Running for ADD_FIXED_MLP") {
		THEN("Should return all Steps except those coming before ADD_FIXED_MLP") {
			REQUIRE(that_and_after(Step::ADD_FIXED_MLP) == std::set<Step> {
				Step::ADD_FIXED_MLP,
				Step::SOLVE_ALL_EIP,
				Step::SOLVE_ALL_CE,
				Step::DETECT_INDIVIDUAL_EDGES,
				Step::DETECT_AND_SOLVE_TCMLP,
				Step::DETECT_INTERVALS,
				Step::DETECT_INTERVALS_PER_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_INTERVALS,
				Step::MESH
			});
		}
	}
	WHEN("Running for SOLVE_ALL_EIP") {
		THEN("Should return all Steps except those coming before SOLVE_ALL_EIP") {
			REQUIRE(that_and_after(Step::SOLVE_ALL_EIP) == std::set<Step> {
				Step::SOLVE_ALL_EIP,
				Step::SOLVE_ALL_CE,
				Step::DETECT_INDIVIDUAL_EDGES,
				Step::DETECT_AND_SOLVE_TCMLP,
				Step::DETECT_INTERVALS,
				Step::DETECT_INTERVALS_PER_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_INTERVALS,
				Step::MESH
			});
		}
	}
	WHEN("Running for SOLVE_ALL_CE") {
		THEN("Should return all Steps except those coming before SOLVE_ALL_CE") {
			REQUIRE(that_and_after(Step::SOLVE_ALL_CE) == std::set<Step> {
				Step::SOLVE_ALL_CE,
				Step::DETECT_INDIVIDUAL_EDGES,
				Step::DETECT_AND_SOLVE_TCMLP,
				Step::DETECT_INTERVALS,
				Step::DETECT_INTERVALS_PER_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_INTERVALS,
				Step::MESH
			});
		}
	}
	WHEN("Running for DETECT_INDIVIDUAL_EDGES") {
		THEN("Should return all Steps except those coming before DETECT_INDIVIDUAL_EDGES") {
			REQUIRE(that_and_after(Step::DETECT_INDIVIDUAL_EDGES) == std::set<Step> {
				Step::DETECT_INDIVIDUAL_EDGES,
				Step::DETECT_AND_SOLVE_TCMLP,
				Step::DETECT_INTERVALS,
				Step::DETECT_INTERVALS_PER_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_INTERVALS,
				Step::MESH
			});
		}
	}
	WHEN("Running for DETECT_AND_SOLVE_TCMLP") {
		THEN("Should return all Steps except those coming before DETECT_AND_SOLVE_TCMLP") {
			REQUIRE(that_and_after(Step::DETECT_AND_SOLVE_TCMLP) == std::set<Step> {
				Step::DETECT_AND_SOLVE_TCMLP,
				Step::DETECT_INTERVALS,
				Step::DETECT_INTERVALS_PER_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_INTERVALS,
				Step::MESH
			});
		}
	}
	WHEN("Running for DETECT_INTERVALS") {
		THEN("Should return all Steps except those coming before DETECT_INTERVALS") {
			REQUIRE(that_and_after(Step::DETECT_INTERVALS) == std::set<Step> {
				Step::DETECT_INTERVALS,
				Step::DETECT_INTERVALS_PER_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_INTERVALS,
				Step::MESH
			});
		}
	}
	WHEN("Running for DETECT_INTERVALS_PER_DIAG_ZONES") {
		THEN("Should return all Steps except those coming before DETECT_INTERVALS_PER_DIAG_ZONES") {
			REQUIRE(that_and_after(Step::DETECT_INTERVALS_PER_DIAG_ZONES) == std::set<Step> {
				Step::DETECT_INTERVALS_PER_DIAG_ZONES,
				Step::SOLVE_DIAG_ZONES_INTERVALS,
				Step::MESH
			});
		}
	}
	WHEN("Running for SOLVE_DIAG_ZONES_INTERVALS") {
		THEN("Should return all Steps except those coming before SOLVE_DIAG_ZONES_INTERVALS") {
			REQUIRE(that_and_after(Step::SOLVE_DIAG_ZONES_INTERVALS) == std::set<Step> {
				Step::SOLVE_DIAG_ZONES_INTERVALS,
				Step::MESH
			});
		}
	}
	WHEN("Running for MESH") {
		THEN("Should return all Steps except those coming before MESH") {
			REQUIRE(that_and_after(Step::MESH) == std::set<Step> {
				Step::MESH
			});
		}
	}
}
