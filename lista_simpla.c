#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cafenea Cafenea;
typedef struct Nod Nod;

struct Cafenea {
	char* nume;
	int nrLocuri;
	float suprafata;
};

struct Nod {
	Cafenea info;
	Nod* next;
};

Cafenea initCafenea(const char* nume, int nrLocuri, float suprafata) {
	Cafenea c;
	c.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(c.nume, nume);
	c.nrLocuri = nrLocuri;
	c.suprafata = suprafata;
	return c;
}

Nod* inserareInceput(Nod* lista, Cafenea c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initCafenea(c.nume, c.nrLocuri, c.suprafata);
	nou->next = lista;
	return nou;
}

Nod* inserareFinal(Nod* lista, Cafenea c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initCafenea(c.nume, c.nrLocuri, c.suprafata);
	nou->next = NULL;
	if (lista == NULL) {
		return nou;
	}
	else {
		Nod* aux = lista;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
		return lista;
	}
}

Nod* inserareSortata(Nod* lista, Cafenea c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initCafenea(c.nume, c.nrLocuri, c.suprafata);
	nou->next = NULL;
	if (lista == NULL) {
		return nou;
	}
	else {
		if (lista->info.nrLocuri > nou->info.nrLocuri) {
			nou->next = lista;
			return nou;
		}
		else {
			Nod* aux = lista;
			while (aux->next != NULL && aux->next->info.nrLocuri < nou->info.nrLocuri) {
				aux = aux->next;
			}
			if (aux->next == NULL) {
				aux->next = nou;
			}
			else {
				nou->next = aux->next;
				aux->next = nou;
			}
			return lista;
		}
	}
}

void afisareCafenea(Cafenea c) {
	printf("%s %d %3.2f\n", c.nume, c.nrLocuri, c.suprafata);
}

void afisareCafenele(Nod* lista) {
	while (lista != NULL) {
		afisareCafenea(lista->info);
		lista = lista->next;
	}
}

void citesteFisier(const char* numeFisier, Nod** lista) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f != NULL) {
			char linie[100];
			char delimitator[] = ",\n";
			while (fgets(linie, 100, f)) {
				char* token = strtok(linie, delimitator);
				Cafenea c;
				c.nume = (char*)malloc(sizeof(char) * (strlen(token) + 1));
				strcpy(c.nume, token);
				token = strtok(NULL, delimitator);
				c.nrLocuri = atoi(token);
				token = strtok(NULL, delimitator);
				c.suprafata = atof(token);

				*lista = inserareSortata(*lista, c);
			}
			fclose(f);
		}
	}
}

//numele cafenelei cu densitatea cea mai mica
char* numeCafeneaDensitateMica(Nod* lista) {
	if (lista != NULL) {
		if (lista->info.suprafata != 0) {
			float minim = lista->info.nrLocuri / lista->info.suprafata;
			Nod* aux = lista;
			lista = lista->next;
			while (lista != NULL) {
				if (lista->info.nrLocuri / lista->info.suprafata < minim) {
					minim = lista->info.nrLocuri / lista->info.suprafata;
					aux = lista;
				}
				lista = lista->next;
			}
			char* nume = (char*)malloc(sizeof(char) * (strlen(aux->info.nume) + 1));
			strcpy(nume, aux->info.nume);
			return nume;
		}
	}
	return NULL;
}

void dezalocare(Nod** lista) {
	while ((*lista) != NULL) {
		Nod* aux = *lista;
		(*lista) = (*lista)->next;
		free(aux->info.nume);
		free(aux);
	}
	*lista = NULL;
}

//copiere elemente in vector
Cafenea* copiereInVector(Nod* lista, int pragLocuri, int* n) {
	*n = 0;
	Nod* aux = lista;
	while (aux != NULL) {
		if (aux->info.nrLocuri >= pragLocuri) {
			(*n)++;
		}
		aux = aux->next;
	}
	if (*n == 0) {
		return NULL;
	}
	else {
		int k = 0;
		Cafenea* vector = (Cafenea*)malloc(sizeof(Cafenea) * (*n));
		while (lista != NULL) {
			if (lista->info.nrLocuri >= pragLocuri) {
				vector[k++] = initCafenea(lista->info.nume, lista->info.nrLocuri, lista->info.suprafata);
			}
			lista = lista->next;
		}
		return vector;
	}
	return NULL;
}

void dezalocareVector(Cafenea** c, int* nr) {
	if ((*c) != NULL) {
		for (int i = 0; i < *nr; i++) {
			free((*c)[i].nume);
		}
		free(*c);
		*c = NULL;
		*nr = 0;
	}
}

//lista simpla circulara
Nod* inserareInceputListaCirculara(Nod* lista, Cafenea c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initCafenea(c.nume, c.nrLocuri, c.suprafata);
	if (lista == NULL) {
		nou->next = nou;
		return nou;
	}
	else {
		Nod* aux = lista;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
		nou->next = lista;
		return nou;
	}
}

void afisareListaCirculara(Nod* lista) {
	if (lista == NULL) {
		printf("Lista este goala");
	}
	Nod* aux = lista;
	do {
		afisareCafenea(aux->info);
		aux = aux->next;
	} while (aux != lista);
}

void dezalocareCirculara(Nod** lista) {
	if ((*lista) == NULL) {
		printf("nu");
	}
	Nod* aux = (*lista)->next;
	Nod* temp;
	while (aux != *lista) {
		temp = aux;
		aux = aux->next;
		free(temp);
	}
	free(*lista);
}

void main() {
	Nod* lista = NULL;
	Cafenea cafenea1 = initCafenea("Tucano", 12, 20);
	Cafenea cafenea2 = initCafenea("Teds", 17, 17);
	Cafenea cafenea3 = initCafenea("Urban", 65, 33);
	Cafenea sb = initCafenea("Starbucks", 25, 43);
	lista = inserareFinal(lista, cafenea1);
	lista = inserareFinal(lista, cafenea2);
	lista = inserareFinal(lista, cafenea3);
	afisareCafenele(lista);
	printf("\n");
	lista = inserareInceput(lista, sb);
	afisareCafenele(lista);

	printf("\n");
	Nod* listaFisier = NULL;
	citesteFisier("cafenele.txt", &listaFisier);
	afisareCafenele(listaFisier);

	char* nume = numeCafeneaDensitateMica(lista);
	printf("\n%s\n\n", nume);

	int nr;
	Cafenea* vector = copiereInVector(lista, 20, &nr);
	if (vector != NULL) {
		for (int i = 0; i < nr; i++) {
			afisareCafenea(vector[i]);
		}
		dezalocareVector(&vector, &nr);
	}
	else {
		printf("Nu exista elemente");
	}

	free(cafenea1.nume);
	cafenea1.nume = NULL;
	free(cafenea2.nume);
	cafenea2.nume = NULL;
	free(cafenea3.nume);
	cafenea3.nume = NULL;
	free(sb.nume);
	sb.nume = NULL;
	free(nume);
	nume = NULL;

	dezalocare(&lista);
}