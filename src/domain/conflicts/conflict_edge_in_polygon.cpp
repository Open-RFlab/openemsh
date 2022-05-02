///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

#include <algorithm>

#include "utils/signum.hpp"
#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/polygon.hpp"

#include "conflict_edge_in_polygon.hpp"

using namespace std;

//******************************************************************************
void sort_overlaps_by_p0_by_vector_orientation(vector<Overlap>& overlaps, Point const& vector) {
	int8_t x_sign = signum(vector.x);
	int8_t y_sign = signum(vector.y);
	if(x_sign > 0 && y_sign == 0)
		sort(begin(overlaps), end(overlaps),
			[](Overlap const& a, Overlap const& b) {
				return (get<RANGE>(a).p0().x < get<RANGE>(b).p0().x);
			});
	else if(x_sign == 0 && y_sign > 0)
		sort(begin(overlaps), end(overlaps),
			[](Overlap const& a, Overlap const& b) {
				return (get<RANGE>(a).p0().y < get<RANGE>(b).p0().y);
			});
	else if(x_sign < 0 && y_sign == 0)
		sort(begin(overlaps), end(overlaps),
			[](Overlap const& a, Overlap const& b) {
				return (get<RANGE>(a).p0().x > get<RANGE>(b).p0().x);
			});
	else if(x_sign == 0 && y_sign < 0)
		sort(begin(overlaps), end(overlaps),
			[](Overlap const& a, Overlap const& b) {
				return (get<RANGE>(a).p0().y > get<RANGE>(b).p0().y);
			});
	else if(x_sign > 0 && y_sign > 0)
		sort(begin(overlaps), end(overlaps),
			[](Overlap const& a, Overlap const& b) {
				return (get<RANGE>(a).p0().x < get<RANGE>(b).p0().x
					&& get<RANGE>(a).p0().y < get<RANGE>(b).p0().y);
			});
	else if(x_sign < 0 && y_sign > 0)
		sort(begin(overlaps), end(overlaps),
			[](Overlap const& a, Overlap const& b) {
				return (get<RANGE>(a).p0().x > get<RANGE>(b).p0().x
					&& get<RANGE>(a).p0().y < get<RANGE>(b).p0().y);
			});
	else if(x_sign < 0 && y_sign < 0)
		sort(begin(overlaps), end(overlaps),
			[](Overlap const& a, Overlap const& b) {
				return (get<RANGE>(a).p0().x > get<RANGE>(b).p0().x
					&& get<RANGE>(a).p0().y > get<RANGE>(b).p0().y);
			});
	else if(x_sign > 0 && y_sign < 0)
		sort(begin(overlaps), end(overlaps),
			[](Overlap const& a, Overlap const& b) {
				return (get<RANGE>(a).p0().x < get<RANGE>(b).p0().x
					&& get<RANGE>(a).p0().y > get<RANGE>(b).p0().y);
			});
}

//******************************************************************************
ConflictEdgeInPolygon::ConflictEdgeInPolygon(Edge* a, Polygon const* polygon, Range const range, optional<Edge const*> b)
: Conflict(Kind::EDGE_IN_POLYGON)
, edge(a) {
	overlaps.emplace_back(polygon, range, b);
}

//******************************************************************************
void ConflictEdgeInPolygon::append(Polygon const* polygon, Range const range, optional<Edge const*> edge) {
	overlaps.emplace_back(polygon, range, edge);
}

//******************************************************************************
void ConflictEdgeInPolygon::auto_solve(MeshlinePolicyManager& /*line_policy_manager*/) {
	sort_overlaps_by_p0_by_vector_orientation(overlaps, edge->vec);
	Range merged_range(get<RANGE>(overlaps.front()));

	for(Overlap const& overlap : overlaps) {
		if(!get<EDGE>(overlap).has_value()
		|| get<EDGE>(overlap).value()->normal != edge->normal) {
			if(optional<Range> r = merge(get<RANGE>(overlap), merged_range))
				merged_range = r.value();
			else
				break;
		} else {
			break;
		}
	}

	edge->to_mesh = !(merged_range == *edge);
	solution = edge;
	is_solved = true;
}

#ifdef DEBUG
//******************************************************************************
void ConflictEdgeInPolygon::print() const {
	cout << "ConflictEdgeInPolygon :" << endl;
	if(edge) {
		cout << "edge : " << edge << endl;
		edge->print();
	} else {
		cout << "edge : nullptr" << endl;
	}
	cout << "overlaps : " << overlaps.size() << endl;
	for(auto const& overlap : overlaps) {
		cout << "overlap :" << endl;
		cout << "polygon : " << get<POLYGON>(overlap) << endl;
//		get<POLYGON>(overlap)->print();
//		cout << "range :" << endl;
		get<RANGE>(overlap).print();
		if(get<EDGE>(overlap).has_value()) {
			cout << "optedge : " << get<EDGE>(overlap).value() << endl;
			get<EDGE>(overlap).value()->print();
		} else {
			cout << "optedge : nullopt" << endl;
		}
	}
}
#endif // DEBUG
