#ifndef HASHING_H
#define HASHING_H

#include <string.h>
#include "./list.h"

typedef struct library_node
{
	char* key;
	GEN_LIST_PTR books_list;
	struct library_node * next;
} *LIBRARY_NODE_PTR, LIBRARY_NODE;

typedef struct groups
{
	int n_el;
	LIBRARY_NODE_PTR beginning;
} *GROUPS_PTR, GROUPS;

typedef struct library
{
	//char* name;
	GROUPS_PTR _groups;
} *LIBRARY_PTR, LIBRARY;

extern LIBRARY_PTR library_hash;
extern void destroyBook(void* b);
extern void destroyRequest(void* var);

LIBRARY_PTR createLibrary();
GROUPS_PTR createGroups();
LIBRARY_NODE_PTR createLibraryNode();

LIBRARY_NODE_PTR setLibraryNode(char* key, GEN_LIST_PTR books_list);

int addNodeToGroupByKey(LIBRARY_PTR main_lib, LIBRARY_NODE_PTR lib_node);

LIBRARY_NODE_PTR searchArea(char* var);
void printHash(LIBRARY_PTR library, void (printData)(void*));
void destroyLibrary(LIBRARY_PTR L);

#endif // HASHING_H
