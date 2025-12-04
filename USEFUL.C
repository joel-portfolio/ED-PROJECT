#include "./useful.h"

#define PRINT_COUNT 20

TIMESTAMP setEmptyTimestamp() {
	TIMESTAMP timestamp;
	timestamp.day = 0;
	timestamp.month = 0;
	timestamp.year = 0;
	timestamp.hour = 0;
	timestamp.minute = 0;
	timestamp.seconds = 0;
	timestamp.mili_sec = 0;
	return timestamp;
}
TIMESTAMP setTimestamp()
{
	SYSTEMTIME st;

	GetSystemTime(&st);

	TIMESTAMP var;

	var.day = st.wDay;
	var.month = st.wMonth;
	var.year = st.wYear;
	var.hour = st.wHour;
	var.minute = st.wMinute;
	var.seconds = st.wSecond;
	var.mili_sec = st.wMilliseconds;

	return var;
}
TIMESTAMP charToTimestamp(char* var) {
	char* d = NULL, * m = NULL, * y = NULL;
	TIMESTAMP date = setEmptyTimestamp();
	if (sscanf_j(var, "%s-%s-%s", &d, &m, &y) == 3 && ((strlen(d) == strlen(m)) && strlen(d) == 2 && strlen(y)))
	{
		date.day = atoi(d);
		date.month = atoi(m);
		date.year = atoi(y);
	}
	return date;
}
char* showTimeStampDate(TIMESTAMP date) {
	char* result = (char*)myMALLOC(sizeof(char) * 11);
	if (!result)
		return NULL;
	snprintf(result, 11, "%02d-%02d-%04d", date.day, date.month, date.year);
	return result;
}
char* showErrorTimeStamp(TIMESTAMP date) {
	char* result = (char*)myMALLOC(sizeof(char) * 20);
	if (!result)
		return NULL;
	snprintf(result, 28, "[%02d/%02d/%04d-%02d:%02d:%02d.%03d]", date.day, date.month, date.year, date.hour, date.minute, date.seconds, date.mili_sec);
	return result;
}
bool leapYear(int year) {
	return (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);
}
bool thirtyDays(int month) {
	return month == 4 || month == 6 || month == 9 || month == 11;
}
bool timestampIsValid(TIMESTAMP _timestamp) {
	TIMESTAMP actual_time = setTimestamp();
	bool valid_date = (_timestamp.day > 0 &&
		(
			(_timestamp.month == 2 && _timestamp.day <= (leapYear(_timestamp.year) ? 29 : 28)) ||
			(thirtyDays(_timestamp.month) && _timestamp.day <= 30) ||
			(!thirtyDays(_timestamp.month) && _timestamp.day <= 31)
			)
		) &&
		(_timestamp.month > 0 && _timestamp.month <= 12) &&
		(_timestamp.year > 1500 && _timestamp.year <= actual_time.year);

	if (_timestamp.month == 2)
		valid_date = valid_date && (_timestamp.day <= (leapYear(_timestamp.year) ? 29 : 28));
	else if (_timestamp.month == 4 || _timestamp.month == 6 || _timestamp.month == 9 || _timestamp.month == 11)
		valid_date = valid_date && (_timestamp.day <= 30);
	else
		valid_date = valid_date && (_timestamp.day <= 31);

	bool valid_time = (_timestamp.hour >= 0 && _timestamp.hour < 24) &&
		(_timestamp.minute >= 0 && _timestamp.minute < 60) &&
		(_timestamp.seconds >= 0 && _timestamp.seconds < 60) &&
		(_timestamp.mili_sec >= 0 && _timestamp.mili_sec < 1000);

	bool not_late_date = (_timestamp.year < actual_time.year) ||
		(_timestamp.year == actual_time.year && _timestamp.month < actual_time.month) ||
		(_timestamp.year == actual_time.year && _timestamp.month == actual_time.month && _timestamp.day <= actual_time.day);
	return valid_date && valid_time && not_late_date;
}


void* myMALLOC(size_t size) {
	void* ptr = malloc(size);
	if (!ptr)
		perror("Error: Memory allocation failed");
	return ptr;
}
char* filePath(const char* file)
{

}
FILE* myFOPEN(bool default_path, char* file_name, char* file_type, char* mode)
{
	if (file_name && file_type && mode)
	{
		char* file_path = NULL, * folder_path = (char*)myMALLOC(sizeof(char) * 260);
		BROWSEINFO bi = { 0 };
		if (default_path) {
			bi.lpszTitle = L"Selecione uma pasta";
			LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
			if (pidl)
			{
				if (SHGetPathFromIDListW(pidl, folder_path))
				{
					size_t length = strlen(folder_path) + strlen("\\.") + strlen(file_name) + strlen(file_type) + 1;
					file_path = (char*)myMALLOC(sizeof(char) * length);
					if (file_path)
						sprintf_s(file_path, length, "%s\\%s.%s", folder_path, file_name, file_type);
					FILE* file = fopen(file_path, mode);
					if (!file)
						perror("Error: Failed to open file");
					free(folder_path);
					CoTaskMemFree(pidl);
					return file;
				}
				else
					printf("Erro ao obter o caminho da pasta.\n");
			}
			else printf("O utilizador cancelou a operação.\n");
		}
		else {
			strcpy(folder_path, ".");
			size_t length = strlen(".\\.") + strlen(file_name) + strlen(file_type) + 1;
			file_path = (char*)myMALLOC(sizeof(char) * length);
			if (file_path)
				sprintf_s(file_path, length, ".\\%s.%s", file_name, file_type);
			FILE* file = fopen(file_path, mode);
			if (!file)
				perror("Error: Failed to open file");
			free(folder_path);
			return file;
		}
	}return NULL;
}

int breakListing(int* count) {
	char opt = ' ';
	if (*count >= PRINT_COUNT) {
		*count = 0;
		do {
			printf("\n[0] sair\t[1] continuar...\n");
			scanf(" %c", &opt);
		} while (opt != '0' && opt != '1');
		system("cls");
	}
	(*count)++;
	return (opt == '0');
}

int fileExists(const char* filename) {
	DWORD fileAttributes = GetFileAttributes(filename);
	return (fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
}

AUX_PTR createAuxVar() {
	AUX_PTR aux_var = (AUX_PTR)myMALLOC(sizeof(AUX));
	if (aux_var) {
		aux_var->count = 0;
		aux_var->aux = NULL;
	}
	return aux_var;
}
void destroyAuxPtr(void* aux_ptr) {
	free(((AUX_PTR)(aux_ptr))->aux);
	free(((AUX_PTR)(aux_ptr)));
}
int checkAuxPtr(void* var1, void* var2) {
	return strcmp((*(AUX_PTR*)var1)->aux, (*(AUX_PTR*)var2)->aux);
}

void mainScanf_j(char** var) {
	getchar();
	scanf_j(var);
}

int sscanf_j(const char* line, const char* format, ...) {
	int variables_read = 0;
	size_t length;
	char** output = NULL;
	const char* ptr = format, * start = NULL;

	va_list args;
	va_start(args, format);

	while (*ptr != '\0') {
		if (*ptr == '%')
			if (*(ptr + 1) == 's') {
				output = va_arg(args, char**);
				start = line;
				length = 0;
				while (*line != *(ptr + 2))
					line++, length++;
				line++, length++;
				*output = (char*)malloc(length);
				if (*output != NULL) {
					snprintf(*output, length, "%s", start);

					char* start_ptr = *output;
					char* end_ptr = *output + length - 1;

					while (isspace(*start_ptr)) {
						start_ptr++;
					}

					while (end_ptr > start_ptr && isspace(*end_ptr)) {
						end_ptr--;
					}

					*(end_ptr + 1) = '\0';

					if (start_ptr != *output) {
						memmove(*output, start_ptr, end_ptr - start_ptr + 2);
					}

					variables_read++;
				}
			}
		ptr++;
	}
	va_end(args);
	return variables_read;
}

long int getline_j(char** lineptr, size_t* n, FILE* stream) {
	if (lineptr == NULL || n == NULL || stream == NULL)
		return -1;

	if (*lineptr == NULL) {
		*n = 128;
		*lineptr = malloc(*n);
		if (*lineptr == NULL)
			return -1;
	}

	size_t pos = 0;
	int c;
	while ((c = fgetc(stream)) != EOF) {
		if (pos + 1 >= *n) {
			*n *= 2;
			char* new_ptr = realloc(*lineptr, *n);
			if (new_ptr == NULL)
				return -1;
			*lineptr = new_ptr;
		}
		(*lineptr)[pos++] = (char)c;
		if (c == '\n')
			break;
	}

	if (pos == 0 && c == EOF)
		return -1;

	(*lineptr)[pos] = '\0';
	return pos;
}

void scanf_j(char** var) {
	char* input = NULL, * temp = NULL;
	size_t length = 0;
	long int nread = 0;

	nread = getline_j(&input, &length, stdin);
	if (nread == -1) {
		perror("Error reading input");
		return;
	}
	if (*input == '\n') {
		free(input);
		*var = NULL;
		return;
	}

	if (input[nread - 1] == '\n') {
		input[nread - 1] = '\0';
		nread--;
	}

	temp = input;

	while (*temp == ' ')
		temp++;

	if (temp != input) {
		char* dest = input;
		while (*temp != '\0') {
			*dest = *temp;
			dest++;
			temp++;
		}
		*dest = '\0';
	}

	*var = realloc(*var, nread + 1);
	if (!*var) {
		perror("Error reallocating memory");
		free(input);
		return;
	}

	strcpy(*var, input);
	free(input);
}

void error_j(const char* message, ...) {
	if (message)
	{
		FILE* file = myFOPEN(false, "logs", "txt", "a");
		if (file)
		{
			fprintf(file, "%s ", showErrorTimeStamp(setTimestamp()));

			va_list args;
			va_start(args, message);
			vfprintf(file, message, args);
			va_end(args);

			fprintf(file, "\n");

			fclose(file);
		}
	}
}

void wait_j() {
	printf("\nClique enter para continuar...\n");
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	getchar();
}

void toUppercase(char* str) {
	for (char* p = str; p && *p; ++p)
		*p = toupper((unsigned char)*p);
}

int getOption(char* output, char* msg, int max_val) {
	int opt = -1, value;
	char* input = NULL;
	bool input_bool = false;
	do {
		system("cls");
		if (msg)
			printf("%s", msg);
		printf(output);
		scanf_j(&input);
		if (input)
		{
			input_bool = true;
			for (int i = 0; *(input + i) != '\0'; i++)
				if (!isdigit(*(input + i))) {
					input_bool = false;
					break;
				}
			value = input_bool ? atoi(input) : -1;
			input_bool = input_bool && (value >= 0 && value <= max_val);
		}
	} while (!input_bool);
	system("cls");
	return atoi(input);
}