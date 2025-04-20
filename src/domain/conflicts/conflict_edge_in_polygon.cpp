///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "domain/geometrics/segment.hpp"
#include "domain/geometrics/space.hpp"

#include <algorithm>

#include "utils/signum.hpp"
#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/polygon.hpp"

#include "conflict_edge_in_polygon.hpp"

namespace domain {

using namespace std;

//******************************************************************************
void sort_overlaps_by_p0_by_vector_orientation(vector<Overlap>& overlaps, Point const& vector) {
	int8_t x_sign = signum(vector.x);
	int8_t y_sign = signum(vector.y);
	if(x_sign > 0 && y_sign == 0)
		ranges::sort(overlaps, [](Overlap const& a, Overlap const& b) {
			return (get<RANGE>(a).p0().x < get<RANGE>(b).p0().x);
		});
	else if(x_sign == 0 && y_sign > 0)
		ranges::sort(overlaps, [](Overlap const& a, Overlap const& b) {
			return (get<RANGE>(a).p0().y < get<RANGE>(b).p0().y);
		});
	else if(x_sign < 0 && y_sign == 0)
		ranges::sort(overlaps, [](Overlap const& a, Overlap const& b) {
			return (get<RANGE>(a).p0().x > get<RANGE>(b).p0().x);
		});
	else if(x_sign == 0 && y_sign < 0)
		ranges::sort(overlaps, [](Overlap const& a, Overlap const& b) {
			return (get<RANGE>(a).p0().y > get<RANGE>(b).p0().y);
		});
	else if(x_sign > 0 && y_sign > 0)
		ranges::sort(overlaps, [](Overlap const& a, Overlap const& b) {
			return (get<RANGE>(a).p0().x < get<RANGE>(b).p0().x
				&& get<RANGE>(a).p0().y < get<RANGE>(b).p0().y);
		});
	else if(x_sign < 0 && y_sign > 0)
		ranges::sort(overlaps, [](Overlap const& a, Overlap const& b) {
			return (get<RANGE>(a).p0().x > get<RANGE>(b).p0().x
				&& get<RANGE>(a).p0().y < get<RANGE>(b).p0().y);
		});
	else if(x_sign < 0 && y_sign < 0)
		ranges::sort(overlaps, [](Overlap const& a, Overlap const& b) {
			return (get<RANGE>(a).p0().x > get<RANGE>(b).p0().x
				&& get<RANGE>(a).p0().y > get<RANGE>(b).p0().y);
		});
	else if(x_sign > 0 && y_sign < 0)
		ranges::sort(overlaps, [](Overlap const& a, Overlap const& b) {
			return (get<RANGE>(a).p0().x < get<RANGE>(b).p0().x
				&& get<RANGE>(a).p0().y > get<RANGE>(b).p0().y);
		});
}

//******************************************************************************
ConflictEdgeInPolygon::ConflictEdgeInPolygon(Plane plane, Edge* a, Polygon const* polygon, Range const range, optional<Edge const*> b, Timepoint* t)
: Originator(t, { .overlaps = {{ polygon, range, b }} })
, Conflict(Kind::EDGE_IN_POLYGON)
, plane(plane)
, edge(a)
{}

//******************************************************************************
void ConflictEdgeInPolygon::append(Polygon const* polygon, Range const range, optional<Edge const*> edge, Timepoint* t) {
	auto state = get_current_state();
	state.overlaps.emplace_back(polygon, range, edge);
	set_given_or_next_state(state, t);
}

//******************************************************************************
void ConflictEdgeInPolygon::auto_solve(MeshlinePolicyManager& /*line_policy_manager*/) {
	auto [t, state] = make_next_state();

	sort_overlaps_by_p0_by_vector_orientation(state.overlaps, edge->vec);
	Range merged_range(get<RANGE>(state.overlaps.front()));

	for(Overlap const& overlap : state.overlaps) {
		if(!get<EDGE>(overlap).has_value()
		|| get<EDGE>(overlap).value()->normal != edge->normal) {
			if(optional<Range> r = merge(get<RANGE>(overlap), merged_range); r)
				merged_range = r.value();
			else
				break;
		} else {
			break;
		}
	}

	auto state_e = edge->get_current_state();
	state_e.to_mesh = !(merged_range == *edge);
	state_e.conflict = this;
	state.solution = edge;
	state.is_solved = true;

	edge->set_state(t, state_e);
	set_state(t, state);
}

} // namespace domain
