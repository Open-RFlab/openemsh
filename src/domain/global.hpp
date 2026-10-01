///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <cstddef>
#include <map>
#include <optional>
#include <utility>
#include <variant>
#include <vector>

#include "geometrics/space.hpp"
#include "utils/state_management.hpp"
#include "material.hpp"

namespace domain {

//******************************************************************************
struct Params {
	bool has_grid_already = false; // TODO would better fit in infra layer?
	double proximity_limit = 1; // TODO must be linked to initial d

	std::size_t diagonal_lmin = 2;
	double diagonal_dmax = 0.2;
	double consecutive_diagonal_minimal_angle = 20; // Limite between acute / obtuse angles.

	std::vector<std::pair<Axis, double>> input_fixed_meshlines;

	template<typename Criteria, typename Value>
	using PerAxisPer = std::map<std::tuple<std::optional<Axis>, std::optional<Criteria>>, Value>;
	static auto constexpr ALL = std::nullopt;

	using Mat = std::variant<Material::Type, std::string>;

	PerAxisPer<Mat, double> dmax = {{{ ALL, ALL }, 2.5 }};
	PerAxisPer<Mat, size_t> lmin = {{{ ALL, ALL }, 2 }};
	PerAxisPer<Mat, double> smoothness = {{{ ALL, ALL }, 2 }};
};

//******************************************************************************
class GlobalParams : public Originator<Params const> {
public:
	explicit GlobalParams(Timepoint* t);
	GlobalParams(Params params, Timepoint* t);

	auto get_dmax(Axis axis, Material const* material = nullptr) const -> decltype(Params::dmax)::mapped_type const&;
	auto get_lmin(Axis axis, Material const* material = nullptr) const -> decltype(Params::lmin)::mapped_type const&;
	auto get_smoothness(Axis axis, Material const* material = nullptr) const -> decltype(Params::smoothness)::mapped_type const&;

	auto get_dmax(Axis axis, Material const* material, Params const& state) const -> decltype(Params::dmax)::mapped_type const&;
	auto get_lmin(Axis axis, Material const* material, Params const& state) const -> decltype(Params::lmin)::mapped_type const&;
	auto get_smoothness(Axis axis, Material const* material, Params const& state) const -> decltype(Params::smoothness)::mapped_type const&;

private:
	template<auto Member, typename MemberType>
	auto const& get_per_axis_per_criteria(Axis axis, Material const* material) const;
	template<auto Member, typename MemberType>
	auto const& get_per_axis_per_criteria(Axis axis, Material const* material, Params const& state) const;
};

inline double equality_tolerance = 1e-8;

} // namespace domain
