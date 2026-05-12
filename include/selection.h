#ifndef SELECTION_H
#define SELECTION_H

#include "deportista.h"

int particionar(Deportista arr[], int izquierda, int derecha);

Deportista quick_select(Deportista arr[], int izquierda, int derecha, int k);

int mediana_de_tres(Deportista arr[], int izquierda, int derecha);

int particionar_mediana(Deportista arr[], int izquierda, int derecha);

Deportista quick_select_mediana3(Deportista arr[], int izquierda, int derecha, int k);

void ejecutarExperimentoT3();

#endif