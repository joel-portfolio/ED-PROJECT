#include "./geo_entities.h"
DISTRICT_PTR createDistrict(char* _cod, char* _name)
{
	DISTRICT_PTR _district = (DISTRICT_PTR)myMALLOC(sizeof(DISTRICT));
	if (_district && _cod && _name)
	{
		_district->cod = _cod;
		_district->name = _name;
	}
	return _district;
}
COUNTY_PTR createCounty(char* _cod, char* _name, NODE_PTR _district)
{
	COUNTY_PTR _county = (COUNTY_PTR)myMALLOC(sizeof(COUNTY));
	if (_county && _cod && _name && _district)
	{
		_county->cod = _cod;
		_county->name = _name;
		_county->this_district = _district;
	}
	return _county;
}
PARISH_PTR createParish(char* _cod, char* _name, NODE_PTR _county)
{
	PARISH_PTR _parish = (PARISH_PTR)myMALLOC(sizeof(PARISH));
	if (_parish && _cod && _name && _county) {
		_parish->cod = _cod;
		_parish->name = _name;
		_parish->this_county = _county;
	}
	return _parish;
}

DISTRICT_PTR emptyDistrict(char* _cod, char* _name)
{
	DISTRICT_PTR _district = (DISTRICT_PTR)myMALLOC(sizeof(DISTRICT));
	if (_district)
	{
		_district->cod = _cod;
		_district->name = _name;
	}
	return _district;
}
COUNTY_PTR emptyCounty(char* _cod, char* _name, NODE_PTR var3)
{
	COUNTY_PTR _county = (COUNTY_PTR)myMALLOC(sizeof(COUNTY));
	if (_county)
	{
		_county->cod = _cod;
		_county->name = _name;
		_county->this_district = var3;
	}
	return _county;
}
PARISH_PTR emptyParish(char* _cod, char* _name, NODE_PTR _county)
{
	PARISH_PTR parish = (PARISH_PTR)myMALLOC(sizeof(PARISH));
	if (parish)
	{
		parish->cod = _cod;
		parish->name = _name;
		parish->this_county = _county;
	}
	return parish;

}

NODE_PTR getDistrict(char* code)
{
	NODE_PTR aux = districts_list->beginning;
	while (aux != NULL)
	{
		if (!strncmp(((DISTRICT_PTR)(aux->data))->cod, code, 2))
			return aux;
		aux = aux->next;
	}
	return NULL;
}
NODE_PTR getCounty(char* code) {
	if (!code || !counties_list)
		return NULL;

	COUNTY_PTR aux_county = emptyCounty(code, NULL, NULL);

	NODE_PTR aux = counties_list->beginning;
	while (aux) {
		COUNTY_PTR county = (COUNTY_PTR)(aux->data);
		if (!county || !county->this_district) {
			aux = aux->next;
			continue;
		}

		DISTRICT_PTR district = (DISTRICT_PTR)(county->this_district->data);
		if (!district) {
			aux = aux->next;
			continue;
		}

		char* county_full_code = (char*)myMALLOC(sizeof(char) * 5);
		snprintf(county_full_code, 5, "%2s%2s", district->cod, county->cod);

		if (!strncmp(county_full_code, code, 4)) {
			free(county_full_code); // Libera a memória alocada antes de retornar
			return aux;
		}

		free(county_full_code); // Libera a memória alocada antes de passar para o próximo nó
		aux = aux->next;
	}

	return NULL;
}
NODE_PTR getParish(char* code) {
	if (!code || !parishes_list)
		return NULL;

	PARISH_PTR aux_parish = emptyParish(code, NULL, NULL);

	NODE_PTR aux = parishes_list->beginning;
	while (aux) {
		char* var1 = getParishFullCode((PARISH_PTR)(aux->data));
		int val = strcmp(var1, code);
		if (!strcmp(var1, code))
			return aux;
		aux = aux->next;
	}
	return NULL;
}

char* getCountyFullCode(COUNTY_PTR _county) {
	char* c_code = _county->cod;
	char* d_code = ((DISTRICT_PTR)(_county->this_district->data))->cod;
	char* p_f_c = (char*)myMALLOC(sizeof(char) * 5);
	sprintf(p_f_c, "%s%s", d_code, c_code);
	return p_f_c;
}
char* getParishFullCode(PARISH_PTR _parish) {
	char* d_code = ((DISTRICT_PTR)(((COUNTY_PTR)(_parish->this_county->data))->this_district->data))->cod;
	char* c_code = ((COUNTY_PTR)(_parish->this_county->data))->cod;
	char* p_code = _parish->cod;
	char* d_c_p = (char*)myMALLOC(sizeof(char) * 7);
	sprintf(d_c_p, "%s%s%s", d_code, c_code, p_code);
	return d_c_p;
}

int checkDistrictName(void* aux1, void* aux2) {
	return strcmp(((DISTRICT_PTR)(aux1))->name, ((DISTRICT_PTR)(aux2))->name);
}
int checkCountyName(void* aux1, void* aux2) {
	return strcmp(((COUNTY_PTR)(aux1))->name, ((COUNTY_PTR)(aux2))->name);
}

int checkDistrict(void* aux1, void* aux2) {
	return strcmp(((DISTRICT_PTR)(aux1))->cod, ((DISTRICT_PTR)(aux2))->cod);
}
int checkCounty(void* aux1, void* aux2) {
	int var = strcmp(((COUNTY_PTR)aux1)->cod, ((COUNTY_PTR)aux2)->cod);
	if (!var)
	{
		DISTRICT_PTR dis1 = ((COUNTY_PTR)aux1)->this_district->data, dis2 = ((COUNTY_PTR)aux2)->this_district->data;
		return strcmp(dis1->cod, dis2->cod);
	}
	return var;
}
int checkParish(void* aux1, void* aux2) {
	int var = strcmp(((PARISH_PTR)aux1)->cod, ((PARISH_PTR)aux2)->cod);
	if (!var)
	{
		COUNTY_PTR _county1 = ((PARISH_PTR)aux1)->this_county->data, _county2 = ((PARISH_PTR)aux2)->this_county->data;
		var = strcmp(_county1->cod, _county2->cod);
		if (!var)
		{
			DISTRICT_PTR dis1 = _county1->this_district->data, dis2 = _county2->this_district->data;
			return strcmp(dis1->cod, dis2->cod);
		}
	}
	return var;
}

int sameDistrict(void* aux1, void* aux2) {
	return strcmp(((DISTRICT_PTR)(aux1))->cod, ((DISTRICT_PTR)(aux2))->cod);
}
int sameCounty(void* aux1, void* aux2) {
	return strcmp(((COUNTY_PTR)(aux1))->cod, ((COUNTY_PTR)(aux2))->cod);
}
int sameParish(void* aux1, void* aux2) {
	return strcmp(((PARISH_PTR)(aux1))->cod, ((PARISH_PTR)(aux2))->cod);
}

void showD(void* var) {
	DISTRICT_PTR aux = var;
	printf("\n|%s| |%s|\n", aux->cod, aux->name);
}
void showC(void* var) {
	COUNTY_PTR aux = var;
	DISTRICT_PTR aux1 = aux->this_district->data;
	printf("\n|%s| |%s| |%s|\n", aux->cod, aux->name, getCountyFullCode(aux));
}
void showP(void* var) {
	PARISH_PTR aux = var;
	COUNTY_PTR aux2 = aux->this_county->data;
	DISTRICT_PTR aux3 = aux2->this_district->data;
		printf("\n|%s| |%s| |%s|\n", aux->cod, aux->name, getParishFullCode(aux));
}