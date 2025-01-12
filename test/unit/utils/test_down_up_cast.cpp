///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include <memory>
#include <iostream>

#include "utils/down_up_cast.hpp"

/// @test template<typename UpFrom, typename UpTo, std::derived_from<UpTo> ...Down>
///       UpTo* down_up_cast(UpFrom* from)
///*****************************************************************************

using namespace std;

struct X { virtual ~X() = default; };
struct Y { virtual ~Y() = default; };

struct A : public X, public Y {};
struct B : public X, public Y {};
struct C : public Y {};
struct D : public X {};

//******************************************************************************
SCENARIO("template<typename UpFrom, typename UpTo, std::derived_from<UpTo> ...Down> \
UpTo* down_up_cast(UpFrom* from)", "[utils][down_up_cast]") {
	GIVEN("Two base classes X, Y; two classes A, B that derive from both; two class C, D that derive from just one of the two each") {
		AND_GIVEN("An A object ax stored as a pointer to X") {
			unique_ptr<X> ax(new A);

			WHEN("Down-up casting ax to Y through A") {
				THEN("Should succeed") {
					REQUIRE(down_up_cast<Y, A>(ax.get()) == dynamic_cast<Y*>(dynamic_cast<A*>(ax.get())));
					REQUIRE(down_up_cast<Y, A>(ax.get()) != nullptr);
				}
			}

			WHEN("Down-up casting ax to Y through B") {
				THEN("Should fail") {
					REQUIRE(down_up_cast<Y, B>(ax.get()) == dynamic_cast<Y*>(dynamic_cast<B*>(ax.get())));
					REQUIRE(down_up_cast<Y, B>(ax.get()) == nullptr);
				}
			}

			WHEN("Down-up casting ax to Y through C") {
				THEN("Should fail") {
					REQUIRE(down_up_cast<Y, C>(ax.get()) == dynamic_cast<Y*>(dynamic_cast<C*>(ax.get())));
					REQUIRE(down_up_cast<Y, C>(ax.get()) == nullptr);
				}
			}

			WHEN("Down-up casting ax to Y through D") {
				THEN("Should not compile because D is not derived from Y") {
					// REQUIRE(down_up_cast<Y, D>(ax.get()) == dynamic_cast<Y*>(dynamic_cast<D*>(ax.get())));
					// REQUIRE(down_up_cast<Y, D>(ax.get()) == nullptr);
				}
			}

			WHEN("Down-up casting ax to Y through A then B") {
					REQUIRE(down_up_cast<Y, A, B>(ax.get()) == dynamic_cast<Y*>(dynamic_cast<A*>(ax.get())));
					REQUIRE(down_up_cast<Y, A, B>(ax.get()) != nullptr);
				THEN("Should succeed") {
				}
			}

			WHEN("Down-up casting ax to Y through B then A") {
				THEN("Should succeed") {
					REQUIRE(down_up_cast<Y, B, A>(ax.get()) == dynamic_cast<Y*>(dynamic_cast<A*>(ax.get())));
					REQUIRE(down_up_cast<Y, B, A>(ax.get()) != nullptr);
				}
			}

			WHEN("Down-up casting ax to Y through B then C") {
				THEN("Should fail") {
					REQUIRE(down_up_cast<Y, B, C>(ax.get()) == dynamic_cast<Y*>(dynamic_cast<C*>(ax.get())));
					REQUIRE(down_up_cast<Y, B, C>(ax.get()) == dynamic_cast<Y*>(dynamic_cast<D*>(ax.get())));
					REQUIRE(down_up_cast<Y, B, C>(ax.get()) == nullptr);
				}
			}
		}
	}
}
