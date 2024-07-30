#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Student {
	int id;
	char* nume;
	float avg;
};

int calculHash(char* nume, int dimensiune) {
	int sum = 0;
	for (int i = 0; i < strlen(nume); i++) {
		sum += nume[i];
	}
	return sum % dimensiune;
}

char inserareStudent(struct Student* ht, int dim, struct Student stud) {
	int pozitie = calculHash(stud.nume, dim);
	char inserare = 0;
	for (int i = pozitie; i < dim && !inserare; i++) {
		if (ht[i].nume == NULL) {
			ht[i] = stud;
			inserare = 1;
		}
	}
	return inserare;
}

int cautareStudent(struct Student* ht, int dim, char* nume) {
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

void citesteFisier(const char* numeFisier, struct Student* hashTable) {
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
				char insert = inserareStudent(hashTable, dim, s);
				int noua_dim = dim;

				while (!insert) {
					struct Student* hash;
					noua_dim += dim;
					hash = (struct Student*)malloc(sizeof(struct Student) * noua_dim);

					for (int i = 0; i < noua_dim; i++) {
						hash[i].nume = NULL;
					}

					insert = 1;

					for (int i = 0; i < dim && insert; i++) {
						if (hashTable[i].nume) {
							insert = inserareStudent(hash, noua_dim, hashTable[i]);
						}
					}

					if (!insert) {
						free(hash);
					}
					else {
						free(hashTable);

						hashTable = hash;
						dim = noua_dim;

						insert = inserareStudent(hashTable, dim, s);
					}
				}
			}
			fclose(f);
		}
	}
}

void main() {
	struct Student* hashTable;
	int dim = 100;
	hashTable = (struct Student*)malloc(sizeof(struct Student) * dim);
	for (int i = 0; i < dim; i++) {
		hashTable[i].nume = NULL;
	}
	citesteFisier("studenti.txt", hashTable);
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