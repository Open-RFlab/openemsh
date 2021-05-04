///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/point.hpp"

#include "domain/range.hpp"

/// @test Bounding bounding(Segment const& a)
/// @test std::optional<Range> merge(Segment const* a, Segment const* b)
///*****************************************************************************

SCENARIO("Bounding bounding(Segment const& a)", "[range][segment]") {
	GIVEN("An horizontal range") {
		Point a0(1, 1), a1(2, 1);
		Range a(a0, a1), b(a1, a0);
		Bounding c = bounding(a);
		Bounding d = bounding(b);
		THEN("Should calcul the bounding box") {
			REQUIRE(a.axis == Segment::Axis::X);
			REQUIRE(c[XMIN] == 1);
			REQUIRE(c[XMAX] == 2);
			REQUIRE(c[YMIN] == 1);
			REQUIRE(c[YMAX] == 1);
			REQUIRE(b.axis == Segment::Axis::X);
			REQUIRE(d[XMIN] == 1);
			REQUIRE(d[XMAX] == 2);
			REQUIRE(d[YMIN] == 1);
			REQUIRE(d[YMAX] == 1);
		}
	}

	GIVEN("A vertical range") {
		Point a0(1, 1), a1(1, 2);
		Range a(a0, a1), b(a1, a0);
		Bounding c = bounding(a);
		Bounding d = bounding(b);
		THEN("Should calcul the bounding box") {
			REQUIRE(a.axis == Segment::Axis::Y);
			REQUIRE(c[XMIN] == 1);
			REQUIRE(c[XMAX] == 1);
			REQUIRE(c[YMIN] == 1);
			REQUIRE(c[YMAX] == 2);
			REQUIRE(b.axis == Segment::Axis::Y);
			REQUIRE(d[XMIN] == 1);
			REQUIRE(d[XMAX] == 1);
			REQUIRE(d[YMIN] == 1);
			REQUIRE(d[YMAX] == 2);
		}
	}

	GIVEN("A diagonal range") {
		Point a0(1, 1), a1(2, 2);
		Range a(a0, a1), b(a1, a0);
		Bounding c = bounding(a);
		Bounding d = bounding(b);
		THEN("Should calcul the bounding box") {
			REQUIRE(a.axis == Segment::Axis::DIAGONAL);
			REQUIRE(c[XMIN] == 1);
			REQUIRE(c[XMAX] == 2);
			REQUIRE(c[YMIN] == 1);
			REQUIRE(c[YMAX] == 2);
			REQUIRE(b.axis == Segment::Axis::DIAGONAL);
			REQUIRE(d[XMIN] == 1);
			REQUIRE(d[XMAX] == 2);
			REQUIRE(d[YMIN] == 1);
			REQUIRE(d[YMAX] == 2);
		}
	}
}

//******************************************************************************
SCENARIO("std::optional<Range> merge(Segment const* a, Segment const* b)", "[range][segment]") {
	GIVEN("Two ranges") {
		WHEN("Two horizontal ranges are overlapping") {
			Point a0(1, 1), a1(3, 1);
			Point b0(2, 1), b1(4, 1);
			Range a(a0, a1), b(b0, b1);
			std::optional<Range> r(merge(a, b));
			std::optional<Range> s(merge(b, a));
			THEN("Should calcul merged range") {
				REQUIRE(a.relation_to(b) == relation::SegmentSegment::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Segment::Axis::X);
				REQUIRE(r->p0().x == 1);
				REQUIRE(r->p1().x == 4);
				REQUIRE(r->p0().y == 1);
				REQUIRE(r->p1().y == 1);
				REQUIRE(s);
				REQUIRE(s->axis == Segment::Axis::X);
				REQUIRE(s->p0().x == 1);
				REQUIRE(s->p1().x == 4);
				REQUIRE(s->p0().y == 1);
				REQUIRE(s->p1().y == 1);
			}
		}

		WHEN("An horizontal range is totally inside another horizontal range") {
			Point a0(1, 1), a1(4, 1);
			Point b0(3, 1), b1(2, 1);
			Range a(a0, a1), b(b0, b1);
			std::optional<Range> r(merge(a, b));
			std::optional<Range> s(merge(b, a));
			THEN("Should calcul merged range") {
				REQUIRE(a.relation_to(b) == relation::SegmentSegment::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Segment::Axis::X);
				REQUIRE(r->p0().x == 1);
				REQUIRE(r->p1().x == 4);
				REQUIRE(r->p0().y == 1);
				REQUIRE(r->p1().y == 1);
				REQUIRE(s);
				REQUIRE(s->axis == Segment::Axis::X);
				REQUIRE(s->p0().x == 1);
				REQUIRE(s->p1().x == 4);
				REQUIRE(s->p0().y == 1);
				REQUIRE(s->p1().y == 1);
			}
		}

		WHEN("Two horizontal ranges are colinear and touching by just an extremity") {
			Point a0(1, 1), a1(2, 1);
			Point b0(2, 1), b1(3, 1);
			Range a(a0, a1), b(b0, b1);
			std::optional<Range> r(merge(a, b));
			std::optional<Range> s(merge(a, b));
			THEN("Should calcul merged range") {
				REQUIRE(a.relation_to(b) == relation::SegmentSegment::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Segment::Axis::X);
				REQUIRE(r->p0().x == 1);
				REQUIRE(r->p1().x == 3);
				REQUIRE(r->p0().y == 1);
				REQUIRE(r->p1().y == 1);
				REQUIRE(s);
				REQUIRE(s->axis == Segment::Axis::X);
				REQUIRE(s->p0().x == 1);
				REQUIRE(s->p1().x == 3);
				REQUIRE(s->p0().y == 1);
				REQUIRE(s->p1().y == 1);
			}
		}

		WHEN("Two horizontal ranges are colinear") {
			Point a0(1, 1), a1(2, 1);
			Point b0(3, 1), b1(4, 1);
			Range a(a0, a1), b(b0, b1);
			std::optional<Range> r(merge(a, b));
			std::optional<Range> s(merge(b, a));
			THEN("There should not be any merged range") {
				REQUIRE(a.relation_to(b) == relation::SegmentSegment::COLINEAR);
				REQUIRE_FALSE(r);
				REQUIRE_FALSE(s);
			}
		}

		WHEN("Two horizontal ranges are parallels") {
			Point a0(1, 1), a1(2, 1);
			Point b0(3, 2), b1(4, 2);
			Range a(a0, a1), b(b0, b1);
			std::optional<Range> r(merge(a, b));
			std::optional<Range> s(merge(b, a));
			THEN("There should not be any merged range") {
				REQUIRE(a.relation_to(b) == relation::SegmentSegment::APART);
				REQUIRE_FALSE(r);
				REQUIRE_FALSE(s);
			}
		}

		WHEN("Two vertical ranges are overlapping") {
			Point a0(1, 1), a1(1, 3);
			Point b0(1, 2), b1(1, 4);
			Range a(a0, a1), b(b0, b1);
			std::optional<Range> r(merge(a, b));
			std::optional<Range> s(merge(b, a));
			THEN("Should calcul merged range") {
				REQUIRE(a.relation_to(b) == relation::SegmentSegment::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Segment::Axis::Y);
				REQUIRE(r->p0().y == 1);
				REQUIRE(r->p1().y == 4);
				REQUIRE(r->p0().x == 1);
				REQUIRE(r->p1().x == 1);
				REQUIRE(s);
				REQUIRE(s->axis == Segment::Axis::Y);
				REQUIRE(s->p0().y == 1);
				REQUIRE(s->p1().y == 4);
				REQUIRE(s->p0().x == 1);
				REQUIRE(s->p1().x == 1);
			}
		}

		WHEN("A vertical range is totally inside another vertical range") {
			Point a0(1, 1), a1(1, 4);
			Point b0(1, 2), b1(1, 3);
			Range a(a0, a1), b(b0, b1);
			std::optional<Range> r(merge(a, b));
			std::optional<Range> s(merge(b, a));
			THEN("Should calcul merged range") {
				REQUIRE(a.relation_to(b) == relation::SegmentSegment::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Segment::Axis::Y);
				REQUIRE(r->p0().y == 1);
				REQUIRE(r->p1().y == 4);
				REQUIRE(r->p0().x == 1);
				REQUIRE(r->p1().x == 1);
				REQUIRE(s);
				REQUIRE(s->axis == Segment::Axis::Y);
				REQUIRE(s->p0().y == 1);
				REQUIRE(s->p1().y == 4);
				REQUIRE(s->p0().x == 1);
				REQUIRE(s->p1().x == 1);
			}
		}

		WHEN("Two vertical ranges are colinear and touching by just an extremity") {
			Point a0(1, 1), a1(1, 2);
			Point b0(1, 2), b1(1, 3);
			Range a(a0, a1), b(b0, b1);
			std::optional<Range> r(merge(a, b));
			std::optional<Range> s(merge(a, b));
			THEN("Should calcul overlap range") {
				REQUIRE(a.relation_to(b) == relation::SegmentSegment::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Segment::Axis::Y);
				REQUIRE(r->p0().y == 1);
				REQUIRE(r->p1().y == 3);
				REQUIRE(r->p0().x == 1);
				REQUIRE(r->p1().x == 1);
				REQUIRE(s);
				REQUIRE(s->axis == Segment::Axis::Y);
				REQUIRE(s->p0().y == 1);
				REQUIRE(s->p1().y == 3);
				REQUIRE(s->p0().x == 1);
				REQUIRE(s->p1().x == 1);
			}
		}

		WHEN("Two vertical ranges are colinear") {
			Point a0(1, 1), a1(1, 2);
			Point b0(1, 3), b1(1, 4);
			Range a(a0, a1), b(b0, b1);
			std::optional<Range> r(merge(a, b));
			std::optional<Range> s(merge(b, a));
			THEN("There should not be any merged range") {
				REQUIRE(a.relation_to(b) == relation::SegmentSegment::COLINEAR);
				REQUIRE_FALSE(r);
				REQUIRE_FALSE(s);
			}
		}

		WHEN("Two vertical ranges are parallels") {
			Point a0(1, 1), a1(1, 2);
			Point b0(2, 3), b1(2, 4);
			Range a(a0, a1), b(b0, b1);
			std::optional<Range> r(merge(a, b));
			std::optional<Range> s(merge(b, a));
			THEN("There should not be any merged range") {
				REQUIRE(a.relation_to(b) == relation::SegmentSegment::APART);
				REQUIRE_FALSE(r);
				REQUIRE_FALSE(s);
			}
		}

		WHEN("Two diagonal ranges are overlapping") {
			Point a0(1, 1), a1(3, 5);
			Point b0(2, 3), b1(4, 7);
			Range a(a0, a1), b(b0, b1), c(b1, b0);
			std::optional<Range> r(merge(a, b));
			std::optional<Range> s(merge(b, a));
			std::optional<Range> t(merge(a, c));
			THEN("Should calcul merged range") {
				REQUIRE(a.relation_to(b) == relation::SegmentSegment::OVERLAPPING);
				REQUIRE(a.relation_to(c) == relation::SegmentSegment::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Segment::Axis::DIAGONAL);
				REQUIRE(r->p0().x == 1);
				REQUIRE(r->p1().x == 4);
				REQUIRE(r->p0().y == 1);
				REQUIRE(r->p1().y == 7);
				REQUIRE(s);
				REQUIRE(s->axis == Segment::Axis::DIAGONAL);
				REQUIRE(r->p0().x == 1);
				REQUIRE(r->p1().x == 4);
				REQUIRE(r->p0().y == 1);
				REQUIRE(r->p1().y == 7);
				REQUIRE(t);
				REQUIRE(t->axis == Segment::Axis::DIAGONAL);
				REQUIRE(r->p0().x == 1);
				REQUIRE(r->p1().x == 4);
				REQUIRE(r->p0().y == 1);
				REQUIRE(r->p1().y == 7);
			}
		}

		WHEN("Two diagonal ranges are colinear and touching by just an extremity") {
			Point a0(1, 1), a1(2, 2);
			Point b0(2, 2), b1(3, 3);
			Range a(a0, a1), b(b0, b1);
			std::optional<Range> r(merge(a, b));
			std::optional<Range> s(merge(a, b));
			THEN("Should calcul overlap range") {
				REQUIRE(a.relation_to(b) == relation::SegmentSegment::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Segment::Axis::DIAGONAL);
				REQUIRE(r->p0().x == 1);
				REQUIRE(r->p1().x == 3);
				REQUIRE(r->p0().y == 1);
				REQUIRE(r->p1().y == 3);
				REQUIRE(s);
				REQUIRE(s->axis == Segment::Axis::DIAGONAL);
				REQUIRE(s->p0().x == 1);
				REQUIRE(s->p1().x == 3);
				REQUIRE(s->p0().y == 1);
				REQUIRE(s->p1().y == 3);
			}
		}

		WHEN("Two ranges are crossing") {
			Point a0(1, 1), a1(3, 3);
			Point b0(1, 3), b1(3, 1);
			Range a(a0, a1), b(b0, b1);
			std::optional<Range> r(merge(a, b));
			std::optional<Range> s(merge(b, a));
			THEN("There should not be any merged range") {
				REQUIRE(a.relation_to(b) == relation::SegmentSegment::CROSSING);
				REQUIRE_FALSE(r);
				REQUIRE_FALSE(s);
			}
		}

		WHEN("Two ranges are apart") {
			Point a0(1, 1), a1(1, 2);
			Point b0(2, 2), b1(4, 4);
			Range a(a0, a1), b(b0, b1);
			std::optional<Range> r(merge(a, b));
			std::optional<Range> s(merge(b, a));
			THEN("There should not be any merged range") {
				REQUIRE(a.relation_to(b) == relation::SegmentSegment::APART);
				REQUIRE_FALSE(r);
				REQUIRE_FALSE(s);
			}
		}
	}
}
