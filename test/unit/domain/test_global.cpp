///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include <string>

#include "domain/global.hpp"

/// @test template<auto Member, typename MemberType>
///       auto const& GlobalParams::get_per_axis_per_criteria(
///       	Axis axis,
///       	Material const* material,
///       	Params const& state) const
///*****************************************************************************

using namespace domain;
using namespace std::string_literals;

// Rules precedence order:
// 1- material name, axis
// 2- material name, wildcard axis
// 3- material type, axis
// 4- material type, wildcard axis
// 5- wildcard material, axis
// 6- wildcard material, wildcard axis (default rule)
//******************************************************************************
SCENARIO("template<auto Member, typename MemberType> \
auto const& GlobalParams::get_per_axis_per_criteria( \
Axis axis, \
Material const* material, \
Params const& state) const", "[domain][global]") {
	Timepoint* t = Caretaker::singleton().get_history_root();
	GIVEN("Global parameters containing some matching rules along with unmatching rules") {
		decltype(Params::lmin) unmatching_rules = {
			{{ Y, "abc" }, 10 },
			{{ Params::ALL, "d" }, 20 },
			{{ Y, Material::Type::DIELECTRIC }, 30 },
			{{ Params::ALL, Material::Type::AIR }, 40 },
			{{ Y, Params::ALL }, 50 },
		};
		GlobalParams p(t);
		Material named_material(Material::Type::DIELECTRIC, "abc");
		Material unnamed_material(Material::Type::DIELECTRIC, "");

		WHEN("The matching rules { 1, 2, 3, 4, 5, 6 } are available") {
			Params params;
			params.lmin = {
				{{ X, "abc" }, 1 },
				{{ Params::ALL, "abc" }, 2 },
				{{ X, Material::Type::DIELECTRIC }, 3 },
				{{ Params::ALL, Material::Type::DIELECTRIC }, 4 },
				{{ X, Params::ALL }, 5 },
				{{ Params::ALL, Params::ALL }, 6 },
			};
			params.lmin.merge(unmatching_rules);
			p.set_next_state(params);
			WHEN("Requesting property for an axis and a named material") {
				THEN("Should return the value associated with matching rule 1: material name, axis") {
					REQUIRE(p.get_lmin(X, &named_material) == 1);
				}
			}
			WHEN("Requesting property for an axis and an unnamed material") {
				THEN("Should return the value associated with matching rule 3: material type, axis") {
					REQUIRE(p.get_lmin(X, &unnamed_material) == 3);
				}
			}
			WHEN("Requesting property for an axis and no material") {
				THEN("Should return the value associated with matching rule 5: wildcard material, axis") {
					REQUIRE(p.get_lmin(X, nullptr) == 5);
				}
			}
		}

		WHEN("The matching rules { 2, 3, 4, 5, 6 } are available") {
			Params params;
			params.lmin = {
				{{ Params::ALL, "abc" }, 2 },
				{{ X, Material::Type::DIELECTRIC }, 3 },
				{{ Params::ALL, Material::Type::DIELECTRIC }, 4 },
				{{ X, Params::ALL }, 5 },
				{{ Params::ALL, Params::ALL }, 6 },
			};
			params.lmin.merge(unmatching_rules);
			p.set_next_state(params);
			WHEN("Requesting property for an axis and a named material") {
				THEN("Should return the value associated with matching rule 2: material name, wildcard axis") {
					REQUIRE(p.get_lmin(X, &named_material) == 2);
				}
			}
			WHEN("Requesting property for an axis and an unnamed material") {
				THEN("Should return the value associated with matching rule 3: material type, axis") {
					REQUIRE(p.get_lmin(X, &unnamed_material) == 3);
				}
			}
			WHEN("Requesting property for an axis and no material") {
				THEN("Should return the value associated with matching rule 5: wildcard material, axis") {
					REQUIRE(p.get_lmin(X, nullptr) == 5);
				}
			}
		}

		WHEN("The matching rules { 3, 4, 5, 6 } are available") {
			Params params;
			params.lmin = {
				{{ X, Material::Type::DIELECTRIC }, 3 },
				{{ Params::ALL, Material::Type::DIELECTRIC }, 4 },
				{{ X, Params::ALL }, 5 },
				{{ Params::ALL, Params::ALL }, 6 },
			};
			params.lmin.merge(unmatching_rules);
			p.set_next_state(params);

			WHEN("Requesting property for an axis and a named material") {
				THEN("Should return the value associated with matching rule 3: material type, axis") {
					REQUIRE(p.get_lmin(X, &named_material) == 3);
				}
			}
			WHEN("Requesting property for an axis and an unnamed material") {
				THEN("Should return the value associated with matching rule 3: material type, axis") {
					REQUIRE(p.get_lmin(X, &unnamed_material) == 3);
				}
			}
			WHEN("Requesting property for an axis and no material") {
				THEN("Should return the value associated with matching rule 5: wildcard material, axis") {
					REQUIRE(p.get_lmin(X, nullptr) == 5);
				}
			}
		}

		WHEN("The matching rules { 4, 5, 6 } are available") {
			Params params;
			params.lmin = {
				{{ Params::ALL, Material::Type::DIELECTRIC }, 4 },
				{{ X, Params::ALL }, 5 },
				{{ Params::ALL, Params::ALL }, 6 },
			};
			params.lmin.merge(unmatching_rules);
			p.set_next_state(params);

			WHEN("Requesting property for an axis and a named material") {
				THEN("Should return the value associated with matching rule 4: material type, wildcard axis") {
					REQUIRE(p.get_lmin(X, &named_material) == 4);
				}
			}
			WHEN("Requesting property for an axis and an unnamed material") {
				THEN("Should return the value associated with matching rule 4: material type, wildcard axis") {
					REQUIRE(p.get_lmin(X, &unnamed_material) == 4);
				}
			}
			WHEN("Requesting property for an axis and no material") {
				THEN("Should return the value associated with matching rule 5: wildcard material, axis") {
					REQUIRE(p.get_lmin(X, nullptr) == 5);
				}
			}
		}

		WHEN("The matching rules { 5, 6 } are available") {
			Params params;
			params.lmin = {
				{{ X, Params::ALL }, 5 },
				{{ Params::ALL, Params::ALL }, 6 },
			};
			params.lmin.merge(unmatching_rules);
			p.set_next_state(params);
			WHEN("Requesting property for an axis and a named material") {
				THEN("Should return the value associated with matching rule 5: wildcard material, axis") {
					REQUIRE(p.get_lmin(X, &named_material) == 5);
				}
			}
			WHEN("Requesting property for an axis and an unnamed material") {
				THEN("Should return the value associated with matching rule 5: wildcard material, axis") {
					REQUIRE(p.get_lmin(X, &unnamed_material) == 5);
				}
			}
			WHEN("Requesting property for an axis and no material") {
				THEN("Should return the value associated with matching rule 5: wildcard material, axis") {
					REQUIRE(p.get_lmin(X, nullptr) == 5);
				}
			}
		}

		WHEN("The matching rules { 6 } are available") {
			Params params;
			params.lmin = {
				{{ Params::ALL, Params::ALL }, 6 },
			};
			params.lmin.merge(unmatching_rules);
			p.set_next_state(params);
			WHEN("Requesting property for an axis and a named material") {
				THEN("Should return the value associated with matching rule 6: wildcard material, wildcard axis (default rule)") {
					REQUIRE(p.get_lmin(X, &named_material) == 6);
				}
			}
			WHEN("Requesting property for an axis and an unnamed material") {
				THEN("Should return the value associated with matching rule 6: wildcard material, wildcard axis (default rule)") {
					REQUIRE(p.get_lmin(X, &unnamed_material) == 6);
				}
			}
			WHEN("Requesting property for an axis and no material") {
				THEN("Should return the value associated with matching rule 6: wildcard material, wildcard axis (default rule)") {
					REQUIRE(p.get_lmin(X, nullptr) == 6);
				}
			}
		}

		WHEN("No matching rule is available") {
			Params params;
			params.lmin = unmatching_rules;
			p.set_next_state(params);
			THEN("Should abort at runtime") {
				// REQUIRE(p.get_lmin(X, nullptr));
			}
		}
	}
}
