#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student Student;
typedef struct Nod Nod;

struct Student {
	int id;
	char* nume;
	float medie;
};

struct Nod {
	Student info;
	Nod* st;
	Nod* dr;
};

Nod* inserareInArbore(Nod* radacina, Student s, int* inserat) {
	if (radacina) {
		if (radacina->info.id > s.id) {
			radacina->st = inserareInArbore(radacina->st, s, inserat);
		}
		else {
			if (radacina->info.id < s.id) {
				radacina->dr = inserareInArbore(radacina->dr, s, inserat);
			}
			else {
				*inserat = 0;
			}
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = s;
		nou->st = NULL;
		nou->dr = NULL;

		radacina = nou;
		*inserat = 1;
	}
	return radacina;
}

void citesteDinFisier(const char* numeFisier, Nod** arbore) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f != NULL) {
			char linie[256];
			char delimitator[] = ",\n";
			while (fgets(linie, 256, f)) {
				char* token = strtok(linie, delimitator);
				Student s;
				s.id = atoi(token);
				token = strtok(NULL, delimitator);
				s.nume = (char*)malloc(sizeof(char) * (strlen(token) + 1));
				strcpy(s.nume, token);
				token = strtok(NULL, delimitator);
				s.medie = atof(token);

				int inserat;
				*arbore = inserareInArbore(*arbore, s, &inserat);
				if (!inserat) {
					free(s.nume);
				}
			}
			fclose(f);
		}
	}
}

//R-S-D
void traversarePreordine(Nod* radacina) {
	if (radacina) {
		printf("%d %s %3.2f\n", radacina->info.id, radacina->info.nume, radacina->info.medie);
		traversarePreordine(radacina->st);
		traversarePreordine(radacina->dr);
	}
}

//S-R-D
void traversareInordine(Nod* radacina) {
	if (radacina) {
		traversareInordine(radacina->st);
		printf("%d %s %3.2f\n", radacina->info.id, radacina->info.nume, radacina->info.medie);
		traversareInordine(radacina->dr);
	}
}

//S-D-R
void traversarePostordine(Nod* radacina) {
	if (radacina) {
		traversarePostordine(radacina->st);
		traversarePostordine(radacina->dr);
		printf("%d %s %3.2f\n", radacina->info.id, radacina->info.nume, radacina->info.medie);
	}
}

Nod* dezalocareArbore(Nod* radacina) {
	if (radacina) {
		radacina->st = dezalocareArbore(radacina->st);
		radacina->dr = dezalocareArbore(radacina->dr);
		free(radacina->info.nume);
		free(radacina);
		radacina = NULL;
	}
	return radacina;
}

void main() {
	Nod* arbore = NULL;
	citesteDinFisier("studenti.txt", &arbore);

	printf("\n");
	traversarePreordine(arbore);
	printf("\n");
	traversareInordine(arbore);
	printf("\n");
	traversarePostordine(arbore);

	arbore = dezalocareArbore(arbore);
	traversareInordine(arbore);
}