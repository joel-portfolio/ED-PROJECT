#ifndef GEO_ENTITIES_H
#define GEO_ENTITIES_H
#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include "./list.h"
#include "./useful.h"

extern GEN_LIST_PTR districts_list;
extern GEN_LIST_PTR counties_list;
extern GEN_LIST_PTR parishes_list;

void showD(void* data);

typedef struct district
{
	char* cod, * name;
} *DISTRICT_PTR, DISTRICT;
typedef struct county
{
	NODE_PTR this_district;
	char* cod, * name;
} *COUNTY_PTR, COUNTY;
typedef struct parish
{
	NODE_PTR this_county;
	char* cod, * name;
} *PARISH_PTR, PARISH;

DISTRICT_PTR createDistrict(char* _cod, char* _name);
COUNTY_PTR createCounty(char* _cod, char* _name, NODE_PTR _dis);
PARISH_PTR createParish(char* _cod, char* _name, NODE_PTR _dis);

DISTRICT_PTR emptyDistrict(char* var1, char* var2);
COUNTY_PTR emptyCounty(char* var1, char* var2, NODE_PTR var3);
PARISH_PTR emptyParish(char* _cod, char* _name, NODE_PTR _county);

NODE_PTR getDistrict(char* code);
NODE_PTR getCounty(char* code);
NODE_PTR getParish(char* code);

char* getCountyFullCode(COUNTY_PTR _county);
char* getParishFullCode(PARISH_PTR parish);

int checkDistrictName(void* var1, void* var2);
int checkCountyName(void* var1, void* var2);

int checkDistrict(void* aux1, void* aux2);
int checkCounty(void* aux1, void* aux2);
int checkParish(void* aux1, void* aux2);

int sameDistrict(void* aux1, void* aux2);
int sameCounty(void* aux1, void* aux2);
int sameParish(void* aux1, void* aux2);

#endif // GEO_ENTITIES_H
