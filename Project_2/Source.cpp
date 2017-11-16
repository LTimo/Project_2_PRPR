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
	struct auta *dalsi;

}AUTA;

void	fnc_n(AUTA **auta_first, AUTA **auta_act);
char*	safe_copy_string(FILE *f);
int		safe_copy_int(FILE *f);
void	alloc_auta(AUTA* auta_alloc);


void main() {
	AUTA *auta_first, *auta_act;
	char function_char;

	auta_first = NULL;
	auta_act = NULL;
	function_char = 'a';

	while (function_char != 'k') {
		scanf("%c", &function_char);
		
		switch (function_char)
		{
		case 'n':
			fnc_n(&auta_first, &auta_act);			
			break;
		case 'v':
			printf("treba spravit\n");
			printf("%s\n", auta_first->kategoria);
			break;
		default:
			break;
		}

	}

	return;
}

void fnc_n(AUTA **auta_first, AUTA **auta_act) {
	FILE *file_to_read;
	int number_of_records, type_of_record;
	char ch;

	//inicialization of variables
	number_of_records = 0;


	//opening of file
	file_to_read = fopen("auta.txt", "r");
	if (file_to_read == NULL) {
		printf("Zaznamy nebolu nacitane\n");
		return;
	}

	//inicial allocation of struct
	*auta_first = (AUTA *)malloc(sizeof(AUTA));
	*auta_act = (AUTA *)malloc(sizeof(AUTA));



	while (!feof(file_to_read)) {
		//recognizing start and counting of records
		if ((ch = fgetc(file_to_read)) == '$') {
			number_of_records++;
		}
		else
		{
			alloc_auta(*auta_act);

			(*auta_act)->kategoria = safe_copy_string(file_to_read);
			(*auta_act)->znacka = safe_copy_string(file_to_read);
			(*auta_act)->predajca = safe_copy_string(file_to_read);
			(*auta_act)->cena = safe_copy_int(file_to_read);
			(*auta_act)->rok_vyroby = safe_copy_int(file_to_read);
			(*auta_act)->stav_vozidla = safe_copy_string(file_to_read);

			if (number_of_records == 1) {
				//saving first element of linked list
				*auta_first = *auta_act;
				*auta_act = (*auta_act)->dalsi;
				*auta_act = (AUTA *)malloc(sizeof(AUTA));
				alloc_auta(*auta_act);
			}
			else
			{
				//new element of linked list
				*auta_act = (*auta_act)->dalsi;
				*auta_act = (AUTA *)malloc(sizeof(AUTA));
				alloc_auta(*auta_act);
			}
		}
	}

	printf("Nacitalo sa %d zaznamov\n", number_of_records);
}


char*	safe_copy_string(FILE *f) {
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
		buffer_string = (char *)malloc(51 * sizeof(char));
		if (buffer_string == NULL) {
			printf("error");
		}

		for (i = 0; i < 50; i++) {
			buffer_char = fgetc(f);
			if ((buffer_char == '\n') || (feof(f))) {
				buffer_string[i] = '\0';
				break;
			}
			buffer_string[i] = buffer_char;
		}

		return buffer_string;
}

int	safe_copy_int(FILE *f) {
	char integer_str[11];
	int integer;

	for (int i = 0; i < 11; i++) {
		integer_str[i] = fgetc(f);
		if (integer_str[i] == '\n') {
			integer_str[i] = '\0';
			break;
		}
	}
	sscanf(integer_str, "%d", &integer);

	return integer;
}

void   alloc_auta(AUTA* auta_act) {
	auta_act->kategoria = (char*)malloc(kategoria_size * sizeof(char));
	auta_act->znacka = (char*)malloc(znacka_size * sizeof(char));
	auta_act->predajca = (char*)malloc(predajca_size * sizeof(char));
	auta_act->stav_vozidla = (char*)malloc(stav_vozidla_size * sizeof(char));
	auta_act->dalsi = NULL;
}