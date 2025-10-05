///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/material.hpp"

/// @test std::strong_ordering Material::operator<=>(Material const& other) const noexcept
/// @test Material::Type Material::deduce_type(double epsilon, double mue, double kappa)
///*****************************************************************************

using namespace domain;

//******************************************************************************
SCENARIO("std::strong_ordering Material::operator<=>(Material const& other) const noexcept", "[domain][material]") {
	Material conductor(Material::Type::CONDUCTOR, "");
	Material dielectric(Material::Type::DIELECTRIC, "");
	Material air(Material::Type::AIR, "");
	WHEN("Comparing CONDUCTOR to CONDUCTOR") {
		THEN("Should return equivalent") {
			REQUIRE(conductor <=> conductor == std::strong_ordering::equivalent);
		}
	}
	WHEN("Comparing CONDUCTOR to DIELECTRIC") {
		THEN("Should return greater") {
			REQUIRE(conductor <=> dielectric == std::strong_ordering::greater);
		}
	}
	WHEN("Comparing CONDUCTOR to AIR") {
		THEN("Should return greater") {
			REQUIRE(conductor <=> air == std::strong_ordering::greater);
		}
	}
	WHEN("Comparing DIELECTRIC to CONDUCTOR") {
		THEN("Should return less") {
			REQUIRE(dielectric <=> conductor == std::strong_ordering::less);
		}
	}
	WHEN("Comparing DIELECTRIC to DIELECTRIC") {
		THEN("Should return equivalent") {
			REQUIRE(dielectric <=> dielectric == std::strong_ordering::equivalent);
		}
	}
	WHEN("Comparing DIELECTRIC to AIR") {
		THEN("Should return greater") {
			REQUIRE(dielectric <=> air == std::strong_ordering::greater);
		}
	}
	WHEN("Comparing AIR to CONDUCTOR") {
		THEN("Should return less") {
			REQUIRE(air <=> conductor == std::strong_ordering::less);
		}
	}
	WHEN("Comparing AIR to DIELECTRIC") {
		THEN("Should return less") {
			REQUIRE(air <=> dielectric == std::strong_ordering::less);
		}
	}
	WHEN("Comparing AIR to AIR") {
		THEN("Should return equivalent") {
			REQUIRE(air <=> air == std::strong_ordering::equivalent);
		}
	}
}

// Use materials from
// https://github.com/VolkerMuehlhaus/openems_ihp_sg13g2/blob/820044c/workflow/output/run_dual_dipole_data/sub-1/run_dual_dipole.xml
//******************************************************************************
SCENARIO("Material::Type Material::deduce_type(double epsilon, double mue, double kappa)", "[domain][material]") {
	WHEN("Deducing Type of default values") {
		double epsilon = 1;
		double mue = 1;
		double kappa = 0;
		THEN("Should return AIR") {
			REQUIRE(Material::deduce_type(epsilon, mue, kappa) == Material::Type::AIR);
		}
	}
	WHEN("Deducing Type of Metal1") {
		double epsilon = 1.0;
		double mue = 1.0;
		double kappa = 2.164000e+07;
		THEN("Should return CONDUCTOR") {
			REQUIRE(Material::deduce_type(epsilon, mue, kappa) == Material::Type::CONDUCTOR);
		}
	}
	WHEN("Deducing Type of Metal2") {
		double epsilon = 1.0;
		double mue = 1.0;
		double kappa = 2.319000e+07;
		THEN("Should return CONDUCTOR") {
			REQUIRE(Material::deduce_type(epsilon, mue, kappa) == Material::Type::CONDUCTOR);
		}
	}
	WHEN("Deducing Type of TopMetal1") {
		double epsilon = 1.0;
		double mue = 1.0;
		double kappa = 2.780000e+07;
		THEN("Should return CONDUCTOR") {
			REQUIRE(Material::deduce_type(epsilon, mue, kappa) == Material::Type::CONDUCTOR);
		}
	}
	WHEN("Deducing Type of TopMetal2") {
		double epsilon = 1.0;
		double mue = 1.0;
		double kappa = 3.030000e+07;
		THEN("Should return CONDUCTOR") {
			REQUIRE(Material::deduce_type(epsilon, mue, kappa) == Material::Type::CONDUCTOR);
		}
	}
	WHEN("Deducing Type of TopVia1") {
		double epsilon = 1.0;
		double mue = 1.0;
		double kappa = 2.191000e+06;
		THEN("Should return CONDUCTOR") {
			REQUIRE(Material::deduce_type(epsilon, mue, kappa) == Material::Type::CONDUCTOR);
		}
	}
	WHEN("Deducing Type of TopVia2") {
		double epsilon = 1.0;
		double mue = 1.0;
		double kappa = 3.143000e+06;
		THEN("Should return CONDUCTOR") {
			REQUIRE(Material::deduce_type(epsilon, mue, kappa) == Material::Type::CONDUCTOR);
		}
	}
	WHEN("Deducing Type of Via1") {
		double epsilon = 1.0;
		double mue = 1.0;
		double kappa = 1.660000e+06;
		THEN("Should return CONDUCTOR") {
			REQUIRE(Material::deduce_type(epsilon, mue, kappa) == Material::Type::CONDUCTOR);
		}
	}
	WHEN("Deducing Type of AIR") {
		double epsilon = 1.0;
		double mue = 1.0;
		double kappa = 0.0;
		THEN("Should return AIR") {
			REQUIRE(Material::deduce_type(epsilon, mue, kappa) == Material::Type::AIR);
		}
	}
	WHEN("Deducing Type of Passive") {
		double epsilon = 6.600000e+00;
		double mue = 1.0;
		double kappa = 0.0;
		THEN("Should return DIELECTRIC") {
			REQUIRE(Material::deduce_type(epsilon, mue, kappa) == Material::Type::DIELECTRIC);
		}
	}
	WHEN("Deducing Type of SiO2") {
		double epsilon = 4.100000e+00;
		double mue = 1.0;
		double kappa = 0.0;
		THEN("Should return DIELECTRIC") {
			REQUIRE(Material::deduce_type(epsilon, mue, kappa) == Material::Type::DIELECTRIC);
		}
	}
	WHEN("Deducing Type of EPI") {
		double epsilon = 1.190000e+01;
		double mue = 1.0;
		double kappa = 5.000000e+00;
		THEN("Should return DIELECTRIC") {
			REQUIRE(Material::deduce_type(epsilon, mue, kappa) == Material::Type::DIELECTRIC);
		}
	}
	WHEN("Deducing Type of Substrate") {
		double epsilon = 1.190000e+01;
		double mue = 1.0;
		double kappa = 2.000000e+00;
		THEN("Should return DIELECTRIC") {
			REQUIRE(Material::deduce_type(epsilon, mue, kappa) == Material::Type::DIELECTRIC);
		}
	}
}
