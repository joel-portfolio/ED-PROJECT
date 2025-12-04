#include <stdio.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS

#include "./hashing.h"
#include <locale.h>

extern int getOption(char* output, char* msg, int max_val);

extern void booksMenu();
extern void requestersMenu();
extern void requestsMenu();

extern char* saveDataToXML();
extern char* byteToKilobytes(size_t bytes);
extern char* byteToMegabyte(size_t bytes);
extern size_t getProgramMem();
extern GEN_LIST_PTR fetchListsData(const char* file_name);
extern LIBRARY_PTR fetchHashingData(const char* file_type);
extern void storeListGen(GEN_LIST_PTR L, char* type);
extern void storeHash(LIBRARY_PTR lib);
extern void freeAllMemo();
extern void scanf_j(char** var);
extern void mainScanf_j(char** var);
extern int checkDistrict(void*, void*);
extern int checkCounty(void*, void*);
extern int checkParish(void*, void*);
extern int checkRequesterByFullName(const void* var1, const void* var2);
extern void printRequester(const void*);

GEN_LIST_PTR districts_list;
GEN_LIST_PTR counties_list;
GEN_LIST_PTR parishes_list;
GEN_LIST_PTR requesters_list;
GEN_LIST_PTR requests_list;
LIBRARY_PTR library_hash;

char* FILES[] = { "distritos", "concelhos", "freguesias", "requesters", "requests","books" };

void mainMenu() {
	int opt = -1;
	char* msg = NULL;
	do
	{
		opt = getOption("\nMENU\n\t[1] Gerir os livros\n\t[2] Gerir a lista de requisitantes\n\t[3] Gerir as requisicoes de livros\n\t[4] Memória Ocupada\n\t[5] Guardar dados em XML\n\t[6] Guardar dados nos respetivos ficheiros\n\t[0] SAIR\nR.: ", msg, 6);
		msg = NULL;
		switch (opt)
		{
		case 1:
			booksMenu(NULL);
			break;
		case 2:
			requestersMenu(NULL);
			break;
		case 3:
			requestsMenu(NULL);
			break;
		case 4:
			printf("Memória total alocada: %s\n", byteToMegabyte(getProgramMem()));
			printf("Memória total alocada: %s\n", byteToKilobytes(getProgramMem()));
			printf("Memória total alocada: %zu byte's\n", getProgramMem());
			wait_j();
			break;
		case 5:
			msg = saveDataToXML();
			break;
		case 6:
		{
			storeListGen(requesters_list, FILES[3]);
			storeListGen(requests_list, FILES[4]);
			storeHash(library_hash);
			printf("Dados guardado com sucesso!");
		}
		break;
		}
	} while (opt);
	storeListGen(requesters_list, FILES[3]);
	storeListGen(requests_list, FILES[4]);
	storeHash(library_hash);
	freeAllMemo();
}
int main()
{
	setlocale(LC_ALL, "Portuguese");

	districts_list = fetchListsData(FILES[0]);
	//quickSort(districts_list->beginning, lastNode(districts_list), checkDistrict);

	counties_list = fetchListsData(FILES[1]);
	//quickSort(counties_list->beginning, lastNode(counties_list), checkCounty);

	parishes_list = fetchListsData(FILES[2]);
	//quickSort(parishes_list->beginning, lastNode(parishes_list), checkParish);

	requesters_list = fetchListsData(FILES[3]);

	library_hash = fetchHashingData(FILES[5]);

	requests_list = fetchListsData(FILES[4]);

	mainMenu();

	return 0;
}