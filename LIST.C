#include "./list.h"

GEN_LIST_PTR createList()
{
	GEN_LIST_PTR list = (GEN_LIST_PTR)myMALLOC(sizeof(GEN_LIST));
	if (list)
	{
		list->beginning = NULL;
		list->n_el = 0;
	}
	return list;
}

NODE_PTR searchNode(GEN_LIST_PTR L, void* data, int (*checkNodeValue)(void*, void*)) {
	if (!L || !data)
		return NULL;

	NODE_PTR aux1 = L->beginning;

	while (aux1) {
		if (!checkNodeValue(aux1->data, data))
			return aux1;
		aux1 = aux1->next;
	}
	return NULL;
}

void addNodeStart(GEN_LIST_PTR L, void* data) // adicionar nó no beginning
{
	if (!L || !data)
		return;
	NODE_PTR node = (NODE_PTR)myMALLOC(sizeof(NODE));
	node->data = data;
	node->next = L->beginning;
	L->beginning = node;
	L->n_el++;
}
int addNodePos(GEN_LIST_PTR L, void* data, int (*checkNodeValue)(void*, void*))
{
	if (L && data)
	{
		NODE_PTR node = (NODE_PTR)myMALLOC(sizeof(NODE));
		node->data = data;
		node->next = NULL;
		NODE_PTR aux1 = L->beginning;
		if(aux1)
		{
			NODE_PTR previous = NULL;
			while (aux1 && checkNodeValue(aux1->data, data) > 0)
			{
				previous = aux1;
				aux1 = aux1->next;
			}
			if (previous) {
				previous->next = node;
				node->next = aux1;
			}
			else {
				node->next = L->beginning;
				L->beginning = node;
			}
		}else
			L->beginning = node;
		L->n_el++;
		return 1;
	}
	else return 0;
}
void addNodeEnd(GEN_LIST_PTR L, void* data) // adicionar nó no fim
{
	NODE_PTR node = (NODE_PTR)myMALLOC(sizeof(NODE));
	node->next = NULL;
	node->data = data;

	NODE_PTR aux1 = L->beginning, aux2 = NULL;
	if (!aux1)
		L->beginning = node;
	else
	{
		while (aux1->next)
			aux1 = aux1->next;
		aux1->next = node;
	}
	L->n_el++;
}
void printList(GEN_LIST_PTR main_list, void (printFunc)(const void*)) {
	if (main_list)
	{
		NODE_PTR node = main_list->beginning;
		int count = 0;
		while (node) {
			printFunc(node->data);
			node = node->next;
			if (breakListing(&count)) break;
		}
	}
}
void destroyList(GEN_LIST_PTR L, void (destroyData)(const void*))
{
	if (L)
	{
		NODE_PTR node = L->beginning, next_node = NULL;
		while (node)
		{
			next_node = node->next;
			node->data = NULL;
			free(node);
			node = next_node;
		}
		free(L);
	}
}

GEN_LIST_PTR duplicateList(GEN_LIST_PTR main_list) {
	if (main_list && main_list->beginning)
	{
		GEN_LIST_PTR copied_list = createList();
		copied_list->n_el = main_list->n_el;

		NODE_PTR original_node = main_list->beginning, actual_node = NULL, new_beginning = NULL;

		while (original_node) {
			NODE_PTR new_node = (NODE_PTR)malloc(sizeof(NODE));
			new_node->data = original_node->data;
			new_node->next = NULL;

			if (actual_node)
				actual_node->next = new_node;
			else {
				copied_list->beginning = new_node;
				new_beginning = new_node;
			}

			original_node = original_node->next;
			actual_node = new_node;
		}
		return copied_list;
	}
	else return NULL;
}

void switchNodeData(void** node1, void** node2) {
	void* temp = *node1;
	*node1 = *node2;
	*node2 = temp;
}
void bubbleSort(NODE_PTR f_node, int (*checkNodeData)(const void*, const void*)) {
	if (!f_node) return;

	int bool_val = 0;
	NODE_PTR ptr1;
	NODE_PTR lptr = NULL;

	do {
		bool_val = 0;
		ptr1 = f_node;

		while (ptr1->next != lptr) {
			if (checkNodeData(ptr1->data, ptr1->next->data) > 0) {
				switchNodeData(&(ptr1->data), &(ptr1->next->data));
				bool_val = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (bool_val);
}

int comparString(const void* a, const void* b) {
	return strcmp(*(const char**)a, *(const char**)b);
}
void destroyNode(GEN_LIST_PTR L, NODE_PTR node, void (*destroyData)(void*)) {
	if (L && node)
	{
		if (L->beginning == node)
			L->beginning = node->next;
		else {
			NODE_PTR current = L->beginning;
			while (current != NULL && current->next != node)
				current = current->next;
			if (current == NULL) return;
			current->next = node->next;
		}
		destroyData(node->data);
		free(node);
		L->n_el--;
	}
}


NODE_PTR lastNode(GEN_LIST_PTR _list) {
	NODE_PTR node = _list->beginning;
	while (node->next)
		node = node->next;
	return node;
}
NODE_PTR partition(NODE_PTR low, NODE_PTR high, int (*checkNode)(const void*, const void*)) {
	void* pivot = high->data;
	NODE_PTR i = low, j = low;

	while (j != high) {
		if (checkNode(j->data, pivot) <= 0) {
			switchNodeData(&(i->data), &(j->data));
			i = i->next;
		}
		j = j->next;
	}
	switchNodeData(&(i->data), &(high->data));
	return i;
}
void quickSort(NODE_PTR f_node, NODE_PTR l_node, int (*checkNode)(const void*, const void*)) {
	if (f_node && l_node && f_node != l_node) {
		NODE_PTR pivot = partition(f_node, l_node, checkNode);

		if (f_node != pivot) {
			NODE_PTR temp = f_node;
			while (temp->next != pivot)
				temp = temp->next;
			quickSort(f_node, temp, checkNode);
		}

		if (pivot->next != l_node)
			quickSort(pivot->next, l_node, checkNode);
	}
}
