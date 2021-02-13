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

//	Polygon p(Polygon::Rotation::UNKNOWN, {{ 1, 2 }, { 3, 4 }, { 5, 6 }});
//	p.print();

//	Polygon* q;
//	q = new Polygon(Polygon::Rotation::UNKNOWN, {{ 1, 2 }, { 3, 4 }, { 5, 6 }});
//	q->print();
//	delete q;

	Board* b;
	{
		vector<unique_ptr<Polygon>> tmp;
		tmp.push_back(make_unique<Polygon>(Polygon(Polygon::Rotation::UNKNOWN, {{ 1, 2 }, { 3, 4 }, { 4, 2 }}))); //CW
		tmp.push_back(make_unique<Polygon>(Polygon(Polygon::Rotation::UNKNOWN, {{ 1, 2 }, { 4, 2 }, { 3, 4 }}))); //CCW
		tmp.push_back(make_unique<Polygon>(Polygon(Polygon::Rotation::UNKNOWN, {{ 1, 2 }, { 3, 4 }, { 5, 6 }}))); //LINE
		b = new Board(tmp);
	}


	b->print();
	b->detect_halfs_rule_edges();


	delete b;

	return 0;
}
