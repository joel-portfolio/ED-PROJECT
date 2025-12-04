#ifndef BOOK_H
#define BOOK_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>

#include "./hashing.h"
#include "./useful.h"

extern LIBRARY_PTR library_hash;
extern GEN_LIST_PTR requests_list;
extern void storeHash(LIBRARY_PTR lib);
extern void mainMenu();

typedef struct
{
	char* title, * author, * ISBN;
	int publy_year;
	bool requested;
} *BOOK_PTR, BOOK;

void booksMenu();

int checkBookByISBN(void* var1, void* var2);
int checkBookByTitle(void* var1, void* var2);
int checkBookByTitleISBN(void* var1, void* var2);
int bookExist(char* var);

BOOK_PTR setBook(char* title, char* author, char* ISBN, char* publy_year,char* requested);

void listBook(void* var,FILE*file);

char* addNewBook();

void areaWMostBooks(AUX_PTR aux);

void showBook(char* ISBN);

char* showBookISBN(void* var);

int mostRecentYear();

GEN_LIST_PTR showRecentBooks();

AUX_PTR showMostRequestedBook();

AUX_PTR showMostRequestedArea();

void destroyBook(void* b);

#endif // BOOK_H
