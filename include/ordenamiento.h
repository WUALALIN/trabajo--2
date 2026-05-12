#ifndef ORDENAMIENTO_H
#define ORDENAMIENTO_H

#include "deportista.h"

// Merge Sort clasico - O(n log n) todos los casos
void mergeSort(Deportista *a, int n, int criterio);

// Merge Sort con umbral: subarreglos <= umbral usan Insertion Sort
void mergeSortOpt(Deportista *a, int n, int criterio, int umbral);

// Quick Sort Lomuto - pivote: 'u' ultimo | 'p' primero | 'r' aleatorio | 'm' mediana de tres
void quickSort(Deportista *a, int n, int criterio, char pivote);

// Experimento de tiempos T2 - genera db/tiempos_t2.csv
void ejecutarExperimentoT2();

#endif