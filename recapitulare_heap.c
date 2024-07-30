#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Quiz Quiz;
typedef struct Heap Heap;

struct Quiz {
	char* materie;
	int grileGresite;
};

struct Heap {
	Quiz* vector;
	int dimensiune;
	int dimensiuneTotala;
};

Quiz initQuiz(const char* materie, int grileGresite) {
	Quiz q;
	q.materie = (char*)malloc(sizeof(char) * (strlen(materie) + 1));
	strcpy(q.materie, materie);
	q.grileGresite = grileGresite;
	return q;
}

Heap initHeap(int nr) {
	Heap heap;
	heap.dimensiune = 0;
	heap.dimensiuneTotala = nr;
	heap.vector = (Quiz*)malloc(sizeof(Quiz) * nr);
	return heap;
}

void afisareQuiz(Quiz q) {
	printf("%s %d\n", q.materie, q.grileGresite);
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.dimensiune; i++) {
		afisareQuiz(heap.vector[i]);
	}
}

void afisareHeapTotala(Heap heap) {
	for (int i = 0; i < heap.dimensiuneTotala; i++) {
		afisareQuiz(heap.vector[i]);
	}
}

void filtreazaHeapMin(Heap heap, int pozRadacina) {
	int nodStanga = 2 * pozRadacina + 1;
	int nodDreapta = 2 * pozRadacina + 2;
	int pozMin = pozRadacina;
	if (nodStanga < heap.dimensiune && heap.vector[nodStanga].grileGresite < heap.vector[pozMin].grileGresite) {
		pozMin = nodStanga;
	}
	if (nodDreapta < heap.dimensiune && heap.vector[nodDreapta].grileGresite < heap.vector[pozMin].grileGresite) {
		pozMin = nodDreapta;
	}
	if (pozMin != pozRadacina) {
		Quiz aux = heap.vector[pozRadacina];
		heap.vector[pozRadacina] = heap.vector[pozMin];
		heap.vector[pozMin] = aux;
		if (2 * pozMin + 1 < heap.dimensiune - 1) {
			filtreazaHeapMin(heap, pozMin);
		}
	}
}

void filtreazaHeapMax(Heap heap, int pozRadacina) {
	int nodStanga = 2 * pozRadacina + 1;
	int nodDreapta = 2 * pozRadacina + 2;
	int pozMax = pozRadacina;
	if (nodStanga < heap.dimensiune && heap.vector[nodStanga].grileGresite > heap.vector[pozMax].grileGresite) {
		pozMax = nodStanga;
	}
	if (nodDreapta < heap.dimensiune && heap.vector[nodDreapta].grileGresite > heap.vector[pozMax].grileGresite) {
		pozMax = nodDreapta;
	}
	if (pozMax != pozRadacina) {
		Quiz aux = heap.vector[pozRadacina];
		heap.vector[pozRadacina] = heap.vector[pozMax];
		heap.vector[pozMax] = aux;
		if (2 * pozMax + 1 < heap.dimensiune - 1) {
			filtreazaHeapMax(heap, pozMax);
		}
	}
}

Heap adaugaQuiz(Heap heap, Quiz q) {
	if (heap.dimensiune < heap.dimensiuneTotala) {
		heap.vector[heap.dimensiune] = q;
		heap.dimensiune++;
	}
	return heap;
}

void citesteDinFisier(const char* numeFisier, Heap* heap) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f) {
			char linie[256];
			char delimitator[] = ",\n";
			while (fgets(linie, sizeof(linie), f)) {
				char* token = strtok(linie, delimitator);
				Quiz q;
				q.materie = (char*)malloc(sizeof(char) * (strlen(token) + 1));
				strcpy(q.materie, token);
				token = strtok(NULL, delimitator);
				q.grileGresite = atoi(token);

				*heap = adaugaQuiz(*heap, q);
			}
			fclose(f);
			for (int i = (heap->dimensiune) / 2 - 1; i >= 0; i--) {
				filtreazaHeapMax(*heap, i);
			}
		}
	}
}

Quiz extragereMin(Heap* heap) {
	if (heap->dimensiune > 0) {
		Quiz aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->dimensiune - 1];
		heap->vector[heap->dimensiune - 1] = aux;
		heap->dimensiune--;
		for (int i = (heap->dimensiune) / 2 - 1; i >= 0; i--) {
			filtreazaHeapMin(*heap, i);
		}
		return aux;
	}
	return initQuiz("N/A", -1);
}

Quiz extragereMax(Heap* heap) {
	if (heap->dimensiune > 0) {
		Quiz aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->dimensiune - 1];
		heap->vector[heap->dimensiune - 1] = aux;
		heap->dimensiune--;
		for (int i = (heap->dimensiune) / 2 - 1; i >= 0; i--) {
			filtreazaHeapMax(*heap, i);
		}
		return aux;
	}
	return initQuiz("N/A", -1);
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->dimensiuneTotala; i++) {
		free(heap->vector[i].materie);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->dimensiune = 0;
	heap->dimensiuneTotala = 0;
}

void main() {

}