#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Camion Camion;
typedef struct NodSecundar NodSecundar;
typedef struct NodPrincipal NodPrincipal;
typedef struct Nod Nod;

struct Camion {
	int serie;
	char* marca;
	float greutate;
};

struct NodSecundar {
	NodPrincipal* nod;
	NodSecundar* next;
};

struct NodPrincipal {
	Camion info;
	NodSecundar* vecini;
	NodPrincipal* next;
};

struct Nod {
	int info;
	Nod* next;
};

NodPrincipal* inserarePrincipal(NodPrincipal* graf, Camion c) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->info = c;
	nou->next = graf;
	nou->vecini = NULL;
	return nou;
}

NodPrincipal* cautaNodDupaSerie(NodPrincipal* graf, int serie) {
	while (graf != NULL && graf->info.serie != serie) {
		graf = graf->next;
	}
	return graf;
}

void inserareSecundara(NodSecundar** lista, NodPrincipal* nod) {
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nou->nod = nod;
	nou->next = NULL;
	if (*lista) {
		NodSecundar* aux = *lista;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*lista = nou;
	}
}

void inserareMuchie(NodPrincipal* graf, int serieStart, int serieStop) {
	NodPrincipal* nodStart = cautaNodDupaSerie(graf, serieStart);
	NodPrincipal* nodStop = cautaNodDupaSerie(graf, serieStop);

	inserareSecundara(&(nodStart)->vecini, nodStop);
	inserareSecundara(&(nodStop)->vecini, nodStart);
}


Camion initCamion(int serie, const char* marca, float greutate) {
	Camion c;
	c.serie = serie;
	c.marca = (char*)malloc(sizeof(char) * (strlen(marca) + 1));
	strcpy(c.marca, marca);
	c.greutate = greutate;
	return c;
}

int calculNrNoduri(NodPrincipal* graf) {
	int nr = 0;
	while (graf != NULL) {
		nr++;
		graf = graf->next;
	}
	return nr;
}

void pushCoada(Nod** coada, int seria) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = seria;
	nod->next = NULL;
	if (*coada) {
		Nod* aux = *coada;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nod;
	}
	else {
		*coada = nod;
	}
}

void pushStiva(Nod** stiva, int seria) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = seria;
	nod->next = *stiva;
	*stiva = nod;
}

int popCoada(Nod** coada) {
	if (*coada) {
		int rez = (*coada)->info;
		Nod* aux = *coada;
		*coada = (*coada)->next;
		free(aux);
		return rez;
	}
	return -1;
}

int popStiva(Nod** stiva) {
	return popCoada(stiva);
}

void afisareCamion(Camion c) {
	printf("%d %s %3.2f\n", c.serie, c.marca, c.greutate);
}

void stergereListaVecini(NodSecundar** vecini) {
	while (*vecini) {
		NodSecundar* aux = *vecini;
		*vecini = (*vecini)->next;
		free(aux);
	}
}

void stergereGraf(NodPrincipal** graf) {
	while (*graf) {
		NodPrincipal* aux = *graf;
		free(aux->info.marca);
		stergereListaVecini(&(aux->vecini));
		*graf = aux->next;
		free(aux);
	}
}

//parcuerge in latime - coada
void parcurgereInLatime(NodPrincipal* graf, int serieStart) {
	if (graf) {
		Nod* coada = NULL;
		int nrNoduri = calculNrNoduri(graf);
		char* vizitat = (char*)malloc(sizeof(char) * nrNoduri);
		for (int i = 0; i < nrNoduri; i++) {
			vizitat[i] = 0;
		}
		vizitat[serieStart] = 1;
		pushCoada(&coada, serieStart);
		while (coada) {
			int seriaCurenta = popCoada(&coada);
			NodPrincipal* nodCurent = cautaNodDupaSerie(graf, seriaCurenta);
			afisareCamion(nodCurent->info);
			NodSecundar* vecini = nodCurent->vecini;
			while (vecini) {
				if (vizitat[vecini->nod->info.serie] == 0) {
					pushCoada(&coada, vecini->nod->info.serie);
					vizitat[vecini->nod->info.serie] = 1;
				}
				vecini = vecini->next;
			}
		}
		if (vizitat) {
			free(vizitat);
		}
	}
}

//parcurgere in adancime - stiva
void parcurgereInAdancime(NodPrincipal* graf, int serieStart) {
	if (graf) {
		Nod* stiva = NULL;
		int nrNoduri = calculNrNoduri(graf);
		char* vizitat = (char*)malloc(sizeof(char) * nrNoduri);
		for (int i = 0; i < nrNoduri; i++) {
			vizitat[i] = 0;
		}
		vizitat[serieStart] = 1;
		pushStiva(&stiva, serieStart);
		while (stiva) {
			int seriaCurenta = popStiva(&stiva);
			NodPrincipal* nodCurent = cautaNodDupaSerie(graf, seriaCurenta);
			afisareCamion(nodCurent->info);
			NodSecundar* vecini = nodCurent->vecini;
			while (vecini) {
				if (vizitat[vecini->nod->info.serie] == 0) {
					pushStiva(&stiva, vecini->nod->info.serie);
					vizitat[vecini->nod->info.serie] = 1;
				}
				vecini = vecini->next;
			}
		}
		if (vizitat) {
			free(vizitat);
		}
	}
}

void parcurgeread(NodPrincipal* graf, int serieStart) {
	if (graf) {
		Nod* stiva = NULL;
		int nrNoduri = calculNrNoduri(graf);
		char* vizitat = (char*)malloc(sizeof(char) * nrNoduri);
		for (int i = 0; i < nrNoduri; i++) {
			vizitat[i] = 0;
		}
		vizitat[serieStart] = 1;
		pushStiva(&stiva, serieStart);
		while (stiva) {
			int seriaCurenta = popStiva(&stiva);
			NodPrincipal* nodCurent = cautaNodDupaSerie(graf, seriaCurenta);
			afisareCamion(nodCurent->info);
			NodSecundar* vecini = nodCurent->vecini;
			while (vecini) {
				if (vizitat[vecini->nod->info.serie] == 0) {
					pushStiva(&stiva, vecini->nod->info.serie);
					vizitat[vecini->nod->info.serie] = 1;
				}
				vecini = vecini->next;
			}
		}
		if (vizitat) {
			free(vizitat);
		}
	}
}

void main() {
	NodPrincipal* graf = NULL;

	graf = inserarePrincipal(graf, initCamion(4, "Scania", 20));
	graf = inserarePrincipal(graf, initCamion(3, "Volvo", 30));
	graf = inserarePrincipal(graf, initCamion(2, "Renault", 25));
	graf = inserarePrincipal(graf, initCamion(1, "MAN", 15));
	graf = inserarePrincipal(graf, initCamion(0, "Mercedes", 40));

	inserareMuchie(graf, 0, 1);
	inserareMuchie(graf, 1, 2);
	inserareMuchie(graf, 0, 3);
	inserareMuchie(graf, 2, 4);
	inserareMuchie(graf, 3, 4);
	inserareMuchie(graf, 1, 3);

	parcurgereInLatime(graf, 4);
	printf("\n\n");
	parcurgereInAdancime(graf, 4);

	stergereGraf(&graf);
}