#ifndef MEMO_MANAGEMENT_h
#define MEMO_MANAGEMENT_h

#include <stdio.h>
#include "./list.h"
#include "./hashing.h"
#include "./geo_entities.h"
#include "./requester.h"
#include "./requests.h"

extern GEN_LIST_PTR districts_list;
extern GEN_LIST_PTR counties_list;
extern GEN_LIST_PTR parishes_list;
extern GEN_LIST_PTR requesters_list;
extern GEN_LIST_PTR requests_list;
extern LIBRARY_PTR library_hash;
extern int exitProgram(int val);

size_t getPTRCharMem(char* var);
size_t getGroupMem(GROUPS_PTR var, size_t getEleMem(void*));
size_t getHashMem(LIBRARY_PTR var, size_t getEleMem(void*));
size_t getListMem(GEN_LIST_PTR var, size_t getEleMem(void*));
size_t getDistrictMem(DISTRICT_PTR var);
size_t getCountyMem(COUNTY_PTR var);
size_t getParishMem(PARISH_PTR var);
size_t getRequesterMem(REQUESTER_PTR var);
size_t getRequestMem(REQUEST_PTR var);
size_t getBookMem(BOOK_PTR var);

char* byteToKilobytes(size_t bytes);
char* byteToMegabyte(size_t bytes);

size_t getProgramMem();

void freeGenListMemo(GEN_LIST_PTR gen_list);
void freeHashMemo(LIBRARY_PTR lib_hash);

void freeAllMemo();

#endif /* MEMO_MANAGEMENT_h */