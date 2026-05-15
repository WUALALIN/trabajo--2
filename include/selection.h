#ifndef SELECTION_H
#define SELECTION_H

#include "deportista.h"

int particionar(Deportista arr[], int izquierda, int derecha);

Deportista quick_select(Deportista arr[], int izquierda, int derecha, int k);

int mediana_de_tres(Deportista arr[], int izquierda, int derecha);

int particionar_mediana(Deportista arr[], int izquierda, int derecha);

Deportista quick_select_mediana3(Deportista arr[], int izquierda, int derecha, int k);

int particionPuntaje(Deportista arr[], int inicio, int fin);

void ordenarPorPuntaje(Deportista arr[], int inicio, int fin);

void ejecutarExperimentoT3();

int comparar_desc(const void *a, const void *b);

Deportista* top_n_deportistas(Deportista arr[], int n_total, int N);

Deportista k_esimo_mejor(Deportista arr[], int n, int k);

#endif