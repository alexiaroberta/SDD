#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Avion {
	int nrPasageri;
	char* companie;
	float pretBilet;
};

struct Avion initializareAvion(int nrPasageri, const char* companie, float pretBilet) {
	struct Avion a;
	a.nrPasageri = nrPasageri;
	a.companie = (char*)malloc(sizeof(char) * (strlen(companie) + 1));
	strcpy(a.companie, companie);
	a.pretBilet = pretBilet;
	return a;
}

void afisareAvion(struct Avion avion) {
	printf("%s are %d pasageri si pretul biletului de %3.2f RON\n", avion.companie, avion.nrPasageri, avion.pretBilet);
}

void afisareVectorAvioane(int nrAvioane,struct Avion* avioane) {
	if (avioane != NULL) {
		for (int i = 0; i < nrAvioane; i++) {
			afisareAvion(avioane[i]);
		}
	}
}

void dezalocareAvioane(struct Avion** avioane, int* nrAvioane) {
	if ((*avioane) != NULL) {
		for (int i = 0; i < *nrAvioane; i++) {
			free((*avioane)[i].companie);
		}
		free(*avioane);
		*avioane = NULL;
		*nrAvioane = 0;
	}
}

struct Avion getPrimulAvionDupaCompanie(struct Avion* avioane, int nrAvioane, const char* companie) {
	if (avioane != NULL && companie != NULL) {
		for (int i = 0; i < nrAvioane; i++) {
			if (strcmp(avioane[i].companie, companie) == 0) {
				return initializareAvion(avioane[i].nrPasageri, avioane[i].companie, avioane[i].pretBilet);
			}
		}
	}
	return initializareAvion(0, "N/A", 0);
}

struct Avion* copiazaPrimeleNAvioane(struct Avion* avioane, int nrAvioane, int nrAvioaneCopiate) {
	if (nrAvioaneCopiate > 0 && nrAvioaneCopiate < nrAvioane) {
		struct Avion* avioaneCopiate = (struct Avion*)malloc(sizeof(struct Avion) * nrAvioaneCopiate);
		for (int i = 0; i < nrAvioaneCopiate; i++) {
			avioaneCopiate[i] = initializareAvion(avioane[i].nrPasageri, avioane[i].companie, avioane[i].pretBilet);
		}
		return avioaneCopiate;
	}
	else {
		return NULL;
	}
}

void copiazaAvioaneCuBileteReduse(struct Avion* avioane, int nrAvioane, float pragPret, struct Avion** avioaneNou, int* dim) {
	if(*avioaneNou != NULL) {
		dezalocareAvioane(avioaneNou, dim);
	}
	else {
		*dim = 0;
	}
	for (int i = 0; i < nrAvioane; i++) {
		if (avioane[i].pretBilet < pragPret) {
			(*dim)++;
		}
	}
	*avioaneNou = (struct Avion*)malloc(sizeof(struct Avion) * (*dim));
	int k = 0;
	for (int i = 0; i < nrAvioane; i++) {
		if (avioane[i].pretBilet < pragPret) {
			(*avioaneNou)[k++] = initializareAvion(avioane[i].nrPasageri, avioane[i].companie, avioane[i].pretBilet);
		}
	}
}

void main() {
	int nrAvioane = 5;
	struct Avion* avioane = (struct Avion*)malloc(sizeof(struct Avion) * nrAvioane);
	for (int i = 0; i < nrAvioane; i++) {
		avioane[i] = initializareAvion(i + 50, "Tarom", 10 * i + 120);
	}
	afisareVectorAvioane(nrAvioane, avioane);

	printf("\n\n");
	int nrAvioaneCopiate = 3;
	struct Avion* avioaneCopiate = copiazaPrimeleNAvioane(avioane, nrAvioane, nrAvioaneCopiate);
	afisareVectorAvioane(nrAvioaneCopiate, avioaneCopiate);

	printf("\n");
	struct Avion avion = getPrimulAvionDupaCompanie(avioane, nrAvioane, "Tarom");
	afisareAvion(avion);
	free(avion.companie);

	printf("\n--");
	struct Avion* avioaneCuBiletRedus = NULL;
	int dim = 0;
	copiazaAvioaneCuBileteReduse(avioane, nrAvioane, 160, &avioaneCuBiletRedus, &dim);
	afisareVectorAvioane(dim,avioaneCuBiletRedus);
	dezalocareAvioane(&avioaneCuBiletRedus, &dim);

	dezalocareAvioane(&avioane, &nrAvioane);
	dezalocareAvioane(&avioaneCopiate, &nrAvioaneCopiate);
}