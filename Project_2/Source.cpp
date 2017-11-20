#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string>

#define kategoria_size  50
#define znacka_size  50
#define predajca_size  100
#define stav_vozidla_size  200

typedef struct auta
{
	char *kategoria;
	char *znacka;
	char *predajca;
	int cena;
	int rok_vyroby;
	char *stav_vozidla;
	struct auta * dalsi;

}AUTA;

int		fnc_n(AUTA **auta_first);
void	fnc_v(AUTA *auta_first);
void	fnc_p(AUTA **auta_first, int number_of_records);
void	fnc_z(AUTA *auta_first, int number_of_records);

//custom function for cleaner code
char*	safe_copy_string_form_file(FILE *f);
int		safe_copy_int_from_file(FILE *f);
void	alloc_auta(AUTA* auta_alloc);
void	fnc_free(AUTA **auta_first);
int		scan_int();



void main() {
	AUTA *auta_first;
	char function_char;
	int number_of_records = 0;

	auta_first = NULL;
	function_char = 'a';

	while (function_char != 'k') {
		scanf("%c", &function_char);

		switch (function_char)
		{
		case 'n':
			number_of_records = fnc_n(&auta_first);
			break;
		case 'v':
			fnc_v(auta_first);
			//printf("%s", auta_first->dalsi->dalsi->dalsi->kategoria);
			break;
		case 'p':
			fnc_p(&auta_first, number_of_records);
			number_of_records++;
			printf("number of records %d\n", number_of_records);
			break;
		case 'f':
			fnc_free(&auta_first);
			break;
		case 'z':
			fnc_z(auta_first,number_of_records);
			number_of_records--;
			break;
		default:
			break;
		}

	}

	return;
}

int fnc_n(AUTA **auta_first) {
	FILE *file_to_read;
	AUTA *auta_act, *temp = NULL;
	int type_of_record, number_of_records;
	char ch;

	number_of_records = 0;

	//opening of file
	file_to_read = fopen("auta.txt", "r");
	if (file_to_read == NULL) {
		printf("Zaznamy nebolu nacitane\n");
		return 0;
	}

	//dealoc 
	while (*auta_first != NULL)
	{
		temp = *auta_first;
		*auta_first = (*auta_first)->dalsi;
		free(temp);

	}
	

	//inicial allocation of struct + error handling
	auta_act = (AUTA *)malloc(sizeof(AUTA));
	if (auta_act == NULL) {
		printf("nedostatok pamati\n");
		return 0;
	}
	alloc_auta(auta_act);
	auta_act->dalsi = NULL;
	*auta_first = auta_act;


	while (!feof(file_to_read)) {
		//recognizing start and counting of records
		if ((ch = fgetc(file_to_read)) == '$') {
			number_of_records++;

			if (ch == '\0') {
				break;
			}
			else
			{
				if (number_of_records > 1) {
					auta_act->dalsi = (auta*)malloc(sizeof(auta));
					alloc_auta(auta_act->dalsi);
					auta_act = auta_act->dalsi;
					auta_act->dalsi = NULL;
				}
				//reading from file and saving it to linked list
				ch = fgetc(file_to_read);
				auta_act->kategoria = safe_copy_string_form_file(file_to_read);
				auta_act->znacka = safe_copy_string_form_file(file_to_read);
				auta_act->predajca = safe_copy_string_form_file(file_to_read);
				auta_act->cena = safe_copy_int_from_file(file_to_read);
				auta_act->rok_vyroby = safe_copy_int_from_file(file_to_read);
				auta_act->stav_vozidla = safe_copy_string_form_file(file_to_read);
				
			}
		}
	}
		printf("Nacitalo sa %d zaznamov\n", number_of_records);
		free(auta_act->dalsi);
		fclose(file_to_read);
		return number_of_records;
	}


void fnc_v(AUTA *auta_first) {
	AUTA* auta_print = auta_first;
	int number_of_elements = 0;

	if (auta_first == NULL) {
		return;
	}

	while (auta_print != NULL) {
		printf("%d.\n", ++number_of_elements);
		printf("kategoria: %s", auta_print->kategoria);
		printf("zancka: %s", auta_print->znacka);
		printf("predajca: %s", auta_print->predajca);
		printf("cena: %d\n", auta_print->cena);
		printf("rok_vyroby: %d\n", auta_print->rok_vyroby);
		printf("stav_vozidla: %s", auta_print->stav_vozidla);
		auta_print = auta_print->dalsi;
	}

}

void fnc_p(AUTA **auta_first, int number_of_records) {
	AUTA *auta_act, *auta_to_add;
	int position_to_add, position_actual, junk_data;


	auta_to_add = NULL;
	scanf("%d", &position_to_add);
	auta_act = *auta_first;
	position_actual = 0;

	if (auta_to_add == NULL) {
		auta_to_add = (AUTA *)malloc(sizeof(AUTA));
		alloc_auta(auta_to_add);
		auta_to_add->dalsi = NULL;
	}
	getc(stdin);
	fgets(auta_to_add->kategoria, kategoria_size, stdin);
	fgets(auta_to_add->znacka, znacka_size, stdin);
	fgets(auta_to_add->predajca, predajca_size, stdin);
	auta_to_add->cena = scan_int();
	auta_to_add->rok_vyroby = scan_int();
	fgets(auta_to_add->stav_vozidla, stav_vozidla_size, stdin);


	if (position_to_add == 1) {
		*auta_first = auta_to_add;
		auta_to_add->dalsi = auta_act;
		return;
	}

	while (auta_act != NULL) {
		position_actual++;
		if ((position_actual == position_to_add - 1)|| (position_actual == number_of_records)) {
			break;
		}
		else
		{
			auta_act = auta_act->dalsi;
		}
	}
	
	auta_to_add->dalsi = auta_act->dalsi;
	auta_act->dalsi = auta_to_add;

}


void fnc_z(AUTA *auta_first, int number_of_records) {
	AUTA *auta_act;
	char *string_to_search, *lower_kategoria;
	int found, finding, number_of_deleted;
	
	//inicialization and allocation of all needed variables and pointers
	number_of_deleted = 0;
	auta_act = auta_first;
	string_to_search = (char*)malloc(znacka_size);
	lower_kategoria = (char*)malloc(znacka_size);
	getc(stdin);
	fgets(string_to_search, znacka_size, stdin);
	//remov '\n' from strinf_to_search
	for (int i = 0; i < znacka_size; i++) {
		if (string_to_search[i] == '\n') {
			string_to_search[i] = '\0';
			found = i - 1;
			break;
		}
	}

	while (auta_act != NULL) {
		finding = 0;
		for (int i = 0; i < znacka_size; i++) {
			if (finding == found) {
				number_of_deleted++;
				break;
			}
			else if (auta_act->kategoria[i] == string_to_search[finding]){
				finding++;
			}
			else
			{
				finding = 0;
			}
		}
		auta_act = auta_act->dalsi;
	}
	printf("najdeno %d\n", number_of_deleted);
}



char*	safe_copy_string_form_file(FILE *f) {
		/*
		* making of string from  file without '\n'
		*	copy from file until the '\n'
		*	'\n' replaced by '\0'
		*	returned the string
		*/
		char buffer_char = '\0';
		char *buffer_string = NULL;
		int i = 0;

		//allocation of *buffer_string
		//need to change the 51 nuber to dinamic number
		buffer_string = (char *)malloc(stav_vozidla_size * sizeof(char));
		if (buffer_string == NULL) {
			printf("error");
		}

		for (i = 0; i < 50; i++) {
			buffer_char = fgetc(f);
			if ((buffer_char == '\n') || (feof(f))) {
				buffer_string[i] = '\n';
				buffer_string[i + 1] = '\0';
				break;
			}
			buffer_string[i] = buffer_char;
		}

		return buffer_string;
}

int	safe_copy_int_from_file(FILE *f) {
	char integer_str[11];
	int integer;
	for (int i = 0; i < 11; i++) {
		integer_str[i] = fgetc(f);
		if (integer_str[i] == '\n') {
			integer_str[i] = '\n';
			integer_str[i + 1] = '\0';
			break;
		}
	}
	sscanf(integer_str, "%d", &integer);

	return integer;
}

void   alloc_auta(AUTA* auta_act) {
	if (auta_act == NULL) {
		printf("error alloc auta\n");
		return;
	}
	auta_act->kategoria = (char*)malloc(kategoria_size * sizeof(char));
	auta_act->znacka = (char*)malloc(znacka_size * sizeof(char));
	auta_act->predajca = (char*)malloc(predajca_size * sizeof(char));
	auta_act->stav_vozidla = (char*)malloc(stav_vozidla_size * sizeof(char));
}

void	fnc_free(AUTA **auta_first) { 
	/*
	 *   test of deallocation
	*/
	
	AUTA *temp;

	while (*auta_first != NULL)
	{
		temp = *auta_first;
		*auta_first = (*auta_first)->dalsi;
		free(temp);
	}
	printf("free\n");
}

int		scan_int() {
	char buf[200], *end;
	do {
		if (!fgets(buf, sizeof buf, stdin))
			break;

		// remove \n
		buf[strlen(buf) - 1] = 0;

		return strtol(buf, &end, 10);
	} while (end != buf + strlen(buf));
}