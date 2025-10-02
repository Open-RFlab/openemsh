///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <compare>
#include <string>

namespace domain {

//******************************************************************************
class Material {
public:

	enum class Type {
//		DUMP,
//		PEC,
		CONDUCTOR,
		DIELECTRIC,
		AIR
	} const type;

	std::strong_ordering operator<=>(Material const& other) const noexcept;

	Material(Type type, std::string const& name);

	// https://wiki.openems.de/index.php/Material_Property.html
	// Epsilon: relative electric permittivity (must be >=1)
	// Mue: relative magnetic permeability (must be >=1)
	// Kappa: electric conductivity (must be >=0)
	// Sigma: magnetic conductivity (non-physical property, must be >=0)
	// Values from CSXCAD.
	static double constexpr default_epsilon = 1;
	static double constexpr default_mue = 1;
	static double constexpr default_kappa = 0;
	static double constexpr default_sigma = 0;

	static Type deduce_type(double epsilon, double mue, double kappa);

	std::string const name;
};

} // namespace domain
