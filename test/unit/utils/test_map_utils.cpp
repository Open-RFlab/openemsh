///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include <string>

#include "utils/map_utils.hpp"

/// @test template<typename K, typename V, typename CIn, typename COut = std::less<>>
///       std::map<V, K, COut> reverse_kv(std::map<K, V, CIn> const& in)
///*****************************************************************************

//******************************************************************************
SCENARIO("template<typename K, typename V, typename CIn, typename COut = std::less<>> \
std::map<V, K, COut> reverse_kv(std::map<K, V, CIn> const& in)", "[utils][map_utils]") {
	enum class Value {
		ZERO,
		ONE,
		TWO,
		THREE
	};
	GIVEN("A map of string keys and enum values") {
		std::map<std::string, Value> m {
			{"zero", Value::ZERO },
			{"one", Value::ONE },
			{"two", Value::TWO },
			{"three", Value::THREE }
		};
		THEN("Should return a map of enum keys and string values") {
			std::map<Value, std::string> r = reverse_kv(m);
			REQUIRE(r.size() == m.size());
			REQUIRE(r.at(Value::ZERO) == "zero");
			REQUIRE(r.at(Value::ONE) == "one");
			REQUIRE(r.at(Value::TWO) == "two");
			REQUIRE(r.at(Value::THREE) == "three");
		}
	}
}
