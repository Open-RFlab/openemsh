///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <memory>
#include <vector>
#include <iostream>

#include "domain/board.hpp"
#include "domain/polygon.hpp"

using namespace std;

//******************************************************************************
int main() {

//	Polygon p({{ 1, 2 }, { 3, 4 }, { 5, 6 }});
//	p.print();

//	Polygon* q;
//	q = new Polygon({{ 1, 2 }, { 3, 4 }, { 5, 6 }});
//	q->print();
//	delete q;

	Board* b;
	{
		vector<unique_ptr<Polygon>> tmp;
		tmp.push_back(make_unique<Polygon>(Polygon({{ 1, 2 }, { 3, 4 }, { 4, 2 }}))); //CW
		tmp.push_back(make_unique<Polygon>(Polygon({{ 1, 2 }, { 4, 2 }, { 3, 4 }}))); //CCW
		tmp.push_back(make_unique<Polygon>(Polygon({{ 1, 2 }, { 3, 4 }, { 5, 6 }}))); //LINE
		tmp.push_back(make_unique<Polygon>(Polygon({{ 2, 1 }, { 3, 4 }, { 5, 6 }}))); //CROSSING
		b = new Board(tmp);
	}

/*
	Board* b = new Board(vector<unique_ptr<Polygon>>({
		make_unique<Polygon>(Polygon({{ 1, 2 }, { 3, 4 }, { 4, 2 }})),
		make_unique<Polygon>(Polygon({{ 1, 2 }, { 4, 2 }, { 3, 4 }})),
		make_unique<Polygon>(Polygon({{ 1, 2 }, { 3, 4 }, { 5, 6 }})),
		make_unique<Polygon>(Polygon({{ 2, 1 }, { 3, 4 }, { 5, 6 }})) }));
*/

//	b->print();
	b->detect_colinear_edges();
	b->detect_edges_in_polygons();


	delete b;

	return 0;
}
