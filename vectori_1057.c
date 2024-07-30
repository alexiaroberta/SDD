#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Farmacie {
	int id;
	char* nume;
	float suprafata;
};

struct Farmacie initFarmacie(int id, const char* nume, float suprafata) {
	struct Farmacie f;
	f.id = id;
	f.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(f.nume, nume);
	f.suprafata = suprafata;
	return f;
}

void afisareFarmacie(struct Farmacie f) {
	printf("%d %s %3.2f\n", f.id, f.nume, f.suprafata);
}

void afisareVectorFarmacii(struct Farmacie* f, int nrFarmacii) {
	if (f != NULL) {
		for (int i = 0; i < nrFarmacii; i++) {
			afisareFarmacie(f[i]);
		}
	}
}

struct Farmacie* copiazaNElemente(struct Farmacie* farmacii, int nrFarmacii,int nrFarmaciiCopiate) {
	if (nrFarmaciiCopiate > 0 && nrFarmaciiCopiate < nrFarmacii) {
		struct Farmacie* farmCopiate = (struct Farmacie*)malloc(sizeof(struct Farmacie) * nrFarmaciiCopiate);
		for (int i = 0; i < nrFarmaciiCopiate; i++) {
			farmCopiate[i] = initFarmacie(farmacii[i].id, farmacii[i].nume, farmacii[i].suprafata);
		}
		return farmCopiate;
	}
	else {
		return NULL;
	}
}

struct Farmacie* copiazaFarmaciiMici(struct Farmacie* farmacii, int nrFarmacii, float prag, int* nrFarmaciiMici) {
	*nrFarmaciiMici = 0;
	for (int i = 0; i < nrFarmacii; i++) {
		if (farmacii[i].suprafata < prag) {
			(*nrFarmaciiMici)++;
		}
	}

	struct Farmacie* farmaciiMici = (struct Farmacie*)malloc(sizeof(struct Farmacie) * (*nrFarmaciiMici));
	int k = 0;
	for (int i = 0; i < nrFarmacii; i++) {
		if (farmacii[i].suprafata < prag) {
			farmaciiMici[k++] = initFarmacie(farmacii[i].id, farmacii[i].nume, farmacii[i].suprafata);
		}
	}

	return farmaciiMici;
}

struct Farmacie getFarmacieDupaId(struct Farmacie* farmacii, int nrFarmacii, int idCautat) {
	for (int i = 0; i < nrFarmacii; i++) {
		if (idCautat == farmacii[i].id) {
			return initFarmacie(farmacii[i].id, farmacii[i].nume, farmacii[i].suprafata);
		}
	}

	return initFarmacie(-1, "N/A", 0);
}

void dezalocareFarmacii(struct Farmacie** f, int* dim) {
	for (int i = 0; i < *dim; i++) {
		free((*f)[i].nume);
	}
	free(*f);
	*f = NULL;
	*dim = 0;
}

void main() {
	int nrFarmacii = 4;
	struct Farmacie* farmacii = (struct Farmacie*)malloc(sizeof(struct Farmacie) * nrFarmacii);
	for (int i = 0; i < nrFarmacii; i++) {
		farmacii[i] = initFarmacie(i, "Catena", (i + 1) * 80.5);
	}
	afisareVectorFarmacii(farmacii, nrFarmacii);

	printf("\n\n");
	int nrFarmaciiCopiate = 2;
	struct Farmacie* farmaciiCopiate = copiazaNElemente(farmacii, nrFarmacii, nrFarmaciiCopiate);
	afisareVectorFarmacii(farmaciiCopiate, nrFarmaciiCopiate);

	printf("\n\n");
	int nrFarmaciiMici = 0;
	struct Farmacie* farmaciiMici = copiazaFarmaciiMici(farmacii, nrFarmacii, 90, &nrFarmaciiMici);
	afisareVectorFarmacii(farmaciiMici, nrFarmaciiMici);

	struct Farmacie f = getFarmacieDupaId(farmacii, nrFarmacii, 3);
	printf("\n");
	afisareFarmacie(f);
	free(f.nume);

	dezalocareFarmacii(&farmacii, &nrFarmacii);
	dezalocareFarmacii(&farmaciiCopiate, &nrFarmaciiCopiate);
	dezalocareFarmacii(&farmaciiMici, &nrFarmaciiMici);
}