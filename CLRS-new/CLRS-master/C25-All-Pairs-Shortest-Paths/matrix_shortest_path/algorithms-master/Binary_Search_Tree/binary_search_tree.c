#include <stdio.h>
#include <stdlib.h>

#include "binary_search_tree.h"

struct sTreeNode TreeNodeNIL = {
	.content = NULL,
	.parent = &TreeNodeNIL,
	.leftchild = &TreeNodeNIL,
	.rightchild = &TreeNodeNIL
};

struct sTreeNode *tree_lookup(const struct sTree *tree, void *key)
{
	struct sTreeNode *tmp;

	tmp = tree->root;
	while (tmp != &TreeNodeNIL) {
		if (tree->compare(tree->key(tmp), key) == 0)
			return tmp;
		else if (tree->compare(tree->key(tmp), key) > 0)
			tmp = tmp->leftchild;
		else
			tmp = tmp->rightchild;
	}
	return tmp;
}

struct sTreeNode *tree_minimum(const struct sTreeNode *treenode)
{
	struct sTreeNode *tmp;

	tmp = treenode;
	while (tmp != &TreeNodeNIL) {
		if (tmp->leftchild != &TreeNodeNIL)
			tmp = tmp->leftchild;
		else
			break;
	}
	
	return tmp;
}

struct sTreeNode *tree_maximum(const struct sTreeNode *treenode)
{
	struct sTreeNode *tmp;

	tmp = treenode;
	while (tmp != &TreeNodeNIL) {
		if (tmp->rightchild != &TreeNodeNIL)
			tmp = tmp->rightchild;
		else
			break;
	}

	return tmp;
}

struct sTreeNode *tree_predecessor(const struct sTree *tree, const struct sTreeNode *node)
{
	struct sTreeNode *tmp, *parent, *root;

	tmp = node;
	root = tree->root;
	parent = tmp->parent;
	if (tmp->leftchild != &TreeNodeNIL)
		return tree_maximum(tmp->leftchild);

	while (tmp != root && tmp == parent->leftchild) {
		tmp = parent;
		parent = tmp->parent;
	}

	return parent;
}

struct sTreeNode *tree_successor(const struct sTree *tree, const struct sTreeNode *node)
{
	struct sTreeNode *tmp, *parent, *root;

	tmp = node;
	root = tree->root;
	parent = node->parent;
	if (tmp->rightchild != &TreeNodeNIL)
		return tree_minimum(tmp->rightchild);

	while (tmp != root && tmp == parent->rightchild) {
		tmp = parent;
		parent = tmp->parent;
	}

	return parent;
}

void tree_insert(struct sTree *tree, struct sTreeNode *newnode)
{
	struct sTreeNode *tmp, *parent;

	tmp = tree->root;
	parent = tmp->parent;
	while (tmp != &TreeNodeNIL) {
		parent = tmp;
		if (tree->compare(tree->key(tmp), tree->key(newnode)) >= 0)
			tmp = tmp->leftchild;
		else
			tmp = tmp->rightchild;
	}
	if (parent == &TreeNodeNIL) {
		tree->root = newnode;
	} else {
		newnode->parent = parent;
		if (tree->compare(tree->key(parent), tree->key(newnode)) >= 0)
			parent->leftchild = newnode;
		else
			parent->rightchild = newnode;
	}
}

void tree_transplant(struct sTree *tree, struct sTreeNode *pre, struct sTreeNode *lat)
{
	if (pre->parent == &TreeNodeNIL)
		tree->root = lat;
	else if (pre == pre->parent->leftchild)
		pre->parent->leftchild = lat;
	else
		pre->parent->rightchild = lat;
	if (lat != &TreeNodeNIL)
		lat->parent = pre->parent;
}

void tree_delete(struct sTree *tree, struct sTreeNode *deletenode)
{
	struct sTreeNode *tmp;

	if (deletenode->leftchild == &TreeNodeNIL) {
		tree_transplant(tree, deletenode, deletenode->rightchild);
	} else if (deletenode->rightchild == &TreeNodeNIL) {
		tree_transplant(tree, deletenode, deletenode->leftchild);
	} else {
		tmp = tree_minimum(deletenode->rightchild);
		if (tmp->parent != deletenode) {
			tree_transplant(tree, tmp, tmp->rightchild);
			tmp->rightchild = deletenode->rightchild;
			if (deletenode->rightchild != &TreeNodeNIL)
				deletenode->rightchild->parent = tmp;
		}
		tree_transplant(tree, deletenode, tmp);
		tmp->leftchild = deletenode->leftchild;
		if (deletenode->leftchild != &TreeNodeNIL)
			deletenode->leftchild->parent = tmp;
	}
}

struct sTreeNode *new_tree_node(void *content)
{
	struct sTreeNode *newnode;

	newnode = alloc(struct sTreeNode, 1);
	newnode->content = content;
	newnode->parent = &TreeNodeNIL;
	newnode->leftchild = &TreeNodeNIL;
	newnode->rightchild = &TreeNodeNIL;

	return newnode;
}

int empty_tree_node(struct sTreeNode *node)
{
	return (node == &TreeNodeNIL);
}

static void *__point(struct sTreeNode *node)
{
	return node->content;
}

static int64_t __compare_by_point(void *key1, void *key2)
{
	return key1 - key2;
}

struct sTree *new_tree(void *(*key_function_pointer)(struct sTreeNode *), \
			int64_t (*compare_function_pointer)(void *key1, void *key2))
{
	struct sTree *newtree;

	newtree = alloc(struct sTree, 1);
	newtree->root = &TreeNodeNIL;
	if (key_function_pointer == NULL)
		newtree->key = __point;
	else 
		newtree->key = key_function_pointer;
	if (compare_function_pointer == NULL)
		newtree->compare = &__compare_by_point;
	else
		newtree->compare = compare_function_pointer;

	return newtree;
}

void destory_tree(struct sTree *tree)
{
	destory_node_rec(tree->root);
	free(tree);
}

void destory_node_rec(struct sTreeNode *node)
{
	if (node == &TreeNodeNIL)
		return ;

	destory_node_rec(node->leftchild);
	destory_node_rec(node->rightchild);
	free(node);
}
