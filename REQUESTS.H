#ifndef REQUESTS_H
#define REQUESTS_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "./requester.h"
#include "./book.h"

extern GEN_LIST_PTR requesters_list;
extern GEN_LIST_PTR requests_list;
extern LIBRARY_PTR library_hash;
extern char* FILES[];

typedef struct Request
{
	int id;
	TIMESTAMP request_date, delivery_date;
	REQUESTER_PTR requester;
	NODE_PTR book;
} *REQUEST_PTR, REQUEST;

void requestsMenu();

REQUEST_PTR createRequest(int id, REQUESTER_PTR requester, NODE_PTR book, TIMESTAMP req_date, TIMESTAMP deli_date);
char* requestBook();
void setRequestDelivery(REQUEST_PTR request);

int checkRequestById(void* var1, void* var2);
int getNewId();

GEN_LIST_PTR searchRequestsByRequesterId(char* id, void* wBookReq);

int checkRequestByISBN(void* var1, void* var2);

void basicShowRequestedBooks();
char* complexShowRequestedBooks();

void printRequest(const void* var,FILE *file);

void destroyRequest(void* var);

#endif // REQUESTS_H