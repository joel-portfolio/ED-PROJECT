#include "./db_management.h"

void* getLineContent(char* line, int type) {
	char* var1 = NULL, * var2 = NULL, * var3 = NULL, * var4 = NULL, * var5 = NULL, * var6 = NULL, * var7 = NULL;
	NODE_PTR node1 = NULL, node2 = NULL, node3 = NULL;
	switch (type)
	{
	case 1: {
		if (sscanf_j(line, "%s\t%s\n", &var1, &var2) == 2)
			return createDistrict(var1, var2);
		return NULL;
	}break;
	case 2: { // CONCELHOS
		if (sscanf_j(line, "%s\t%s\n", &var1, &var2) == 2)
		{
			var3 = (char*)myMALLOC(sizeof(char) * 3);
			var4 = (char*)myMALLOC(sizeof(char) * 3);
			strncpy(var3, var1, 2);
			*(var3 + 2) = '\0';
			strncpy(var4, var1 + 2, 2);
			*(var4 + 2) = '\0';
			node1 = searchNode(districts_list, emptyDistrict(var3, NULL), checkDistrict);
			if (node1)
				return createCounty(var4, var2, node1);
		}
		return NULL;
	}break;
	case 3: { // FREGUESIAS
		if (sscanf_j(line, "%s\t%s\n", &var1, &var2) == 2)
		{
			var3 = (char*)myMALLOC(sizeof(char) * 3);
			var4 = (char*)myMALLOC(sizeof(char) * 3);
			var5 = (char*)myMALLOC(sizeof(char) * 3);
			strncpy(var3, var1, 2);
			*(var3 + 2) = '\0';
			strncpy(var4, var1 + 2, 2);
			*(var4 + 2) = '\0';
			strncpy(var5, var1 + 4, 2);
			*(var5 + 2) = '\0';
			node1 = searchNode(districts_list, emptyDistrict(var3, NULL), checkDistrict);
			if (node1)
			{
				node2 = searchNode(counties_list, emptyCounty(var4, NULL, node1), checkCounty);
				if (node2)
					return createParish(var5, var2, node2);
			}
		}
		return NULL;
	}break;
	case 4: { // REQUESITANTES
		if (sscanf_j(line, "%s\t%s\t%s\t%s\n", &var1, &var2, &var3, &var4) == 4 && strlen(var4) == 6)
		{
			var7 = (char*)myMALLOC(sizeof(char) * 3);
			var6 = (char*)myMALLOC(sizeof(char) * 3);
			var5 = (char*)myMALLOC(sizeof(char) * 3);
			strncpy(var7, var4, 2);
			*(var7 + 2) = '\0';
			strncpy(var6, var4 + 2, 2);
			*(var6 + 2) = '\0';
			strncpy(var5, var4 + 4, 2);
			*(var5 + 2) = '\0';
			node1 = searchNode(districts_list, emptyDistrict(var7, NULL), checkDistrict);
			if (node1)
			{
				node2 = searchNode(counties_list, emptyCounty(var6, NULL, node1), checkCounty);
				if (node2)
				{
					node3 = searchNode(parishes_list, emptyParish(var5, NULL, node2), checkParish);
					if (node3)
						return createRequester(var1, var2, charToTimestamp(var3), node3);
				}
			}
		}
		else
		{
			error_j("FAILED: '%s\t%s\t%s\t%s' - invalid format", var1, var2, var3, var4);
			return NULL;
		}
		return NULL;
	}break;
	case 5:
	{
		if (sscanf_j(line, "%s\t%s\t%s\t%s\t%s\n", &var1, &var2, &var3, &var4, &var5) == 5)
		{
			LIBRARY_NODE_PTR lib_node = library_hash->_groups->beginning;
			while (lib_node) {
				node1 = searchNode(lib_node->books_list, setBook(NULL, NULL, var2, NULL, NULL), checkBookByISBN);
				if (node1)
				{
					node2 = searchNode(requesters_list, createEmptyRequester(var3, NULL, setEmptyTimestamp(), NULL), checkRequesterById);
					if (node2)
					{
						REQUESTER_PTR _requester = (REQUESTER_PTR)(node2->data);
						if (_requester)
							return createRequest(atoi(var1), _requester, node1, charToTimestamp(var4), charToTimestamp(var5));
					}
				}
				lib_node = lib_node->next;
			}
		}
		return NULL;
	}break;
	default:
		break;
	}
	free(var1);
	free(var2);
	free(var3);
	free(var4);
	free(var5);
}
extern void printRequester(const void*);
GEN_LIST_PTR fetchListsData(const char* file_type)
{
	GEN_LIST_PTR list = createList();
	char* line = NULL;
	size_t line_size = 0;
	FILE* file = myFOPEN(false,file_type, "txt", "r+");
	if (file)
	{
		if (list)
		{
			while (getline_j(&line, &line_size, file) != -1)
			{
				if (!strcmp(file_type, FILES[0]))
					addNodePos(list, getLineContent(line, 1), checkDistrict);
				else if (!strcmp(file_type, FILES[1]))
					addNodePos(list, getLineContent(line, 2), checkCounty);
				else if (!strcmp(file_type, FILES[2]))
					addNodePos(list, getLineContent(line, 3), checkParish);
				else if (!strcmp(file_type, FILES[3]))
					addNodePos(list, getLineContent(line, 4), checkRequesterById);
				else if (!strcmp(file_type, FILES[4]))
					addNodePos(list, getLineContent(line, 5), checkRequestById);
				else
				{
					free(line);
					fclose(file);
					return NULL;
				}
			}
			if (line)
				free(line);
		}
		fclose(file);
	}
	return list;
}

LIBRARY_PTR fetchHashingData(const char* file_type)
{
	char* line = NULL, * isbn = NULL, * title = NULL, * author = NULL, * area = NULL, * year = NULL, * requested = NULL;
	size_t line_size = 0;
	LIBRARY_PTR main_lib = NULL;
	LIBRARY_NODE_PTR lib_node = NULL;
	FILE* file = myFOPEN(false, file_type, "txt", "r+");
	if (file)
	{
		main_lib = createLibrary();
		if (main_lib && main_lib->_groups)
		{
			while (getline_j(&line, &line_size, file) != -1)
				if (sscanf_j(line, "%s\t%s\t%s\t%s\t%s\t%s\n", &isbn, &title, &author, &area, &year, &requested) == 6)
				{
					toUppercase(area);
					lib_node = searchArea(main_lib, area);
					if (lib_node)
						addNodePos(lib_node->books_list, setBook(title, author, isbn, year, requested), checkBookByISBN);
					else
					{
						lib_node = createLibraryNode();
						if (lib_node)
						{
							lib_node->key = area;
							addNodePos(lib_node->books_list, setBook(title, author, isbn, year, requested), checkBookByISBN);
							addNodeToGroupByKey(main_lib, lib_node);
						}
						else
							return NULL;
					}
				}
			if (line)
				free(line);
		}
		fclose(file);
	}
	return main_lib;
}

char* saveDataToXML() {
	LIBRARY_NODE_PTR lib_node = library_hash->_groups->beginning;
	char* xml_name = NULL, * file_path = NULL, * text = NULL;
	NODE_PTR d_node = districts_list->beginning, c_node = counties_list->beginning, p_node = parishes_list->beginning, node = NULL;
	FILE* xml_file = NULL;
	struct stat st;
	printf("Nome do ficheiro XML:");
	scanf_j(&xml_name);
	if (!xml_name)
		return;
	if (!strpbrk(xml_name, "\\/:*?\"<>|"))
	{
		xml_file = myFOPEN(true, xml_name, "xml", "wx");
		if (xml_file)
		{
			fprintf(xml_file, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
			fprintf(xml_file, "\n<info>");
			fprintf(xml_file, "\n\t<localidades>");
			while (d_node) {
				DISTRICT_PTR d_data = d_node->data;
				fprintf(xml_file, "\n\t\t<distrito cod=\"%s\" nome=\"%s\">", d_data->cod, d_data->name);
				c_node = counties_list->beginning;
				while (c_node) {
					COUNTY_PTR c_data = c_node->data;
					if (!strcmp(d_data->cod, ((DISTRICT_PTR)(c_data->this_district->data))->cod))
					{
						fprintf(xml_file, "\n\t\t\t<concelho cod=\"%s\" nome=\"%s\">", c_data->cod, c_data->name);
						p_node = parishes_list->beginning;
						while (p_node)
						{
							PARISH_PTR p_data = p_node->data;
							if (!strcmp(c_data->cod, ((COUNTY_PTR)(p_data->this_county->data))->cod)) fprintf(xml_file, "\n\t\t\t\t<freguesia cod=\"%s\" nome=\"%s\"/>", p_data->cod, p_data->name);
							p_node = p_node->next;
						}
						fprintf(xml_file, "\n\t\t\t</concelho>");
					}
					c_node = c_node->next;
				}
				fprintf(xml_file, "\n\t\t</distrito>");
				d_node = d_node->next;
			}
			fprintf(xml_file, "\n\t</localidades>");

			node = requesters_list->beginning;
			fprintf(xml_file, "\n\t<requesitantes>");
			while (node->next) {
				REQUESTER_PTR data = node->data;
				fprintf(xml_file, "\n\t\t<requisitante id=\"%s\" cod_localidade=\"%s\">\n\t\t\t<nome>%s</nome>\n\t\t\t<data_de_nascimento>%s</data_de_nascimento>\n\t\t</requisitante>", data->id, getParishFullCode((PARISH_PTR)(data->this_parish->data)), data->name, showTimeStampDate(data->birth_date));
				node = node->next;
			}
			fprintf(xml_file, "\n\t</requesitantes>");
			if (requests_list)
			{
				node = requests_list->beginning;
				fprintf(xml_file, "\n\t<requisicoes>");
				while (node) {
					REQUEST_PTR data = node->data;
					fprintf(xml_file, "\n\t\t<requisicao>\n\t\t\t<id>%d</id>\n\t\t\t<titulo>%s</titulo>\n\t\t\t<requisitante>%s</requisitante>\n\t\t\t<data_de_requisicao>%s</data_de_requisicao>\n\t\t\t<data_de_entrega>%s</data_de_entrega>\n\t\t</requisicao>", data->id, ((BOOK_PTR)(data->book->data))->title, data->requester->name, showTimeStampDate(data->request_date), showTimeStampDate(data->delivery_date));
					node = node->next;
				}
				fprintf(xml_file, "\n\t</requisicoes>");
			}
			if (lib_node)
			{
				fprintf(xml_file, "\n\t<livros>");
				while (lib_node) {
					fprintf(xml_file, "\n\t\t<area>%s</area>", lib_node->key);
					NODE_PTR book = lib_node->books_list->beginning;
					while (book) {
						BOOK_PTR book_data = book->data;
						fprintf(xml_file, "\n\t\t\t<livro>\n\t\t\t\t<ISBN>%s</ISBN>\n\t\t\t\t<titulo>%s</titulo>\n\t\t\t\t<autor>%s</autor>\n\t\t\t\t<ano_de_publicacao>%d</ano_de_publicacao>\n\t\t\t\t<requisitado>%s</requisitado>\n\t\t\t</livro>", book_data->ISBN, book_data->title, book_data->author, book_data->publy_year, (book_data->requested ? "sim" : "não"));
						book = book->next;
					}
					lib_node = lib_node->next;
				}
				fprintf(xml_file, "\n\t</livros>");
			}
			fprintf(xml_file, "\n</info>");
			text = (char*)myMALLOC(sizeof(char) * (strlen(xml_name) + strlen(".xml criado!")) + 1);
			sprintf(text, "%s.xml criado!", xml_name);
			free(xml_name);
			fclose(xml_file);
			return text;
		}
		else
		{
			text = (char*)myMALLOC(sizeof(char) * (strlen(xml_name) + strlen(".xml já existe!")) + 1);
			sprintf(text, "%s.xml já existe!", xml_name);
			free(xml_name);
			return text;
		}
	}
	else
		return "Nome do ficheiro não pode conter os seguintes símbolos[ / : ¦ < > * ? \" ]";
	free(xml_name);
}

void printfXMLStruct(int type, void* data) {
	char* text = NULL;
	switch (type) {
	case 0://distrito
		sprintf(text, "\n\t\t\t<cod>%s</cod>\n\t\t\t<nome>%s</nome>", ((DISTRICT_PTR)data)->cod, ((DISTRICT_PTR)data)->name);
		break;
	case 1://concelho
		break;
	case 2://freguesia
		sprintf(text, "<freguesia cod=\"%s\" name=\"%s\"/>", ((PARISH_PTR)data)->cod, ((PARISH_PTR)data)->name);
		break;
	case 3:break;
	case 4:break;
	case 5:break;
	default:break;
	}
}

void storeListGen(GEN_LIST_PTR L, const char* type)
{
	//			LIVROS			REQUESITANTES			REQUISIÇÕES
	NODE_PTR aux_node = L->beginning;
	FILE* file = myFOPEN(false, type, "txt", "w");
	int count = 1;
	if (file)
	{
		while (aux_node)
		{
			if (!strcmp(type, FILES[3]))//REQUESITANTES
			{
				REQUESTER_PTR aux_data = (REQUESTER_PTR)(aux_node->data);
				if (fprintf(file, "%s\t%s\t%s\t%s\n", aux_data->id, aux_data->name, showTimeStampDate(aux_data->birth_date), getParishFullCode((PARISH_PTR)(aux_data->this_parish->data))) <= 0)
					perror("Error writing to file"), count = 0;
			}
			else if (!strcmp(type, FILES[4])) // REQUISIÇÕES
			{
				REQUEST_PTR aux_data = ((REQUEST_PTR)(aux_node->data));
				int id = aux_data->id;
				char* ISBN = ((BOOK_PTR)(aux_data->book->data))->ISBN;
				char* _req_id = aux_data->requester->id;
				char* req_date = showTimeStampDate(aux_data->request_date);
				char* del_date = showTimeStampDate(aux_data->delivery_date);
				if (fprintf(file, "%d\t%s\t%s\t%s\t%s\n", aux_data->id, ((BOOK_PTR)(aux_data->book->data))->ISBN, aux_data->requester->id, showTimeStampDate(aux_data->request_date), showTimeStampDate(aux_data->delivery_date)) <= 0)
					perror("Error writing to file"), count = 0;
			}
			aux_node = aux_node->next;
		}
		fclose(file);
	}
}

void storeHash(LIBRARY_PTR lib) {
	LIBRARY_NODE_PTR groups = lib->_groups->beginning;
	FILE* file = myFOPEN(false, FILES[5], "txt", "w");
	int count = 1;
	if (file)
	{
		while (groups) {
			NODE_PTR books = groups->books_list->beginning;
			while (books) {
				BOOK_PTR aux_data = (BOOK_PTR)books->data;
				if (fprintf(file, "%s\t%s\t%s\t%s\t%d\t%s\n", aux_data->ISBN, aux_data->title, aux_data->author, groups->key, aux_data->publy_year, ((aux_data->requested) ? "1" : "0")) <= 0)
				{
					perror("Error writing to file");
					count = 0;
				}
				books = books->next;
			}
			groups = groups->next;
		}
		fclose(file);
	}
	else
		return;
}

//int storeLibrary() {
//	LIBRARY_NODE_PTR aux = library_hash->_groups->beginning;
//	NODE_PTR aux2 = NULL;
//	BOOK_PTR book_aux = NULL;
//	int count1 = 0, count2 = 0;
//	if (aux) {
//		FILE* file = myFOPEN(filePath("library"), "w");
//		if (file) {
//			while (aux) {
//				aux2 = aux->books_list->beginning;
//				while (aux2) {
//					book_aux = (aux2->data);
//					count2 += fprintf(file, "%s - %s by %s | %s %d;\n", book_aux->ISBN, book_aux->title, book_aux->author, aux->key, book_aux->publy_year);
//					aux2 = aux2->next;
//				}
//				aux = aux->next;
//				count1++;
//			}
//		}
//	}
//	return (count2 / count1) == 5;
//}
//
//int storeRequesters() {
//	NODE_PTR aux_node = requesters_list->beginning;
//	FILE* file = myFOPEN(filePath(FILES[3]), "w");
//	int count = 1;
//	if (file)
//	{
//		while (aux_node)
//		{
//			if (fprintf(file, "%s\t%s\t%s\t%s\n", ((REQUESTER_PTR)aux_node->data)->id, ((REQUESTER_PTR)aux_node->data)->name, showTimeStampDate(((REQUESTER_PTR)aux_node->data)->birth_date), getParishFullCode((PARISH_PTR)(((REQUESTER_PTR)aux_node->data)->this_parish->data))) != 4)
//				count = 0;
//			aux_node = aux_node->next;
//		}
//		fclose(file);
//	}
//	return count;
//}