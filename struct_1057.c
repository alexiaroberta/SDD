#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Cofetarie {
	char* nume;
	int nrAngajati;
	float* salarii;
	char** numeAngajati;
};

struct Cofetarie initCofetarie(const char* nume, int nrAngajati, const float* salarii, const char** numeAngajati) {
	struct Cofetarie c;
	c.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(c.nume, nume);
	c.nrAngajati = nrAngajati;
	c.salarii = (float*)malloc(sizeof(float) * nrAngajati);
	c.numeAngajati = (char**)malloc(sizeof(char*) * nrAngajati);
	for (int i = 0; i < c.nrAngajati; i++) {
		c.salarii[i] = salarii[i];
		c.numeAngajati[i] = (char*)malloc(sizeof(char) * (strlen(numeAngajati[i]) + 1));
		strcpy(c.numeAngajati[i], numeAngajati[i]);
	}
	return c;
}

void afiseazaCofetarie(struct Cofetarie c) {
	printf("Nume: %s\n", c.nume);
	printf("Nr angajati: %d\n", c.nrAngajati);
	if (c.nrAngajati > 0 && c.salarii != NULL) {
		for (int i = 0; i < c.nrAngajati; i++) {
			printf("Salarii: %3.2f\n", c.salarii[i]);
			printf("Nume angajati: %s\n", c.numeAngajati[i]);
		}
	}
}

void dezalocareCofetarie(struct Cofetarie* c) {
	free(c->nume);
	(*c).nume = NULL;
	free(c->salarii);
	(*c).salarii = NULL;
	for (int i = 0; i < c->nrAngajati; i++) {
		free(c->numeAngajati[i]);
	}
	free(c->numeAngajati);
	c->numeAngajati = NULL;
	c->nrAngajati = 0;
}

void main() {
	float salarii[] = {1200, 1300};
	char* nume[] = { "Cherry","Merry" };
	struct Cofetarie c = initCofetarie("Amara", 2, salarii, nume);
	afiseazaCofetarie(c);
	dezalocareCofetarie(&c);
}

