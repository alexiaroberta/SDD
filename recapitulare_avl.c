#define _CRT_SECURE_NO_WARNININGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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

void calculGradEchilbru(Nod* radacina) {
	if (radacina) {
		radacina->gradEchilibru = calculInaltime(radacina->dr) - calculInaltime(radacina->st);
	}
}

Nod* rotireSimplaDreapta(Nod* pivot, Nod* fiuSt) {
	pivot->st = fiuSt->dr;
	calculGradEchilbru(pivot);
	fiuSt->dr = pivot;
	calculGradEchilbru(fiuSt);

	return fiuSt;
}

Nod* rotireSimplaStanga(Nod* pivot, Nod* fiuDr) {
	pivot->dr = fiuDr->st;
	calculGradEchilbru(pivot);
	fiuDr->st = pivot;
	calculGradEchilbru(fiuDr);

	return fiuDr;
}

Nod* rotireDublaStangaDreapta(Nod* pivot, Nod* fiuSt) {
	pivot->st = rotireSimplaStanga(fiuSt, fiuSt->dr);
	calculGradEchilbru(pivot);
	fiuSt = pivot->st;
	fiuSt = rotireSimplaDreapta(pivot, fiuSt);
	calculGradEchilbru(fiuSt);

	return fiuSt;
}

Nod* rotireDublaDreaptaStanga(Nod* pivot, Nod* fiuDr) {
	pivot->dr = rotireSimplaDreapta(fiuDr, fiuDr->st);
	calculGradEchilbru(pivot);
	fiuDr = pivot->dr;
	fiuDr = rotireSimplaStanga(pivot, fiuDr);
	calculGradEchilbru(fiuDr);

	return fiuDr;
}

Nod* inserareInArbore(Nod* radacina, Student s, int* inserat) {
	if (radacina) {
		if (radacina->info.id > s.id) {
			radacina->st = inserareInArbore(radacina->st, s, inserat);
		}
		else {
			if (radacina->info.id < s.id) {
				radacina->dr = inserareInArbore(radacina->dr, s, inserat);
			}
			else {
				*inserat = 0;
			}
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = s;
		nou->st = NULL;
		nou->dr = NULL;

		radacina = nou;
		*inserat = 1;
	}
	calculGradEchilbru(radacina);
	if (radacina->gradEchilibru == 2) {
		if (radacina->dr->gradEchilibru == -1) {
			radacina = rotireDublaDreaptaStanga(radacina, radacina->dr);
		}
		else {
			radacina = rotireSimplaStanga(radacina, radacina->dr);
		}
	}
	else {
		if (radacina->gradEchilibru == -2) {
			if (radacina->st->gradEchilibru == 1) {
				radacina = rotireDublaStangaDreapta(radacina, radacina->st);
			}
			else {
				radacina = rotireSimplaDreapta(radacina, radacina->st);
			}
		}
	}
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
					free(s.nume);
				}
			}
			fclose(f);
		}
	}
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
}