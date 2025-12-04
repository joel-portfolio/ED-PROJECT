#include "./requests.h"

void requestsMenu()
{
	int opt, c, var1;
	NODE_PTR node1 = NULL, node2 = NULL;
	char* msg = NULL;
	do {
		opt = -1, var1 = -1;
		char* str1 = NULL, * str2 = NULL;
		opt = getOption("\nREQUISIÇÕES\n\t[1] Requisitar livro\n\t[2] Devolver livro\n\t[3] Listar livros requisitados\n\t[4] Salvar Requisições\n\t[0] TRÁS\nR.:", msg, 4);
		msg = NULL;
		switch (opt)
		{
		case 1:
		{
			msg = requestBook();

		}
		break;
		case 2:
		{
			printf("Nome de requisitante:");
			scanf_j(&str1);
			if (str1)
			{
				node1 = searchNode(requesters_list, createEmptyRequester(NULL, str1, setEmptyTimestamp(), NULL), checkRequesterByName);
				if (node1)
				{
					GEN_LIST_PTR req_books = searchRequestsByRequesterId(getRequesterId(node1->data), (void*)true);
					printf("ISBN do livro a devolver:");
					scanf_j(&str1);
					if (str1)
						if (bookExist(str1))
						{
							BOOK_PTR _book = setBook(NULL, NULL, str1, NULL, NULL);
							LIBRARY_NODE_PTR _lib_node = library_hash->_groups->beginning;
							while (_lib_node) {
								node2 = searchNode(_lib_node->books_list, _book, checkBookByISBN);
								if (node2)
									break;
								_lib_node = _lib_node->next;
							}
							REQUEST_PTR _req = createRequest(0, NULL, node2, setEmptyTimestamp(), setEmptyTimestamp());
							node1 = searchNode(req_books, _req, checkRequestByISBN);
							if (node1) {
								setRequestDelivery((REQUEST_PTR)(node1->data));
								msg = "Livro entregue!";
							}
							else msg = "Livro não entregue!";
						}
						else msg = "Livro não existe!";
					else msg = "ISBN vazio!";
				}
				else msg = "Nome incorreto!";
			}
			else msg = "Nome vazio!";
		}
		break;
		case 3://PERGUNTAR SE QUER LISTAR OU ESCREVER
		{
			var1 = getOption("BASIC or COMPLEX list?\n[1] BASIC\t[2] COMPLEX\t[0] Atrás\n", NULL, 2);
			if (var1 == 1)
				basicShowRequestedBooks();
			else if (var1 == 2)
				msg = complexShowRequestedBooks();
			else break;
			msg = "Livros requisitados listados com sucesso!";
		}
		break;
		case 4:
		{
			storeListGen(requests_list, FILES[4]);
			msg = "Requisições guardadas com sucesso!";
		}
		break;
		}
	} while (opt);
}

REQUEST_PTR createRequest(int id, REQUESTER_PTR requester, NODE_PTR book, TIMESTAMP req_date, TIMESTAMP deli_date) {
	REQUEST_PTR request = (REQUEST_PTR)myMALLOC(sizeof(REQUEST));
	if (request) {
		request->id = (id ? id : getNewId());
		request->requester = requester;
		request->book = book;
		request->request_date = (timestampIsValid(req_date) ? req_date : setTimestamp());
		request->delivery_date = (timestampIsValid(deli_date) ? deli_date : setEmptyTimestamp());
		if (book->data)
			((BOOK_PTR)(request->book->data))->requested = timestampIsValid(request->delivery_date);
	}
	return request;
}
char* requestBook() {
	NODE_PTR node1 = NULL, book = NULL;
	LIBRARY_NODE_PTR lib_node = library_hash->_groups->beginning;
	char* req_name = NULL, * book_data = NULL, * text = NULL, opt = ' ';
	printf("Nome Completo do requisitante:");
	scanf_j(&req_name);
	if (!req_name)
		return "Nome vazio!";
	node1 = searchNode(requesters_list, createEmptyRequester(NULL, req_name, setEmptyTimestamp(), NULL), checkRequesterByFullName);
	if (node1)
	{
		printf("ISBN/Titulo:");
		scanf_j(&book_data);
		if (!book_data)
			return "Informação vazia!";
		while (lib_node)
		{
			book = searchNode(lib_node->books_list, setBook(book_data, NULL, book_data, NULL, NULL), checkBookByTitleISBN);
			if (book)
			{
				addNodePos(requests_list, createRequest(NULL, ((REQUESTER_PTR)(node1->data)), book, setTimestamp(), setEmptyTimestamp()), checkRequestById);
				text = (char*)myMALLOC(sizeof(char) * (strlen("Livro  requisitado com sucesso!") + strlen(book_data) + 1));
				sprintf(text, "Livro %s requisitado com sucesso!", book_data);
			}
			lib_node = lib_node->next;
		}
		if (!book)
		{
			text = (char*)myMALLOC(sizeof(char) * (strlen("Informação introduzida é inválida!") + 1));
			sprintf(text, "Informação introduzida é inválida!");
		}
	}
	else
	{
		text = (char*)myMALLOC(sizeof(char) * (strlen("Nome inválido!") + 1));
		sprintf(text, "Nome inválido!");
	}
	free(req_name);
	free(book_data);
	return text;
}
void setRequestDelivery(REQUEST_PTR request) {
	if (request)
	{
		request->delivery_date = setTimestamp();
		((BOOK_PTR)(request->book->data))->requested = false;
	}
}

int checkRequestById(void* var1, void* var2)
{
	return ((REQUEST_PTR)var2)->id - ((REQUEST_PTR)var1)->id;
}
int getNewId() {
	NODE_PTR node = NULL;
	if (requests_list) {
		node = requests_list->beginning;
		while (node)
			node = node->next;
	}
	return node ? (((REQUEST_PTR)(node->data))->id + 1) : (1);
}

GEN_LIST_PTR searchRequestsByRequesterId(char* id, void* wBookReq) {//wBookReq -> with books requested
	if (!id || !requests_list->n_el)
		return NULL;
	NODE_PTR aux = requests_list->beginning;
	GEN_LIST_PTR requests = createList();
	int count = 0;
	while (aux)
	{
		if (!strcmp(((REQUEST_PTR)(aux->data))->requester->id, id))
		{
			addNodeStart(requests, aux->data);
			count++;
		}
		aux = aux->next;
	}
	return count > 0 ? requests : NULL;
}

int checkRequestByISBN(void* var1, void* var2) {
	REQUEST_PTR req1 = (REQUEST_PTR)var1, req2 = (REQUEST_PTR)var2;
	BOOK_PTR book1 = ((BOOK_PTR)(req1->book->data)), book2 = ((BOOK_PTR)(req2->book->data));
	return strcmp(book1->ISBN, book2->ISBN);
}

void basicShowRequestedBooks() {
	if (library_hash->_groups->n_el) {
		int opt1 = -1, opt2 = -1, count;
		FILE* file = NULL;
		char* file_name = NULL;
		do {
			count = 0;
			LIBRARY_NODE_PTR lib_node = library_hash->_groups->beginning;
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
			while (lib_node) {
				NODE_PTR node = lib_node->books_list->beginning;
				while (node) {
					BOOK_PTR _book = (BOOK_PTR)(node->data);
					if (_book->requested)
						listBook(node->data, file);
					node = node->next;
					if (breakListing(&count)) break;
				}
				lib_node = lib_node->next;
			}
			opt1 = 0;
			if(file)fclose(file);
			wait_j();
		} while (opt1);
	}
	else
		return "Livros requisitados listados sem sucesso!";
}
char* complexShowRequestedBooks() {
	if (requests_list->n_el && library_hash->_groups->n_el) {
		int count, c;
		char* file_name = NULL;
		int opt1, opt2;
		FILE* file = NULL;
		NODE_PTR group_node = NULL, next_node = NULL;
		LIBRARY_NODE_PTR aux_lib_node = NULL, _library_node = library_hash->_groups->beginning, next_lib_node = NULL;
		LIBRARY_PTR aux_hash = createLibrary();
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
			while (_library_node) {
				aux_lib_node = createLibraryNode();
				aux_lib_node->key = (char*)myMALLOC(sizeof(char) * (strlen(_library_node->key) + 1));
				strcpy(aux_lib_node->key, _library_node->key);
				aux_lib_node->books_list = createList();
				group_node = _library_node->books_list->beginning;
				while (group_node) {
					BOOK_PTR book = ((BOOK_PTR)(group_node->data));
					if (book->requested)
						addNodeStart(aux_lib_node->books_list, group_node->data);
					group_node = group_node->next;
				}
				if (aux_lib_node->books_list->n_el > 0)
					addNodeToGroupByKey(aux_hash, aux_lib_node);
				_library_node = _library_node->next;
			}
			aux_lib_node = aux_hash->_groups->beginning;
			while (aux_lib_node) {
				next_lib_node = aux_lib_node->next;
				system("cls");
				printf("\n%s:\n", aux_lib_node->key);
				free(aux_lib_node->key);
				NODE_PTR node = aux_lib_node->books_list->beginning;
				int count = 0;
				while (node) {
					BOOK_PTR _book = (BOOK_PTR)node->data;
					REQUEST_PTR _req = ((REQUEST_PTR)(searchNode(requests_list, _book, checkRequestByISBN)->data));
					if (_req)
						printRequest(_req, file);
					node = node->next;
					if (breakListing(&count)) break;
				}
				group_node = aux_lib_node->books_list->beginning;
				while (group_node) {
					next_node = group_node->next;
					group_node->data = NULL;
					group_node->next = NULL;
					group_node = next_node;
				}
				free(aux_lib_node->books_list);
				free(aux_lib_node->next);
				aux_lib_node = next_lib_node;
			}
			free(aux_hash);
			wait_j();
		} while (opt1);
	}
	else
		return "Livros requisitados listados sem sucesso!";
}

void printRequest(const void* var, FILE* file) {
	REQUEST_PTR req = ((REQUEST_PTR)(var));
	if (file)
		fprintf("%d\t%s\t%s\t%s\t%s\n", req->id, req->requester->name, ((BOOK_PTR)(req->book->data))->ISBN, showTimeStampDate(req->request_date), showTimeStampDate(req->delivery_date));
	else printf("\t%s[%s] requisitado por %s no dia %s\n", ((BOOK_PTR)(req->book->data))->title, ((BOOK_PTR)(req->book->data))->ISBN, req->requester->name, showTimeStampDate(req->request_date));
}

void destroyRequest(void* var) {
	destroyBook(((REQUEST_PTR)(var))->book->data);
	destroyRequester(((REQUEST_PTR)(var))->requester);
	free(((REQUEST_PTR)(var)));
}