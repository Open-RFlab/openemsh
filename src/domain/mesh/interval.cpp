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

namespace domain {

using namespace std;

//******************************************************************************
Interval::Side::Side(MeshlinePolicy* meshline_policy, size_t lmin, double lambda, Coord h, function<double (double)> d_init)
: meshline_policy(meshline_policy)
, lmin(lmin)
, lambda(lambda)
, d_init_(std::move(d_init))
{
	if(meshline_policy->get_current_state().d > h) {
		auto state = meshline_policy->get_current_state();
		state.d = (double) h;
		meshline_policy->set_next_state(state);
	}
}

//******************************************************************************
double Interval::Side::d_init() const {
	return d_init_(meshline_policy->get_current_state().d);
}

//******************************************************************************
Coord calc_h(Coord const& a, Coord const& b) noexcept {
	return distance(a, b) / 2;
}

//******************************************************************************
Interval::Interval(MeshlinePolicy* before, MeshlinePolicy* after, Axis axis, GlobalParams* global_params, Timepoint* t)
: Originator(t, {
	.dmax = global_params->get_current_state().dmax,
	.before = Side(before, global_params->get_current_state().lmin, global_params->get_current_state().lambda, calc_h(before->coord, after->coord), [before](double d) noexcept {
		switch(before->get_current_state().policy) {
		case MeshlinePolicy::Policy::ONELINE: return 0.0;
		case MeshlinePolicy::Policy::HALFS: return d / 2.0;
		case MeshlinePolicy::Policy::THIRDS: return [&] {
			switch(before->get_current_state().normal) {
			case MeshlinePolicy::Normal::MAX:
				return 2.0/3.0 * d;
			case MeshlinePolicy::Normal::MIN:
				return 1.0/3.0 * d;
			default:
				return d;
			}
		} ();
		default: ::unreachable();
		}
	}),
	.after = Side(after, global_params->get_current_state().lmin, global_params->get_current_state().lambda, calc_h(before->coord, after->coord), [after](double d) noexcept {
		switch(after->get_current_state().policy) {
		case MeshlinePolicy::Policy::ONELINE: return 0.0;
		case MeshlinePolicy::Policy::HALFS: return d / 2.0;
		case MeshlinePolicy::Policy::THIRDS: return [&] {
			switch(after->get_current_state().normal) {
			case MeshlinePolicy::Normal::MAX:
				return 1.0/3.0 * d;
			case MeshlinePolicy::Normal::MIN:
				return 2.0/3.0 * d;
			default:
				return d;
			}
		} ();
		default: ::unreachable();
		}
	})
})
, global_params(global_params)
, axis(axis)
, h(calc_h(before->coord, after->coord))
, m(mid(before->coord, after->coord))
{}

//******************************************************************************
Coord Interval::s(Interval::Side const& side) const {
	return h - side.d_init_(side.meshline_policy->get_current_state().d);
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
bool is_ls_valid_for_dmax_lmin_lambda(vector<Coord> const& ls, double d, double lambda, double dmax, size_t lmin) {
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
void Interval::update_ls(IntervalState& state) {
	update_ls(state.before);
	update_ls(state.after);
}

//******************************************************************************
void Interval::update_ls(Interval::Side& side) {
	side.ls = find_ls(side.meshline_policy->get_current_state().d, side.lambda, get_current_state().dmax, s(side));
}

//******************************************************************************
tuple<double, bool> Interval::adjust_d_for_dmax_lmin(Interval::Side const& side, size_t iter_limit) const {
	size_t const step = 1000;
	double current_d = min(side.meshline_policy->get_current_state().d, get_current_state().dmax);

	vector<Coord> current_ls = side.ls;

	size_t counter = 0;
	bool is_limit_reached = false;
	while(!is_ls_valid_for_dmax_lmin_lambda(current_ls, current_d, side.lambda, get_current_state().dmax, side.lmin)) {
		current_d -= current_d / step;
		current_ls = find_ls(current_d, side.lambda, get_current_state().dmax, s(side, current_d));

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
//	double current_d = side.meshline_policy->get_current_state().d;
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

		current_ls = find_ls(side.meshline_policy->get_current_state().d, current_lambda, dmax, s(side));

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

		auto next_ls = find_ls(side.meshline_policy->get_current_state().d, next_lambda, get_current_state().dmax, s(side));
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

		current_ls = find_ls(a.meshline_policy->get_current_state().d, current_lambda, dmax, s(a));
	}

	if(i >= iter_limit)
		is_limit_reached = true;
	return { current_lambda, i >= iter_limit };
*/

	return { current_lambda, is_limit_reached };
}

//******************************************************************************
void Interval::auto_solve_d() {
	auto [t, state] = make_next_state();

	update_ls(state);

	state.dmax = domain::find_dmax(state.before, state.after, state.dmax);
	update_ls(state);

	auto state_b = state.before.meshline_policy->get_current_state();
	state_b.d = get<0>(adjust_d_for_dmax_lmin(state.before/*, 10000*/));
	state.before.meshline_policy->set_state(t, state_b);
	update_ls(state.before);


	auto state_a = state.after.meshline_policy->get_current_state();
	state_a.d = get<0>(adjust_d_for_dmax_lmin(state.after/*, 10000*/));
	state.after.meshline_policy->set_state(t, state_a);
	update_ls(state.after);

	set_state(t, state);
}

//******************************************************************************
void Interval::auto_solve_lambda() {
	auto state = get_current_state();

	update_ls(state);

	state.before.lambda = get<0>(adjust_lambda_for_s(state.before));
	state.after.lambda = get<0>(adjust_lambda_for_s(state.after));
	update_ls(state);

	set_next_state(state);
}

//******************************************************************************
vector<shared_ptr<Meshline>> Interval::mesh() const {
	auto const& state = get_current_state();

	double const d_init_before = state.before.d_init();
	double const d_init_after = state.after.d_init();
	vector<shared_ptr<Meshline>> meshlines;

	if(state.before.meshline_policy->get_current_state().policy != MeshlinePolicy::Policy::ONELINE)
		meshlines.push_back(make_shared<Meshline>(
			state.before.meshline_policy->coord + d_init_before,
			this,
			state.before.meshline_policy));
	if(state.before.ls.size())
		for(auto it = begin(state.before.ls); it != prev(end(state.before.ls)); ++it)
			meshlines.push_back(make_shared<Meshline>(
				state.before.meshline_policy->coord + d_init_before + (*it),
				this,
				state.before.meshline_policy));

	meshlines.push_back(make_shared<Meshline>(m, this));

	if(state.after.ls.size())
		for(auto it = next(rbegin(state.after.ls)); it != rend(state.after.ls); ++it)
			meshlines.push_back(make_shared<Meshline>(
				state.after.meshline_policy->coord - d_init_after - (*it),
				this,
				state.after.meshline_policy));
	if(state.after.meshline_policy->get_current_state().policy != MeshlinePolicy::Policy::ONELINE)
		meshlines.push_back(make_shared<Meshline>(
			state.after.meshline_policy->coord - d_init_after,
			this,
			state.after.meshline_policy));

	meshlines.shrink_to_fit();
	return meshlines;
}

} // namespace domain
