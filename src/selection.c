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
    int valores_n[] = {
    5000, 10000, 15000, 20000, 25000,
    30000, 35000, 40000, 45000, 50000,
    55000, 60000, 65000, 70000, 75000,
    80000, 85000, 90000, 95000, 100000
    };
    int num_values = 20;
    int repeticiones = 10; 

    ExecResult1 resultados[20];
    
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
        printf("Posicion a encontrar: %d\n", k);
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

// comparación para qsort (orden descendente)
int comparar_desc(const void *a, const void *b) {
    double p1 = ((Deportista *)a)->puntaje;
    double p2 = ((Deportista *)b)->puntaje;

    if (p1 < p2) return 1; 
    if (p1 > p2) return -1;  
    return 0;
}

Deportista* top_n_deportistas(Deportista arr[], int n_total, int N) {
    if (N > n_total) N = n_total;

    int indice = n_total - N;

    quick_select_mediana3(arr, 0, n_total - 1, indice);

    // reservar memoria para el resultado
    Deportista *top = malloc(N * sizeof(Deportista));

    // copiar los N mejores
    for (int i = 0; i < N; i++) {
        top[i] = arr[indice + i];
    }

    // ordenar el top N
    qsort(top, N, sizeof(Deportista), comparar_desc);
    return top;
}

Deportista k_esimo_mejor(Deportista arr[], int n, int k) {
    if (k < 1 || k > n) {
        Deportista vacio = {"", -1}; // manejo simple de error
        return vacio;
    }

    int indice = n - k;

    return quick_select(arr, 0, n - 1, indice);
}