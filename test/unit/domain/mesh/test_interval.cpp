///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include <vector>

#include "domain/mesh/meshline.hpp"
#include "domain/mesh/meshline_policy.hpp"

#include "domain/mesh/interval.hpp"

/// @test Interval::Side::Side(MeshlinePolicy* meshline_policy, size_t lmin, double lambda, Coord h, function<double (double)> d_init)
/// @test double Interval::Side::d_init_(double d)
/// @test Coord Interval::s(Interval::Side const& side) const @todo
/// @test Coord Interval::s(Interval::Side const& side, double d) const @todo
/// @test std::vector<Coord> find_ls(double d, double lambda, double dmax, Coord s)
/// @test bool is_ls_valid_for_dmax_lmin_lambda(std::vector<Coord> ls, double d, double lambda, double dmax, size_t lmin)
/// @test void Interval::update_ls() @todo
/// @test void Interval::update_ls(Interval::Side& side) @todo
/// @test double find_dmax(Interval::Side const& side, double dmax)
/// @test double find_dmax(Interval::Side const& side, Interval::Side const& b, double dmax)
/// @test std::tuple<double, bool> Interval::adjust_d_for_dmax_lmin(Interval::Side const& side, size_t iter_limit) const
/// @test std::tuple<double, bool> Interval::adjust_lambda_for_s(Interval::Side const& side, size_t iter_limit) const
/// @test void Interval::auto_solve_d() @todo
/// @test void Interval::auto_solve_lambda() @todo
/// @test std::vector<std::unique_ptr<Meshline>> Interval::mesh() const
///*****************************************************************************

using namespace domain;

//******************************************************************************
SCENARIO("Interval::Side::Side(MeshlinePolicy* meshline_policy, size_t lmin, double lambda, Coord h, function<double (double)> d_init)", "[interval]") {
	GIVEN("Two meshline policies") {
		Params p;
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::ONELINE,
			MeshlinePolicy::Normal::NONE,
			p,
			10);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::ONELINE,
			MeshlinePolicy::Normal::NONE,
			p,
			20);
		a.d = 2;
		b.d = 15;
		WHEN("Creating an Interval's sides") {
			Interval i(&a, &b, Y, p);
			THEN("d should be limited to the half of the distance between both policies") {
				REQUIRE(a.d == 2);
				REQUIRE(b.d == 5);
			}
		}
	}
}

//******************************************************************************
SCENARIO("double Interval::Side::d_init_(double d)", "[interval]") {
	Params p;
	double d = 1;

	GIVEN("An Interval between two MeshlinePolicy") {
		WHEN("Meshline policies are ONELINE") {
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::ONELINE,
				MeshlinePolicy::Normal::NONE,
				p,
				10);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::ONELINE,
				MeshlinePolicy::Normal::NONE,
				p,
				20);
			Interval i(&a, &b, Y, p);
			THEN("d_init should be 0") {
				REQUIRE(i.before.d_init_(d) == 0);
				REQUIRE(i.after.d_init_(d) == 0);
			}
		}

		WHEN("Meshline policies are HALFS") {
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				10);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				20);
			Interval i(&a, &b, Y, p);
			THEN("d_init should be d/2") {
				REQUIRE(i.before.d_init_(d) == d/2);
				REQUIRE(i.after.d_init_(d) == d/2);
			}
		}

		WHEN("Meshline policies are THIRDS") {
			AND_WHEN("Meshline Normal are MIN") {
				MeshlinePolicy a(
					Y,
					MeshlinePolicy::Policy::THIRDS,
					MeshlinePolicy::Normal::MIN,
					p,
					10);
				MeshlinePolicy b(
					Y,
					MeshlinePolicy::Policy::THIRDS,
					MeshlinePolicy::Normal::MIN,
					p,
					20);
				Interval i(&a, &b, Y, p);
				THEN("d_init should be d/3 inside and 2d/3 outside") {
					REQUIRE(i.before.d_init_(d) == 1.0/3.0 * d);
					REQUIRE(i.after.d_init_(d) == 2.0/3.0 * d);
				}
			}

			AND_WHEN("Meshline Normal are MAX") {
				MeshlinePolicy a(
					Y,
					MeshlinePolicy::Policy::THIRDS,
					MeshlinePolicy::Normal::MAX,
					p,
					10);
				MeshlinePolicy b(
					Y,
					MeshlinePolicy::Policy::THIRDS,
					MeshlinePolicy::Normal::MAX,
					p,
					20);
				Interval i(&a, &b, Y, p);
				THEN("d_init should be d/3 inside and 2d/3 outside") {
					REQUIRE(i.before.d_init_(d) == 2.0/3.0 * d);
					REQUIRE(i.after.d_init_(d) == 1.0/3.0 * d);
				}
			}

			AND_WHEN("Meshline Normal are NONE") {
				MeshlinePolicy a(
					Y,
					MeshlinePolicy::Policy::THIRDS,
					MeshlinePolicy::Normal::NONE,
					p,
					10);
				MeshlinePolicy b(
					Y,
					MeshlinePolicy::Policy::THIRDS,
					MeshlinePolicy::Normal::NONE,
					p,
					20);
				Interval i(&a, &b, Y, p);
				THEN("d_init should be d") {
					REQUIRE(i.before.d_init_(d) == d);
					REQUIRE(i.after.d_init_(d) == d);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("std::vector<Coord> find_ls(double d, double lambda, double dmax, Coord s)", "[interval]") {
	GIVEN("d > dmax") {
		std::vector<Coord> ls = find_ls(4.0, 2.0, 3.0, 10.0);
		THEN("Coordinates should grow according 'ls[n] = ls[n-1] + dmax' algorithm") {
			REQUIRE(ls.size() == 4);
			REQUIRE(ls[0] == Coord(3.0));
			REQUIRE(ls[1] == Coord(6.0));
			REQUIRE(ls[2] == Coord(9.0));
			AND_THEN("Should stop once s is passed") {
				REQUIRE(ls[3] == Coord(12.0));
			}
		}
	}

	GIVEN("dmax will be reached to fill s") {
		std::vector<Coord> ls = find_ls(0.1, 2.0, 3.0, 10.0);
		THEN("Coordinates should grow according 'ls[n] = ls[n-1] + ls[n-1] * lambda' algorithm until ls[n] reaches dmax") {
			REQUIRE(ls.size() == 7);
			REQUIRE(ls[0] == Coord(0.2));
			REQUIRE(ls[1] == Coord(0.6));
			REQUIRE(ls[2] == Coord(1.4));
			REQUIRE(ls[3] == Coord(3.0));
			AND_THEN("Coordinates should grow according 'ls[n] = ls[n-1] + dmax' algorithm") {
				REQUIRE(ls[4] == Coord(6.0));
				REQUIRE(ls[5].value() == Coord(9.0));
				AND_THEN("Should stop once s is passed") {
					REQUIRE(ls[6] == Coord(12.0));
				}
			}
		}
	}

	GIVEN("dmax will not be reached to fill s") {
		std::vector<Coord> ls = find_ls(0.1, 2.0, 30.0, 10.0);
		THEN("Coordinates should grow according 'ls[n] = ls[n-1] + ls[n-1] * lambda' algorithm") {
			REQUIRE(ls.size() == 6);
			REQUIRE(ls[0] == Coord(0.2));
			REQUIRE(ls[1] == Coord(0.6));
			REQUIRE(ls[2] == Coord(1.4));
			REQUIRE(ls[3] == Coord(3.0));
			REQUIRE(ls[4] == Coord(6.2));
			AND_THEN("Should stop once s is passed") {
				REQUIRE(ls[5] == Coord(12.6));
			}
		}
	}

	GIVEN("d > s") {
		std::vector<Coord> ls = find_ls(3.0, 2.0, 7.0, 2.0);
		THEN("Should return only one coordinate, after s") {
			REQUIRE(ls.size() == 1);
			REQUIRE(ls[0] == Coord(6.0));
		}
	}
}

//******************************************************************************
SCENARIO("bool is_ls_valid_for_dmax_lmin_lambda(std::vector<Coord> ls, double d, double lambda, double dmax, size_t lmin)", "[interval]") {
	GIVEN("An empty ls vector") {
		THEN("Should not be valid, disregarding lmin") {
			REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda({}, 1.0, 2.0, 3.0, 2));
			REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda({}, 1.0, 2.0, 3.0, 0));
		}
	}

	GIVEN("An ls vector containing one line") {
		std::vector<Coord> ls { 0.2 };
		WHEN("There is less than lmin lines") {
			THEN("Should not be valid") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(ls, 0.1, 2.0, 30.0, 10));
			}
		}

		WHEN("A space between two lines, including d exceeds dmax") {
			THEN("Should not be valid") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(ls, 0.1, 2.0, 0.05, 5));
			}
		}

		WHEN("A space between two lines is not between lambda times and 1/lambda times its adjacent spaces, d being the space previous the first ls space") {
			THEN("Should not be valid") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(ls, 0.05, 2.0, 30.0, 5));
			}
		}

		WHEN("There is lmin lines or more") {
			AND_WHEN("No space between two lines exceeds dmax") {
				AND_WHEN("Every space between two lines is between lambda times and 1/lambda times its adjacent spaces") {
					THEN("Should be valid") {
						REQUIRE(is_ls_valid_for_dmax_lmin_lambda(ls, 0.1, 2.0, 30.0, 1));
					}
				}
			}
		}
	}

	GIVEN("An arbitrary sized ls vector") {
		std::vector<Coord> ls { 0.2, 0.6, 1.4, 3.0, 6.2, 12.6 };
		WHEN("There is less than lmin lines") {
			THEN("Should not be valid") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(ls, 0.1, 2.0, 30.0, 10));
			}
		}

		WHEN("A space between two lines, including d exceeds dmax") {
			THEN("Should not be valid") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(ls, 0.1, 2.0, 0.05, 5));
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(ls, 0.1, 2.0, 0.15, 5));
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(ls, 0.1, 2.0, 0.25, 5));
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(ls, 0.1, 2.0, 5.0, 5));
			}
		}

		WHEN("A space between two lines is not between lambda times and 1/lambda times its adjacent spaces, d being the space previous the first ls space") {
			THEN("Should not be valid") {
				std::vector<Coord> ls1 { 0.3, 0.6, 1.4, 3.0, 6.2, 12.6 };
				std::vector<Coord> ls2 { 0.2, 1.0, 1.4, 3.0, 6.2, 12.6 };
				std::vector<Coord> ls3 { 0.2, 0.6, 1.4, 3.0, 6.2, 13.6 };
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(ls, 0.05, 2.0, 30.0, 5));
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(ls1, 0.1, 2.0, 30.0, 5));
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(ls2, 0.1, 2.0, 30.0, 5));
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(ls3, 0.1, 2.0, 30.0, 5));

			}
		}

		WHEN("There is lmin lines or more") {
			AND_WHEN("No space between two lines exceeds dmax") {
				AND_WHEN("Every space between two lines is between lambda times and 1/lambda times its adjacent spaces") {
					THEN("Should be valid") {
						REQUIRE(is_ls_valid_for_dmax_lmin_lambda(ls, 0.1, 2.0, 30.0, 5));
					}
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("double find_dmax(Interval::Side const& side, double dmax)", "[interval]") {
	GIVEN("A Side of an Interval with previously computed ls") {
		Params p;
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::NONE,
			p,
			10);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::NONE,
			p,
			20);
		a.d = 1;
		p.lambda = 2;
		Interval i(&a, &b, Y, p);
		WHEN("ls contains no lines") {
			THEN("Should return dmax") {
				REQUIRE_FALSE(i.before.ls.size());
				REQUIRE(find_dmax(i.before, 2.0) == 2.0);
			}
		}

		WHEN("ls contains one line") {
			i.before.ls = { 3.0 };
			AND_WHEN("The line coord does not exceed dmax") {
				THEN("Should return lambda times the line coord") {
					REQUIRE(i.before.ls.size() == 1);
					REQUIRE(find_dmax(i.before, 8.0) == 6.0);
				}
			}

			AND_WHEN("The line coord exceed dmax") {
				THEN("Should return dmax") {
					REQUIRE(i.before.ls.size() == 1);
					REQUIRE(find_dmax(i.before, 2.0) == 2.0);
				}
			}
		}

		WHEN("ls contains some lines") {
			i.before.ls = { 1.0, 3.0, 6.0 };
			AND_WHEN("The last space does not exceed dmax") {
				THEN("Should return lambda times the last space") {
					REQUIRE(i.before.ls.size() == 3);
					REQUIRE(find_dmax(i.before, 8.0) == 6.0);
				}
			}

			AND_WHEN("The last space exceed dmax") {
				THEN("Should return dmax") {
					REQUIRE(i.before.ls.size() == 3);
					REQUIRE(find_dmax(i.before, 2.0) == 2.0);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("double find_dmax(Interval::Side const& side, Interval::Side const& b, double dmax)", "[interval]") {
	GIVEN("Two Sides of an Interval with previously computed ls") {
		Params p;
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::NONE,
			p,
			10);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::NONE,
			p,
			20);
		a.d = 1;
		p.lambda = 2;
		Interval i(&a, &b, Y, p);
		i.before.ls = { 2.0 };
		i.after.ls = { 1.0, 3.0, 6.0 };
		THEN("Should return the minimal dmax of both Sides'") {
			REQUIRE(find_dmax(i.before, 5.0) == 4.0);
			REQUIRE(find_dmax(i.after, 5.0) == 5.0);
			REQUIRE(find_dmax(i.before, i.after, 5.0) == 4.0);
			REQUIRE(find_dmax(i.after, i.before, 5.0) == 4.0);
		}
	}
}

//******************************************************************************
SCENARIO("std::tuple<double, bool> Interval::adjust_d_for_dmax_lmin(Interval::Side const& side, size_t iter_limit) const", "[interval]") {
	GIVEN("A Side of an Interval with previously computed ls") {
		WHEN("dmax and lmin end criteras are already respected") {
			Params p;
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				10);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				20);
			p.dmax = 1.2;
			p.lmin = 2;
			a.d = 1.0;
			Interval i(&a, &b, Y, p);
			i.update_ls();
			THEN("Initial ls should be conform") {
				REQUIRE(is_ls_valid_for_dmax_lmin_lambda(i.before.ls, a.d, i.before.lambda, p.dmax, p.lmin));
			}

			THEN("Side's d should be unchanged") {
				auto new_d = std::get<0>(i.adjust_d_for_dmax_lmin(i.before));
				REQUIRE(new_d == 1.0);
			}
		}

		WHEN("dmax end critera is already respected but not lmin one") {
			Params p;
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				10);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				20);
			p.dmax = 2.0;
			p.lmin = 10;
			a.d = 1.0;
			Interval i(&a, &b, Y, p);
			i.update_ls();
			THEN("Initial ls should not be conform") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(i.before.ls, a.d, i.before.lambda, p.dmax, p.lmin));
			}

			THEN("Side's d should be reduced") {
				a.d = std::get<0>(i.adjust_d_for_dmax_lmin(i.before));
				REQUIRE(a.d < 1.0);
				AND_THEN("New ls should be conform") {
					i.update_ls();
					REQUIRE(is_ls_valid_for_dmax_lmin_lambda(i.before.ls, a.d, i.before.lambda, p.dmax, p.lmin));
				}
			}
		}

		WHEN("lmin end critera is already respected but not dmax one") {
			Params p;
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				10);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				20);
			p.dmax = 0.8;
			p.lmin = 2;
			a.d = 1.0;
			Interval i(&a, &b, Y, p);
			i.update_ls();
			THEN("Initial ls should not be conform") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(i.before.ls, a.d, i.before.lambda, p.dmax, p.lmin));
			}

			THEN("Side's d should be reduced") {
				a.d = std::get<0>(i.adjust_d_for_dmax_lmin(i.before));
				REQUIRE(a.d <= 0.8);
				AND_THEN("New ls should be conform") {
					i.update_ls();
					REQUIRE(is_ls_valid_for_dmax_lmin_lambda(i.before.ls, a.d, i.before.lambda, p.dmax, p.lmin));
				}
			}
		}

		WHEN("dmax and lmin end criteras are not respected") {
			Params p;
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				10);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				20);
			p.dmax = 0.8;
			p.lmin = 10;
			a.d = 1.0;
			Interval i(&a, &b, Y, p);
			i.update_ls();
			THEN("Initial ls should not be conform") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(i.before.ls, a.d, i.before.lambda, p.dmax, p.lmin));
			}

			AND_WHEN("Process iterations are unlimited") {
				THEN("Side's d should be reduced") {
					auto [new_d, is_limit_reached] = i.adjust_d_for_dmax_lmin(i.before);
					a.d = new_d;
					REQUIRE_FALSE(is_limit_reached);
					REQUIRE(a.d <= 0.8);
					AND_THEN("New ls should be conform") {
						i.update_ls();
						REQUIRE(is_ls_valid_for_dmax_lmin_lambda(i.before.ls, a.d, i.before.lambda, p.dmax, p.lmin));
					}
				}
			}

			AND_WHEN("Process iterations are limited") {
				THEN("Side's d should be reduced") {
					auto [new_d, is_limit_reached] = i.adjust_d_for_dmax_lmin(i.before, 20);
					a.d = new_d;
					REQUIRE(is_limit_reached);
					REQUIRE(a.d <= 0.8);
					AND_THEN("New ls should not be conform") {
						i.update_ls();
						REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_lambda(i.before.ls, a.d, i.before.lambda, p.dmax, p.lmin));
					}
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("std::tuple<double, bool> Interval::adjust_lambda_for_s(Interval::Side const& side, size_t iter_limit) const", "[interval]") {
	GIVEN("A Side of an Interval with previously computed valid ls with lambda superior to 2") {
		Params p;
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			p,
			0);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			p,
			20);
		p.dmax = 30.0;
		p.lmin = 5;
		p.lambda = 2;
		a.d = 0.1;
		Interval i(&a, &b, Y, p);
		Interval j(&a, &b, Y, p);
		i.update_ls();
		j.update_ls();
		Coord old_last_space = i.before.ls.back() - i.s(i.before);
		REQUIRE(is_ls_valid_for_dmax_lmin_lambda(i.before.ls, a.d, i.before.lambda, p.dmax, p.lmin));
		REQUIRE(is_ls_valid_for_dmax_lmin_lambda(j.before.ls, a.d, j.before.lambda, p.dmax, p.lmin));
		WHEN("Process iterations are unlimited") {
			THEN("Side's lambda should be reduced") {
				{
					auto [new_lambda, is_limit_reached] = i.adjust_lambda_for_s(i.before);
					i.before.lambda = new_lambda;
					i.update_ls();
					REQUIRE_FALSE(is_limit_reached);
					REQUIRE(i.before.lambda < p.lambda);
				}

				AND_THEN("The space between the last ls line and the middle of the Interval should be reduced") {
					Coord new_last_space = i.before.ls.back() - i.s(i.before);
					REQUIRE(new_last_space < old_last_space);

					AND_WHEN("Process iterations are limited") {
						THEN("Side's lambda should be reduced") {
							{
								auto [new_lambda, is_limit_reached] = j.adjust_lambda_for_s(j.before, 20);
								j.before.lambda = new_lambda;
								j.update_ls();
								REQUIRE(is_limit_reached);
								REQUIRE(j.before.lambda < p.lambda);
							}

							AND_THEN("The space between the last ls line and the middle of the Interval should be reduced, but less than if unlimited") {
								Coord new_last_space_limited = j.before.ls.back() - j.s(i.before);
								REQUIRE(new_last_space_limited < old_last_space);
								REQUIRE(new_last_space_limited > new_last_space);
							}
						}
					}
				}
			}
		}
	}

	GIVEN("A Side of an Interval with previously computed valid ls with lambda being 1") {
		Params p;
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			p,
			0);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			p,
			20);
		p.dmax = 30.0;
		p.lmin = 5;
		p.lambda = 1;
		a.d = 0.1;
		Interval i(&a, &b, Y, p);
		i.update_ls();
		Coord old_last_space = i.before.ls.back() - i.s(i.before);
		REQUIRE(is_ls_valid_for_dmax_lmin_lambda(i.before.ls, a.d, i.before.lambda, p.dmax, p.lmin));
		THEN("Side's lambda should not be reduced") {
			auto [new_lambda, is_limit_reached] = i.adjust_lambda_for_s(i.before);
			i.before.lambda = new_lambda;
			i.update_ls();
			REQUIRE_FALSE(is_limit_reached);
			REQUIRE(new_lambda == p.lambda);

			AND_THEN("The space between the last ls line and the middle of the Interval should not be reduced") {
				Coord new_last_space = i.before.ls.back() - i.s(i.before);
				REQUIRE(new_last_space == old_last_space);
			}
		}
	}
}

//******************************************************************************
SCENARIO("std::vector<std::unique_ptr<Meshline>> Interval::mesh() const", "[interval]") {
	auto const match_line =
		[](std::vector<std::unique_ptr<Meshline>> const& mesh, Coord const& line) {
			for(auto const& it : mesh)
				if(it->coord == line)
					return true;
			return false;
		};

	GIVEN("An Interval with ls computed for both Sides") {
		WHEN("Sides' policies are HALFS or THIRDS") {
			Params p;
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				10);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::THIRDS,
				MeshlinePolicy::Normal::MAX,
				p,
				30);
			MeshlinePolicy c(
				Y,
				MeshlinePolicy::Policy::THIRDS,
				MeshlinePolicy::Normal::MAX,
				p,
				10);
			MeshlinePolicy d(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				30);
			p.dmax = 2;
			p.lmin = 5;
			p.lambda = 2;
			a.d = 0.1;
			b.d = 0.1;
			c.d = 0.1;
			d.d = 0.1;
			Interval i(&a, &b, Y, p);
			Interval j(&c, &d, Y, p);
			i.update_ls();
			j.update_ls();
			auto mesh_i = i.mesh();
			auto mesh_j = j.mesh();
			THEN("Policy lines should be placed") {
				REQUIRE(mesh_i.size() >= 2);
				REQUIRE(mesh_j.size() >= 2);
				REQUIRE(mesh_i.front()->coord == Coord(10.05));
				REQUIRE(mesh_i.back()->coord.value() == Coord(30 - 0.1/3));
				REQUIRE(mesh_j.front()->coord == Coord(10 + 2 * 0.1/3));
				REQUIRE(mesh_j.back()->coord.value() == Coord(29.95));
			}

			THEN("Last line of both sides should not be placed") {
				REQUIRE_FALSE(match_line(mesh_i, Coord(
					i.before.meshline_policy->coord
					+ i.before.d_init()
					+ i.before.ls.back())));
				REQUIRE_FALSE(match_line(mesh_i, Coord(
					i.after.meshline_policy->coord
					- i.after.d_init()
					- i.after.ls.back())));

				REQUIRE_FALSE(match_line(mesh_j, Coord(
					j.before.meshline_policy->coord
					+ j.before.d_init()
					+ j.before.ls.back())));
				REQUIRE_FALSE(match_line(mesh_j, Coord(
					j.after.meshline_policy->coord
					- j.after.d_init()
					- j.after.ls.back())));

				AND_THEN("Other lines should be placed") {
					for(auto it = begin(i.before.ls); it != prev(end(i.before.ls)); ++it)
						REQUIRE(match_line(mesh_i, Coord(
							i.before.meshline_policy->coord
							+ i.before.d_init()
							+ (*it))));
					for(auto it = begin(i.after.ls); it != prev(end(i.after.ls)); ++it)
						REQUIRE(match_line(mesh_i, Coord(
							i.after.meshline_policy->coord
							- i.after.d_init()
							- (*it))));

					for(auto it = begin(j.before.ls); it != prev(end(j.before.ls)); ++it)
						REQUIRE(match_line(mesh_j, Coord(
							j.before.meshline_policy->coord
							+ j.before.d_init()
							+ (*it))));
					for(auto it = begin(j.after.ls); it != prev(end(j.after.ls)); ++it)
						REQUIRE(match_line(mesh_j, Coord(
							j.after.meshline_policy->coord
							- j.after.d_init()
							- (*it))));
				}
			}

			THEN("Middle line should be placed instead") {
				REQUIRE(match_line(mesh_i, i.m));
				REQUIRE(match_line(mesh_j, j.m));
			}
		}

		WHEN("Sides' policies are ONELINE") {
			Params p;
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::ONELINE,
				MeshlinePolicy::Normal::NONE,
				p,
				10);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::ONELINE,
				MeshlinePolicy::Normal::NONE,
				p,
				30);
			p.dmax = 1.5;
			p.lmin = 5;
			p.lambda = 2;
			a.d = 0.1;
			b.d = 0.1;
			Interval i(&a, &b, Y, p);
			i.update_ls();
			auto mesh = i.mesh();
			THEN("Policy lines should not be placed") {
				REQUIRE_FALSE(match_line(mesh, i.before.meshline_policy->coord));
				REQUIRE_FALSE(match_line(mesh, i.after.meshline_policy->coord));
			}

			THEN("Last line of both sides should not be placed") {
				REQUIRE_FALSE(match_line(mesh, Coord(
					i.before.meshline_policy->coord
					+ i.before.d_init()
					+ i.before.ls.back())));
				REQUIRE_FALSE(match_line(mesh, Coord(
					i.after.meshline_policy->coord
					- i.after.d_init()
					- i.after.ls.back())));

				AND_THEN("Other lines should be placed") {
					for(auto it = begin(i.before.ls); it != prev(end(i.before.ls)); ++it)
						REQUIRE(match_line(mesh, Coord(
							i.before.meshline_policy->coord
							+ i.before.d_init()
							+ (*it))));
					for(auto it = begin(i.after.ls); it != prev(end(i.after.ls)); ++it)
						REQUIRE(match_line(mesh, Coord(
							i.after.meshline_policy->coord
							- i.after.d_init()
							- (*it))));
				}
			}

			THEN("Middle line should be placed instead") {
				REQUIRE(match_line(mesh, i.m));
				REQUIRE(match_line(mesh, i.m));
			}
		}
	}

	GIVEN("An Interval without ls computed for both Sides") {
		WHEN("Sides' policies are HALFS or THIRDS") {
			Params p;
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				10);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::THIRDS,
				MeshlinePolicy::Normal::MAX,
				p,
				30);
			MeshlinePolicy c(
				Y,
				MeshlinePolicy::Policy::THIRDS,
				MeshlinePolicy::Normal::MAX,
				p,
				10);
			MeshlinePolicy d(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				30);
			p.dmax = 2;
			p.lmin = 5;
			p.lambda = 2;
			a.d = 0.1;
			b.d = 0.1;
			c.d = 0.1;
			d.d = 0.1;
			Interval i(&a, &b, Y, p);
			Interval j(&c, &d, Y, p);
			auto mesh_i = i.mesh();
			auto mesh_j = j.mesh();
			THEN("The only lines should be policy lines and middle line") {
				REQUIRE(mesh_i.size() == 3);
				REQUIRE(mesh_j.size() == 3);
				REQUIRE(mesh_i[0]->coord == Coord(10.05));
				REQUIRE(mesh_i[1]->coord == i.m);
				REQUIRE(mesh_i[2]->coord == Coord(30 - 0.1/3));
				REQUIRE(mesh_j[0]->coord == Coord(10 + 2 * 0.1/3));
				REQUIRE(mesh_j[1]->coord == j.m);
				REQUIRE(mesh_j[2]->coord == Coord(29.95));
			}
		}

		WHEN("Sides' policies are ONELINE") {
			Params p;
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::ONELINE,
				MeshlinePolicy::Normal::NONE,
				p,
				10);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::ONELINE,
				MeshlinePolicy::Normal::NONE,
				p,
				30);
			p.dmax = 1.5;
			p.lmin = 5;
			p.lambda = 2;
			a.d = 0.1;
			b.d = 0.1;
			Interval i(&a, &b, Y, p);
			auto mesh = i.mesh();
			THEN("The only line should be the middle line") {
				REQUIRE(mesh.size() == 1);
				REQUIRE(mesh.front()->coord == i.m);
			}
		}
	}
}

// test adjust_d
// TODO init : random ls
// TODO init : d init = 2
// TODO init : invalid ls
// TODO after : different d
// TODO after : valid ls
