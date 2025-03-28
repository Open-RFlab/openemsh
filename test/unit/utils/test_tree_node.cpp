///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

//#include <memory>
//#include <iostream>
#include <algorithm>

#include "utils/vector_utils.hpp"

#include "utils/tree_node.hpp"

/// @test TreeNode& TreeNode::add_child()
/// @test bool operator==(TreeNode const& b) const
/// @test TreeNode* TreeNode::parent() const
/// @test TreeNode* TreeNode::root()
/// @test std::vector<TreeNode*> TreeNode::ancestors(bool include_itself)
/// @test std::vector<TreeNode*> TreeNode::leafs(bool include_itself)
/// @test std::vector<TreeNode*> TreeNode::cluster(bool include_itself)
/// @test bool TreeNode::is_descendant_of(TreeNode const& node) const
/// @test bool TreeNode::is_ancestor_of(TreeNode const& node) const
/// @test void TreeNode::erase_from_descendants(std::set<TreeNode*> const& nodes)
/// @test TreeNode* TreeNode::common_ancestor(TreeNode& node, bool include_themselves)
/// @test TreeNode* common_ancestor(TreeNode& a, TreeNode& b, bool include_themselves) @todo
/// @test bool TreeNode::is_root() const
/// @test bool TreeNode::is_leaf() const
///*****************************************************************************

using namespace std;

//******************************************************************************
SCENARIO("TreeNode& TreeNode::add_child()", "[utils][tree_node]") {
	GIVEN("A TreeNode") {
		TreeNode a;

		WHEN("Adding a child") {
			TreeNode& b = a.add_child();

			THEN("Another TreeNode must be added in children and has parent correctly set") {
				REQUIRE(a.children.size() == 1);
				REQUIRE(&b == &a.children.back());
				REQUIRE(&a == b.parent());
			}
		}
	}
}

//******************************************************************************
SCENARIO("bool operator==(TreeNode const& b) const", "[utils][tree_node]") {
	GIVEN("A TreeNode") {
		TreeNode a;

		THEN("Should be equal to itself") {
			REQUIRE(a == a);
		}

		AND_GIVEN("A second TreeNode") {
			TreeNode b;

			THEN("Both should not be equal") {
				REQUIRE_FALSE(a == b);
				REQUIRE_FALSE(b == a);
			}
		}
	}
}

//******************************************************************************
SCENARIO("TreeNode* TreeNode::parent() const", "[utils][tree_node]") {
	GIVEN("A standalone TreeNode") {
		TreeNode a;

		THEN("Should return nullptr") {
			REQUIRE(a.parent() == nullptr);
		}

		AND_GIVEN("A child TreeNode") {
			TreeNode& b = a.add_child();

			THEN("Should return its parent address") {
				REQUIRE(&a == b.parent());
			}
		}
	}
}

//******************************************************************************
SCENARIO("TreeNode* TreeNode::root()", "[utils][tree_node]") {
	GIVEN("A standalone TreeNode") {
		TreeNode a;

		THEN("Should return itself") {
			REQUIRE(a.root() == &a);
		}

		AND_GIVEN("A child TreeNode") {
			TreeNode& b = a.add_child();

			THEN("Should return its parent") {
				REQUIRE(b.root() == b.parent());
				REQUIRE(b.root() == &a);
			}

			AND_GIVEN("Further descendants TreeNodes") {
				TreeNode& c = b.add_child();
				TreeNode& d = c.add_child();

				THEN("Should return the standalone TreeNode") {
					REQUIRE(c.root() == &a);
					REQUIRE(d.root() == &a);
					REQUIRE_FALSE(c.root() == c.parent());
					REQUIRE_FALSE(d.root() == d.parent());
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("std::vector<TreeNode*> TreeNode::ancestors(bool include_itself)", "[utils][tree_node]") {
	GIVEN("A multilevel tree of TreeNodes") {
		// + a
		//   + b
		//     + c1
		//     | + d1
		//     | + d2 <---
		//     | | + e1
		//     | | + e2
		//     | + d3
		//     + c2

		[[maybe_unused]] TreeNode a;
		[[maybe_unused]] TreeNode& b = a.add_child();
		[[maybe_unused]] TreeNode& c1 = b.add_child();
		[[maybe_unused]] TreeNode& c2 = b.add_child();
		[[maybe_unused]] TreeNode& d1 = c1.add_child();
		[[maybe_unused]] TreeNode& d2 = c1.add_child();
		[[maybe_unused]] TreeNode& d3 = c1.add_child();
		[[maybe_unused]] TreeNode& e1 = d2.add_child();
		[[maybe_unused]] TreeNode& e2 = d2.add_child();

		WHEN("Retrieving ancestors of a TreeNode in the middle of the tree") {
			auto x = d2.ancestors();

			THEN("Should return all and only the TreeNodes between itself and the root (including the root), in ascending order") {
				REQUIRE(x.size() == 3);
				REQUIRE(x[0] == &c1);
				REQUIRE(x[1] == &b);
				REQUIRE(x[2] == &a);
			}

			AND_WHEN("Specifying include_itself") {
				auto y = d2.ancestors(true);

				THEN("Should return itself along other ancestors") {
					REQUIRE(y.size() == 4);
					REQUIRE(y[0] == &d2);
					REQUIRE(y[1] == &c1);
					REQUIRE(y[2] == &b);
					REQUIRE(y[3] == &a);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("std::vector<TreeNode*> TreeNode::leafs(bool include_itself)", "[utils][tree_node]") {
	GIVEN("A multilevel tree of TreeNodes") {
		// + a
		//   + b
		//     + c1 <---
		//     | + d1
		//     | + d2
		//     | | + e1
		//     | | + e2
		//     | + d3
		//     + c2 <---

		[[maybe_unused]] TreeNode a;
		[[maybe_unused]] TreeNode& b = a.add_child();
		[[maybe_unused]] TreeNode& c1 = b.add_child();
		[[maybe_unused]] TreeNode& c2 = b.add_child();
		[[maybe_unused]] TreeNode& d1 = c1.add_child();
		[[maybe_unused]] TreeNode& d2 = c1.add_child();
		[[maybe_unused]] TreeNode& d3 = c1.add_child();
		[[maybe_unused]] TreeNode& e1 = d2.add_child();
		[[maybe_unused]] TreeNode& e2 = d2.add_child();

		WHEN("Retrieving leafs of a TreeNode in the middle of the tree") {
			auto x = c1.leafs();

			THEN("Should return all and only the TreeNodes at leaf positions below itself") {
				REQUIRE(x.size() == 4);
				REQUIRE(contains(x, &d1));
				REQUIRE(contains(x, &d3));
				REQUIRE(contains(x, &e1));
				REQUIRE(contains(x, &e2));
			}

			AND_WHEN("Specifying include_itself") {
				auto y = c1.leafs(true);

				THEN("Should return the same TreeNodes") {
					REQUIRE(y.size() == 4);
					REQUIRE(contains(y, &d1));
					REQUIRE(contains(y, &d3));
					REQUIRE(contains(y, &e1));
					REQUIRE(contains(y, &e2));
				}
			}
		}

		WHEN("Retrieving leafs of a TreeNode at a leaf position") {
			auto x = c2.leafs();

			THEN("Should return nothing") {
				REQUIRE(x.empty());
			}

			AND_WHEN("Specifying include_itself") {
				auto y = c2.leafs(true);

				THEN("Should return itself only") {
					REQUIRE(y.size() == 1);
					REQUIRE(contains(y, &c2));
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("std::vector<TreeNode*> TreeNode::cluster(bool include_itself)", "[utils][tree_node]") {
	GIVEN("A multilevel tree of TreeNodes") {
		// + a
		//   + b
		//     + c1 <---
		//     | + d1
		//     | + d2
		//     | | + e1
		//     | | + e2
		//     | + d3
		//     + c2

		[[maybe_unused]] TreeNode a;
		[[maybe_unused]] TreeNode& b = a.add_child();
		[[maybe_unused]] TreeNode& c1 = b.add_child();
		[[maybe_unused]] TreeNode& c2 = b.add_child();
		[[maybe_unused]] TreeNode& d1 = c1.add_child();
		[[maybe_unused]] TreeNode& d2 = c1.add_child();
		[[maybe_unused]] TreeNode& d3 = c1.add_child();
		[[maybe_unused]] TreeNode& e1 = d2.add_child();
		[[maybe_unused]] TreeNode& e2 = d2.add_child();

		WHEN("Retrieving cluster of a TreeNode in the middle of the tree") {
			auto x = c1.cluster();

			THEN("Should return all and only the TreeNodes below itself") {
				REQUIRE(x.size() == 5);
				REQUIRE(contains(x, &d1));
				REQUIRE(contains(x, &d2));
				REQUIRE(contains(x, &d3));
				REQUIRE(contains(x, &e1));
				REQUIRE(contains(x, &e2));
			}

			AND_WHEN("Specifying include_itself") {
				auto y = c1.cluster(true);

				THEN("Should return itself along other TreeNodes") {
					REQUIRE(y.size() == 6);
					REQUIRE(contains(y, &c1));
					REQUIRE(contains(y, &d1));
					REQUIRE(contains(y, &d2));
					REQUIRE(contains(y, &d3));
					REQUIRE(contains(y, &e1));
					REQUIRE(contains(y, &e2));
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("bool TreeNode::is_descendant_of(TreeNode const& node) const", "[utils][tree_node]") {
	GIVEN("A multilevel tree of TreeNodes") {
		// + a
		//   + b
		//     + c1 <---
		//     | + d1
		//     | + d2
		//     | | + e1
		//     | | + e2
		//     | + d3
		//     + c2

		[[maybe_unused]] TreeNode a;
		[[maybe_unused]] TreeNode& b = a.add_child();
		[[maybe_unused]] TreeNode& c1 = b.add_child();
		[[maybe_unused]] TreeNode& c2 = b.add_child();
		[[maybe_unused]] TreeNode& d1 = c1.add_child();
		[[maybe_unused]] TreeNode& d2 = c1.add_child();
		[[maybe_unused]] TreeNode& d3 = c1.add_child();
		[[maybe_unused]] TreeNode& e1 = d2.add_child();
		[[maybe_unused]] TreeNode& e2 = d2.add_child();

		AND_GIVEN("A TreeNode in the middle of the tree") {
			WHEN("Checking against itself") {
				THEN("Should return false") {
					REQUIRE_FALSE(c1.is_descendant_of(c1));
				}
			}

			WHEN("Checking against TreeNodes that are above itself") {
				THEN("Should return true") {
					REQUIRE(c1.is_descendant_of(b));
					REQUIRE(c1.is_descendant_of(a));
				}
			}

			WHEN("Checking against TreeNodes that are below itself") {
				THEN("Should return false") {
					REQUIRE_FALSE(c1.is_descendant_of(d1));
					REQUIRE_FALSE(c1.is_descendant_of(d2));
					REQUIRE_FALSE(c1.is_descendant_of(d3));
					REQUIRE_FALSE(c1.is_descendant_of(e1));
					REQUIRE_FALSE(c1.is_descendant_of(e2));
				}
			}

			WHEN("Checking against TreeNodes that are at the same level than itself") {
				THEN("Should return false") {
					REQUIRE_FALSE(c1.is_descendant_of(c2));
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("bool TreeNode::is_ancestor_of(TreeNode const& node) const", "[utils][tree_node]") {
	GIVEN("A multilevel tree of TreeNodes") {
		// + a
		//   + b
		//     + c1 <---
		//     | + d1
		//     | + d2
		//     | | + e1
		//     | | + e2
		//     | + d3
		//     + c2

		[[maybe_unused]] TreeNode a;
		[[maybe_unused]] TreeNode& b = a.add_child();
		[[maybe_unused]] TreeNode& c1 = b.add_child();
		[[maybe_unused]] TreeNode& c2 = b.add_child();
		[[maybe_unused]] TreeNode& d1 = c1.add_child();
		[[maybe_unused]] TreeNode& d2 = c1.add_child();
		[[maybe_unused]] TreeNode& d3 = c1.add_child();
		[[maybe_unused]] TreeNode& e1 = d2.add_child();
		[[maybe_unused]] TreeNode& e2 = d2.add_child();

		AND_GIVEN("A TreeNode in the middle of the tree") {
			WHEN("Checking against itself") {
				THEN("Should return false") {
					REQUIRE_FALSE(c1.is_ancestor_of(c1));
				}
			}

			WHEN("Checking against TreeNodes that are above itself") {
				THEN("Should return false") {
					REQUIRE_FALSE(c1.is_ancestor_of(b));
					REQUIRE_FALSE(c1.is_ancestor_of(a));
				}
			}

			WHEN("Checking against TreeNodes that are below itself") {
				THEN("Should return true") {
					REQUIRE(c1.is_ancestor_of(d1));
					REQUIRE(c1.is_ancestor_of(d2));
					REQUIRE(c1.is_ancestor_of(d3));
					REQUIRE(c1.is_ancestor_of(e1));
					REQUIRE(c1.is_ancestor_of(e2));
				}
			}

			WHEN("Checking against TreeNodes that are at the same level than itself") {
				THEN("Should return false") {
					REQUIRE_FALSE(c1.is_ancestor_of(c2));
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("void TreeNode::erase_from_descendants(std::set<TreeNode*> const& nodes)", "[utils][tree_node]") {
	GIVEN("A multilevel tree of TreeNodes") {
		// + a
		//   + b
		//     + c1 <---
		//     | + d1
		//     | + d2
		//     | | + e1
		//     | | + e2
		//     | + d3
		//     + c2

		[[maybe_unused]] TreeNode a;
		[[maybe_unused]] TreeNode& b = a.add_child();
		[[maybe_unused]] TreeNode& c1 = b.add_child();
		[[maybe_unused]] TreeNode& c2 = b.add_child();
		[[maybe_unused]] TreeNode& d1 = c1.add_child();
		[[maybe_unused]] TreeNode& d2 = c1.add_child();
		[[maybe_unused]] TreeNode& d3 = c1.add_child();
		[[maybe_unused]] TreeNode& e1 = d2.add_child();
		[[maybe_unused]] TreeNode& e2 = d2.add_child();

		[[maybe_unused]] TreeNode x;
		[[maybe_unused]] TreeNode& y = x.add_child();

		auto const contains = [](std::list<TreeNode> const& list, TreeNode const& node) {
			return std::ranges::find(list, node) != std::end(list);
		};

		AND_GIVEN("A TreeNode in the middle of the tree") {
			WHEN("Trying to erase itself") {
				THEN("Should fail") {
					c1.erase_from_descendants({ &c1 });
					REQUIRE(contains(b.children, c1));
				}
			}

			WHEN("Trying to erase a TreeNode that is above itself") {
				THEN("Should fail") {
					c1.erase_from_descendants({ &b });
					REQUIRE(contains(a.children, b));
				}
			}

			WHEN("Trying to erase a TreeNode that is below itself") {
				THEN("Should success") {
					c1.erase_from_descendants({ &d1 });
					c1.erase_from_descendants({ &e1 });
					REQUIRE_FALSE(contains(c1.children, d1));
					REQUIRE_FALSE(contains(d2.children, e1));
				}
			}

			WHEN("Trying to erase a TreeNode that is somewhere else in the tree") {
				THEN("Should fail") {
					c1.erase_from_descendants({ &c2 });
					REQUIRE(contains(b.children, c2));
				}
			}

			WHEN("Trying to erase a TreeNode that is not part of the tree") {
				THEN("Should fail") {
					c1.erase_from_descendants({ &y });
					REQUIRE(contains(x.children, y));
				}
			}

			WHEN("Trying to erase all the cases above at once") {
				THEN("Should erase the erasable TreeNodes") {
					c1.erase_from_descendants({ &c1, &b, &d1, &e1, &c2, &y });
					REQUIRE(contains(b.children, c1));
					REQUIRE(contains(a.children, b));
					REQUIRE_FALSE(contains(c1.children, d1));
					REQUIRE_FALSE(contains(d2.children, e1));
					REQUIRE(contains(b.children, c2));
					REQUIRE(contains(x.children, y));
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("TreeNode* TreeNode::common_ancestor(TreeNode& node, bool include_themselves)", "[utils][tree_node]") {
	GIVEN("A multilevel tree of TreeNodes") {
		// + a
		//   + b
		//     + c1 <---
		//     | + d1
		//     | + d2 <---
		//     | | + e1
		//     | | + e2
		//     | + d3
		//     + c2
		//       + f1
		//       + f2
		//         + g1 <--
		//         + g2

		[[maybe_unused]] TreeNode a;
		[[maybe_unused]] TreeNode& b = a.add_child();
		[[maybe_unused]] TreeNode& c1 = b.add_child();
		[[maybe_unused]] TreeNode& c2 = b.add_child();
		[[maybe_unused]] TreeNode& d1 = c1.add_child();
		[[maybe_unused]] TreeNode& d2 = c1.add_child();
		[[maybe_unused]] TreeNode& d3 = c1.add_child();
		[[maybe_unused]] TreeNode& e1 = d2.add_child();
		[[maybe_unused]] TreeNode& e2 = d2.add_child();
		[[maybe_unused]] TreeNode& f1 = c2.add_child();
		[[maybe_unused]] TreeNode& f2 = c2.add_child();
		[[maybe_unused]] TreeNode& g1 = f2.add_child();
		[[maybe_unused]] TreeNode& g2 = f2.add_child();

		AND_GIVEN("A TreeNode in the middle of the tree") {
			WHEN("Retrieving common ancestor with itself") {
				THEN("Should return itself") {
					REQUIRE(d2.common_ancestor(d2) == &d2);
				}

				AND_WHEN("Specifying not to include_themselves") {
					THEN("Should return its parent (if any)") {
						REQUIRE(d2.common_ancestor(d2, false) == &c1);
						REQUIRE(a.common_ancestor(a, false) == nullptr);
					}
				}
			}

			WHEN("Retrieving common ancestor with a TreeNode that is above itself") {
				THEN("Should return such a TreeNode") {
					REQUIRE(d2.common_ancestor(c1) == &c1);
					REQUIRE(d2.common_ancestor(b) == &b);
					REQUIRE(d2.common_ancestor(a) == &a);
				}

				AND_WHEN("Specifying not to include_themselves") {
					THEN("Should return the parent (if any) of such a TreeNode") {
						REQUIRE(d2.common_ancestor(c1, false) == &b);
						REQUIRE(d2.common_ancestor(b, false) == &a);
						REQUIRE(d2.common_ancestor(a, false) == nullptr);
					}
				}
			}

			WHEN("Retrieving common ancestor with a TreeNode that is below itself") {
				THEN("Should return itself") {
					REQUIRE(c1.common_ancestor(d1) == &c1);
					REQUIRE(c1.common_ancestor(d2) == &c1);
					REQUIRE(c1.common_ancestor(d3) == &c1);
					REQUIRE(c1.common_ancestor(e1) == &c1);
					REQUIRE(c1.common_ancestor(e2) == &c1);
				}

				AND_WHEN("Specifying not to include_themselves") {
					THEN("Should return its own parent (if any)") {
						REQUIRE(c1.common_ancestor(d1, false) == &b);
						REQUIRE(c1.common_ancestor(d2, false) == &b);
						REQUIRE(c1.common_ancestor(d3, false) == &b);
						REQUIRE(c1.common_ancestor(e1, false) == &b);
						REQUIRE(c1.common_ancestor(e2, false) == &b);
						REQUIRE(a.common_ancestor(b, false) == nullptr);
					}
				}
			}

			WHEN("Retrieving common ancestor with a TreeNode somewhere else in the tree") {
				THEN("Should return their common ancestor") {
					REQUIRE(d2.common_ancestor(g1) == &b);
				}

				AND_WHEN("Specifying not to include_themselves") {
					THEN("Should return their common ancestor") {
						REQUIRE(d2.common_ancestor(g1, false) == &b);
					}
				}
			}

			WHEN("Retrieving common ancestor with a TreeNode that is not part of the tree") {
				THEN("Should return nullptr") {
					TreeNode x;
					REQUIRE(d2.common_ancestor(x) == nullptr);
					REQUIRE(d2.common_ancestor(x, false) == nullptr);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("TreeNode* common_ancestor(TreeNode& a, TreeNode& b, bool include_themselves)", "[utils][tree_node]") {
	GIVEN("A multilevel tree of TreeNodes") {
		// + a
		//   + b
		//     + c1 <---
		//     | + d1
		//     | + d2 <---
		//     | | + e1
		//     | | + e2
		//     | + d3
		//     + c2
		//       + f1
		//       + f2
		//         + g1 <--
		//         + g2

		[[maybe_unused]] TreeNode a;
		[[maybe_unused]] TreeNode& b = a.add_child();
		[[maybe_unused]] TreeNode& c1 = b.add_child();
		[[maybe_unused]] TreeNode& c2 = b.add_child();
		[[maybe_unused]] TreeNode& d1 = c1.add_child();
		[[maybe_unused]] TreeNode& d2 = c1.add_child();
		[[maybe_unused]] TreeNode& d3 = c1.add_child();
		[[maybe_unused]] TreeNode& e1 = d2.add_child();
		[[maybe_unused]] TreeNode& e2 = d2.add_child();
		[[maybe_unused]] TreeNode& f1 = c2.add_child();
		[[maybe_unused]] TreeNode& f2 = c2.add_child();
		[[maybe_unused]] TreeNode& g1 = f2.add_child();
		[[maybe_unused]] TreeNode& g2 = f2.add_child();

		AND_GIVEN("A TreeNode in the middle of the tree") {
			WHEN("Retrieving common ancestor with itself") {
				THEN("Should return itself") {
					REQUIRE(common_ancestor(d2, d2) == &d2);
				}

				AND_WHEN("Specifying not to include_themselves") {
					THEN("Should return its parent (if any)") {
						REQUIRE(common_ancestor(d2, d2, false) == &c1);
						REQUIRE(common_ancestor(a, a, false) == nullptr);
					}
				}
			}

			WHEN("Retrieving common ancestor with a TreeNode that is above itself") {
				THEN("Should return such a TreeNode") {
					REQUIRE(common_ancestor(d2, c1) == &c1);
					REQUIRE(common_ancestor(d2, b) == &b);
					REQUIRE(common_ancestor(d2, a) == &a);

					REQUIRE(common_ancestor(c1, d2) == &c1);
					REQUIRE(common_ancestor(b, d2) == &b);
					REQUIRE(common_ancestor(a, d2) == &a);
				}

				AND_WHEN("Specifying not to include_themselves") {
					THEN("Should return the parent (if any) of such a TreeNode") {
						REQUIRE(common_ancestor(d2, c1, false) == &b);
						REQUIRE(common_ancestor(d2, b, false) == &a);
						REQUIRE(common_ancestor(d2, a, false) == nullptr);

						REQUIRE(common_ancestor(c1, d2, false) == &b);
						REQUIRE(common_ancestor(b, d2, false) == &a);
						REQUIRE(common_ancestor(a, d2, false) == nullptr);
					}
				}
			}

			WHEN("Retrieving common ancestor with a TreeNode that is below itself") {
				THEN("Should return itself") {
					REQUIRE(common_ancestor(c1, d1) == &c1);
					REQUIRE(common_ancestor(c1, d2) == &c1);
					REQUIRE(common_ancestor(c1, d3) == &c1);
					REQUIRE(common_ancestor(c1, e1) == &c1);
					REQUIRE(common_ancestor(c1, e2) == &c1);

					REQUIRE(common_ancestor(d1, c1) == &c1);
					REQUIRE(common_ancestor(d2, c1) == &c1);
					REQUIRE(common_ancestor(d3, c1) == &c1);
					REQUIRE(common_ancestor(e1, c1) == &c1);
					REQUIRE(common_ancestor(e2, c1) == &c1);
				}

				AND_WHEN("Specifying not to include_themselves") {
					THEN("Should return its own parent (if any)") {
						REQUIRE(common_ancestor(c1, d1, false) == &b);
						REQUIRE(common_ancestor(c1, d2, false) == &b);
						REQUIRE(common_ancestor(c1, d3, false) == &b);
						REQUIRE(common_ancestor(c1, e1, false) == &b);
						REQUIRE(common_ancestor(c1, e2, false) == &b);
						REQUIRE(common_ancestor(a, b, false) == nullptr);

						REQUIRE(common_ancestor(d1, c1, false) == &b);
						REQUIRE(common_ancestor(d2, c1, false) == &b);
						REQUIRE(common_ancestor(d3, c1, false) == &b);
						REQUIRE(common_ancestor(e1, c1, false) == &b);
						REQUIRE(common_ancestor(e2, c1, false) == &b);
						REQUIRE(common_ancestor(b, a, false) == nullptr);
					}
				}
			}

			WHEN("Retrieving common ancestor with a TreeNode somewhere else in the tree") {
				THEN("Should return their common ancestor") {
					REQUIRE(common_ancestor(d2, g1) == &b);
					REQUIRE(common_ancestor(g1, d2) == &b);

				}

				AND_WHEN("Specifying not to include_themselves") {
					THEN("Should return their common ancestor") {
						REQUIRE(common_ancestor(d2, g1, false) == &b);
						REQUIRE(common_ancestor(g1, d2, false) == &b);
					}
				}
			}

			WHEN("Retrieving common ancestor with a TreeNode that is not part of the tree") {
				THEN("Should return nullptr") {
					TreeNode x;
					REQUIRE(common_ancestor(d2, x) == nullptr);
					REQUIRE(common_ancestor(d2, x, false) == nullptr);

					REQUIRE(common_ancestor(x, d2) == nullptr);
					REQUIRE(common_ancestor(x, d2, false) == nullptr);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("bool TreeNode::is_root() const", "[utils][tree_node]") {
	GIVEN("A TreeNode, its child and its child") {
		TreeNode a;
		TreeNode& b = a.add_child();
		TreeNode& c = b.add_child();

		WHEN("Checking against the parent") {
			THEN("Should return true") {
				REQUIRE(a.is_root());
			}
		}

		WHEN("Checking against the middle child") {
			THEN("Should return false") {
				REQUIRE_FALSE(b.is_root());
			}
		}

		WHEN("Checking against the last child") {
			THEN("Should return false") {
				REQUIRE_FALSE(c.is_root());
			}
		}
	}
}

//******************************************************************************
SCENARIO("bool TreeNode::is_leaf() const", "[utils][tree_node]") {
	GIVEN("A TreeNode, its child and its child") {
		TreeNode a;
		TreeNode& b = a.add_child();
		TreeNode& c = b.add_child();

		WHEN("Checking against the parent") {
			THEN("Should return false") {
				REQUIRE_FALSE(a.is_leaf());
			}
		}

		WHEN("Checking against the middle child") {
			THEN("Should return false") {
				REQUIRE_FALSE(b.is_leaf());
			}
		}

		WHEN("Checking against the last child") {
			THEN("Should return true") {
				REQUIRE(c.is_leaf());
			}
		}
	}
}
