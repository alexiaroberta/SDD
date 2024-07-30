#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student Student;
typedef struct Nod Nod;

struct Student {
	int id;
	char* nume;
	float medie;
};

struct Nod {
	Student info;
	int gradEchilibru;
	Nod* st;
	Nod* dr;
};

int calculInaltime(Nod* radacina) {
	if (radacina) {
		int inaltimeSt = calculInaltime(radacina->st);
		int inaltimeDr = calculInaltime(radacina->dr);
		if (inaltimeSt > inaltimeDr) {
			return 1 + inaltimeSt;
		}
		else {
			return 1 + inaltimeDr;
		}
	}
	else {
		return 0;
	}
}

void calculGradEchilibru(Nod* radacina) {
	if (radacina) {
		int inaltimeSubarboreDrept = calculInaltime(radacina->dr);
		int inaltimeSubarboreStang = calculInaltime(radacina->st);
		radacina->gradEchilibru = inaltimeSubarboreDrept - inaltimeSubarboreStang;
	}
}

//rotire simpla dreapta 
Nod* rotireSimplaDreapta(Nod* pivot, Nod* fiuSt) {
	pivot->st = fiuSt->dr;
	calculGradEchilibru(pivot);
	fiuSt->dr = pivot;
	calculGradEchilibru(fiuSt);

	return fiuSt;
}

//rotire simpla stanga
Nod* rotireSimplaStanga(Nod* pivot, Nod* fiuDr) {
	pivot->dr = fiuDr->st;
	calculGradEchilibru(pivot);
	fiuDr->st = pivot;
	calculGradEchilibru(fiuDr);

	return fiuDr;
}

//rotire dubla stanga dreapta
Nod* rotireDublaStangaDreapta(Nod* pivot, Nod* fiuSt) {
	//aducerea dezechilbrului pe aceeasi directie
	pivot->st = rotireSimplaStanga(fiuSt, fiuSt->dr);
	calculGradEchilibru(pivot);
	fiuSt = pivot->st;
	//rotire propriu-zisa in pivot
	fiuSt = rotireSimplaDreapta(pivot, fiuSt);
	calculGradEchilibru(fiuSt);

	return fiuSt;
}

//rotire dubla dreapta stanga
Nod* rotireDublaDreaptaStanga(Nod* pivot, Nod* fiuDr) {
	//aducerea dezechilibrului de acceasi directie
	pivot->dr = rotireSimplaDreapta(fiuDr, fiuDr->st);
	calculGradEchilibru(pivot);
	fiuDr = pivot->dr;
	//rotire propriu-zisa in pivot
	fiuDr = rotireSimplaStanga(pivot, fiuDr);
	calculGradEchilibru(fiuDr);

	return fiuDr;
}

Nod* inserareInArbore(Nod* radacina, Student s, int* inserare) {
	if (radacina) {
		if (radacina->info.id > s.id) {
			radacina->st = inserareInArbore(radacina->st, s, inserare);
		}
		else {
			if (radacina->info.id < s.id) {
				radacina->dr = inserareInArbore(radacina->dr, s, inserare);
			}
			else {
				*inserare = 0;
			}
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = s;
		nou->st = NULL;
		nou->dr = NULL;
		
		*inserare = 1;
		radacina = nou;
	}
	//calculez gradul de echilibru pt nodul curent
	calculGradEchilibru(radacina);
	if (radacina->gradEchilibru == 2) {
		if (radacina->dr->gradEchilibru == -1) {
			//dezechilibru dreapta stanga
			radacina = rotireDublaDreaptaStanga(radacina, radacina->dr);
		}
		else {
			//dezechilibru dreapta
			radacina = rotireSimplaStanga(radacina, radacina->dr);
		}
	}
	else {
		if (radacina->gradEchilibru == -2) {
			if (radacina->st->gradEchilibru == 1) {
				//dezechilibru stanga dreapta
				radacina = rotireDublaStangaDreapta(radacina, radacina->st);
			}
			else {
				//dezechilibru stanga
				radacina = rotireSimplaDreapta(radacina, radacina->st);
			}
		}
	}
	return radacina;
}

void citesteDinFisier(const char* numeFisier, Nod** arbore) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f != NULL) {
			char linie[256];
			char delimitator[] = ",\n";
			while (fgets(linie, 256, f)) {
				char* token = strtok(linie, delimitator);
				Student s;
				s.id = atoi(token);
				token = strtok(NULL, delimitator);
				s.nume = (char*)malloc(sizeof(char) * (strlen(token) + 1));
				strcpy(s.nume, token);
				token = strtok(NULL, delimitator);
				s.medie = atof(token);

				int inserat;
				*arbore = inserareInArbore(*arbore, s, &inserat);
				if (!inserat) {
					printf("Nodul %d nu a putut di inserat", s.id);
					free(s.nume);
				}
			}
			fclose(f);
		}
	}
}

//S-R-D
void traversareInordine(Nod* radacina) {
	if (radacina) {
		traversareInordine(radacina->st);
		printf("%d cu ge = %d\n", radacina->info.id, radacina->gradEchilibru);
		traversareInordine(radacina->dr);
	}
}

//R-S-D
void traversarePreordine(Nod* radacina) {
	if (radacina) {
		printf("%d cu ge = %d\n", radacina->info.id, radacina->gradEchilibru);
		traversareInordine(radacina->st);
		traversareInordine(radacina->dr);
	}
}

//S-D-R
void traversarePostordine(Nod* radacina) {
	traversareInordine(radacina->st);
	traversareInordine(radacina->dr);
	printf("%d cu ge = %d\n", radacina->info.id, radacina->gradEchilibru);
}

Nod* dezalocareArbore(Nod* radacina) {
	if (radacina) {
		radacina->st = dezalocareArbore(radacina->st);
		radacina->dr = dezalocareArbore(radacina->dr);
		free(radacina->info.nume);
		free(radacina);
		radacina = NULL;
	}
	return radacina;
}

void main() {
	Nod* arbore = NULL;
	citesteDinFisier("studenti.txt", &arbore);
	traversareInordine(arbore);
	printf("\n\n");
	traversarePreordine(arbore);
	printf("\n\n");
	traversarePostordine(arbore);
	printf("\n\n");

	arbore = dezalocareArbore(arbore);
	traversareInordine(arbore);
}