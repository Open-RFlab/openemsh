///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

#include "edge.hpp"
#include "polygon.hpp"

#include "conflict_edge_in_polygon.hpp"

using namespace std;

//******************************************************************************
ConflictEdgeInPolygon::ConflictEdgeInPolygon(Edge const* a, Polygon const* _polygon, Range const _range, optional<Edge const*> b)
: Conflict(Kind::EDGE_IN_POLYGON)
, edge(a) {
	overlaps.emplace_back(_polygon, make_unique<Range const>(_range), b);
}

//******************************************************************************
void ConflictEdgeInPolygon::append(Polygon const* polygon, Range const range, optional<Edge const*> edge) {
	overlaps.emplace_back(polygon, make_unique<Range const>(range), edge);
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
		get<RANGE>(overlap)->print();
		if(get<EDGE>(overlap).has_value()) {
			cout << "optedge : " << get<EDGE>(overlap).value() << endl;
			get<EDGE>(overlap).value()->print();
		} else {
			cout << "optedge : nullopt" << endl;
		}
	}
}
#endif // DEBUG
