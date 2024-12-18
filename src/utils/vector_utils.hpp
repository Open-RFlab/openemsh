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

//******************************************************************************
template<typename T>
std::vector<T*> create_view(std::vector<std::unique_ptr<T>> const& original) noexcept {
	std::vector<T*> view(original.size());

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
	return std::find(std::begin(vector), std::end(vector), value) != std::end(vector);
}

//******************************************************************************
template<typename T, typename P>
bool contains_that(std::vector<T> const& vector, P&& predicate) noexcept {
	return std::find_if(std::begin(vector), std::end(vector), std::forward<decltype(predicate)>(predicate))
	    != std::end(vector);
}
