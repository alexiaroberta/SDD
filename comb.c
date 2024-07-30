#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Locuinta {
	int id;
	char* strada;
	int numar;
};

void inserareLocuinta(struct Locuinta** vector, int* dim, struct Locuinta l) {
	struct Locuinta* copie = (struct Locuinta*)malloc(sizeof(struct Locuinta) * (++(*dim)));
	for (int i = 0; i < (*dim) - 1; i++) {
		copie[i] = (*vector)[i];
	}
	copie[(*dim) - 1] = l;
	if ((*vector) != NULL) {
		free((*vector));
	}
	(*vector) = copie;
}

void citesteDinFisier(const char* numeFisier, struct Locuinta** locuinte,int* dim) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f != NULL) {
			char linie[100];
			char delimitator[] = ",\n";
			while (fgets(linie, 100, f)) {
				char* token = strtok(linie, delimitator);
				struct Locuinta l;
				l.id = atoi(token);
				token = strtok(NULL, delimitator);
				l.strada = (char*)malloc(sizeof(char) * (strlen(token) + 1));
				strcpy(l.strada, token);
				token = strtok(NULL, delimitator);
				l.numar = atoi(token);

				inserareLocuinta(locuinte, dim, l);
			}
			fclose(f);
		}
	}
}

void afisareLocuinta(struct Locuinta l) {
	printf("%d %s %d\n", l.id, l.strada, l.numar);
}

void afisareVector(struct Locuinta* l, int dim) {
	for (int i = 0; i < dim; i++) {
		afisareLocuinta(l[i]);
	}
}

void stergeLocuinta(struct Locuinta** locuinte, int* dim, int id) {
	char flag = 0;
	for (int i = 0; i < (*dim); i++) {
		if ((*locuinte)[i].id == id) {
			flag = 1;
			break;
		}
	}
	if (flag == 1) {
		struct Locuinta* copie = (struct Locuinta*)malloc(sizeof(struct Locuinta) * ((*dim) - 1));
		int j = 0;
		for (int i = 0; i < (*dim); i++) {
			if ((*locuinte)[i].id != id) {
				copie[j++] = (*locuinte)[i];
			}
		}
		(*dim)--;
		if ((*locuinte) != NULL) {
			free((*locuinte));
		}
		*locuinte = copie;
	}
}

void dezalocareVector(struct Locuinta** locuinte, int* dim) {
	for (int i = 0; i < (*dim); i++) {
		free((*locuinte)[i].strada);
	}
	free(*locuinte);
	*locuinte = NULL;
	*dim = 0;
}

void citesteMatriceDinFisier(const char* numeFisier, struct Locuinta** matrice, int nrLinii, int* nrColoane) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f != NULL) {
			char linie[100];
			char delimitator[] = ",\n";
			while (fgets(linie, 100, f)) {
				char* token = strtok(linie, delimitator);
				struct Locuinta l;
				l.id = atoi(token);
				token = strtok(NULL, delimitator);
				l.strada = (char*)malloc(sizeof(char) * (strlen(token) + 1));
				strcpy(l.strada, token);
				token = strtok(NULL, delimitator);
				l.numar = atoi(token);
				int pozitie = l.numar % 2;
				inserareLocuinta(&(matrice[pozitie]), &(nrColoane[pozitie]), l);

			}
			fclose(f);
		}
	}
}

void afisareMatrice(struct Locuinta** locuinte, int nrLinii, int* nrColoane) {
	for (int i = 0; i < nrLinii; i++) {
		for (int j = 0; j < nrColoane[i]; j++) {
			afisareLocuinta(locuinte[i][j]);
		}
		printf("\n");
	}
}

struct Locuinta cautareLocuintaDupaId(struct Locuinta** locuinte, int nrLinii, int* nrColoane, int idCautat) {
	for (int i = 0; i < nrLinii; i++) {
		for (int j = 0; j < nrColoane[i]; j++) {
			if (locuinte[i][j].id == idCautat) {
				return locuinte[i][j];
			}
		}
	}

	struct Locuinta loc;
	loc.id = -1;
	loc.strada = NULL;
	loc.numar = 0;
	return loc;
}

void dezalocareMatrice(struct Locuinta*** l, int* nrLinii, int** nrColoane) {
	for (int i = 0; i < (*nrLinii); i++) {
		for (int j = 0; j < (*nrColoane)[i]; j++) {
			free((*l)[i][j].strada);
			(*l)[i][j].strada = NULL;
		}
		free((*l)[i]);
	}
	free(*l);
	*l = NULL;
	(*nrLinii) = 0;
	free(*nrColoane);
	*nrColoane = NULL;
}

void main() {
	/*struct Locuinta* locuinte = NULL;
	int dim = 0;
	citesteDinFisier("locuinte.txt", &locuinte, &dim);
	afisareVector(locuinte, dim);

	printf("\n\n");
	stergeLocuinta(&locuinte, &dim, 22);
	afisareVector(locuinte, dim);

	dezalocareVector(&locuinte, &dim);*/

	int nrLinii;
	int* nrColoane;
	struct Locuinta** locuinte;

	nrLinii = 2;
	nrColoane = (int*)malloc(sizeof(int) * nrLinii);
	locuinte = (struct Locuinta**)malloc(sizeof(struct Locuinta*) * nrLinii);
	for (int i = 0; i < nrLinii; i++) {
		locuinte[i] = NULL;
		nrColoane[i] = 0;
	}
	citesteMatriceDinFisier("locuinte.txt", locuinte, nrLinii, nrColoane);
	afisareMatrice(locuinte, nrLinii, nrColoane);

	struct Locuinta loc = cautareLocuintaDupaId(locuinte, nrLinii, nrColoane, 13);
	afisareLocuinta(loc);

	dezalocareMatrice(&locuinte, &nrLinii, &nrColoane);
}