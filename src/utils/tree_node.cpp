///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <list>
#include <string>
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <vector>

#include "id_generator.hpp"

#include "tree_node.hpp"

using namespace std;

//******************************************************************************
static IdGenerator id_generator;

//******************************************************************************
TreeNode::TreeNode(TreeNode* parent)
: id(id_generator())
, _parent(parent)
{}

//******************************************************************************
TreeNode& TreeNode::add_child() {
	return children.emplace_back(this);
}

//******************************************************************************
bool TreeNode::operator==(TreeNode const& b) const {
	return this == &b;
//	return id == b.id;
}

//******************************************************************************
TreeNode* TreeNode::parent() const {
	return _parent;
}

//******************************************************************************
TreeNode* TreeNode::root() {
	auto* it = this;
	while(it->parent())
		it = it->parent();
	return it;
}

//******************************************************************************
vector<TreeNode*> TreeNode::ancestors(bool include_itself) {
	auto* it = this;
	vector<TreeNode*> parents;
	if(include_itself)
		parents.push_back(it);
	while(it->parent()) {
		it = it->parent();
		parents.push_back(it);
	}
	return parents;
}

// TODO recursive approach, can be improved if needed
//******************************************************************************
vector<TreeNode*> TreeNode::leafs(bool include_itself) {
//	auto* it = this;
	vector<TreeNode*> leafs;

	if(children.empty()) {
		if(include_itself)
			leafs.push_back(this);
	} else {
		for(auto& child : children)
			ranges::move(child.leafs(true), back_inserter(leafs));
	}

	return leafs;
}

//******************************************************************************
void TreeNode::erase_from_descendants(set<TreeNode*> const& nodes) {
	erase_if(children, [&](auto& node) {
		return nodes.contains(&node);
	});

	for(auto& child : children)
		child.erase_from_descendants(nodes);
}

// TODO recursive approach, can be improved if needed
//******************************************************************************
vector<TreeNode*> TreeNode::cluster(bool include_itself) {
	vector<TreeNode*> nodes;

	if(include_itself)
		nodes.push_back(this);

	for(auto& child : children)
		ranges::move(child.cluster(true), back_inserter(nodes));

	return nodes;
}

//******************************************************************************
TreeNode* TreeNode::common_ancestor(TreeNode& node, bool include_themselves) {
	for(auto* it_a : ancestors(include_themselves))
		for(auto* it_b : node.ancestors(include_themselves))
			if(it_a == it_b)
				return it_a;
	return nullptr;
}

//******************************************************************************
TreeNode* common_ancestor(TreeNode& a, TreeNode& b, bool include_themselves) {
//	return a.common_ancestor(b, include_themselves);
	for(auto* it_a : a.ancestors(include_themselves))
		for(auto* it_b : b.ancestors(include_themselves))
			if(it_a == it_b)
				return it_a;
	return nullptr;
}

//******************************************************************************
bool TreeNode::is_descendant_of(TreeNode const& node) const {
	auto* it = this;
	while(it->parent()) {
		it = it->parent();
		if(it == &node)
			return true;
	}
	return false;
}

//******************************************************************************
bool TreeNode::is_ancestor_of(TreeNode const& node) const {
	return node.is_descendant_of(*this);
}

//******************************************************************************
bool TreeNode::is_root() const {
	return _parent == nullptr;
}

//******************************************************************************
bool TreeNode::is_leaf() const {
	return children.empty();
}
