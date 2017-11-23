/*
 *	Timotej Lábský
 *	Projekt è.2
 *	´+ bonus Z
*/

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
int		fnc_z(AUTA **auta_first, int number_of_records);
void	fnc_h(AUTA *auta_first);
void	fnc_a(AUTA **auta_first, int number_of_records);

//custom function for cleaner code
char*	safe_copy_string_form_file(FILE *f, int size_of_string);
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
			break;
		case 'p':
			fnc_p(&auta_first, number_of_records);
			number_of_records++;
			break;
		case 'z':
			number_of_records = fnc_z(&auta_first,number_of_records);
			if (number_of_records == 0) {
				fnc_free(&auta_first);
			}
			break;
		case 'h':
			fnc_h(auta_first);
			break;
		case 'a':
			fnc_a(&auta_first, number_of_records);
			break;
		/*case 'f':
			fnc_free(&auta_first);
			break;*/
		default:
			break;
		}

	}
	fnc_free(&auta_first);
	return;
}

int fnc_n(AUTA **auta_first) {
	/*
	*	fuction to load data from file 
	*	and store them in linked list
	*
	*	data order in file
	*		$ - start of new record
	*		*kategory (*char)
	*		*znacka	(*char)
	*		*predajca (*char)
	*		*cena (int)
	*		*rok_vyroby (int)
	*		*stav_vozidla (*char)
	*/

	FILE *file_to_read;
	AUTA *auta_act, *temp = NULL;
	int number_of_records;
	char ch;

	number_of_records = 0;

	//opening of file and error handling
	file_to_read = fopen("auta.txt", "r");
	if (file_to_read == NULL) {
		printf("Zaznamy nebolu nacitane\n");
		return 0;
	}

	//dealocation of linked list
	fnc_free(auta_first);
	

	//inicial allocation of struct + error handling
	auta_act = (AUTA *)malloc(sizeof(AUTA));
	if (auta_act == NULL) {
		printf("nedostatok pamati\n");
		return 0;
	}
	alloc_auta(auta_act);
	auta_act->dalsi = NULL;

	//auta_act => to navigate thorugh linked list
	*auta_first = auta_act;


	while (!feof(file_to_read)) {
		//recognizing start of record and counting of records
		if ((ch = fgetc(file_to_read)) == '$') {
			number_of_records++;

			//end of file handling there might be added option for more "stupid" proof program
			if (ch == '\0') {
				break;
			}
			else
			{
				//allocate only if there is more than 1 record because of the inicial allocation
				if (number_of_records > 1) {
					if ((auta_act->dalsi = (auta*)malloc(sizeof(auta))) == NULL) {
						printf("malo pamete\n");
					}
					alloc_auta(auta_act->dalsi);
					auta_act = auta_act->dalsi;
					auta_act->dalsi = NULL;
				}

				//reading from file and saving it to linked list
				ch = fgetc(file_to_read);
				auta_act->kategoria = safe_copy_string_form_file(file_to_read,kategoria_size);
				auta_act->znacka = safe_copy_string_form_file(file_to_read,znacka_size);
				auta_act->predajca = safe_copy_string_form_file(file_to_read,predajca_size);
				auta_act->cena = safe_copy_int_from_file(file_to_read);
				auta_act->rok_vyroby = safe_copy_int_from_file(file_to_read);
				auta_act->stav_vozidla = safe_copy_string_form_file(file_to_read,stav_vozidla_size);
				
			}
		}
	}

	//if no record then dealloc
	if (number_of_records == 0) {
		fnc_free(auta_first);
		printf("Nacitalo sa 0 zaznamov");
		fnc_free(&temp);
		return 0;
	}

	//end of function with printing of number of loadede records and return of number records
	printf("Nacitalo sa %d zaznamov\n", number_of_records);
	free(auta_act->dalsi);
	fclose(file_to_read);
	fnc_free(&temp);
	return number_of_records;
	}


void fnc_v(AUTA *auta_first) {
	/*
	*	print whole linked list if there is any
	*/
	AUTA* auta_print = auta_first;
	int number_of_elements = 0;

	//if there is no record do nothing
	if (auta_first == NULL) {
		return;
	}

	//printing untli end of linked list
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
	//dealloc temporary pointer to moving through linked list
	free(auta_print);

}

void fnc_p(AUTA **auta_first, int number_of_records) {
	/*
	*	Add to a linked list new record on position
	*	users input
	*		*position
	*		*kategory (*char)
	*		*znacka	(*char)
	*		*predajca (*char)
	*		*cena (int)
	*		*rok_vyroby (int)
	*		*stav_vozidla (*char)
	*	if no linked list then it will be created
	*	if the postion is greater then last record in linked list 
	*		*will be added at the end of linked list
	*/
	AUTA *auta_act, *auta_to_add;
	int position_to_add, position_actual;

	//inicitalization and scan of position;
	auta_to_add = NULL;
	scanf("%d", &position_to_add);
	auta_act = *auta_first;
	position_actual = 0;

	if (auta_to_add == NULL) {
		if ((auta_to_add = (AUTA *)malloc(sizeof(AUTA))) == NULL) {
		printf("malo pamete\n");
	}
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

	//if no record in linked list
	if (number_of_records == 0) {
		*auta_first = auta_to_add;
		auta_to_add->dalsi = NULL;
		return;
	}
	//if position to add is first
	if (position_to_add == 1) {
		*auta_first = auta_to_add;
		auta_to_add->dalsi = auta_act;
		return;
	}

	//get to the positon 
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


int fnc_z(AUTA **auta_first, int number_of_records) {
	AUTA *auta_act, *auta_pred;
	char *string_to_search, *lower_znacka;
	int found, finding, number_of_deleted, lenght_of_znacka_act, skip = 0;
	
	//inicialization and allocation of all needed variables and pointers
	number_of_deleted = 0;
	auta_act = *auta_first;
	if ((string_to_search = (char*)malloc(znacka_size)) == NULL) {
	printf("malo pamete\n");
	}
	if ((lower_znacka = (char*)malloc(znacka_size)) == NULL) {
	printf("malo pamete\n");
	}
	getc(stdin);
	fgets(string_to_search, znacka_size, stdin);

	//remov '\n' from strinf_to_search
	for (int i = 0; i < znacka_size; i++) {
		if (string_to_search[i] == '\n') {
			string_to_search[i] = '\0';
			found = i;
			break;
		}
		string_to_search[i] = tolower(string_to_search[i]);
	}
	
	auta_pred = auta_act;

	while (auta_act != NULL) {
		
		finding = 0;

		//copy of auta_act->znacka to low kategoria with lower cased all chars
		strcpy(lower_znacka, auta_act->znacka);
		for (lenght_of_znacka_act = 0; lenght_of_znacka_act < znacka_size; lenght_of_znacka_act++) {
			if (lower_znacka[lenght_of_znacka_act] == '\n') {
				lower_znacka[lenght_of_znacka_act] = '\0';
				break;
			}
			lower_znacka[lenght_of_znacka_act] = tolower(lower_znacka[lenght_of_znacka_act]);
		}
		for (int i = 0; i <= lenght_of_znacka_act; i++) {
			if (finding == found) {
				//found
				number_of_deleted++;
				
				//if it is first element in list
				if (auta_pred == auta_act) {
					*auta_first = auta_act->dalsi;
					free(auta_act);
					auta_act = *auta_first;
					auta_pred = auta_act;
					skip = 1;
				}
				else
				{
					auta_pred->dalsi = auta_act->dalsi;
				}

				break;
			}
			if (lower_znacka[i] == string_to_search[finding]){
				finding++;
			}
			else
			{
				finding = 0;
			}
		}

		//handling of list moving
		if (skip == 0) {
			if (number_of_deleted == number_of_records) {
				return 0;
			}
			if ((auta_act != auta_pred) && (auta_pred->dalsi != NULL)) {
				auta_pred = auta_pred->dalsi;
			}
			if (auta_pred == NULL) {
				auta_pred = auta_act;
			}

			if (auta_act == NULL) {
				return (number_of_records - number_of_deleted);
			}
			auta_act = auta_act->dalsi;
		}
		skip = 0;
	}

	//deallocation of not needed stuf
	free(string_to_search);
	free(lower_znacka);
	return (number_of_records - number_of_deleted);
}

void	fnc_h(AUTA *auta_first) {
	AUTA *auta_act = auta_first;
	int cena_search, number_of_found = 0;

	
	scanf("%d", &cena_search);
	while (auta_act != NULL) {
		if (cena_search >= auta_act->cena) {
			number_of_found++;
			printf("%d.\n", number_of_found);
			printf("kategoria: %s", auta_act->kategoria);
			printf("zancka: %s", auta_act->znacka);
			printf("predajca: %s", auta_act->predajca);
			printf("cena: %d\n", auta_act->cena);
			printf("rok_vyroby: %d\n", auta_act->rok_vyroby);
			printf("stav_vozidla: %s", auta_act->stav_vozidla);
		}
		auta_act = auta_act->dalsi;

	}
	free(auta_act);
}

void	fnc_a(AUTA **auta_first, int number_of_records) {
	if (*auta_first == NULL) { return; }

	AUTA *auta_act, *auta_to_add;
	char znacka_search[znacka_size];
	int cena_search;

	if ((auta_to_add = (AUTA *)malloc(sizeof(AUTA))) == NULL) {
	printf("malo pamete\n");
	}
	alloc_auta(auta_to_add);
	auta_to_add->dalsi = NULL;

	auta_act = *auta_first;
	fgetc(stdin);
	fgets(znacka_search, 100, stdin);
	scanf("%d", &cena_search);
	
	getc(stdin);

	fgets(auta_to_add->kategoria, kategoria_size, stdin);
	fgets(auta_to_add->znacka, znacka_size, stdin);
	fgets(auta_to_add->predajca, predajca_size, stdin);
	auta_to_add->cena = scan_int();
	auta_to_add->rok_vyroby = scan_int();
	fgets(auta_to_add->stav_vozidla, stav_vozidla_size, stdin);

	

	while (auta_act != NULL) {
		for (int i = 0; i < znacka_size; i++) {
			if (znacka_search[i] == '\0') {
				if (cena_search == auta_act->cena) {
					auta_act->kategoria = auta_to_add->kategoria;
					auta_act->znacka = auta_to_add->znacka;
					auta_act->predajca = auta_to_add->predajca;
					auta_act->cena = auta_to_add->cena;
					auta_act->rok_vyroby = auta_to_add->rok_vyroby;
					auta_act->stav_vozidla = auta_to_add->stav_vozidla;
				}
				break;
			}
			if (znacka_search[i] != auta_act->znacka[i])
			{
				break;
			}
			
		}
		auta_act = auta_act->dalsi;
	}
}



char*	safe_copy_string_form_file(FILE *f, int size_of_string) {
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
		if ((buffer_string = (char *)malloc(size_of_string * sizeof(char))) == NULL) {
			printf("malo pamete\n");
		}
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
	/*
	 *	function to save int from file
	 *
	 */
	char integer_str[11];
	int integer;
	for (int i = 0; i < 11; i++) {
		integer_str[i] = fgetc(f);
		if (integer_str[i] == '\n') {
			integer_str[i + 1] = '\0';
			break;
		}
	}
	sscanf(integer_str, "%d", &integer);

	return integer;
}

void   alloc_auta(AUTA* auta_act) {
	/*
	 *	allocation of all dynamic arrays in AUTA
	 */
	if (auta_act == NULL) {
		printf("error alloc auta\n");
		return;
	}

	if ((auta_act->kategoria = (char*)malloc(kategoria_size * sizeof(char))) == NULL) {
		printf("malo pamete\n");
	}
	if ((auta_act->znacka = (char*)malloc(znacka_size * sizeof(char))) == NULL) {
		printf("malo pamete\n");
	}
	if ((auta_act->predajca = (char*)malloc(predajca_size * sizeof(char))) == NULL) {
	printf("malo pamete\n");
	}
	if ((auta_act->stav_vozidla = (char*)malloc(stav_vozidla_size * sizeof(char))) == NULL) {
	printf("malo pamete\n");
	}
}

void	fnc_free(AUTA **auta_first) { 
	/*
	 *   fuction to dealloc list of struct
	 *	 
	*/
	
	AUTA *temp;

	while (*auta_first != NULL)
	{
		temp = *auta_first;
		*auta_first = (*auta_first)->dalsi;
		free(temp);
	}
}

int		scan_int() {
	char buf[stav_vozidla_size], *end;
	do {
		if (!fgets(buf, sizeof buf, stdin))
			break;

		// remove \n
		buf[strlen(buf) - 1] = 0;

		return strtol(buf, &end, 10);
	} while (end != buf + strlen(buf));
	return 0;
}