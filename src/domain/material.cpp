///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "utils/unreachable.hpp"

#include "material.hpp"

using namespace std;

namespace domain {

/// Simplified deduction.
///*****************************************************************************
Material::Type Material::deduce_type(double epsilon, double mue, double kappa) {
	     if(epsilon == default_epsilon && mue == default_mue && kappa == default_kappa) return Material::Type::AIR;
	else if(epsilon >  default_epsilon && mue >= default_mue && kappa == default_kappa) return Material::Type::DIELECTRIC;
	else if(epsilon == default_epsilon && mue == default_mue && kappa >  default_kappa) return Material::Type::CONDUCTOR;
	else return Material::Type::DIELECTRIC;
}

//******************************************************************************
Material::Material(Type type, string const& name)
: type(type)
, name(name)
{}

//******************************************************************************
Material::Material(Type type, string const& name, optional<Color> const& fill_color, optional<Color> const& edge_color)
: type(type)
, name(name)
, fill_color(fill_color)
, edge_color(edge_color)
{}

//******************************************************************************
strong_ordering Material::operator<=>(Material const& other) const noexcept {
	switch(type) {
	case Type::CONDUCTOR:
		switch(other.type) {
		case Type::CONDUCTOR: return strong_ordering::equivalent;
		case Type::DIELECTRIC: return strong_ordering::greater;
		case Type::AIR: return strong_ordering::greater;
		default: ::unreachable();
		}
	case Type::DIELECTRIC:
		switch(other.type) {
		case Type::CONDUCTOR: return strong_ordering::less;
		case Type::DIELECTRIC: return strong_ordering::equivalent;
		case Type::AIR: return strong_ordering::greater;
		default: ::unreachable();
		}
	case Type::AIR:
		switch(other.type) {
		case Type::CONDUCTOR: return strong_ordering::less;
		case Type::DIELECTRIC: return strong_ordering::less;
		case Type::AIR: return strong_ordering::equivalent;
		default: ::unreachable();
		}
	default: ::unreachable();
	}
}

} // namespace domain
