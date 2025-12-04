#include "./requester.h"

int STOP_CYCLE = 0;
void requestersMenu()
{
	int var1, c, opt;
	char* msg = NULL;
	do {
		var1 = 0, opt = -1;
		char* str1 = NULL, * str2 = NULL, * text = NULL;
		opt = getOption("\nREQUISITANTES\n\t[1] Adicionar requisitante\n\t[2] Verificar se um requisitante existe (dando o seu nome)\n\t[3] Listar requesitantes\n\t[4] Determinar a idade maxima dos requisitantes\n\t[5] Determinar a media das idades dos requisitantes\n\t[6] Contar o numero de pessoas com idade superior a um valor\n\t[7] Determinar a idade com mais requisitantes\n\t[8] Mostrar as requisicoes de um dado requisitante\n\t[9] Mostrar pessoas que nunca requisitaram nada\n\t[10] Listar requisitantes com livros requisitados\n\t[11] Determinar o sobrenome mais comum dos requisitantes\n\t[12] Contagem de Pessoas por Localidade com determinado Apelido\n\t[13] Salvar informacoes dos requisitantes\n\t[0] TRÁS\nR.: ", msg, 13);
		msg = NULL;
		switch (opt)
		{
		case 1:
		{
			msg = addRequester();
		}
		break;
		case 2:
		{
			printf("[2] Verificar se um requisitante existe (dando o seu nome)\nNome completo:");
			scanf_j(&str1);
			REQUESTER_PTR req = NULL;
			NODE_PTR node = searchNode(requesters_list, createEmptyRequester(NULL, str1, setEmptyTimestamp(), NULL), checkRequesterByFullName);
			if (node)
			{
				req = (REQUESTER_PTR)node->data;
				if (req)
				{
					str2 = showTimeStampDate(req->birth_date);
					msg = (char*)myMALLOC(sizeof(char) * (strlen("\n existe com o id  e nasceu a ") + strlen(str1) + strlen(req->id) + strlen(str2) + 1));
					sprintf(msg, "\n%s existe com o id %s e nasceu a %s", str1, req->id, showTimeStampDate(req->birth_date));
				}
			}
			else
				msg = "Nome inserido está incorreto ou requisitante não consta na lista!";
			free(str1);
		}
		break;
		case 3://PERGUNTAR SE QUER LISTAR OU ESCREVER
		{
			var1 = getOption("[3] Listar requesitantes\nListar por : \n\t[1] nome completo; \n\t[2] freguesia\n\t[3] apelido \n\t[0] TRÁS\nR.:", NULL, 3);
			if (var1) {
				GEN_LIST_PTR cp_list = duplicateList(requesters_list);
				bubbleSort(cp_list->beginning, (var1 == 1) ? checkRequesterByFullName : ((var1 == 2) ? checkRequesterByParish : checkRequesterBySurname));
				printList(cp_list, printRequester);
				destroyList(cp_list, destroyRequester);
				msg = "Listagem completa!";
			}
		}
		break;
		case 4:
		{
			printf("[4] Determinar a idade maxima dos requisitantes\nIdade maxima de todos os requisitantes = %d", getOlderRequester());
			printf("\n\nQualquer tecla para continuar...\n");
			while ((c = getchar()) != '\n' && c != EOF);
		}
		break;
		case 5:
		{
			printf("[5] Determinar a media das idades dos requisitantes\nMedia de idades de todos os requesitantes = %.2f", getRequestersAverageAge());
			wait_j();
		}
		break;
		case 6:
		{
			printf("[6] Contar o numero de pessoas com idade superior a um valor\nIdade pretendida:");
			scanf(" %d", &var1);
			printf("Existem %d pessoas com mais de %d anos.", getRequestersOlderThan(var1), var1);
			wait_j();
		}
		break;
		case 7:
		{
			printf("[7] Determinar a idade com mais requisitantes\n%d anos é a idade com mais requesitantes.", getMostRequesitantsAge());
			wait_j();
		}
		break;
		case 8://PERGUNTAR SE QUER LISTAR OU ESCREVER
		{
			if (requests_list->n_el) {
				str1 = (char*)myMALLOC(sizeof(char) * 100);
				printf("[8] Mostrar as requisicoes de um dado requisitante\nNome completo:");
				scanf_j(&str1);
				msg = showRequesterRequests(str1);
				free(str1);
			}
			else
				msg = "Não foram feitas requisições até agora!";
		}
		break;
		case 9://PERGUNTAR SE QUER LISTAR OU ESCREVER
		{
			msg = showRequestersWithNoRequests();
		}
		break;
		case 10://PERGUNTAR SE QUER LISTAR OU ESCREVER
		{
			msg = showRequestersWithRequestedBooks();
		}
		break;
		case 11:
		{
			AUX_PTR surname = mostUsedSurname();
			printf("[11] Determinar o sobrenome mais comum dos requisitantes\n%s foi usado %d vezes sendo assim o sobrenome mais usado pelos requesitantes.", surname->aux, surname->count);
			wait_j();
		}
		break;
		case 12:
		{
			printf("[12] Contagem de Pessoas por Localidade com determinado Apelido\n\nNome: ");
			scanf_j(&str1);
			printf("Localização(CAPSLOCK para Distrito):");
			scanf_j(&str2);
			if (str1 && str2)
			{
				printf("Existem %d pessoas de %s cujo nome é %s", requestersNumFromXLocation(str1, str2), str2, str1);
				free(str1);
				free(str2);
				wait_j();
			}
		}
		break;
		case 13:
		{
			storeListGen(requesters_list, FILES[3]);
			msg = "Requisitantes guardados com sucesso!";
		}
		break;
		}
	} while (opt);
}

REQUESTER_PTR createRequester(char* id, char* name, TIMESTAMP birth_date, NODE_PTR _parish)
{
	if (!id || !name || !_parish)
		return NULL;
	int count = 0;
	for (int i = 0; i < strlen(id); i++)
		count += *(id + i) - '0';
	if (strlen(id) != 9 || (count % 10) != 0 || !timestampIsValid(birth_date))
	{
		error_j("FAILED: '%s\t%s\t%s\t%s' - invalid format", id, name, showTimeStampDate(birth_date), getParishFullCode(_parish));
		return NULL;
	}
	REQUESTER_PTR _requester = (REQUESTER_PTR)myMALLOC(sizeof(REQUESTER));
	if (_requester)
	{
		_requester->id = (char*)myMALLOC(sizeof(char) * (strlen(id) + 1));
		_requester->name = (char*)myMALLOC(sizeof(char) * (strlen(name) + 1));
		if (_requester->id && _requester->name)
		{
			strcpy(_requester->id, id);
			strcpy(_requester->name, name);
			_requester->birth_date = birth_date;
			_requester->this_parish = _parish;
		}
	}
	return _requester;
}
REQUESTER_PTR createEmptyRequester(char* id, char* name, TIMESTAMP birth_date, NODE_PTR _parish)
{
	REQUESTER_PTR requester = (REQUESTER_PTR)myMALLOC(sizeof(REQUESTER));
	if (requester)
	{
		requester->id = id;
		requester->name = name;
		requester->birth_date = birth_date;
		requester->this_parish = _parish;
	}
	return requester;
}
char* addRequester()
{
	char* id = NULL, * name = NULL, * birth_date = NULL, * loc_cod = NULL, * text = NULL, * var1 = NULL, * var2 = NULL, * var3 = NULL, * msg = NULL;
	NODE_PTR node1 = NULL, node2 = NULL, node3 = NULL;
	printf("[1] Adicionar requisitante\nNOME: ");
	scanf_j(&name);
	printf("DATA DE NASCIMENTO(dd-mm-yyyy):");
	scanf_j(&birth_date);
	printf("CÓDIGO DE FREGUESIA(9 caracteres): ");
	scanf_j(&loc_cod);
	if (name && birth_date && loc_cod) {
		var1 = (char*)myMALLOC(sizeof(char) * 3);
		var2 = (char*)myMALLOC(sizeof(char) * 3);
		var3 = (char*)myMALLOC(sizeof(char) * 3);
		strncpy(var1, loc_cod, 2);
		*(var1 + 2) = '\0';
		strncpy(var2, loc_cod + 2, 2);
		*(var2 + 2) = '\0';
		strncpy(var3, loc_cod + 4, 2);
		*(var3 + 2) = '\0';
		node1 = searchNode(districts_list, emptyDistrict(var1, NULL), checkDistrict);
		if (!node1)
			return NULL;
		node2 = searchNode(counties_list, emptyCounty(var2, NULL, node1), checkCounty);
		if (!node2)
			return NULL;
		node3 = searchNode(parishes_list, emptyParish(var3, NULL, node2), checkParish);
		if (!node3)
			return NULL;
		REQUESTER_PTR _requester = createRequester(getNewRequesterId(), name, charToTimestamp(birth_date), node3);
		free(id);
		free(name);
		free(birth_date);
		free(loc_cod);
		if (!_requester)
			msg = "Dados inválidos\n";

		msg = (char*)myMALLOC(sizeof(char) * (strlen(_requester->name) + strlen("\"\" adicionado com sucesso!") + 1));
		if (addNodePos(requesters_list, _requester, checkRequesterById))
			sprintf(msg, "'%s' adicionado com sucesso!", _requester->name);
	}
	else
		msg = "Requisitante não adicionado!";
	return msg;
}
char* getNewRequesterId() {
	srand(time(NULL));
	int i = 0, count = 0;
	char* id = (char*)malloc(sizeof(char) * 10);
	if (id)

	{
		while (i < 8) {
			*(id + i) = '0' + (rand() % 10);
			count += *(id + i) - '0';
			i++;
		}
		*(id + 8) = '0' + (10 - (count % 10)) % 10;
		*(id + 9) = '\0';
		if (searchNode(requesters_list, createEmptyRequester(id, NULL, setEmptyTimestamp(), NULL), checkRequesterById))
		{
			free(id);
			return getNewRequesterId();
		}
		else return id;
	}
	else return NULL;
}
char* getRequesterId(void* var) {
	return ((REQUESTER_PTR)var)->id;
}
char* getSurname(char* full_name) {
	return strrchr(full_name, ' ') + 1;//+1 para retornar sem espaço
}
int getAge(TIMESTAMP birth_date)
{
	if (!timestampIsValid(birth_date))
		return 0;

	TIMESTAMP now = setTimestamp();

	int age = now.year - birth_date.year;
	if (now.month < birth_date.month || (now.month == birth_date.month && now.day < birth_date.day))
		age--;
	return age;
}
char* longerSurname() {
	NODE_PTR requesters = requesters_list->beginning;
	size_t surname_len = 0, size = 0;
	char* bigSurname = NULL, * surname = NULL;
	while (requesters) {
		surname = ((REQUESTER_PTR)requesters->data)->name;
		size = strlen(surname);
		if (size > surname_len) {
			surname_len = size;
			bigSurname = surname;
		}
	}
	return bigSurname;
}

int compareLocations(char* loc1, char* loc2) {
	return !strcmp(loc1, loc2) ? 1 : 0;
}
bool checkName(char* full_name, char* comp_name) {
	char* token = strtok(full_name, " ");
	while (token != NULL) {
		if (!strcmp(token, comp_name))
			return true;
		token = strtok(NULL, " ");
	}
	return false;
}
int checkRequesterByName(const void* var1, const void* var2) {
	char* p1 = ((REQUESTER_PTR)var1)->name, * p2 = ((REQUESTER_PTR)var2)->name;

	while (*p1 != '\0')
		if (*p1 == *p2) {
			p1++;
			p2++;
			if (*p2 == '\0')
				return 0;
		}
		else {
			p2 = ((REQUESTER_PTR)var2)->name;
			p1++;
		}
	return 1;
}
int checkRequesterByFullName(const void* var1, const void* var2) {
	return strcmp(((REQUESTER_PTR)var1)->name, ((REQUESTER_PTR)var2)->name);
}
int checkRequesterBySurname(const void* var1, const void* var2)
{
	return strcmp(getSurname((*(REQUESTER_PTR*)var1)->name), getSurname((*(REQUESTER_PTR*)var2)->name));
}
int checkRequesterByParish(const void* var1, const void* var2) {
	const char* code1 = ((PARISH_PTR)((REQUESTER_PTR)var1)->this_parish->data)->cod;
	const char* code2 = ((PARISH_PTR)((REQUESTER_PTR)var2)->this_parish->data)->cod;
	const char* countyCode1 = ((COUNTY_PTR)((PARISH_PTR)((REQUESTER_PTR)var1)->this_parish->data)->this_county->data)->cod;
	const char* countyCode2 = ((COUNTY_PTR)((PARISH_PTR)((REQUESTER_PTR)var2)->this_parish->data)->this_county->data)->cod;
	const char* districtCode1 = ((DISTRICT_PTR)((COUNTY_PTR)((PARISH_PTR)((REQUESTER_PTR)var1)->this_parish->data)->this_county->data)->this_district->data)->cod;
	const char* districtCode2 = ((DISTRICT_PTR)((COUNTY_PTR)((PARISH_PTR)((REQUESTER_PTR)var2)->this_parish->data)->this_county->data)->this_district->data)->cod;

	int result = strncmp(districtCode1, districtCode2, 2);
	if (result == 0) {
		result = strncmp(countyCode1, countyCode2, 2);
		if (result == 0)
			result = strncmp(code1, code2, 2);
	}
	return result;
}
int checkRequesterById(void* var1, void* var2)
{
	return strcmp(((REQUESTER_PTR)var1)->id, ((REQUESTER_PTR)var2)->id);
}
int checkRequesterByAge(void* var1, void* var2)
{
	return getAge((*(REQUESTER_PTR*)var1)->birth_date) - getAge((*(REQUESTER_PTR*)var1)->birth_date);
}

void printRequester(const void* var) {
	if (!var) return;
	REQUESTER_PTR req = ((const REQUESTER_PTR)var);
	printf("[%s]\t%s\t%s\t%s\n", req->id, req->name, showTimeStampDate(req->birth_date), ((PARISH_PTR)(((NODE_PTR)(req->this_parish))->data))->name);
}

int getOlderRequester()
{
	NODE_PTR node = requesters_list->beginning;
	REQUESTER_PTR olderRequester = NULL, req_aux = NULL;
	int max_age = 0;
	while (node)
	{
		req_aux = (REQUESTER_PTR)(node->data);
		int age = getAge(req_aux->birth_date);
		if (age > max_age)
		{
			max_age = age;
			olderRequester = req_aux;
		}
		node = node->next;
	}
	return max_age;
}
double getRequestersAverageAge()	//média das idades de todos os requisitantes
{
	NODE_PTR node = requesters_list->beginning;
	REQUESTER_PTR requester = NULL;
	double total_age = 0;
	int count = 0;
	while (node)
	{
		requester = (REQUESTER_PTR)(node->data);
		total_age += (double)getAge(requester->birth_date);
		count++;
		node = node->next;
	}
	return total_age / count;
}
int getRequestersOlderThan(int age) {	//pessoas cuja idade é superior a
	NODE_PTR node = requesters_list->beginning;
	int count = 0;
	while (node) {
		count += (getAge(((REQUESTER_PTR)node->data)->birth_date) > age);
		node = node->next;
	}
	return count;
}
int getMostRequesitantsAge() {
	int max_age = getOlderRequester(), i, most_req_age = 0, max = 0;
	int* ages = (int*)myMALLOC((max_age + 1) * sizeof(int));
	if (ages) {
		for (i = 0; i < max_age; i++)
			ages[i] = 0;
		NODE_PTR node = requesters_list->beginning;
		while (node) {
			REQUESTER_PTR req_aux = (REQUESTER_PTR)(node->data);
			ages[getAge(req_aux->birth_date)]++;
			node = node->next;
		}
		for (i = 0; i <= max_age; i++)
			if (ages[i] > max) {
				max = ages[i];
				most_req_age = i;
			}
	}
	free(ages);
	return most_req_age;
}
char* showRequesterRequests(char* full_name) {
	char* msg = NULL;
	STOP_CYCLE = 0;
	if (requests_list)
	{
		if (requests_list->n_el)
		{
			char* file_name = NULL;
			int opt1, opt2;
			do {
				FILE* file = NULL;
				opt1 = getOption("Listar em:\n\t[1]terminal\n\t{2]ficheiro\n\t[0]voltar atrás\nR.:", NULL, 2);
				if (!opt1)
					break;
				if (opt1 == 2) {
					printf("Nome do ficheiro:");
					scanf_j(&file_name);
					opt2 = getOption("Tipo de ficheiro:\n[1] .csv\t[2] .xlsx\t[0] voltar atrás\nR.:", NULL, 2);
					if (!opt2)
						continue;
					file = myFOPEN(true, file_name, opt2 == 1 ? "csv" : "xlxs", "w");
					fprintf(file, "Id\tNome\tISBN\tData de Requisicao\tData de Entrega\n");
				}
				REQUESTER_PTR _req = ((REQUESTER_PTR)searchNode(requesters_list, createEmptyRequester(NULL, full_name, setEmptyTimestamp(), NULL), checkRequesterByName)->data);
				if (_req)
				{
					GEN_LIST_PTR requests = searchRequestsByRequesterId(_req->id, NULL);
					if (requests && requests->n_el) {
						printf("Requisições de %s:\n", full_name);
						NODE_PTR node = requests->beginning;
						while (node)
						{
							printRequest(node->data, file);
							node = node->next;
							if (breakListing(&STOP_CYCLE)) break;
						}
						free(node);
						opt1 = 0;
						if(file) fclose(file);
						msg = (char*)malloc((strlen("Listagem das requisições de  realizada com sucesso!") + strlen(full_name) + 1) * sizeof(char));
						sprintf(msg, "Listagem das requisições de %s realizada com sucesso!", full_name);
					}
					else if (!(requests->n_el))
					{
						msg = (char*)malloc((strlen(" não tem nenhuma requisição ou nome incorreto!") + strlen(full_name) + 1) * sizeof(char));
						sprintf(msg, "%s não tem nenhuma requisição ou nome incorreto!", full_name);
					}
					destroyList(requests, destroyRequest);
				}
				else
				{
					msg = (char*)malloc((strlen(" não tem nenhuma requisição!") + strlen(full_name) + 1) * sizeof(char));
					sprintf(msg, "%s não tem nenhuma requisição ou nome incorreto!", full_name);
				}
			} while (opt1);
		}
		else
			return "Não existem requisições!";
	}
	else
		return "Não existem requisições!";
	return msg;
}
char* showRequestersWithNoRequests() {
	char* msg = NULL;
	if (requests_list->n_el) {
		char* file_name = NULL;
		int opt1, opt2, count=0;
		FILE* file = NULL;
		do {
			opt1 = getOption("Listar em:\n\t[1]terminal\n\t{2]ficheiro\n\t[0]voltar atrás\nR.:", NULL, 2);
			if (!opt1)
				break;
			if (opt1 == 2) {
				printf("Nome do ficheiro:");
				scanf_j(&file_name);
				opt2 = getOption("Tipo de ficheiro:\n[1] .csv\t[2] .xlsx\t[0] voltar atrás\nR.:", NULL, 2);
				if (!opt2)
					continue;
				file = myFOPEN(true, file_name, opt2 == 1 ? "csv" : "xlxs", "w");
				free(file_name);
				fprintf(file, "Id\Nome\Idade\n");
			}
			STOP_CYCLE = 0;
			NODE_PTR requesters_node = requesters_list->beginning;
			REQUESTER_PTR requester = NULL;
			count = 0;
			printf("[9] Mostrar pessoas que nunca requisitaram nada\nRequisitores sem requisições:\n");
			while (requesters_node) {
				requester = requesters_node->data;
				if (!searchRequestsByRequesterId(requester->id, NULL))
				{
					if (file)
						fprintf(file, "%s\n%s\n%d\n", requester->id, requester->name, getAge(requester->birth_date));
					else
						printf("\t[%s] %s com %d anos\n", requester->id, requester->name, getAge(requester->birth_date));
					count++;
				}
				requesters_node = requesters_node->next;
				if (breakListing(&STOP_CYCLE) < 0) break;
			}
			if(file)fclose(file);
			opt1 = 0;
		} while (opt1);
		msg = (count ? "Listagem com sucesso!" : "Todos os requesitantes têm requisições!");
	}
	else
		msg = "Não foram feitas requisições até agora!";
	return msg;
}
char* showRequestersWithRequestedBooks() {
	char* msg = NULL;
	if (requests_list) {
		STOP_CYCLE = 0;
		NODE_PTR requesters_node = requesters_list->beginning;
		NODE_PTR request = NULL;
		int count,opt1,opt2;
		char* file_name = NULL;
		FILE* file = NULL;
		printf("[10] Listar requisitantes com livros requisitados\nRequisitores com livros requisitados:\n");
		do {
			count = 0;
			opt1 = getOption("Listar em:\n\t[1]terminal\n\t{2]ficheiro\n\t[0]voltar atrás\nR.:", NULL, 2);
			if (!opt1)
				break;
			if (opt1 == 2) {
				printf("Nome do ficheiro:");
				scanf_j(&file_name);
				opt2 = getOption("Tipo de ficheiro:\n[1] .csv\t[2] .xlsx\t[0] voltar atrás\nR.:", NULL, 2);
				if (!opt2)
					continue;
				file = myFOPEN(true, file_name, opt2 == 1 ? "csv" : "xlxs", "w");
				fprintf(file, "ISBN\tTitulo\tAutor\tAno de Publicacao\n");
			}
			while (requesters_node) {
				GEN_LIST_PTR new_list = searchRequestsByRequesterId(((REQUESTER_PTR)(requesters_node->data))->id, (void*)true);
				if (new_list)
				{
					request = new_list->beginning;
					printf("\t%s:", ((REQUESTER_PTR)(requesters_node->data))->name);
					while (request) {
						printRequest(request,file);
						count++;
						request = request->next;
					}
					requesters_node = requesters_node->next;
					if (breakListing(&STOP_CYCLE) < 0)
						break;
				}
			}
			msg = (count ? "Listagem completa!" : "Todos os livros foram entregues!");
			if(file)fclose(file);
			opt1 = 0;
		} while (opt1);
	}
	else
		msg = "Não foram feitas requisições até agora!";
	return msg;
}
AUX_PTR mostUsedSurname() {
	NODE_PTR requesters = requesters_list->beginning;
	GEN_LIST_PTR surnames_list = createList();
	AUX_PTR most_used_surname = createAuxVar();

	if (surnames_list || most_used_surname) {

		while (requesters) {
			char* actual_surname = getSurname(((REQUESTER_PTR)(requesters->data))->name);
			NODE_PTR surname_node = surnames_list->beginning;
			bool found = false;

			while (surname_node) {
				AUX_PTR current_aux = (AUX_PTR)(surname_node->data);
				if (!strcmp(current_aux->aux, actual_surname)) {
					current_aux->count++, found = true;
					if (current_aux->count > most_used_surname->count)
						most_used_surname = current_aux;
					break;
				}
				surname_node = surname_node->next;
			}

			if (!found) {
				AUX_PTR new_surname = createAuxVar();
				if (new_surname) {
					new_surname->aux = actual_surname;
					new_surname->count = 1;
					addNodeStart(surnames_list, new_surname);
					if (new_surname->count > most_used_surname->count)
						most_used_surname = new_surname;
				}
			}
			requesters = requesters->next;
		}
	}
	destroyList(surnames_list, destroyAuxPtr);
	return most_used_surname;
}
int requestersNumFromXLocation(char* name, char* loc_str) {
	if (!loc_str || !name)
		return 0;
	int d_count = 0, c_count = 0;
	REQUESTER_PTR requesters_data = createEmptyRequester(NULL, name, setEmptyTimestamp(), NULL), actual_requester = NULL;
	DISTRICT_PTR d_data = NULL;
	COUNTY_PTR c_data = NULL;
	NODE_PTR location = searchNode(districts_list, emptyDistrict(NULL, loc_str), checkDistrictName);
	if (location)
		d_data = (DISTRICT_PTR)(location->data);
	else
	{
		location = searchNode(counties_list, emptyCounty(NULL, loc_str, NULL), checkCountyName);
		if (location)
			c_data = (COUNTY_PTR)(location->data);
	}
	int count = 0;
	NODE_PTR requesters = requesters_list->beginning;
	while (requesters) {
		actual_requester = requesters->data;
		if (!checkRequesterByName(actual_requester, requesters_data))
			if (d_data && !strcmp(((DISTRICT_PTR)(((COUNTY_PTR)(((PARISH_PTR)(actual_requester->this_parish->data))->this_county->data))->this_district->data))->name, d_data->name))
				d_count++;
			else if (c_data && !strcmp(((COUNTY_PTR)(((PARISH_PTR)(actual_requester->this_parish->data))->this_county->data))->name, c_data->name))
				c_count++;
		requesters = requesters->next;
	}

	return (d_data) ? (d_count) : (c_data ? c_count : 0);
}

void destroyRequester(const void* data) {
	if (!data) return;
	REQUESTER_PTR req = ((const REQUESTER_PTR)data);
	free(req->id);
	free(req->name);
	free(req);
}

char* weekDayWZellersCongruence(TIMESTAMP date) {//obter dia da semana com a formula de congruência de zeller's
	char* week[7] = { "segunda","terça" ,"quarta" ,"quinta" ,"sexta" ,"sábado" ,"domingo" };

	if (date.month < 3) {
		date.month += 12;
		date.year--;
	}
	int century = date.year / 100, yearOfCentury = date.year % 100;

	int week_day = (date.day + ((13 * (date.month + 1)) / 5) + yearOfCentury + (yearOfCentury / 4) + (century / 4) - (2 * century)) % 7;

	week_day += (week_day <= 1 ? 5 : -2);// Ajusta o índice do dia da semana para começar de segunda-feira como 0

	return week[week_day];
}