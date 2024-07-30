#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct RaportAnalizeMedicale RaportAnalizeMedicale;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct RaportAnalizeMedicale {
	unsigned int codRaport;
	unsigned int codPacient;
	unsigned char nrAnalize;
	char** denumiriAnalize;
	float* valReferinta;
	float* valDeterminata;
};

struct Nod {
	RaportAnalizeMedicale info;
	Nod* anterior;
	Nod* urmator;
};

struct ListaDubla {
	Nod* cap;
	Nod* coada;
};

void inserareLista(ListaDubla* lista, RaportAnalizeMedicale r) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = r;
	nou->anterior = NULL;
	nou->urmator = lista->cap;
	if (lista->cap != NULL) {
		lista->cap->anterior = nou;
	}
	else {
		lista->coada = nou;
	}
	lista->cap = nou;
}

void citesteDinFisier(const char* numeFisier, ListaDubla* lista) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f) {
			char linie[256];
			char delimitator[] = ",\n";
			while (fgets(linie, sizeof(linie), f)) {
				char* token = strtok(linie, delimitator);
				RaportAnalizeMedicale r;
				r.codRaport = atoi(token);
				token = strtok(NULL, delimitator);
				r.codPacient = atoi(token);
				token = strtok(NULL, delimitator);
				r.nrAnalize = atoi(token);
				r.denumiriAnalize = (char**)malloc(sizeof(char*) * r.nrAnalize);
				r.valReferinta = (float*)malloc(sizeof(float) * r.nrAnalize);
				r.valDeterminata = (float*)malloc(sizeof(float) * r.nrAnalize);
				for (int i = 0; i < r.nrAnalize; i++) {
					token = strtok(NULL, delimitator);
					r.denumiriAnalize[i] = (char*)malloc(sizeof(char) * (strlen(token) + 1));
					strcpy(r.denumiriAnalize[i], token);
					token = strtok(NULL, delimitator);
					r.valReferinta[i] = atof(token);
					token = strtok(NULL, delimitator);
					r.valDeterminata[i] = atof(token);
				}
				inserareLista(lista, r);
			}
			fclose(f);
		}
	}
}

void afisareLista(ListaDubla lista) {
	Nod* aux = lista.cap;
	while (aux) {
		printf("%d %d \n%d\n", aux->info.codRaport, aux->info.codPacient, aux->info.nrAnalize);
		for (int i = 0; i < aux->info.nrAnalize; i++) {
			printf("%s %.2f %.2f\n", aux->info.denumiriAnalize[i], aux->info.valReferinta[i], aux->info.valDeterminata[i]);
		}
		aux = aux->urmator;
	}
}

void main() {
	ListaDubla lista;
	lista.cap = NULL;
	lista.coada = NULL;
	citesteDinFisier("analize.txt", &lista);
	afisareLista(lista);
}