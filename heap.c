#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	heap.vector = (Quiz*)malloc(sizeof(Quiz) * nr);
	heap.dimensiune = 0;
	heap.dimensiuneTotala = nr;
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
	if (nodStanga < heap.dimensiune && heap.vector[nodStanga].grileGresite < heap.vector[pozMin].grileGresite) { //exista nod stanga si este mai mic decat radacina
		pozMin = nodStanga;
	}
	if (nodDreapta < heap.dimensiune && heap.vector[nodDreapta].grileGresite < heap.vector[pozMin].grileGresite) { //exista nod dreapta si este mai mic decat radacina
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

Heap adaugaQuiz(Heap heap, Quiz quiz) {
	if (heap.dimensiune < heap.dimensiuneTotala) {
		heap.vector[heap.dimensiune] = quiz;
		heap.dimensiune++;
	}
	return heap;
}

void citesteDinFisier(const char* numeFisier, Heap* heap) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f != NULL) {
			char linie[100];
			char delimitator[] = ",\n";
			while (fgets(linie, 100, f)) {
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
		Quiz aux;
		aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->dimensiune - 1];
		heap->vector[heap->dimensiune - 1] = aux;
		heap->dimensiune--;
		for (int i = (heap->dimensiune) / 2 - 1; i >= 0; i--) {
			filtreazaHeapMin(*heap, i);
		}
		return aux;
	}
	else {
		return initQuiz("N/A", -1);
	}
}

Quiz extragereMax(Heap* heap) {
	if (heap->dimensiune > 0) {
		Quiz aux;
		aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->dimensiune - 1];
		heap->vector[heap->dimensiune - 1] = aux;
		heap->dimensiune--;
		for (int i = (heap->dimensiune) / 2 - 1; i >= 0; i--) {
			filtreazaHeapMax(*heap, i);
		}
		return aux;
	}
	else {
		return initQuiz("N/A", -1);
	}
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
	Heap heap = initHeap(7);
	heap.vector[0] = initQuiz("Structuri de date", 3);
	heap.vector[1] = initQuiz("Java", 7);
	heap.vector[2] = initQuiz("Baze de date", 5);
	heap.vector[3] = initQuiz("ATP", 2);
	heap.vector[4] = initQuiz("POO", 9);
	heap.vector[5] = initQuiz("Analiza", 4);
	heap.dimensiune = 6;
	afisareHeap(heap);

	Quiz q = initQuiz("Paw", 1);
	heap = adaugaQuiz(heap, q);
	for (int i = (heap.dimensiune) / 2 - 1; i >= 0; i--) {
		filtreazaHeapMin(heap, i);
	}
	printf("\n\n");
	afisareHeapTotala(heap);

	printf("\n\n");
	Heap heapFisier = initHeap(5);
	citesteDinFisier("quiz.txt", &heapFisier);
	afisareHeapTotala(heapFisier);

	printf("\n\n");
	afisareQuiz(extragereMax(&heapFisier));

	printf("\n\n");
	afisareQuiz(extragereMin(&heap));

	dezalocareHeap(&heap);
	dezalocareHeap(&heapFisier);
}