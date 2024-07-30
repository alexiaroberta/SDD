#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cafenea Cafenea;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Cafenea {
	char* nume;
	int nrLocuri;
	float suprafata;
};

struct Nod {
	Cafenea info;
	Nod* anterior;
	Nod* urmator;
};

struct ListaDubla {
	Nod* cap;
	Nod* coada;
};

Cafenea initCafenea(const char* nume, int nrLocuri, float suprafata) {
	Cafenea c;
	c.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(c.nume, nume);
	c.nrLocuri = nrLocuri;
	c.suprafata = suprafata;
	return c;
}

void inserareInceput(ListaDubla* lista, Cafenea c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initCafenea(c.nume, c.nrLocuri, c.suprafata);
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

void inserareSfarsit(ListaDubla* lista, Cafenea c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initCafenea(c.nume, c.nrLocuri, c.suprafata);
	nou->anterior = lista->coada;
	nou->urmator = NULL;
	if (lista->coada != NULL) {
		lista->coada->urmator = nou;
	}
	else {
		lista->cap = nou;
	}
	lista->coada = nou;
}

void inserareSortata(ListaDubla* lista, Cafenea c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initCafenea(c.nume, c.nrLocuri, c.suprafata);
	if (lista->cap == NULL) {
		nou->anterior = NULL;
		nou->urmator = NULL;
		lista->cap = nou;
		lista->coada = nou;
	}
	else {
		if (lista->cap->info.nrLocuri > c.nrLocuri) {
			//inserare la inceput
			nou->anterior = NULL;
			nou->urmator = lista->cap;
			lista->cap->anterior = nou;
			lista->cap = nou;
		}
		else {
			Nod* aux = lista->cap;
			while (aux->urmator != NULL && aux->info.nrLocuri < c.nrLocuri) {
				aux = aux->urmator;
			}
			if (aux == lista->coada) {
				//inserare la sfarsit
				nou->anterior = lista->coada;
				nou->urmator = NULL;
				lista->coada->urmator = nou;
				lista->coada = nou;
			}
			else {
				nou->anterior = aux;
				nou->urmator = aux->urmator;
				aux->urmator->anterior = nou;
				aux->urmator = nou;
			}
		}
	}
}

void afisareCafenea(Cafenea c) {
	printf("%s %d %3.2f\n", c.nume, c.nrLocuri, c.suprafata);
}

void afisareLista(ListaDubla lista) {
	Nod* aux = lista.cap;
	while (aux != NULL) {
		afisareCafenea(aux->info);
		aux = aux->urmator;
	}
	printf("\n\n");
}

void afisareListaInvers(ListaDubla lista) {
	Nod* aux = lista.coada;
	while (aux != NULL) {
		afisareCafenea(aux->info);
		aux = aux->anterior;
	}
	printf("\n\n");
}

void citesteDinFisier(const char* numeFisier, ListaDubla* lista) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f != NULL) {
			char linie[256];
			char delimitator[] = ",\n";
			while (fgets(linie, 256, f)) {
				char* token = strtok(linie, delimitator);
				Cafenea c;
				c.nume = (char*)malloc(sizeof(char) * (strlen(token) + 1));
				strcpy(c.nume, token);
				token = strtok(NULL, delimitator);
				c.nrLocuri = atoi(token);
				token = strtok(NULL, delimitator);
				c.suprafata = atof(token);

				inserareSortata(lista, c);
			}
			fclose(f);
		}
	}
}

//nr total de locuri
int nrTotalLocuri(ListaDubla lista) {
	if (lista.cap == NULL) {
		return 0;
	}
	int nr = 0;
	Nod* aux = lista.cap;
	while (aux != NULL) {
		nr += aux->info.nrLocuri;
		aux = aux->urmator;
	}
	return nr;
}

void dezalocareListaDubla(ListaDubla* lista) {
	while (lista->cap != NULL) {
		Nod* aux = lista->cap;
		lista->cap = lista->cap->urmator;
		free(aux->info.nume);
		free(aux);
	}
	lista->cap = NULL;
	lista->coada = NULL;
}

ListaDubla stergeCafeneaDupaNume(ListaDubla lista, const char* nume) {
	if (nume == NULL) {
		return lista;
	}
	Nod* aux = lista.cap;
	while (aux && strcmp(aux->info.nume, nume) != 0) {
		aux = aux->urmator;
	}
	if (aux != NULL) {
		if (aux->anterior == NULL && aux->urmator == NULL) {
			lista.cap = NULL;
			lista.coada = NULL;
		}
		else {
			if (aux->anterior == NULL) {
				lista.cap = aux->urmator;
				lista.cap->anterior = NULL;
			}
			if (aux->urmator == NULL) {
				lista.coada = aux->anterior;
				lista.coada->urmator = NULL;
			}
			else {
				aux->anterior->urmator = aux->urmator;
				aux->urmator->anterior = aux->anterior;
			}
		}
		free(aux->info.nume);
		free(aux);
	}
	return lista;
}

Cafenea* copiazaInVector(ListaDubla lista, int pragLocuri, int* n) {
	*n = 0;
	Nod* aux = lista.cap;
	while (aux != NULL) {
		if (aux->info.nrLocuri >= pragLocuri) {
			(*n)++;
		}
		aux = aux->urmator;
	}
	if (*n == 0) {
		return NULL;
	}
	else {
		Cafenea* vector = (Cafenea*)malloc(sizeof(Cafenea) * (*n));
		int k = 0;
		while (lista.cap != NULL) {
			if (lista.cap->info.nrLocuri >= pragLocuri) {
				vector[k++] = initCafenea(lista.cap->info.nume, lista.cap->info.nrLocuri, lista.cap->info.suprafata);
			}
			lista.cap = lista.cap->urmator;
		}
		return vector;
	}
}

void dezalocareVector(Cafenea** c, int* dim) {
	if ((*c)!=NULL) {
		for (int i = 0; i < *dim; i++) {
			free((*c)[i].nume);
		}
		free(*c);
		*c = NULL;
		*dim = 0;
	}
}

//lista circulara : cap-> anterior = coada; coada->urmator = cap;
void inserareInceputListaCirculara(ListaDubla* lista, Cafenea c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initCafenea(c.nume, c.nrLocuri, c.suprafata);
	if (lista->cap != NULL) {
		nou->anterior = lista->coada;
		nou->urmator = lista->cap;
		lista->cap->anterior = nou;
		lista->coada->urmator = nou;
		lista->cap = nou;
	}
	else {
		nou->anterior = nou;
		nou->urmator = nou;
		lista->cap = nou;
		lista->coada = nou;
	}
}

void inserareSfarsitListaCirculara(ListaDubla* lista, Cafenea c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initCafenea(c.nume, c.nrLocuri, c.suprafata);
	if (lista->coada != NULL) {
		nou->anterior = lista->coada;
		nou->urmator = lista->cap;
		lista->coada->urmator = nou;
		lista->cap->anterior = nou;
		lista->coada = nou;
	}
	else {
		nou->anterior = nou;
		nou->urmator = nou;
		lista->cap = nou;
		lista->coada = nou;
	}
}

void inserareSortataListaCirculara(ListaDubla* lista, Cafenea c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initCafenea(c.nume, c.nrLocuri, c.suprafata);
	if (lista->cap == NULL) {
		nou->anterior = nou;
		nou->urmator = nou;
		lista->cap = nou;
		lista->coada = nou;
	}
	else {
		if (lista->cap->info.nrLocuri > c.nrLocuri) {
			nou->anterior = lista->coada;
			nou->urmator = lista->cap;
			lista->cap->anterior = nou;
			lista->coada->urmator = nou;
			lista->cap = nou;
		}
		else {
			Nod* aux = lista->cap;
			while (aux->urmator != lista->cap && aux->info.nrLocuri < c.nrLocuri) {
				aux = aux->urmator;
			}
			if (aux == lista->coada) {
				nou->anterior = lista->coada;
				nou->urmator = lista->cap;
				lista->cap->anterior = nou;
				lista->coada->urmator = nou;
				lista->coada = nou;
			}
			else {
				nou->anterior = aux;
				nou->urmator = aux->urmator;
				aux->urmator->anterior = nou;
				aux->urmator = nou;
			}
		}
	}
}

void afisareListaCirculara(ListaDubla lista) {
	Nod* aux = lista.cap;
	if (aux == NULL) {
		printf("NU");
	}
	else {
		do {
			afisareCafenea(aux->info);
			aux = aux->urmator;
		} while (aux != lista.cap);
	}
}

void afisareListaCircularaInvers(ListaDubla lista) {
	Nod* aux = lista.coada;
	if (aux == NULL) {
		printf("NU");
	}
	else {
		do {
			afisareCafenea(aux->info);
			aux = aux->anterior;
		} while (aux != lista.coada);
	}
}

void dezalocareListaCirculara(ListaDubla* lista) {
	Nod* temp = lista->cap;
	Nod* cap = temp;
	do {
		Nod* next = temp->urmator;
		free(temp->info.nume);
		free(temp);
		temp = next;
	} while (temp != cap);
	//NU UITA CAP SI COADA NULL!!!!!!!!!!!!!!!
	lista->cap = NULL;
	lista->coada = NULL;
}

void main() {
	ListaDubla lista;
	lista.cap = NULL;
	lista.coada = NULL;

	Cafenea cafenea1 = initCafenea("Tucano", 12, 20);
	Cafenea cafenea2 = initCafenea("Teds", 17, 17);
	Cafenea cafenea3 = initCafenea("Urban", 65, 33);
	Cafenea sb = initCafenea("Starbucks", 25, 43);

	inserareInceput(&lista, cafenea1);
	inserareInceput(&lista, cafenea2);
	inserareInceput(&lista, cafenea3);
	inserareSfarsit(&lista, sb);
	afisareLista(lista);
	afisareListaInvers(lista);

	ListaDubla listaFisier;
	listaFisier.cap = NULL;
	listaFisier.coada = NULL;
	citesteDinFisier("cafenele.txt", &listaFisier);
	afisareLista(listaFisier);

	int nrTotal = nrTotalLocuri(listaFisier);
	printf("\n%d\n", nrTotal);

	int nr;
	Cafenea* vector = copiazaInVector(lista, 20, &nr);
	if (vector != NULL) {
		for (int i = 0; i < nr; i++) {
			afisareCafenea(vector[i]);
		}
		dezalocareVector(&vector, &nr);
	}
	else {
		printf("Nu exista elemente");
	}

	printf("\n\n");
	lista = stergeCafeneaDupaNume(lista, "Teds");
	afisareLista(lista);

	dezalocareListaDubla(&lista);
	dezalocareListaDubla(&listaFisier);

	//lista circulara -> cap.anterior = coada, coada.urmator = cap
	ListaDubla listaCirculara;
	listaCirculara.cap = NULL;
	listaCirculara.coada = NULL;
	inserareInceputListaCirculara(&listaCirculara, cafenea1);
	inserareSfarsitListaCirculara(&listaCirculara, cafenea2);
	afisareListaCirculara(listaCirculara);
	printf("\n");
	afisareListaCircularaInvers(listaCirculara);

	dezalocareListaCirculara(&listaCirculara);
}