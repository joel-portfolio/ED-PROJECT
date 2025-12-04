#include "./book.h"

void booksMenu()
{
	int opt, c;
	char* msg = NULL;
	do {
		char* str = NULL;
		AUX_PTR aux = NULL;
		opt = getOption("\nLIVROS\n\t[1] Adicionar livro  \n\t[2] Listar livros\n\t[3] Área com mais livros\n\t[4] Mostrar livro\n\t[5] Livros recentes\n\t[6] Livro mais requisitado\n\t[7] Área mais requisitada\n\t[8] Salvar informacoes dos livros\n\t[0] TRÁS\nR.:", msg, 8);
		msg = NULL;
		switch (opt)
		{
		case 1:
		{
			msg = addNewBook();
		}
		break;
		case 2://PERGUNTAR SE QUER LISTAR OU ESCREVER
		{
			printHash(library_hash, listBook);
			msg = "Listagem Completa!";
		}
		break;
		case 3:
		{
			aux = createAuxVar();
			areaWMostBooks(aux);
			printf("%s tem %d livros sendo assim a área com mais livros.", aux->aux, aux->count);
			free(aux);
			wait_j();
		}
		break;
		case 4:
		{
			printf("ISBN: ");
			scanf_j(&str);
			if (str)
			{
				showBook(str);
				free(str);
				wait_j();
			}
			else
				msg = "ISBN inválido!";
		}
		break;
		case 5:
		{
			printList(showRecentBooks(), listBook);
			msg = "Listagem dos livros mais recentes completa!";
			wait_j();
		}
		break;
		case 6:
		{
			aux = showMostRequestedBook();
			if (aux->count && aux->aux)
			{
				printf("O livro ""%s"" tem %d requisições, sendo assim o livro mais requisitado.", aux->aux, aux->count);
				wait_j();
			}
			else msg = "Não existem requisições!";
		}
		break;
		case 7:
		{
			aux = showMostRequestedBook();
			if (aux->count && aux->aux)
			{
				aux = showMostRequestedArea();
				printf("A área ""%s"" tem %d requisições, sendo assim a área mais requisitada.", aux->aux, aux->count);
				wait_j();
			}
			else msg = "Não existem requisições!";
		}
		break;
		case 8:
		{
			storeHash(library_hash);
			msg = "Livros guardados com sucesso!";
		}
		break;
		}
	} while (opt);
}

int checkBookByISBN(void* var1, void* var2)
{
	return strcmp(((BOOK_PTR)(var1))->ISBN, ((BOOK_PTR)(var2))->ISBN);
}
int checkBookByTitle(void* var1, void* var2) {
	return strcmp(((BOOK_PTR)var1)->title, ((BOOK_PTR)var2)->title);
}
int checkBookByTitleISBN(void* var1, void* var2) {
	return checkBookByISBN(var1, var2) && checkBookByTitle(var1, var2);
}
int bookExist(char* var) {
	LIBRARY_NODE_PTR books_list = library_hash->_groups->beginning;
	while (books_list) {
		if (searchNode(books_list->books_list, setBook(NULL, NULL, var, NULL, NULL), checkBookByISBN))
			return 1;
		books_list = books_list->next;
	}
	return 0;
}

BOOK_PTR setBook(char* title, char* author, char* ISBN, char* publy_year, char* requested)
{
	BOOK_PTR book = (BOOK_PTR)myMALLOC(sizeof(BOOK));
	if (book)
	{
		book->title = title;
		book->author = author;
		book->ISBN = ISBN;
		book->publy_year = publy_year ? atoi(publy_year) : 0;
		book->requested = (requested) ? (!strcmp(requested, "1") ? true : false) : (false);
	}
	return book;
}

void listBook(void* var, FILE* file) {
	BOOK_PTR book = (BOOK_PTR)var;
	if (file)
		fprintf(file, "%s\t%s\t%s\t%s\n", book->ISBN, book->title, book->author, book->publy_year);
	else printf("\t[%s] - %s publicado por %s em %d\n", book->ISBN, book->title, book->author, book->publy_year);
}

char* addNewBook()
{
	char* ISBN = NULL, * title = NULL, * author = NULL, * area = NULL, * text = NULL, * publy_year = NULL;
	bool no_spaces = true, added = false;
	LIBRARY_NODE_PTR list_lib = NULL;

	printf("Informação do Livro:\n\tISBN:");
	scanf_j(&ISBN);
	if (!ISBN)
		return "ISBN inválido!";
	for (int i = 0; (*(ISBN + i) != '\0'); i++)
		if (*(ISBN + i) == ' ')
		{
			no_spaces = false;
			break;
		}
	if (bookExist(ISBN) && strlen(ISBN) <= 0 && !no_spaces)
		return "ISBN inválido!";
	printf("\n\tTítulo:");
	scanf_j(&title);
	if (!title)
		return "Título Vazio!";
	printf("\n\tAutor:");
	scanf_j(&author);
	if (!author)
		return "Autor Vazio!";
	printf("\n\tAno de publicação:");
	scanf_j(&publy_year);
	if (!publy_year)
		return "Ano Vazio!";
	if (atoi(publy_year) <= 1500 && atoi(publy_year) > setTimestamp().year)
		return "Ano inválido!";
	printf("\n\tÁrea:");
	scanf_j(&area);
	if (!area)
		return "Área vazia!";
	toUppercase(area);
	list_lib = searchArea(library_hash, area);
	if (list_lib)
		added = addNodePos(list_lib->books_list, setBook(title, author, ISBN, publy_year, "0"), checkBookByISBN);
	else
	{
		list_lib = createLibraryNode();
		list_lib->key = area;
		added = addNodePos(list_lib->books_list, setBook(title, author, ISBN, publy_year, "0"), checkBookByISBN) && addNodeToGroupByKey(library_hash, list_lib);
	}
	if (added)
	{
		text = (char*)myMALLOC(sizeof(char) * (strlen(title) + strlen("'' adicionado com sucesso!") + 1));
		sprintf(text, "'%s' adicionado com sucesso!", title);
	}
	else {
		text = (char*)myMALLOC(sizeof(char) * (strlen(title) + strlen("'' adicionado sem sucesso!") + 1));
		sprintf(text, "'%s' adicionado sem sucesso!", title);
	}
	return text;
}

void areaWMostBooks(AUX_PTR aux) {
	LIBRARY_NODE_PTR lib_node = library_hash->_groups->beginning;
	while (lib_node) {
		if (lib_node->books_list->n_el > aux->count)
		{
			aux->count = lib_node->books_list->n_el;
			aux->aux = lib_node->key;
		}
		lib_node = lib_node->next;
	}
}

void showBook(char* ISBN)
{
	if (ISBN)
	{
		LIBRARY_NODE_PTR lib_node = library_hash->_groups->beginning;
		while (lib_node) {
			NODE_PTR node = lib_node->books_list->beginning;
			while (node) {
				if (!strcmp(((BOOK_PTR)(node->data))->ISBN, ISBN))
				{
					listBook(node->data,NULL);
					return;
				}
				node = node->next;
			}
			lib_node = lib_node->next;
		}
	}
}

char* showBookISBN(void* var) {
	return ((BOOK_PTR)(var))->ISBN;
}

int mostRecentYear() {
	int aux1 = 0, recent_year = 0;
	LIBRARY_NODE_PTR lib_node = library_hash->_groups->beginning;
	while (lib_node) {
		NODE_PTR node = lib_node->books_list->beginning;
		while (node) {
			aux1 = ((BOOK_PTR)(node->data))->publy_year;
			if (aux1 > recent_year)
				recent_year = aux1;
			node = node->next;
		}
		lib_node = lib_node->next;
	}
	return recent_year;
}

GEN_LIST_PTR showRecentBooks() {
	int aux1 = 0, recent_year = mostRecentYear();
	GEN_LIST_PTR recent_books = createList();
	NODE_PTR aux_node = NULL;
	BOOK_PTR book_node = NULL;
	LIBRARY_NODE_PTR lib_node = library_hash->_groups->beginning;
	while (lib_node) {
		aux_node = lib_node->books_list->beginning;
		while (aux_node) {
			book_node = (BOOK_PTR)(aux_node->data);
			if (book_node->publy_year == recent_year)
				addNodeStart(recent_books, book_node);
			aux_node = aux_node->next;
		}
		lib_node = lib_node->next;
	}
	return recent_books;
}

AUX_PTR showMostRequestedBook() {
	if (requests_list) {
		NODE_PTR node = requests_list->beginning, node2 = NULL;
		GEN_LIST_PTR aux_list = createList();
		NODE_PTR aux_node = NULL;
		AUX_PTR aux = createAuxVar();
		while (node) {
			BOOK_PTR book = node->data;
			aux->aux = book->ISBN;
			node2 = searchNode(aux_list, aux, checkAuxPtr);
			if (node2)
				((AUX_PTR)(node2->data))->count++;
			else
				addNodeStart(aux_list, aux);
			node = node->next;
		}
		AUX_PTR var1 = createAuxVar(), var2 = NULL;
		aux_node = aux_list->beginning;
		while (aux_node) {
			var2 = aux_node->data;
			if (var2->count > var1->count)
			{
				var1->count = var2->count;
				var1->aux = var2->aux;
			}
			aux_node = aux_node->next;
		}
		return var1;
	}
}

AUX_PTR showMostRequestedArea() {
	LIBRARY_NODE_PTR node = library_hash->_groups->beginning;
	GEN_LIST_PTR aux_book_list = createList();
	NODE_PTR book_nodes = NULL;
	AUX_PTR aux_node = NULL;
	while (node) {
		aux_node = createAuxVar();
		aux_node->aux = node->key;
		book_nodes = node->books_list->beginning;
		while (book_nodes) {
			aux_node->count += (((BOOK_PTR)(book_nodes->data))->requested ? 1 : 0);
			book_nodes = book_nodes->next;
		}
		addNodeStart(aux_book_list, aux_node);
		node = node->next;
	}
	AUX_PTR var1 = createAuxVar(), var2 = NULL;
	book_nodes = aux_book_list->beginning;
	while (book_nodes) {
		var2 = ((AUX_PTR)(book_nodes->data));
		if (var2->count > var1->count)
		{
			var1->count = var2->count;
			var1->aux = var2->aux;
		}
		book_nodes = book_nodes->next;
	}
	return var1;
}

void destroyBook(void* var)
{
	free(((BOOK_PTR)(var))->author);
	free(((BOOK_PTR)(var))->ISBN);
	free(((BOOK_PTR)(var))->title);
	free(((BOOK_PTR)(var)));
}