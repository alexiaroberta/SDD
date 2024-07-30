#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Galerie Galerie;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Galerie {
	int cod;
	char* nume;
	float pretIntrare;
};

struct Nod {
	Galerie info;
	Nod* next;
};

struct HashTable {
	Nod** vector;
	int dim;
};

Galerie initGalerie(int cod, const char* nume, float pretIntrare) {
	Galerie g;
	g.cod = cod;
	g.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(g.nume, nume);
	g.pretIntrare = pretIntrare;
	return g;
}

HashTable initHashTable(int dim) {
	HashTable tabela;
	tabela.dim = dim;
	tabela.vector = (Nod**)malloc(sizeof(Nod*) * dim);
	for (int i = 0; i < dim; i++) {
		tabela.vector[i] = NULL;
	}
	return tabela;
}

int calculHash(int cod, const char* nume, int dim) {
	if (dim > 0) {
		int rez = cod * strlen(nume);
		return rez % dim;
	}
	return -1;
}

//pe cheie de tip string
//unsigned char calcul_hash(char* cheie, unsigned char dim) {
//	unsigned char suma = 0;
//	for (unsigned char i = 0; i < strlen(cheie); i++) {
//		suma += cheie[i];
//	}
//	return (unsigned char)(suma % dim);
//}

//int pozHash(char* cheie, int dim) {
//	int suma = 0;
//	for (int i = 0; i < strlen(cheie); i++) {
//		suma += cheie[i];
//	}
//	return suma % dim;
//}

void inserareInceput(Nod** lista, Galerie g) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = g;
	nou->next = *lista;
	*lista = nou;
}

void inserareFinal(Nod** lista, Galerie g) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = g;
	nou->next = NULL;
	if (*lista) {
		Nod* aux = *lista;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*lista = nou;
	}
}

void inserareGalerieInTabela(HashTable tabela, Galerie g) {
	if (tabela.dim > 0) {
		int pozitie = calculHash(g.cod, g.nume, tabela.dim);
		inserareFinal(&(tabela.vector[pozitie]), g);
	}
}

void afisareGalerie(Galerie g) {
	printf("%d %s %3.2f\n", g.cod, g.nume, g.pretIntrare);
}

void afisareLista(Nod* lista) {
	while (lista) {
		afisareGalerie(lista->info);
		lista = lista->next;
	}
}

void afisareHashTable(HashTable tabela) {
	for (int i = 0; i < tabela.dim; i++) {
		if (tabela.vector[i] != NULL) {
			printf("%d\n", i + 1);
			afisareLista(tabela.vector[i]);
		}
		printf("\n");
	}
}

void citesteDinFisier(const char* numeFisier, HashTable* tabela) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f != NULL) {
			char linie[100];
			char delimitator[] = ",\n";
			while (fgets(linie, 100, f)) {
				char* token = strtok(linie, delimitator);
				Galerie g;
				g.cod = atoi(token);
				token = strtok(NULL, delimitator);
				g.nume = (char*)malloc(sizeof(char) * (strlen(token) + 1));
				strcpy(g.nume, token);
				token = strtok(NULL, delimitator);
				g.pretIntrare = atof(token);

				inserareGalerieInTabela(*tabela, g);
			}
			fclose(f);
		}
	}
}

Galerie cautaGalerieDupaCheie(int cod, const char* nume, HashTable tabela) {
	int pozitie = calculHash(cod, nume, tabela.dim);
	if (pozitie >= 0 && pozitie <= tabela.dim) {
		Nod* p = tabela.vector[pozitie];
		while (p && p->info.cod != cod && strcmp(p->info.nume, nume) != 0) {
			p = p->next;
		}
		if (p && p->info.cod == cod && strcmp(p->info.nume, nume) == 0) {
			return p->info;
		}
		else {
			return initGalerie(-1, "N/A", 0);
		}
	}
	return initGalerie(-1, "N/A", 0);
}

void dezalocareHash(HashTable* tabela) {
	for (int i = 0; i < tabela->dim; i++) {
		Nod* curent = tabela->vector[i];
		while (curent) {
			Nod* temp = curent;
			curent = curent->next;
			free(temp->info.nume);
			free(temp);
		}
	}
	free(tabela->vector);
	tabela->vector = NULL;
	tabela->dim = 0;
}

void main() {
	HashTable tabela = initHashTable(3);
	citesteDinFisier("galerii.txt", &tabela);
	afisareHashTable(tabela);

	Galerie g7 = cautaGalerieDupaCheie(1, "Valeleu", tabela);
	afisareGalerie(g7);

	dezalocareHash(&tabela);
}