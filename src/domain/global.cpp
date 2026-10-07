///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <cstdlib>
#include <format>
#include <ranges>
#include <source_location>
#include <string>

#include "infra/utils/to_string.hpp"
#include "utils/logger.hpp"

#include "global.hpp"

using namespace std;

//******************************************************************************
template<typename T>
string to_string(T const& t) {
	if constexpr(is_enum_v<T>)
		return to_string(t);
	else
		return format("{}", t);
}

//******************************************************************************
template<typename T, std::size_t N>
string to_string(array<T, N> const& a) {
	return a | views::join_with(", ") | ranges::to<string>();
}

//******************************************************************************
template<typename ...T>
string to_string(variant<T...> const& v) {
	return visit([](auto&& arg) {
		return to_string(arg);
	}, v);
}

//******************************************************************************
template<typename T>
string to_string(optional<T> const& t) {
	if(t.has_value())
		return to_string(t.value());
	else
		return "*";
}

//******************************************************************************
template<typename Criteria, typename Value>
string to_string(domain::Params::PerAxisPer<Criteria, Value> const& m) {
	string res;
	for(auto const& [k, v] : m) {
		auto const& [axis, criteria] = k;
		res += format("    {{ {}, {}, {} }}\n",
			to_string(axis),
			to_string(criteria),
			to_string(v));
	}
	res.pop_back();
	return res;
}

namespace domain {

//******************************************************************************
GlobalParams::GlobalParams(Timepoint* t)
: Originator(t)
{}

//******************************************************************************
GlobalParams::GlobalParams(Params params, Timepoint* t)
: Originator(t, std::move(params))
{}


//******************************************************************************
template<auto Member, typename MemberType>
auto const& GlobalParams::get_per_axis_per_criteria(Axis axis, Material const* material) const {
	return get_per_axis_per_criteria<Member, MemberType>(axis, material, get_current_state());
}

//******************************************************************************
template<auto Member, typename MemberType>
auto const& GlobalParams::get_per_axis_per_criteria(Axis axis, Material const* material, Params const& state) const {
	using Key = MemberType::key_type;

	vector<Key> to_try;
	if(material) {
		if(!material->name.empty()) {
			to_try.emplace_back(axis, material->name);
			to_try.emplace_back(Params::ALL, material->name);
		}
		to_try.emplace_back(axis, material->type);
		to_try.emplace_back(Params::ALL, material->type);
	}
	to_try.emplace_back(axis, Params::ALL);
	to_try.emplace_back(Params::ALL, Params::ALL);

	for(auto const& k : to_try)
		if((state.*Member).contains(k))
			return (state.*Member).at(k);

	{
		[[unlikely]]
		log({
			.level = Logger::Level::ERROR,
			.user_actions = { Logger::UserAction::OK },
			.message = "This should never happen, please report a bug:",
			.informative = "Did not match any value to answer the request",
			.details = format(
				"Requested value:\n"
				"- axis: {}\n"
				"- material: {}\n"
				"Available values:\n{}\n"
				"Location (Member is what matters):\n{}",
				to_string(axis),
				(material
				? format("\n    - type: {}\n    - name: {}",
					to_string(material->type),
					material->name)
				: string("nullptr")),
				to_string(state.*Member),
				source_location::current().function_name()
				)
			});
		abort();
//		::unreachable(); // { ALL, ALL } rule MUST be always present if not better
	}
}

//******************************************************************************
auto GlobalParams::get_dmax(Axis axis, Material const* material) const -> decltype(Params::dmax)::mapped_type const& {
	return get_dmax(axis, material, get_current_state());
}

//******************************************************************************
auto GlobalParams::get_lmin(Axis axis, Material const* material) const -> decltype(Params::lmin)::mapped_type const& {
	return get_lmin(axis, material, get_current_state());
}

//******************************************************************************
auto GlobalParams::get_smoothness(Axis axis, Material const* material) const -> decltype(Params::smoothness)::mapped_type const& {
	return get_smoothness(axis, material, get_current_state());
}

//******************************************************************************
auto GlobalParams::get_dmax(Axis axis, Material const* material, Params const& state) const -> decltype(Params::dmax)::mapped_type const& {
	return get_per_axis_per_criteria<&Params::dmax, decltype(Params::dmax)>(axis, material, state);
}

//******************************************************************************
auto GlobalParams::get_lmin(Axis axis, Material const* material, Params const& state) const -> decltype(Params::lmin)::mapped_type const& {
	return get_per_axis_per_criteria<&Params::lmin, decltype(Params::lmin)>(axis, material, state);
}

//******************************************************************************
auto GlobalParams::get_smoothness(Axis axis, Material const* material, Params const& state) const -> decltype(Params::smoothness)::mapped_type const& {
	return get_per_axis_per_criteria<&Params::smoothness, decltype(Params::smoothness)>(axis, material, state);
}

//******************************************************************************
double GlobalParams::switch_length_between_absolute_and_wavelength_relative(double length) const noexcept {
	return switch_length_between_absolute_and_wavelength_relative(get_current_state(), length);
}

//******************************************************************************
double GlobalParams::switch_length_between_absolute_and_wavelength_relative(Params const& state, double length) noexcept {
	return state.wavelength_min_vacuum / state.delta_unit / length;
}

//******************************************************************************
void GlobalParams::switch_all_lengths_between_absolute_and_wavelength_relative(Params& state) noexcept {
		state.diagonal_dmax = switch_length_between_absolute_and_wavelength_relative(state, state.diagonal_dmax);
		state.proximity_limit = switch_length_between_absolute_and_wavelength_relative(state, state.proximity_limit);
		for(auto& [_, v] : state.dmax)
			v = switch_length_between_absolute_and_wavelength_relative(state, v);
}

} // namespace domain
