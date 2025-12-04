#ifndef USEFUL_H
#define USEFUL_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <windows.h>
#include <errno.h>
#include <stddef.h>
#include <shlobj.h>
#define DEFAULT_DELIMITERS " \t\n"

typedef long int ssize_t;

typedef struct timestamp
{
	int day, month, year, hour, minute, seconds, mili_sec;
} TIMESTAMP;

typedef struct Aux {
	char* aux;
	int count;
}*AUX_PTR, AUX;

char* filePath(const char* file);
int getOption(char* output, char* msg, int max_val);

TIMESTAMP setEmptyTimestamp();
TIMESTAMP setTimestamp();
TIMESTAMP charToTimestamp(char* var);
char* showTimeStampDate(TIMESTAMP date);
char* showErrorTimeStamp(TIMESTAMP date);
bool timestampIsValid(TIMESTAMP _timestamp);

int breakListing(int* count);
int fileExists(const char* filename);

AUX_PTR createAuxVar();
void destroyAuxPtr(void* aux_ptr);
int checkAuxPtr(void* var1, void* var2);

void* myMALLOC(size_t size);

FILE* myFOPEN(bool default_path,char* file_name, char* file_type, char* mode);

int sscanf_j(const char* line, const char* format, ...);

long int getline_j(char** lineptr, size_t* n, FILE* stream);

void mainScanf_j(char** var);
void scanf_j(char** var);

void error_j(const char* message, ...);

void wait_j();

void toUppercase(char* str);

#endif // USEFUL_H
