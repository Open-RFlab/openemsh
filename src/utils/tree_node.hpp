///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <list>
//#include <string>
#include <cstddef>
//#include <iostream>
//#include <algorithm>
#include <vector>
#include <set>

#ifdef UNITTEST
#define private public
#endif // UNITTEST

//******************************************************************************
class TreeNode {
public:
	std::size_t const id;

private:
	TreeNode* _parent;
	std::list<TreeNode> children;

public:
	explicit TreeNode(TreeNode* parent = nullptr);

	TreeNode& add_child();

	bool operator==(TreeNode const& b) const;

	TreeNode* parent() const;
	TreeNode* root();

	//TODO up_iterator
	std::vector<TreeNode*> ancestors(bool include_itself = false);
	std::vector<TreeNode*> leafs(bool include_itself = false);
	std::vector<TreeNode*> cluster(bool include_itself = false);

	void erase_from_descendants(std::set<TreeNode*> const& nodes);

	TreeNode* common_ancestor(TreeNode& node, bool include_themselves = true);

//	TreeNode* find_in_offspring(std::size_t id);
	bool is_descendant_of(TreeNode const& node) const;
	bool is_ancestor_of(TreeNode const& node) const;

	bool is_root() const;
	bool is_leaf() const;
};

#ifdef UNITTEST
#undef private
#endif // UNITTEST

//******************************************************************************
TreeNode* common_ancestor(TreeNode& a, TreeNode& b, bool include_themselves = true);
