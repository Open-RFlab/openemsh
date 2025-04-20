///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include <string>
#include <utility>

#include "utils/vector_utils.hpp"

/// @test template<PointerLike T>
///       std::vector<std::add_pointer_t<typename T::element_type>>
///       	create_view(std::vector<T> const& original) noexcept
/// @test template<typename T> std::vector<std::unique_ptr<T const>> from_init_list(
///       	std::initializer_list<T> const& original) noexcept
/// @test template<typename T> bool contains(
///       	std::vector<T> const& vector,
//        	T const& value) noexcept
/// @test template<typename T, typename P> bool contains_that(
///       	std::vector<T> const& vector,
///       	P&& predicate) noexcept
///*****************************************************************************

//******************************************************************************
SCENARIO("template<PointerLike T> \
std::vector<std::add_pointer_t<typename T::element_type>> \
create_view(std::vector<T> const& original) noexcept", "[utils][vector_utils]") {
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

	GIVEN("A vector of shared_ptr to string with members") {
		std::vector<std::shared_ptr<std::string>> a;
		a.emplace_back(std::make_shared<std::string>("aa"));
		a.emplace_back(std::make_shared<std::string>("b"));
		a.emplace_back(std::make_shared<std::string>("333"));
		a.emplace_back(std::make_shared<std::string>("dd"));
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

//******************************************************************************
SCENARIO("template<typename T> bool contains(std::vector<T> const& vector, T const& value) noexcept", "[vector_utils]") {
	GIVEN("A vector of strings") {
		std::vector<std::string> a;
		a.emplace_back("aa");
		a.emplace_back("b");
		a.emplace_back("333");
		a.emplace_back("dd");
		WHEN("Looking for a string that is in the vector") {
			THEN("Should return true") {
				REQUIRE(contains(a, std::string("b")));
			}
		}

		WHEN("Looking for a string that is not in the vector") {
			THEN("Should return false") {
				REQUIRE_FALSE(contains(a, std::string("ccc")));
			}
		}
	}
}

//******************************************************************************
SCENARIO("template<typename T, typename P> bool contains_that(std::vector<T> const& vector, P&& predicate) noexcept", "[vector_utils]") {
	GIVEN("A vector of unique_ptr to strings") {
		std::vector<std::unique_ptr<std::string>> a;
		a.emplace_back(std::make_unique<std::string>("aa"));
		a.emplace_back(std::make_unique<std::string>("b"));
		a.emplace_back(std::make_unique<std::string>("333"));
		a.emplace_back(std::make_unique<std::string>("dd"));
		WHEN("Looking for a predicate that matches a string in the vector") {
			THEN("Should return true") {
				std::string b("b");
				REQUIRE(contains_that(a,
					[&b](std::unique_ptr<std::string> const& str) {
						return *str == b;
					}));
			}
		}

		WHEN("Looking for a predicate that does not match any string in the vector") {
			THEN("Should return false") {
				std::string b("ccc");
				REQUIRE_FALSE(contains_that(a,
					[&b](std::unique_ptr<std::string> const& str) {
						return *str == b;
					}));
			}
		}
	}
}
