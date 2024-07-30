#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Haina {
	int marime;
	char* marca;
	float pret;
	char gen;
};

struct Haina initHaina(int marime, const char* marca, float pret, char gen) {
	struct Haina h;
	h.marime = marime;
	if (marca != NULL && strlen(marca) > 0) {
		h.marca = (char*)malloc(sizeof(char) * (strlen(marca) + 1));
		strcpy(h.marca, marca);
	}
	else {
		h.marca = NULL;
	}
	h.pret = pret;
	h.gen = gen;
	return h;
}

void afisareHaina(struct Haina h) {
	printf("Marime: %d\n", h.marime);
	if (h.marca != NULL) {
		printf("Marca: %s\n", h.marca);
	}
	printf("Pret: %3.2f\n", h.pret);
	printf("Gen: %c\n\n", h.gen);
}

void dezalocareHaina(struct Haina* h) {
	free(h->marca);
	(*h).marca = NULL;
}

void main() {
	struct Haina haina1;
	haina1.marime = 33;
	haina1.marca = (char*)malloc(sizeof(char) * (strlen("Nike") + 1));
	strcpy(haina1.marca, "Nike");
	haina1.pret = 45.3;
	haina1.gen = 'M';
	afisareHaina(haina1);

	struct Haina haina2 = initHaina(27, "Adidas", 34.2, 'F');
	afisareHaina(haina2);

	dezalocareHaina(&haina1);
	dezalocareHaina(&haina2);

	free(haina1.marca);
	haina1.marca = NULL;
}