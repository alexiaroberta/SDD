#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student Student;

struct Student {
	int id;
	char* nume;
	float avg;
};

int calculHash(char* nume, int dimensiune) {
	int suma = 0;
	for (int i = 0; i < strlen(nume); i++) {
		suma += nume[i];
	}
	return suma % dimensiune;
}

char inserareStudent(Student* ht, int dim, Student st) {
	int pozitie = calculHash(st.nume, dim);
	char inserare = 0;
	for (int i = pozitie; i < dim && !inserare; i++) {
		if (ht[i].nume == NULL) {
			ht[i] = st;
			inserare = 1;
		}
	}
	return inserare;
}

int cautarePozStudent(Student* ht, int dim, char* nume) {
	int pozitie = calculHash(nume, dim);
	char gasit = 0;
	for (int i = pozitie; i < dim && !gasit; i++) {
		if (ht[i].nume == NULL) {
			gasit = -1;
		}
		else if (strcmp(ht[i].nume, nume) == 0) {
			gasit = 1;
			pozitie = i;
		}
	}
	if (gasit == 1) {
		return pozitie;
	}
	else {
		return -1;
	}
}

void citireDinFisier(const char* numeFisier, Student* hashTable) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f != NULL) {
			char linie[100];
			char delimitator[] = ",\n";
			while (fgets(linie, 100, f)) {
				char* token = strtok(linie, delimitator);
				struct Student s;
				s.id = atoi(token);
				token = strtok(NULL, delimitator);
				s.nume = (char*)malloc(sizeof(char) * (strlen(token) + 1));
				strcpy(s.nume, token);
				token = strtok(NULL, delimitator);
				s.avg = atof(token);

				int dim = 100;
				char inserat = inserareStudent(hashTable, dim, s);
				int noua_dim = dim;

				while (!inserat) {
					Student* hash;
					int noua_dim = dim + dim;
					hash = (Student*)malloc(sizeof(Student) * noua_dim);

					for (int i = 0; i < noua_dim; i++) {
						hash[i].nume = NULL;
					}

					inserat = 1;

					for (int i = 0; i < dim && inserat; i++) {
						if (hashTable[i].nume) {
							inserat = inserareStudent(hash, noua_dim, hashTable[i]);
						}
					}

					if (!inserat) {
						free(hash);
					}
					else {
						free(hashTable);
						hashTable = hash;
						dim = noua_dim;

						inserat = inserareStudent(hashTable, dim, s);
					}
				}
			}
			fclose(f);
		}
	}
}

void main() {
	Student* hashTable;
	int dim = 100;
	hashTable = (Student*)malloc(sizeof(Student) * dim);
	for (int i = 0; i < dim; i++) {
		hashTable[i].nume = NULL;
	}
    citireDinFisier("studenti.txt", hashTable);
	for (int i = 0; i < dim; i++) {
		if (hashTable[i].nume) {
			printf("pozitie :%d, %s\n", i, hashTable[i].nume);
		}
	}
	for (int i = 0; i < dim; i++) {
		if (hashTable[i].nume) {
			free(hashTable[i].nume);
		}
	}
	free(hashTable);
	hashTable = NULL;
}