#include "./memo_management.h"

size_t getPTRCharMem(char* var) {
	return (var) ? (sizeof(*var) + strlen(var) + 1) : 0;
}
size_t getGroupMem(GROUPS_PTR var, size_t getEleMem(void*)) {
	if (!var)
		return 0;
	size_t mem = sizeof(*var);
	LIBRARY_NODE_PTR var_node = var->beginning;
	while (var_node) {
		mem += sizeof(var_node) + sizeof(*var_node) + getPTRCharMem(var_node->key) + getListMem(var_node->books_list, getEleMem);
		var_node = var_node->next;
	}
	return mem;
}
size_t getHashMem(LIBRARY_PTR var, size_t getEleMem(void*)) {
	return (var) ? (sizeof(var) + sizeof(*var) + getGroupMem(var->_groups, getEleMem)) : 0;
}
size_t getListMem(GEN_LIST_PTR var, size_t getEleMem(void*)) {
	if (!var)
		return 0;
	size_t mem = sizeof(*var);
	NODE_PTR list_node = var->beginning;
	while (list_node) {
		mem += sizeof(list_node) + getEleMem(list_node->data);
		list_node = list_node->next;
	}
	return mem;
}
size_t getDistrictMem(DISTRICT_PTR var) {
	return (var) ? (sizeof(*var) + getPTRCharMem(var->cod) + getPTRCharMem(var->name)) : 0;
}
size_t getCountyMem(COUNTY_PTR var) {
	return (var) ? (sizeof(*var) + getPTRCharMem(var->cod) + getPTRCharMem(var->name)) : 0;
}
size_t getParishMem(PARISH_PTR var) {
	return (var) ? (sizeof(*var) + getPTRCharMem(var->cod) + getPTRCharMem(var->name)) : 0;
}
size_t getRequesterMem(REQUESTER_PTR var) {
	return (var) ? (sizeof(*var) + sizeof(var->birth_date) + getPTRCharMem(var->id) + getPTRCharMem(var->name)) : 0;
}
size_t getRequestMem(REQUEST_PTR var) {
	return (var) ? (sizeof(*var) + sizeof(var->delivery_date) + sizeof(var->request_date)) : 0;
}
size_t getBookMem(BOOK_PTR var) {
	return (var) ? (sizeof(*var) + getPTRCharMem(var->author) + getPTRCharMem(var->ISBN) + getPTRCharMem(var->title)) : 0;
}
char* byteToKilobytes(size_t bytes) {
	char* str = (char*)myMALLOC(15 * sizeof(char));
	snprintf(str, 15, "%.4f kb", ((double)bytes / 1024));
	return str;
}
char* byteToMegabyte(size_t bytes) {
	char* str = (char*)myMALLOC(10 * sizeof(char));
	snprintf(str, 10, "%.4f mb", ((double)bytes / (1024 * 1024)));
	return str;
}
size_t getProgramMem() {
	return getListMem(districts_list, getDistrictMem) + getListMem(counties_list, getCountyMem) + getListMem(parishes_list, getParishMem) + getListMem(requesters_list, getRequesterMem) + getListMem(requests_list, getRequestMem) + getHashMem(library_hash, getBookMem);
}
void freeGenListMemo(GEN_LIST_PTR gen_list)
{
}
void freeHashMemo(LIBRARY_PTR lib_hash)
{
}

void freeAllMemo() {
	freeGenListMemo(districts_list);
	freeGenListMemo(counties_list);
	freeGenListMemo(parishes_list);
	freeGenListMemo(requesters_list);
	freeGenListMemo(requests_list);
	freeHashMemo(library_hash);
}