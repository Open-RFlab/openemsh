///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

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
