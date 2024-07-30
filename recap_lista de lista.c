#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Angajat Angajat;
typedef struct ListaPrincipala ListaPrincipala;
typedef struct ListaSecundara ListaSecundara;

struct Angajat {
	unsigned int codDepartament;
	char* nume;
	float salariu;
	char* dataAngajare;
};

struct ListaPrincipala {
	int codDepartament;
	ListaSecundara* listaDepartamente;
	ListaPrincipala* next;
};

struct ListaSecundara {
	Angajat info;
	ListaSecundara* next;
};

void inserareListaSecundara(ListaSecundara** listaSecundara, Angajat a) {
	ListaSecundara* nou = (ListaSecundara*)malloc(sizeof(ListaSecundara));
	nou->info = a;
	nou->next = NULL;
	if (*listaSecundara) {
		ListaSecundara* aux = *listaSecundara;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*listaSecundara = nou;
	}
}

ListaPrincipala* cautaNodDupaCod(unsigned int codDepartament, ListaPrincipala* listaPrincipala) {
	ListaPrincipala* aux = listaPrincipala;
	while (aux != NULL && aux->codDepartament != codDepartament) {
		aux = aux->next;
	}
	return aux;
}

void inserareListaPrincipala(ListaPrincipala** listaPrincipala, Angajat a) {
	ListaPrincipala* nou = (ListaPrincipala*)malloc(sizeof(ListaPrincipala));
	nou->codDepartament = a.codDepartament;
	nou->listaDepartamente = NULL;
	nou->next = NULL;
	if (*listaPrincipala == NULL) {
		*listaPrincipala = nou;
		inserareListaSecundara(&(*listaPrincipala)->listaDepartamente, a);
	}
	else {
		if (cautaNodDupaCod(a.codDepartament, *listaPrincipala) == NULL) {
			ListaPrincipala* aux = *listaPrincipala;
			while (aux->next != NULL) {
				aux = aux->next;
			}
			aux->next = nou;
			inserareListaSecundara(&(nou->listaDepartamente), a);
		}
		else {
			inserareListaSecundara(&(cautaNodDupaCod(a.codDepartament, *listaPrincipala)->listaDepartamente), a);
		}
	}
}

void citesteDinFisier(const char* numeFisier, ListaPrincipala** listaPrincipala) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f != NULL) {
			char linie[256];
			char delimitator[] = ",\n";
			while (fgets(linie, sizeof(linie), f)) {
				char* token = strtok(linie, delimitator);
				Angajat a;
				a.codDepartament = atoi(token);
				token = strtok(NULL, delimitator);
				a.nume = (char*)malloc(sizeof(char) * (strlen(token) + 1));
				strcpy(a.nume, token);
				token = strtok(NULL, delimitator);
				a.salariu = atof(token);
				token = strtok(NULL, delimitator);
				a.dataAngajare = (char*)malloc(sizeof(char) * (strlen(token) + 1));
				strcpy(a.dataAngajare, token);

				inserareListaPrincipala(listaPrincipala, a);
			}
			fclose(f);
		}
	}
}

void afisareListaSecundara(ListaSecundara* listaSecundara) {
	while (listaSecundara != NULL) {
		printf("%s %3.2f %s\n", listaSecundara->info.nume, listaSecundara->info.salariu, listaSecundara->info.dataAngajare);
		listaSecundara = listaSecundara->next;
	}
}

void afisareListaPrincipala(ListaPrincipala* listaPrincipala) {
	while (listaPrincipala != NULL) {
		printf("%d\n", listaPrincipala->codDepartament);
		afisareListaSecundara(listaPrincipala->listaDepartamente);
		listaPrincipala = listaPrincipala->next;
	}
}

void dezalocareListaSecundara(ListaSecundara** listaSecundara) {
	ListaSecundara* aux = *listaSecundara;
	while (aux != NULL) {
		ListaSecundara* nod = aux->next;
		free(aux->info.nume);
		free(aux->info.dataAngajare);
		free(aux);
		aux = nod;
	}
}

void dezalocareListaPrincipala(ListaPrincipala** listaPrincipala) {
	ListaPrincipala* aux = *listaPrincipala;
	while (aux != NULL) {
		ListaPrincipala* nod = aux->next;
		dezalocareListaSecundara(&(aux->listaDepartamente));
		free(aux);
		aux = nod;
	}
	*listaPrincipala = NULL;
}

Angajat* copiereInvector(ListaPrincipala* listaPrincipala, float salariu, int* nrAngajati) {
	*nrAngajati = 0;
	ListaPrincipala* aux = listaPrincipala;
	while (aux != NULL) {
		ListaSecundara* secund = aux->listaDepartamente;
		while (secund != NULL) {
			if (secund->info.salariu > salariu) {
				(*nrAngajati)++;
			}
			secund = secund->next;
		}
		aux = aux->next;
	}
	if (*nrAngajati == 0) {
		return NULL;
	}
	else {
		int k = 0;
		Angajat* vector = (Angajat*)malloc(sizeof(Angajat) * (*nrAngajati));
		while (listaPrincipala != NULL) {
			ListaSecundara* lista = listaPrincipala->listaDepartamente;
			while (lista != NULL) {
				if (lista->info.salariu > salariu) {
					vector[k++] = lista->info;
				}
				lista = lista->next;
			}
			listaPrincipala = listaPrincipala->next;
		}
		return vector;
	}
}

void main() {
	ListaPrincipala* lista = NULL;
	citesteDinFisier("angajati.txt", &lista);
	afisareListaPrincipala(lista);

	int nrAng = 0;
	Angajat* vector = NULL;
	vector = copiereInvector(lista, 3000, &nrAng);
	if (vector != NULL) {
		for (int i = 0; i < nrAng; i++) {
			printf("\n\n %s %3.2f %s\n", vector[i].nume, vector[i].salariu, vector[i].dataAngajare);
		}
	}
	else {
		printf("nu");
	}

	dezalocareListaPrincipala(&lista);
	afisareListaPrincipala(lista);
}