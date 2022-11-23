///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <algorithm>

#include "utils/signum.hpp"
#include "utils/unreachable.hpp"
#include "meshline.hpp"
#include "meshline_policy.hpp"

#include "interval.hpp"

using namespace std;

//******************************************************************************
Interval::Side::Side(MeshlinePolicy* meshline_policy, size_t lmin, double lambda, Coord h, function<double (double)> d_init)
: meshline_policy(meshline_policy)
, lmin(lmin)
, lambda(lambda)
, d_init_(std::move(d_init))
{
	if(meshline_policy->d > h)
		meshline_policy->d = (double) h;
}

//******************************************************************************
double Interval::Side::d_init() const {
	return d_init_(meshline_policy->d);
}

//******************************************************************************
Interval::Interval(MeshlinePolicy* before, MeshlinePolicy* after, Axis axis, Params& params)
: params(params)
, axis(axis)
, h(distance(before->coord, after->coord) / 2)
, m(mid(before->coord, after->coord))
, dmax(params.dmax)
, before(before, params.lmin, params.lambda, h, [before](double d) noexcept {
	switch(before->policy) {
	case MeshlinePolicy::Policy::ONELINE: return 0.0;
	case MeshlinePolicy::Policy::HALFS: return d / 2.0;
	case MeshlinePolicy::Policy::THIRDS: return [&] {
		switch(before->normal) {
		case MeshlinePolicy::Normal::MAX:
			return 2.0/3.0 * d;
		case MeshlinePolicy::Normal::MIN:
			return 1.0/3.0 * d;
		default:
			return d;
		}
	} ();
	default: unreachable();
	}
})
, after(after, params.lmin, params.lambda, h, [after](double d) noexcept {
	switch(after->policy) {
	case MeshlinePolicy::Policy::ONELINE: return 0.0;
	case MeshlinePolicy::Policy::HALFS: return d / 2.0;
	case MeshlinePolicy::Policy::THIRDS: return [&] {
		switch(after->normal) {
		case MeshlinePolicy::Normal::MAX:
			return 1.0/3.0 * d;
		case MeshlinePolicy::Normal::MIN:
			return 2.0/3.0 * d;
		default:
			return d;
		}
	} ();
	default: unreachable();
	}
})
{}

//******************************************************************************
Coord Interval::s(Interval::Side const& side) const {
	return h - side.d_init_(side.meshline_policy->d);
}

//******************************************************************************
Coord Interval::s(Interval::Side const& side, double d) const {
	return h - side.d_init_(d);
}

/// Computes the lines that fill the space (z), from the policy line (at d_init
/// from side's coord) to the line after which the distance between adjacent
/// lines (d) is dmax.
///
/// Computation is done regarding d as initial distance between adjacent lines,
/// lambda as smoothness factor and dmax as limitating maximal distance between
/// adjacent lines.
///
/// If z exceeds 2 * h, we round it to 2 * h.
///*****************************************************************************
/*
vector<Coord> find_lz(double d, double lambda, double dmax, Coord h) {
	if(lambda == 1)
		return { numeric_limits<double>::infinity() };

//	double current_d = d; // TODO d * lambda
	double current_d = d * lambda; // TODO d * lambda
	double current_z = 0;
	vector<Coord> lz;
	while(current_d < dmax && current_z < 10 * h) {
		current_d *= lambda;
		if(current_d < dmax) {
			current_z += current_d;
			lz.push_back(current_z);
		}
	}

	return lz;
}
*/

/// Computes the lines that fill the space (s), from the policy line (at d_init
/// from side's coord) to the middle of the interval (at h from side's coord).
///
/// Computation is done regarding d as initial distance between adjacent lines,
/// lambda as smoothness factor and dmax as limitating maximal distance between
/// adjacent lines.
///*****************************************************************************
vector<Coord> find_ls(double d, double lambda, double dmax, Coord s) {
	double current_d = min(d, dmax);
	double current_s = 0;
	vector<Coord> ls;
	while(current_s < s) {
		if(current_d < dmax) {
			current_d *= lambda;
			if(current_d > dmax)
				current_d = dmax;
		}

		current_s += current_d;

		ls.push_back(current_s);
	}

	return ls;
}

/// Verifiy that ls satisfies the following criteras :
/// - At least lmin lines in ls.
/// - No space between adjacent lines should exceed dmax.
/// - No space between adjacent lines should be more than lambda times bigger
///   than the previous one.

/// - The last space between adjacent lines should not be less than dmax/lambda.

///*****************************************************************************
bool is_ls_valid_for_dmax_lmin_lambda(vector<Coord> ls, double d, double lambda, double dmax, size_t lmin) {
	if(d > dmax
	|| ls.size() < lmin
	|| ls.empty())
		return false;

	if(ls[0] > lambda * d + equality_tolerance
	|| ls[0] > dmax + equality_tolerance)
		return false;

	if(ls.size() >= 2) {
		if(ls[1] - ls[0] > lambda * ls[0] + equality_tolerance
		|| ls[1] - ls[0] > dmax + equality_tolerance)
			return false;
	}

	for(size_t i = 2; i < ls.size(); ++i) {
		if(ls[i] - ls[i-1] > lambda * (ls[i-1] - ls[i-2]) + equality_tolerance
		|| ls[i] - ls[i-1] > dmax + equality_tolerance)
			return false;
	}

	return true;
}

//******************************************************************************
double find_dmax(Interval::Side const& side, double dmax) {
/*
	if(side.lz.size() <= side.ls.size())
		return dmax;
	else
		return (double) (side.lz[side.ls.size()-1] - side.lz[side.ls.size()-2]);
	if(side.ls.size())
		return last_d(side.ls) * side.lambda;
*/
	if(side.ls.size() > 1)
		return min(dmax, side.lambda * (double) (side.ls.back() - side.ls[side.ls.size()-2]));
	else if(side.ls.size() == 1)
		return min(dmax, side.lambda * (double) side.ls[0]);
	else
		return dmax;
}

/// Select limiting d between both sides' maximal d and ultimately limitating
/// dmax.
///*****************************************************************************
double find_dmax(Interval::Side const& a, Interval::Side const& b, double dmax) {
	return min(find_dmax(a, dmax), find_dmax(b, dmax));
}

//******************************************************************************
void Interval::update_ls() {
	before.ls = find_ls(before.meshline_policy->d, before.lambda, dmax, s(before));
	after.ls = find_ls(after.meshline_policy->d, after.lambda, dmax, s(after));
}

//******************************************************************************
void Interval::update_ls(Interval::Side& side) {
	side.ls = find_ls(side.meshline_policy->d, side.lambda, dmax, s(side));
}

//******************************************************************************
tuple<double, bool> Interval::adjust_d_for_dmax_lmin(Interval::Side const& side, size_t iter_limit) const {
	size_t const step = 1000;
	double current_d = min(side.meshline_policy->d, dmax);

	vector<Coord> current_ls = side.ls;

	size_t counter = 0;
	bool is_limit_reached = false;
	while(!is_ls_valid_for_dmax_lmin_lambda(current_ls, current_d, side.lambda, dmax, side.lmin)) {
		current_d -= current_d / step;
		current_ls = find_ls(current_d, side.lambda, dmax, s(side, current_d));

		if(counter++ >= iter_limit) {
			is_limit_reached = true;
			break;
		}
	}

	return { current_d, is_limit_reached };

/*
	size_t i;
	for(i = 0; i < iter_limit && !check_ls_dmax_lmin_lambda(current_ls, current_d, side.lambda, dmax, side.lmin); ++i) {

		current_d -= current_d / step;
		current_ls = find_ls(current_d, side.lambda, dmax, s(side, current_d));
	}

	return { current_d, i >= iter_limit };
*/
}

//******************************************************************************
//tuple<double, bool> Interval::adjust_d_for_s(Interval::Side const& side, size_t iter_limit) const {
//	size_t step = 10000;
//	double current_d = side.meshline_policy->d;
//
//	size_t nlines = side.ls.size();
//
//	size_t counter = 0;
//	bool is_limit_reached = false;
//	for(;;) {
//		double next_d = current_d - current_d / step;
//		auto next_ls = find_ls(next_d, side.lambda, dmax, s(side, next_d));
//		if(next_ls.size() > nlines)
//			break;
//
//		current_d = next_d;
//
//		if(counter++ >= iter_limit) {
//			is_limit_reached = true;
//			break;
//		}
//	}
//
///*
//	size_t i;
//	for(i = 0; i < iter_limit; ++i) {
//		double next_d = current_d - current_d / step;
//		auto next_ls = find_ls(next_d, a.lambda, dmax, s(a, next_d));
//		if(next_ls.size() > nlines)
//			break;
//
//		current_d = next_d;
//	}
//
//	if(i >= iter_limit)
//		is_limit_reached = true;
//*/
//
//	return { current_d, is_limit_reached };
//}

// TODO make the previous line go a step further the m line: ls.size()--
//******************************************************************************
tuple<double, bool> Interval::adjust_lambda_for_s(Interval::Side const& side, size_t iter_limit) const {
	size_t step = 10000;
	double current_lambda = side.lambda;

	size_t nlines = side.ls.size();
	vector<Coord> current_ls = side.ls;

	size_t counter = 0;
	bool is_limit_reached = false;
/*
	while(current_lambda > 1 && current_ls.size() <= nlines) {
		current_lambda -= current_lambda / step;
		if(current_lambda < 1)
			current_lambda = 1;

		current_ls = find_ls(side.meshline_policy->d, current_lambda, dmax, s(side));

		++counter;
		if(counter >= iter_limit) {
			is_limit_reached = true;
			break;
		}
	}
*/
	for(;;) {
		double next_lambda = current_lambda - current_lambda / step;
		if(next_lambda < 1)
			next_lambda = 1;

		auto next_ls = find_ls(side.meshline_policy->d, next_lambda, dmax, s(side));
		if(next_ls.size() > nlines)
			break;

		current_lambda = next_lambda;

		if(current_lambda == 1)
			break;

		if(counter++ >= iter_limit) {
			is_limit_reached = true;
			break;
		}
	}

/*
	size_t i;
	for(i = 0; i < iter_limit && (current_lambda > 1 && current_ls.size() <= nlines); ++i) {
		current_lambda -= current_lambda / step;
		if(current_lambda < 1)
			current_lambda = 1;

		current_ls = find_ls(a.meshline_policy->d, current_lambda, dmax, s(a));
	}

	if(i >= iter_limit)
		is_limit_reached = true;
	return { current_lambda, i >= iter_limit };
*/

	return { current_lambda, is_limit_reached };
}

//******************************************************************************
void Interval::auto_solve_d() {
	update_ls();

	dmax = ::find_dmax(before, after, dmax);
	update_ls();

	before.meshline_policy->d = get<0>(adjust_d_for_dmax_lmin(before/*, 10000*/));
	update_ls(before);


	after.meshline_policy->d = get<0>(adjust_d_for_dmax_lmin(after/*, 10000*/));
	update_ls(after);
}

//******************************************************************************
void Interval::auto_solve_lambda() {
	update_ls();

	before.lambda = get<0>(adjust_lambda_for_s(before));
	after.lambda = get<0>(adjust_lambda_for_s(after));
	update_ls();
}

//******************************************************************************
vector<unique_ptr<Meshline>> Interval::mesh() const {
	double const d_init_before = before.d_init();
	double const d_init_after = after.d_init();
	vector<unique_ptr<Meshline>> meshlines;

	if(before.meshline_policy->policy != MeshlinePolicy::Policy::ONELINE)
		meshlines.push_back(make_unique<Meshline>(
			before.meshline_policy->coord + d_init_before,
			this,
			before.meshline_policy));
	if(before.ls.size())
		for(auto it = begin(before.ls); it != prev(end(before.ls)); ++it)
			meshlines.push_back(make_unique<Meshline>(
				before.meshline_policy->coord + d_init_before + (*it),
				this,
				before.meshline_policy));

	meshlines.push_back(make_unique<Meshline>(m, this));

	if(after.ls.size())
		for(auto it = next(rbegin(after.ls)); it != rend(after.ls); ++it)
			meshlines.push_back(make_unique<Meshline>(
				after.meshline_policy->coord - d_init_after - (*it),
				this,
				after.meshline_policy));
	if(after.meshline_policy->policy != MeshlinePolicy::Policy::ONELINE)
		meshlines.push_back(make_unique<Meshline>(
			after.meshline_policy->coord - d_init_after,
			this,
			after.meshline_policy));

	meshlines.shrink_to_fit();
	return meshlines;
}
