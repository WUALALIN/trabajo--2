#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "deportista.h"
#include "funciones.h"
#include "interfaz.h"
#include "ordenamiento.h"

int particionar(Deportista arr[], int izquierda, int derecha) {
    int pivote = arr[derecha].puntaje;
    int i = izquierda - 1;

    for (int j = izquierda; j < derecha; j++) {
        if (arr[j].puntaje <= pivote) {
            i++;

            // swap
            Deportista temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    // colocar pivote en su posición final
    Deportista temp = arr[i + 1];
    arr[i + 1] = arr[derecha];
    arr[derecha] = temp;

    return i + 1;
}

Deportista quick_select(Deportista arr[], int izquierda, int derecha, int k) {
    if (izquierda == derecha) {
        return arr[izquierda];
    }

    int indice_pivote = particionar(arr, izquierda, derecha);

    if (indice_pivote == k) {
        return arr[indice_pivote];
    } else if (k < indice_pivote) {
        return quick_select(arr, izquierda, indice_pivote - 1, k);
    } else {
        return quick_select(arr, indice_pivote + 1, derecha, k);
    }
}

int mediana_de_tres(Deportista arr[], int izquierda, int derecha) {
    int medio = (izquierda + derecha) / 2;

    // ordenar izquierda, medio, derecha

    if (arr[izquierda].puntaje > arr[medio].puntaje) {
        Deportista temp = arr[izquierda];
        arr[izquierda] = arr[medio];
        arr[medio] = temp;
    }

    if (arr[izquierda].puntaje > arr[derecha].puntaje) {
        Deportista temp = arr[izquierda];
        arr[izquierda] = arr[derecha];
        arr[derecha] = temp;
    }

    if (arr[medio].puntaje > arr[derecha].puntaje) {
        Deportista temp = arr[medio];
        arr[medio] = arr[derecha];
        arr[derecha] = temp;
    }

    Deportista temp = arr[medio];
    arr[medio] = arr[derecha];
    arr[derecha] = temp;

    return derecha; // pivote queda en derecha
}

int particionar_mediana(Deportista arr[], int izquierda, int derecha) {
    mediana_de_tres(arr, izquierda, derecha);

    int pivote = arr[derecha].puntaje;
    int i = izquierda - 1;

    for (int j = izquierda; j < derecha; j++) {
        if (arr[j].puntaje <= pivote) {
            i++;

            Deportista temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    Deportista temp = arr[i + 1];
    arr[i + 1] = arr[derecha];
    arr[derecha] = temp;

    return i + 1;
}

Deportista quick_select_mediana3(Deportista arr[], int izquierda, int derecha, int k) {
    if (izquierda == derecha) {
        return arr[izquierda];
    }

    int indice_pivote = particionar_mediana(arr, izquierda, derecha);

    if (indice_pivote == k) {
        return arr[indice_pivote];
    } else if (k < indice_pivote) {
        return quick_select(arr, izquierda, indice_pivote - 1, k);
    } else {
        return quick_select(arr, indice_pivote + 1, derecha, k);
    }
}

typedef struct {
    int n;
    double time_ultimo;
    double time_mediana;
} ExecResult1;


void ejecutarExperimentoT3() {
    int valores_n[] = {100, 500, 1000, 2000, 5000, 10000};
    int num_values = 6;
    int repeticiones = 5; 

    ExecResult1 resultados[6];
    
    printf("\n%sIniciando experimento de medicion de tiempos (%d repeticiones)...%s\n", AMARILLO, repeticiones, RESET);

    for (int i = 0; i < num_values; i++) {
        int n = valores_n[i];
        resultados[i].n = n;
        resultados[i].time_ultimo = 0; resultados[i].time_mediana = 0;

        Deportista *datos_base = NULL;
        generarDatos(&datos_base, &n);
        Deportista *copia = (Deportista *)malloc(n * sizeof(Deportista));

        srand(time(NULL));
        int k = rand() % n;

        for (int r = 0; r < repeticiones; r++) {
            mezclarDatos(datos_base, n); 
            clock_t ini, fin;

            copiarArreglo(datos_base, copia, n);
            ini = clock(); quick_select(copia, 0, n-1, k); fin = clock();
            resultados[i].time_ultimo += (double)(fin - ini) / CLOCKS_PER_SEC;

            copiarArreglo(datos_base, copia, n);
            ini = clock(); quick_select_mediana3(copia, 0, n-1, k); fin = clock();
            resultados[i].time_mediana += (double)(fin - ini) / CLOCKS_PER_SEC;
        }

        resultados[i].time_ultimo /= repeticiones;
        resultados[i].time_mediana /= repeticiones;

        free(datos_base);
        free(copia);
        printf("N=%-6d completado.\n", n);
    }

    FILE *f = fopen("db/tiempos_quickSelect.csv", "w");
    fprintf(f, "n,pivote_ultimo,pivote_mediana\n");
    for (int i = 0; i < num_values; i++) {
        fprintf(f, "%d,%f,%f\n", resultados[i].n, resultados[i].time_ultimo, 
                                                        resultados[i].time_mediana);
    }
    fclose(f);
    printf("%sExperimento finalizado. Datos exportados a tiempos_quickSelect.csv%s\n", VERDE, RESET);
}