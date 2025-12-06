///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <map>
#include <functional>

//******************************************************************************
template<typename K, typename V, typename CIn, typename COut = std::less<V>>
std::map<V, K, COut> reverse_kv(std::map<K, V, CIn> const& in) {
	std::map<V, K, COut> out;
	for(auto const& [k, v] : in)
		out.emplace(v, k);
	return out;
}
