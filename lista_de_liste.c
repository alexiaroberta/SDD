#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	ListaSecundara* nou = malloc(sizeof(ListaSecundara));
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

ListaPrincipala* nodDupaCod(unsigned int codDepartament, ListaPrincipala* listaPrincipala) {
	ListaPrincipala* aux = listaPrincipala;
	while (aux != NULL && aux->codDepartament != codDepartament) {
		aux = aux->next;
	}
	return aux;
}

void inserareListaPrincipala(ListaPrincipala** listaPrincipala, Angajat a) {
	ListaPrincipala* nod = malloc(sizeof(ListaPrincipala));
	nod->codDepartament = a.codDepartament;
	nod->listaDepartamente = NULL;
	nod->next = NULL;
	if (*listaPrincipala == NULL) {
		*listaPrincipala = nod;
		inserareListaSecundara(&(*listaPrincipala)->listaDepartamente, a);
	}
	else {
		if (nodDupaCod(a.codDepartament, *listaPrincipala) == NULL) {
			ListaPrincipala* aux = *listaPrincipala;
			while (aux->next != NULL) {
				aux = aux->next;
			}
			aux->next = nod;
			inserareListaSecundara(&(nod->listaDepartamente), a);
		}
		else {
			inserareListaSecundara(&(nodDupaCod(a.codDepartament, *listaPrincipala)->listaDepartamente),a);
		}
	}
}

void citesteDinFisier(const char* numeFisier, ListaPrincipala** listaPrincipala) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen("angajati.txt", "r");
		if (f) {
			char linie[100];
			char delimitator[] = ",\n";
			while (fgets(linie, 100, f)) {
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
		free(aux->info.dataAngajare);
		free(aux->info.nume);
		free(aux);
		aux = NULL;
		aux = nod;
	}
}

void dezalocareListaPrincipala(ListaPrincipala** listaPrincipala) {
	ListaPrincipala* aux = *listaPrincipala;
	while (aux != NULL) {
		ListaPrincipala* nod = aux->next;
		dezalocareListaSecundara(&(aux->listaDepartamente));
		free(aux);
		aux = NULL;
		aux = nod;
	}
	*listaPrincipala = NULL;
}

Angajat* vectorAngajati(ListaPrincipala* listaPrincipala, float pragSalariu, int* nrAngajati) {
	*nrAngajati = 0;
	ListaPrincipala* aux = listaPrincipala;
	while (aux != NULL) {
		ListaSecundara* sec = aux->listaDepartamente;
		while (sec != NULL) {
			if (sec->info.salariu > pragSalariu) {
				(*nrAngajati)++;
			}
			sec = sec->next;
		}
		aux = aux->next;
	}
	if (*nrAngajati == 0) {
		return NULL;
	}
	else {
		Angajat* vector = (Angajat*)malloc(sizeof(Angajat) * (*nrAngajati));
		int k = 0;
		while (listaPrincipala != NULL) {
			ListaSecundara* temp = listaPrincipala->listaDepartamente;
			while (temp != NULL) {
				if (temp->info.salariu > pragSalariu) {
					vector[k++] = temp->info;
				}
				temp = temp->next;
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

	int nrAngajati = 0;
	Angajat* vector = NULL;
	vector = vectorAngajati(lista, 3000, &nrAngajati);
	for (int i = 0; i < nrAngajati; i++) {
		printf("\n\n%s %3.2f %s\n", vector[i].nume, vector[i].salariu, vector[i].dataAngajare);
	}

	dezalocareListaPrincipala(&lista);
	afisareListaPrincipala(lista);
}