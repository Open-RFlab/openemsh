///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include <string>

#include "utils/vector_view.hpp"

/// @test template<typename T> std::vector<T*> create_view(
///       	std::vector<std::unique_ptr<T>> const& original)
///*****************************************************************************

//******************************************************************************
SCENARIO("template<typename T> std::vector<T*> create_view( \
std::vector<std::unique_ptr<T>> \
const& original)", "[utils][vector_view]") {
	GIVEN("A vector of unique_ptr to string with members") {
		std::vector<std::unique_ptr<std::string>> a;
		a.emplace_back(std::make_unique<std::string>("aa"));
		a.emplace_back(std::make_unique<std::string>("b"));
		a.emplace_back(std::make_unique<std::string>("333"));
		a.emplace_back(std::make_unique<std::string>("dd"));
		THEN("Should return an equivalent vector of raw pointers to the strings") {
			std::vector<std::string *> b = create_view(a);
			REQUIRE(b.size() == a.size());
			for(std::size_t i = 0; i < b.size(); ++i) {
				REQUIRE(b[i] == a[i].get());
				REQUIRE(*b[i] == *a[i]);
			}
		}
	}

	GIVEN("A vector of unique_ptr to string without members") {
		std::vector<std::unique_ptr<std::string>> a;
		THEN("Should return a vector of raw pointers to the strings without members") {
			std::vector<std::string *> b = create_view(a);
			REQUIRE(b.size() == a.size());
			REQUIRE(b.size() == 0);
		}
	}
}
