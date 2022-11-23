///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <functional>
#include <limits>
#include <memory>
#include <tuple>
#include <vector>

#include "domain/geometrics/coord.hpp"
#include "domain/geometrics/space.hpp"
#include "domain/global.hpp"
#include "utils/entity.hpp"

class Meshline;
class MeshlinePolicy;

#ifdef UNITTEST
#define private public
#endif // UNITTEST

//******************************************************************************
class Interval : public Entity, public Visitable<Interval, EntityVisitor> {
private:
	Params& params;

public:
	Axis const axis;

	struct Side {
		MeshlinePolicy* const meshline_policy;
		size_t lmin;      ///< Minimum line number to mesh this half of the interval.
		double lambda;    ///< Smoothness factor = ]1;2] .
		std::function<double (double)> const d_init_;
		std::vector<Coord> ls; // TODO avoid Coord::operator= -> double

		Side(MeshlinePolicy* meshline_policy, size_t lmin, double lambda, Coord h, std::function<double (double)> d_init);

		double d_init() const;
	};

	Coord const h;    ///< Distance between a side's coord and the middle m.
	Coord const m;    ///< Middle between both sides' coord.
	double dmax;      ///< Maximum distance between two adjacent meshlines.
	Side before;
	Side after;

	Interval(MeshlinePolicy* before, MeshlinePolicy* after, Axis axis, Params& params);

	void auto_solve_d();
	void auto_solve_lambda();
	std::vector<std::unique_ptr<Meshline>> mesh() const;

private:
	/// Distance between a side's policy line and the middle m. limit = h - d_init
	Coord s(Side const& side) const;
	Coord s(Side const& side, double d) const;

	void update_ls();
	void update_ls(Side& side);

	std::tuple<double, bool> adjust_d_for_dmax_lmin(
		Side const& side,
		size_t iter_limit=std::numeric_limits<std::size_t>::max()) const;
	std::tuple<double, bool> adjust_lambda_for_s(
		Side const& side,
		size_t iter_limit=std::numeric_limits<std::size_t>::max()) const;
};

#ifdef UNITTEST
#undef private
#endif // UNITTEST

//******************************************************************************
double find_dmax(Interval::Side const& side, double dmax);
double find_dmax(Interval::Side const& a, Interval::Side const& b, double dmax);

//******************************************************************************
std::vector<Coord> find_ls(double d, double lambda, double dmax, Coord s);

//******************************************************************************
bool is_ls_valid_for_dmax_lmin_lambda(std::vector<Coord> ls, double d, double lambda, double dmax, size_t lmin);
