#ifndef __BINARY_SEARCH_TREE_H__
#define __BINARY_SEARCH_TREE_H__

#include <stdint.h>

#include "macros.h"

struct sTreeNode {
	void *content;

	struct sTreeNode *parent;
	struct sTreeNode *leftchild;
	struct sTreeNode *rightchild;
};

struct sTree { 
	struct sTreeNode *root;
	void *(*key)(struct sTreeNode *);
	int64_t (*compare)(void *key1, void *key2);
};

struct sTreeNode *tree_lookup(const struct sTree *, void *key);
struct sTreeNode *tree_minimum(const struct sTreeNode *);
struct sTreeNode *tree_maximum(const struct sTreeNode *);
struct sTreeNode *tree_predecessor(const struct sTree *, const struct sTreeNode *);
struct sTreeNode *tree_successor(const struct sTree *, const struct sTreeNode *);
void tree_delete(struct sTree *, struct sTreeNode *);

struct sTreeNode *new_tree_node(void *);
int empty_tree_node(struct sTreeNode *);
static void *__point(struct sTreeNode *);
static int64_t __compare_by_point(void *key1, void *key2);
struct sTree *new_tree(void *(*key_function_pointer)(struct sTreeNode *) \
			, int64_t (*compare_function_pointer)(void *, void *));
void destory_tree(struct sTree *);
void destory_node_rec(struct sTreeNode *);

#endif
