//LIFO - stiva SI FIFO - coada
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Student Student;
typedef struct Nod Nod;
typedef struct NodStiva NodStiva;

struct Student {
	int id;
	char* nume;
	float medie;
};

struct Nod {
	Student s;
	Nod* next;
};

struct NodStiva {
	Nod* nodListaSimpla;
	NodStiva* next;
};

//la coada se face inserarea la final
NodStiva* push(NodStiva* stiva, Nod* listaNod) {
	NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
	nou->nodListaSimpla = listaNod;
	nou->next = stiva;

	return nou;
}

//la fel la coada
Nod* pop(NodStiva** stiva) {
	if (*stiva) {
		Nod* temp = (*stiva)->nodListaSimpla;
		NodStiva* tempNodStiva = *stiva;
		*stiva = (*stiva)->next;

		free(tempNodStiva);
		return temp;
	}
	return NULL;
}

Nod* inserareInLista(Nod* lista, Student s) {
	Nod* nou = malloc(sizeof(Nod));
	nou->s = s;
	nou->next = lista;

	return nou;
}

void traversareLista(Nod* lista) {
	while (lista != NULL) {
		printf("%d %s %3.2f\n", lista->s.id, lista->s.nume, lista->s.medie);
		lista = lista->next;
	}
}

void traversareStiva(NodStiva* stiva) {
	while (stiva != NULL) {
		printf("%d %s %3.2f\n", stiva->nodListaSimpla->s.id, stiva->nodListaSimpla->s.nume, stiva->nodListaSimpla->s.medie);
		stiva = stiva->next;
	}
}

void dezalocareListaSimpla(Nod** lista) {
	Nod* aux = *lista;
	while (aux) {
		Nod* temp = aux->next;
		free(aux->s.nume);
		free(aux);
		aux = NULL;
		aux = temp;
	}
}

void citesteDinFisier(const char* numeFisier, Nod** lista) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f != NULL) {
			char linie[100];
			char delimitator[] = ",\n";
			while (fgets(linie, 100, f)) {
				char* token = strtok(linie, delimitator);
				Student s;
				s.id = atoi(token);
				token = strtok(NULL, delimitator);
				s.nume = (char*)malloc(sizeof(char) * (strlen(token) + 1));
				strcpy(s.nume, token);
				token = strtok(NULL, delimitator);
				s.medie = atof(token);

				*lista = inserareInLista(*lista, s);
			}
			fclose(f);
		}
	}
}


void main(){
	NodStiva* stiva = NULL;
	Nod* lista = NULL;
	citesteDinFisier("studenti.txt", &lista);
	traversareLista(lista);
	printf("\n\n");
	Nod* temp = lista;
	while (temp) {
		stiva = push(stiva, temp);
		temp = temp->next;
	}
	traversareStiva(stiva);
	dezalocareListaSimpla(&lista);
}