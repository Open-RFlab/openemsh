///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <vector>

#include "concepts.hpp"

//******************************************************************************
template<PointerLike T>
std::vector<std::add_pointer_t<typename T::element_type>> create_view(std::vector<T> const& original) noexcept {
	std::vector<std::add_pointer_t<typename T::element_type>> view(original.size());

	for(size_t i = 0, size = original.size(); i < size; ++i) {
		view[i] = original[i].get();
	}

	return view;
}

//******************************************************************************
template<typename T>
std::vector<std::unique_ptr<T const>> from_init_list(std::initializer_list<T> const& original) noexcept {
	std::vector<std::unique_ptr<T const>> out;

	for(auto const& it : original)
		out.push_back(std::make_unique<T>(it));

	out.shrink_to_fit();
	return out;
}

//******************************************************************************
template<typename T>
bool contains(std::vector<T> const& vector, T const& value) noexcept {
	return std::ranges::find(vector, value) != std::end(vector);
}

//******************************************************************************
template<typename T, typename P>
bool contains_that(std::vector<T> const& vector, P&& predicate) noexcept {
	return std::ranges::find_if(vector, std::forward<decltype(predicate)>(predicate))
	    != std::end(vector);
}

// TODO T = PointerLike or reference
//******************************************************************************
template<typename T, typename P>
std::vector<std::vector<T>>
find_consecutive_matches(std::vector<T> const& original, P const& predicate) {
	std::vector<std::vector<T>> result;

	bool does_prev_matched = false;
	for(auto const& it : original) {
		if(predicate(it)) {
			if(does_prev_matched) {
				result.back().emplace_back(it);
			} else {
				result.push_back({ it });
			}
			does_prev_matched = true;
		} else {
			does_prev_matched = false;
		}
	}

	return result;
}
