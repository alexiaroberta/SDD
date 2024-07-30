//arbore abc - nodurile din stanga sunt mai mici decat radacina,iar cele din dreapta sunt mai mari decat radacina
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
		//continui cautarea locului de inserat
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
		//am gasit loc de inserare
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = c;
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
					printf("Nodul %d nu fost inserat", c.id);
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
	if (radacina) {
		int suma = 0;
		suma = radacina->info.nrFilme;
		suma += nrTotalFilmeRedate(radacina->st);
		suma += nrTotalFilmeRedate(radacina->dr);
		return suma;
	}
	else {
		return 0;
	}
}

void afisareCinema(Cinema c) {
	printf("\n%d %d ", c.id, c.nrFilme);
	for (int i = 0; i < c.nrFilme; i++) {
		printf(" %s ", c.listaFilme[i]);
	}
}

//R-S-D
void afisareArborePreordine(Nod* radacina) {
	if (radacina) {
		afisareCinema(radacina->info);
		afisareArborePreordine(radacina->st);
		afisareArborePreordine(radacina->dr);
	}
}

//S-R-D
void afisareArboreInordine(Nod* radacina) {
	if (radacina) {
		afisareArboreInordine(radacina->st);
		afisareCinema(radacina->info);
		afisareArboreInordine(radacina->dr);
	}
}

//S-D-R
void afisareArborePostordine(Nod* radacina) {
	if (radacina) {
		afisareArborePostordine(radacina->st);
		afisareArborePostordine(radacina->dr);
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
	citesteDinFisier("cinema.txt", &arbore);

	afisareArborePreordine(arbore);
	printf("\n");
	afisareArboreInordine(arbore);
	printf("\n");
	afisareArborePostordine(arbore);
	printf("\n");

	printf("\nNr total filme %d\n", nrTotalFilmeRedate(arbore));

	Cinema* c = NULL;
	c = cautaCinemaDupaId(arbore, 4);
	if (c != NULL) {
		printf("\n%d %d", c->id, c->nrFilme);
	}
	else {
		printf("NU");
	}

	arbore = dezalocareArbore(arbore);
	afisareArboreInordine(arbore);
}