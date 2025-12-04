#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

extern void* myMALLOC(size_t size);
extern int breakListing(int* count);
extern void wait_j();

typedef struct node
{
	struct node * next;
	void* data;
} *NODE_PTR, NODE;
typedef struct gen_list
{
	int n_el;
	NODE_PTR beginning;
} *GEN_LIST_PTR, GEN_LIST;

GEN_LIST_PTR createList();

void printList(GEN_LIST_PTR main_list, void (printFunc)(const void*));
GEN_LIST_PTR duplicateList(GEN_LIST_PTR originalList);

void switchNodeData(void** node1, void** node2);
void bubbleSort(NODE_PTR f_node, int (*checkNode)(const void*, const void*));

NODE_PTR searchNode(GEN_LIST_PTR L, void* data, int (*checkNodeValue)(void*, void*));

void addNodeStart(GEN_LIST_PTR L, void* data);
int addNodePos(GEN_LIST_PTR L, void* data, int (*checkNodeValue)(void*, void*));
void addNodeEnd(GEN_LIST_PTR L, void* data);

void destroyNode(GEN_LIST_PTR L, NODE_PTR node, void (*destroyData)(void*));
void destroyList(GEN_LIST_PTR L, void (destroyData)(const void*));
#endif /* LIST_H */