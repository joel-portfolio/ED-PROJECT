#include "./hashing.h"

//cria um HASH vazio
LIBRARY_PTR createLibrary()
{
	LIBRARY_PTR library = (LIBRARY_PTR)myMALLOC(sizeof(LIBRARY));
	if (library)
		library->_groups = createGroups();
	return library;
}

//create um grupo vazio
GROUPS_PTR createGroups()
{
	GROUPS_PTR groups = (GROUPS_PTR)myMALLOC(sizeof(GROUPS));
	if (groups)
	{
		groups->n_el = 0;
		groups->beginning = NULL;
	}
	return groups;
}

//cria um nó vazio
LIBRARY_NODE_PTR createLibraryNode()
{
	LIBRARY_NODE_PTR lib_node = (LIBRARY_NODE_PTR)myMALLOC(sizeof(LIBRARY_NODE));
	if (lib_node)
	{
		lib_node->key = NULL;
		lib_node->books_list = createList();
		lib_node->next = NULL;
	}
	return lib_node;
}

//cria um nó com valores
LIBRARY_NODE_PTR setLibraryNode(char* key, GEN_LIST_PTR books_list) {
	LIBRARY_NODE_PTR lib_node = (LIBRARY_NODE_PTR)myMALLOC(sizeof(LIBRARY_NODE));
	if (lib_node)
	{
		lib_node->key = key;
		lib_node->books_list = createList();
		lib_node->next = NULL;
	}
	return lib_node;
}

//adiciona nó a um grupo pertencente a um hash dado, ordenado pela chave
int addNodeToGroupByKey(LIBRARY_PTR main_lib, LIBRARY_NODE_PTR lib_node)
{
	if (main_lib && lib_node)
	{
		LIBRARY_NODE_PTR start_lib = main_lib->_groups->beginning, previous = NULL;
		if (start_lib) {
			while (start_lib && (strcmp(start_lib->key, lib_node->key) <= 0))// =0 -> (aa,aa) / <0 -> (aa,bb) / >0 -> (bb,aa)
			{
				previous = start_lib;
				start_lib = start_lib->next;
			}
			if (previous)//adiciona no meio
			{
				previous->next = lib_node;
				lib_node->next = start_lib;
			}
			else {
				lib_node->next = main_lib->_groups->beginning;
				main_lib->_groups->beginning = lib_node;
			}
		}
		else
			main_lib->_groups->beginning = lib_node;
		main_lib->_groups->n_el++;
		return 1;
	}
	else return 0;
}

//procura lista de um grupo pelo valor da key
LIBRARY_NODE_PTR searchArea(LIBRARY_PTR main_lib, char* var) {
	if (main_lib)
		if (main_lib->_groups)
		{
			LIBRARY_NODE_PTR books_list = main_lib->_groups->beginning;
			while (books_list) {
				if (!strcmp(books_list->key, var))
					return books_list;
				books_list = books_list->next;
			}
		}
	return NULL;
}
void printHash(LIBRARY_PTR library, void (printData)(const void*)) {
	system("cls");
	int count = 0, c;
	LIBRARY_NODE_PTR lib_node = library->_groups->beginning;
	while (lib_node) {
		printf("\n%s\n", lib_node->key);
		printList(lib_node->books_list, printData);
		lib_node = lib_node->next;
	}
	wait_j();
}
void destroyLibrary(LIBRARY_PTR L)
{
	if (!L)
		return;
	LIBRARY_NODE_PTR aux1 = L->_groups->beginning, aux2;
	while (aux1)
	{
		destroyList(aux1->books_list, destroyBook);
		free(aux1->key);
		aux2 = aux1->next;
		free(aux1);
		aux1 = aux2;
	}
	free(L);
}