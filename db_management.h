#ifndef DB_MANAGEMENT_H
#define DB_MANAGEMENT_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>

#include "./hashing.h"
#include "./requester.h"
#include "./requests.h"
#include "./book.h"

extern GEN_LIST_PTR counties_list;
extern GEN_LIST_PTR parishes_list;
extern GEN_LIST_PTR requesters_list;

extern char* FILES[];

extern void mainMenu();
void* getLineContent(char* line, int type);
GEN_LIST_PTR fetchListsData(const char* file_name);

LIBRARY_PTR fetchHashingData(const char* file_type);

char* saveDataToXML();

void storeListGen(GEN_LIST_PTR L, const char* type);
void storeHash(LIBRARY_PTR lib);

int storeRequesters();
int storeLibrary();

#endif // DB_MANAGEMENT_H
