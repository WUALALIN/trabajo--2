#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "deportista.h" 

void finalizar_programa();
void help();

void generarDatos(Deportista **arreglo, int *cantidad);
void guardarCSV(Deportista *arreglo, int cantidad, const char *nombreArchivo);
void cargarCSV(Deportista **arreglo, int *cantidad, const char *nombreArchivo);
void mostrarTodo(Deportista *arreglo, int cantidad);

// Fisher-Yates
void mezclarDatos(Deportista *arreglo, int cantidad); 

// Función para comparar
int comparar(Deportista a, Deportista b, int criterio);

// Ordenamiento
void bubbleSort(Deportista *a, int n, int criterio);
void selectionSort(Deportista *a, int n, int criterio);
void insertionSort(Deportista *a, int n, int criterio);
void cocktailSort(Deportista *a, int n, int criterio);

// Búsqueda
void busquedaSecuencial(Deportista *arreglo, int cantidad, int idBuscar);
void busquedaBinaria(Deportista *arreglo, int cantidad, int idBuscar);

// Experimentos de tiempo
void copiarArreglo(Deportista *origen, Deportista *destino, int n);
void ejecutarExperimento();

#endif