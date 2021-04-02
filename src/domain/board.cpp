///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

#include <utility>

#include "conflict_edge_in_polygon.hpp"

#include "board.hpp"

using namespace std;

//******************************************************************************
Board::Board(vector<unique_ptr<Polygon>>& _polygons)
: conflict_manager(conflicts)
, polygons(move(_polygons)) {
	for(unique_ptr<Polygon>& polygon : polygons)
		for(unique_ptr<Edge>& edge : polygon->edges)
			edges.push_back(edge.get());
	polygons.shrink_to_fit();
	edges.shrink_to_fit();
}

/// Detect all EDGE_IN_POLYGON. Will also detect some COLINEAR_EDGES.
/// Overlapping edges should be EDGE_IN_POLYGON and not COLINEAR_EDGES.
///*****************************************************************************
void Board::detect_edges_in_polygons() {
	for(unique_ptr<Polygon>& poly_a : polygons) {
		for(unique_ptr<Polygon>& poly_b : polygons) {
			if(poly_b == poly_a)
				continue;

			bool need_p0_check = true;
			relation::PolygonPoint p0_pos;
			relation::PolygonPoint current_pos;
			for(unique_ptr<Edge>& edge_a : poly_a->edges) {
				if(need_p0_check)
					p0_pos = poly_b->relation_to(poly_a->points.front().get());
				need_p0_check = false;

				unsigned int edge_a_intersections = 0;
				for(unique_ptr<Edge>& edge_b : poly_b->edges) {
					relation::EdgeEdge rel = edge_a->relation_to(edge_b.get());
					switch(rel) {
					case relation::EdgeEdge::APART: break;
					case relation::EdgeEdge::CROSSING:
						++edge_a_intersections;
						toggle(current_pos);
						break;
					case relation::EdgeEdge::COLINEAR: break;
					case relation::EdgeEdge::OVERLAPPING:
//						++edge_a_intersections; // TODO is it an intersection? no, useless : ON invalidate toggle
						current_pos = relation::PolygonPoint::ON;
						break;
					}
				}

				// TODO from current_pos & edge_a_intersections & p0_pos & p1_pos
				// find if edge_a <-> poly_b relation
				switch(current_pos) {
				case relation::PolygonPoint::ON: {
					break;
				} case relation::PolygonPoint::IN: {
					if(!edge_a_intersections) {
						// edge_a is totally inside poly_b
						switch(edge_a->axis) {
						case Edge::Axis::X:
						case Edge::Axis::Y:
						case Edge::Axis::DIAGONAL: break;
						}
						conflict_manager.add_edge_in_polygon(edge_a.get(), poly_b.get());
					}
					break;
				} case relation::PolygonPoint::OUT: {
					break;
				}
				}
			}
		}
	}
/*
	for(size_t i = 0; i < polygons.size() ; ++i) {
		for(size_t j = i + 1; j < polygons.size(); ++j) {
			if(!are_possibly_overlapping(*polygons[i], *polygons[j]))
				continue;
//				for(unique_ptr<Edge>& edge_i : polygons[i]->edges) {
//					for(unique_ptr<Edge>& edge_j : polygons[j]->edges) {
//						relation::EdgeEdge e = edge_i->relation_to(edge_j.get());

			Polygon* poly_a = polygons[i].get();
			Polygon* poly_b = polygons[j].get();

			relation::PolygonEdge side = cast(poly_b->relation_to(poly_a->points.front().get())); // TODO check if first point is in polygon

			for(size_t k = 0; k < poly_a->edges.size(); ++k) {
				Edge* edge_a = poly_a->edges[k].get();

				for(size_t l = k + 1; l < poly_b->edges.size(); ++l) { // TODO k+1 -> broken for?

					Edge* edge_b = poly_b->edges[l].get();

					relation::EdgeEdge e = edge_a->relation_to(edge_b); // TODO e better name
//					edge_a->print();
//					edge_b->print();
					switch(e) {
					case relation::EdgeEdge::APART: {
						break;
					} case relation::EdgeEdge::CROSSING: {
						if(optional<Point> p = intersection(edge_a, edge_b)) {
							relation::PolygonPoint r = poly_a->relation_to(&p.value());
							switch(r) {
							case relation::PolygonPoint::IN:
							case relation::PolygonPoint::ON:
							case relation::PolygonPoint::OUT:
							}
							p->print();
						}

						break;
					} case relation::EdgeEdge::COLINEAR: {
						conflict_manager.add_colinear_edges(edge_a, edge_b);

						break;
					} case relation::EdgeEdge::OVERLAPPING: {
						if(optional<Range> r = overlap(edge_a, edge_b))
							r->print();

						break;
					}
					}
				}
			}
//						cout << "i: " << i << "\tj: " << j << "\t\tAPART" << endl;
//						cout << "i: " << i << "\tj: " << j << "\t\tCROSSING" << endl;
//						cout << "i: " << i << "\tj: " << j << "\t\tCOLINEAR" << endl;
//						cout << "i: " << i << "\tj: " << j << "\t\tOVERLAPPING" << endl;
		}
	}
*/
}

//******************************************************************************
void Board::detect_colinear_edges() {
	for(size_t i = 0; i < edges.size(); ++i) {
		if(edges[i]->axis == Edge::Axis::DIAGONAL)
			continue;

		for(unsigned long j = i + 1; j < edges.size(); ++j) {
			if(edges[j]->axis != edges[i]->axis)
				continue;

			switch(edges[i]->axis) {
			case Edge::Axis::X:
				if(edges[i]->p0->y == edges[j]->p0->y)
					conflict_manager.add_colinear_edges(edges[i], edges[j]);
				break;
			case Edge::Axis::Y:
				if(edges[i]->p0->x == edges[j]->p0->x)
					conflict_manager.add_colinear_edges(edges[i], edges[j]);
				break;
			default:
				break;
			}
		}
	}
}

//******************************************************************************
void Board::print() {
	for(unique_ptr<Polygon>& polygon : polygons)
		polygon->print();
}
