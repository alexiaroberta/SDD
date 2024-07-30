//arbore abc - nodurile din stanga sunt mai mici decat radacina ,iar nodurile din drepta sunt mai mari
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Cinema Cinema;
typedef struct Nod Nod;

struct Cinema {
	int id;
	int nrFilme;
	char** listaFilme;
};

struct Nod {
	Cinema info;
	Nod* st;
	Nod* dr;
};

Nod* inserareInArbore(Nod* radacina, Cinema c, int* inserat) {
	if (radacina) {
		if (radacina->info.id > c.id) {
			radacina->st = inserareInArbore(radacina->st, c, inserat);
		}
		else {
			if (radacina->info.id < c.id) {
				radacina->dr = inserareInArbore(radacina->dr, c, inserat);
			}
			else {
				*inserat = 0;
			}
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = c;
		nou->st = NULL;
		nou->dr = NULL;
		
		radacina = nou;
		*inserat = 1;
	}
	return radacina;
}

void citireDinFisier(const char* numeFisier, Nod** arbore) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f != NULL) {
			char linie[256];
			char delimitator[] = ",\n";
			while (fgets(linie, sizeof(linie), f)) {
				char* token = strtok(linie, delimitator);
				Cinema c;
				c.id = atoi(token);
				token = strtok(NULL, delimitator);
				c.nrFilme = atoi(token);
				token = strtok(NULL, delimitator);
				c.listaFilme = (char**)malloc(sizeof(char*) * c.nrFilme);
				for (int i = 0; i < c.nrFilme; i++) {
					c.listaFilme[i] = (char*)malloc(sizeof(char) * (strlen(token) + 1));
					strcpy(c.listaFilme[i], token);
					token = strtok(NULL, delimitator);
				}

				int inserat;
				*arbore = inserareInArbore(*arbore, c, &inserat);
				if (!inserat) {
					for (int i = 0; i < c.nrFilme; i++) {
						free(c.listaFilme[i]);
					}
					free(c.listaFilme);
				}
			}
			fclose(f);
		}
	}
}

int nrTotalFilmeRedate(Nod* radacina) {
	if (radacina == 0) {
		return 0;
	}
	else {
		int suma = 0;
		suma += radacina->info.nrFilme;
		suma += nrTotalFilmeRedate(radacina->st);
		suma += nrTotalFilmeRedate(radacina->dr);
		return suma;
	}
}

void afisareCinema(Cinema c) {
	printf("\n%d %d", c.id, c.nrFilme);
	for (int i = 0; i < c.nrFilme; i++) {
		printf(" %s ", c.listaFilme[i]);
	}
}

//R-S-D
void traversarePreordine(Nod* radacina) {
	if (radacina) {
		afisareCinema(radacina->info);
		traversarePreordine(radacina->st);
		traversarePreordine(radacina->dr);
	}
}


//S-R-D
void traversareInordine(Nod* radacina) {
	if (radacina) {
		traversareInordine(radacina->st);
		afisareCinema(radacina->info);
		traversareInordine(radacina->dr);
	}
}

//S-D-R
void traversarePostOrdine(Nod* radacina) {
	if (radacina) {
		traversarePostOrdine(radacina->st);
		traversarePostOrdine(radacina->dr);
		afisareCinema(radacina->info);
	}
}

Cinema* cautaCinemaDupaId(Nod* radacina, int id) {
	Cinema* c = NULL;
	if (radacina) {
		if (radacina->info.id > id) {
			c = cautaCinemaDupaId(radacina->st, id);
		}
		else {
			if (radacina->info.id < id) {
				c = cautaCinemaDupaId(radacina->dr, id);
			}
			else {
				c = &(radacina->info);
			}
		}
	}
	return c;
}

Nod* dezalocareArbore(Nod* radacina) {
	if (radacina) {
		radacina->st = dezalocareArbore(radacina->st);
		radacina->dr = dezalocareArbore(radacina->dr);
		for (int i = 0; i < radacina->info.nrFilme; i++) {
			free(radacina->info.listaFilme[i]);
		}
		free(radacina->info.listaFilme);
		free(radacina);
		radacina = NULL;
	}
	return radacina;
}

void main() {
	Nod* arbore = NULL;
	citireDinFisier("cinema.txt", &arbore);

	traversarePreordine(arbore);
	printf("\n");
	traversareInordine(arbore);
	printf("\n");
	traversarePostOrdine(arbore);
	printf("\n");

	printf("\nNr total filme redate %d\n", nrTotalFilmeRedate(arbore));

	Cinema* c = cautaCinemaDupaId(arbore, 4);
	if (c != NULL) {
		printf("\n\n%d %d", c->id, c->nrFilme);
	}
	else {
		printf("nu");
	}

	arbore = dezalocareArbore(arbore);
}