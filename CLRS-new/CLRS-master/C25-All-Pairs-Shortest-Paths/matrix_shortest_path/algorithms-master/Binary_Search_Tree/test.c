#include <stdio.h>
#include <stdlib.h>

#include "binary_search_tree.h"

#define MAXCOMLEN 20

struct content {
	int value;
};

struct content *key(struct sTreeNode *);
int64_t compare(struct content *, struct content *);
void printcontent(struct sTreeNode *);

int main()
{
	char promp;
	char command[MAXCOMLEN];
	struct sTree *tree;
	struct content *cont;
	struct sTreeNode *tmp;

	promp = '>';
//	cont = alloc(struct content, 1);
	tmp = alloc(struct sTreeNode, 1);
	tree = new_tree(&key, &compare);

	printf("%c ", promp);	
	while (scanf("%s", command) != EOF) {
		cont = alloc(struct content, 1);
		scanf("%d", &(cont->value));
		if (strcmp(command, "lookup") == 0) {
			tmp = tree_lookup(tree, cont);
			if (empty_tree_node(tmp))
				printf("node not found!\n");
			else {
				printf("node found!");
				if (!empty_tree_node(tree_predecessor(tree, tmp))) {
					printf("predecessor: ");
					printcontent(tree_predecessor(tree, tmp));
				}	
				if(!empty_tree_node(tree_successor(tree, tmp))) {
					printf("successor: ");
					printcontent(tree_successor(tree, tmp));
				}
				printf("\n");
			}
		} else if(strcmp(command, "insert") == 0) {
			tree_insert(tree, new_tree_node(cont));	
		} else if (strcmp(command, "delete") == 0) {
			tmp = tree_lookup(tree, cont);
			if (empty_tree_node(tmp))
				printf("can not delete this node, because it's not in the tree\n");
			else
				tree_delete(tree, tmp);
		} else if (strcmp(command, "parent") == 0) {
			tmp = tree_lookup(tree, cont);
			if (empty_tree_node(tmp->parent))
				printf("this node is root node.\n");
			else
				printcontent(tmp->parent);
		} else if (strcmp(command, "leftchild") == 0) {
			tmp = tree_lookup(tree, cont);
			if (empty_tree_node(tmp->leftchild))
				printf("leftchild is NIL.\n");
			else
				printcontent(tmp->leftchild);
		} else if (strcmp(command, "rightchild") == 0) {
			tmp = tree_lookup(tree, cont);
			if (empty_tree_node(tmp->rightchild))
				printf("rightchild is NIL\n");
			else
				printcontent(tmp->rightchild);
		}

		printf("%c ", promp);
	}
	
	
	return 0;
}

struct content *key(struct sTreeNode *treenode)
{
	return treenode->content;
}

int64_t compare(struct content *cont1, struct content *cont2)
{
	return cont1->value - cont2->value;
}

void printcontent(struct sTreeNode *node)
{
	if (!empty_tree_node(node))
		printf(" %d ", ((struct content *)(node->content))->value);
	else
		printf(" NULL ");
}
