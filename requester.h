#ifndef REQUESTER_H
#define REQUESTER_H
#define _CRT_SECURE_NO_WARNINGS

#include "./geo_entities.h"

extern GEN_LIST_PTR requesters_list;
extern GEN_LIST_PTR requests_list;
extern char* FILES[];
extern void storeListGen(GEN_LIST_PTR L, const char* type);
extern void printRequest(void* var,FILE*file);
extern void destroyRequest(const void* R);
extern GEN_LIST_PTR searchRequestsByRequesterId(char* id, void * wBookReq);
extern void mainMenu();

typedef struct requester
{
	char* id, * name;
	TIMESTAMP birth_date;
	NODE_PTR this_parish;
} *REQUESTER_PTR, REQUESTER;

void requestersMenu();

REQUESTER_PTR createEmptyRequester(char* id, char* name, TIMESTAMP birth_date, NODE_PTR _parish);
REQUESTER_PTR createRequester(char* id, char* name, TIMESTAMP birth_date, NODE_PTR _parish);
char* addRequester();
char* getNewRequesterId();
char* getRequesterId(void* req_node);
char* getSurname(char* full_name);
int getAge(TIMESTAMP birth_date);
char* longerSurname();

int compareLocations(char* loc1, char* loc2);
bool checkName(char* full_name, char* comp_name);
int checkRequesterByName(const void* var1, const void* var2);
int checkRequesterByFullName(const void* var1, const void* var2);
int checkRequesterBySurname(const void* var1, const void* var2);
int checkRequesterByParish(const void* var1, const void* var2);
int checkRequesterById(void* var1, void* var2);
int checkRequesterByAge(void* var1, void* var2);

void printRequester(const void* var);

//obter idade máxima de todos os requisitantes
int getOlderRequester();
//obter idade média de todos os requisitantes
double getRequestersAverageAge();
//obter requisitantes mais velhos que idade dada
int getRequestersOlderThan(int age);
// idade com mais requisitantes
int getMostRequesitantsAge();
//requisições de um dado requisitante
char* showRequesterRequests(char* full_name);
//pessoas que nunca requisitaram nada
char* showRequestersWithNoRequests();
//requisitantes que no momento têm livros requisitados
char* showRequestersWithRequestedBooks();
//obter sobrenome mais usado
AUX_PTR mostUsedSurname();
//listar requisitantes com o nome e distrito/concelho dado pelo user
int requestersNumFromXLocation(char* name, char* location);

//destroi requisitante
void destroyRequester(const void* data);

char* weekDayWZellersCongruence(TIMESTAMP date);

#endif // REQUESTER_H