///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include <string>
#include <utility>

#include "utils/vector_utils.hpp"

/// @test template<typename T> std::vector<T*> create_view(
///       	std::vector<std::unique_ptr<T>> const& original)
/// @test template<typename T> std::vector<std::unique_ptr<T const>> from_init_list(
///       	std::initializer_list<T> const& original)
///*****************************************************************************

//******************************************************************************
SCENARIO("template<typename T> std::vector<T*> create_view( \
std::vector<std::unique_ptr<T>> \
const& original)", "[utils][vector_utils]") {
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

//******************************************************************************
SCENARIO("template<typename T> std::vector<std::unique_ptr<T const>> from_init_list( \
std::initializer_list<T> const& original)", "[vector_utils]") {
	GIVEN("An initializer list of strings") {
		std::initializer_list<std::string> a {
				std::string("aa"),
				std::string("b"),
				std::string("333"),
				std::string("dd") };

		THEN("Should return an equivalent vector of unique_ptr to const strings") {
		std::vector<std::unique_ptr<std::string const>> b = from_init_list(a);
			REQUIRE(b.size() == a.size());
			for(auto it = std::make_pair(begin(a), begin(b))
			; it.first != end(a)
			; ++it.first, ++it.second) {
				REQUIRE(*(it.second->get()) == *it.first);
			}
		}
	}
}
