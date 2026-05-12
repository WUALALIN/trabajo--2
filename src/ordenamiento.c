#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "deportista.h"
#include "funciones.h"
#include "interfaz.h"
#include "ordenamiento.h"

//AUXILIARES

void swap(Deportista *a, int i, int j) {
    Deportista tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

// Fusiona dos mitades ordenadas en su lugar usando buffers temporales 
void merge(Deportista *a, int inicio, int medio, int fin, int criterio) {
    int izq_n = medio - inicio + 1;
    int der_n = fin - medio;

    Deportista *izq = (Deportista *)malloc(izq_n * sizeof(Deportista));
    Deportista *der = (Deportista *)malloc(der_n * sizeof(Deportista));
    if (!izq || !der) { free(izq); free(der); return; }

    for (int i = 0; i < izq_n; i++) izq[i] = a[inicio + i];
    for (int j = 0; j < der_n; j++) der[j] = a[medio + 1 + j];

    int i = 0, j = 0, k = inicio;
    while (i < izq_n && j < der_n)
        a[k++] = (comparar(izq[i], der[j], criterio) <= 0) ? izq[i++] : der[j++];
    while (i < izq_n) a[k++] = izq[i++];
    while (j < der_n) a[k++] = der[j++];

    free(izq);
    free(der);
}

// Insertion Sort sobre subarreglo a[inicio..fin]
void insertionSortRango(Deportista *a, int inicio, int fin, int criterio) {
    for (int i = inicio + 1; i <= fin; i++) {
        Deportista clave = a[i];
        int j = i - 1;
        while (j >= inicio && comparar(a[j], clave, criterio) > 0) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = clave;
    }
}

 // MERGE SORT CLASICO

void mergeSortRec(Deportista *a, int inicio, int fin, int criterio) {
    if (inicio >= fin) return;
    int medio = inicio + (fin - inicio) / 2;
    mergeSortRec(a, inicio, medio, criterio);
    mergeSortRec(a, medio + 1, fin, criterio);
    merge(a, inicio, medio, fin, criterio);
}

void mergeSort(Deportista *a, int n, int criterio) {
    if (!a || n <= 1) return;
    mergeSortRec(a, 0, n - 1, criterio);
    printf("%sOrdenado con Merge Sort clasico.%s\n", VERDE, RESET);
}

// MERGE SORT OPTIMIZADO


void mergeSortOptRec(Deportista *a, int inicio, int fin, int criterio, int umbral) {
    // Subarreglo pequeño: Insertion Sort es mas eficiente 
    if (fin - inicio + 1 <= umbral) {
        insertionSortRango(a, inicio, fin, criterio);
        return;
    }
    int medio = inicio + (fin - inicio) / 2;
    mergeSortOptRec(a, inicio, medio, criterio, umbral);
    mergeSortOptRec(a, medio + 1, fin, criterio, umbral);
    merge(a, inicio, medio, fin, criterio);
}

void mergeSortOpt(Deportista *a, int n, int criterio, int umbral) {
    if (!a || n <= 1) return;
    if (umbral < 1) umbral = 1;
    mergeSortOptRec(a, 0, n - 1, criterio, umbral);
    printf("%sOrdenado con Merge Sort optimizado (umbral=%d).%s\n", VERDE, umbral, RESET);
}

 // QUICK SORT LOMUTO

// Coloca el pivote elegido en a[fin] para que Lomuto lo tome desde ahi 
void elegirPivote(Deportista *a, int inicio, int fin, int criterio, char pivote) {
    int medio = inicio + (fin - inicio) / 2;
    int idx;

    switch (pivote) {
        case 'u': break; // ultimo: ya esta en fin 
        case 'p': swap(a, inicio, fin); break; // primero: mover al fin 
        case 'r': // aleatorio
            idx = inicio + rand() % (fin - inicio + 1);
            swap(a, idx, fin);
            break;
        case 'm': // mediana de tres 
            if (comparar(a[inicio], a[medio], criterio) > 0) swap(a, inicio, medio);
            if (comparar(a[inicio], a[fin],   criterio) > 0) swap(a, inicio, fin);
            if (comparar(a[medio],  a[fin],   criterio) > 0) swap(a, medio,  fin);
            break;
        default: break;
    }
}

// Particion Lomuto: deja el pivote en su posicion definitiva y retorna ese indice
int particionLomuto(Deportista *a, int inicio, int fin, int criterio) {
    Deportista pivote = a[fin];
    int i = inicio - 1;

    for (int j = inicio; j < fin; j++) {
        if (comparar(a[j], pivote, criterio) <= 0) {
            i++;
            swap(a, i, j);
        }
    }
    swap(a, i + 1, fin);
    return i + 1;
}

void quickSortRec(Deportista *a, int inicio, int fin, int criterio, char pivote) {
    if (inicio >= fin) return;
    elegirPivote(a, inicio, fin, criterio, pivote);
    int p = particionLomuto(a, inicio, fin, criterio);
    quickSortRec(a, inicio, p - 1, criterio, pivote);
    quickSortRec(a, p + 1,  fin,   criterio, pivote);
}

void quickSort(Deportista *a, int n, int criterio, char pivote) {
    if (!a || n <= 1) return;
    srand((unsigned)time(NULL));
    quickSortRec(a, 0, n - 1, criterio, pivote);

    const char *nom;
    switch (pivote) {
        case 'u': nom = "ultimo elemento";  break;
        case 'p': nom = "primer elemento";  break;
        case 'r': nom = "aleatorio";        break;
        case 'm': nom = "mediana de tres";  break;
        default:  nom = "desconocido";      break;
    }
    printf("%sOrdenado con Quick Sort Lomuto (pivote: %s).%s\n", VERDE, nom, RESET);
}


 // Experimento de tiempos
 

void ejecutarExperimentoT2() {
    int valores_n[]  = {100, 500, 1000, 2000, 5000, 10000};
    int num_vals     = 6;
    int repeticiones = 5;
    int umbral_opt   = 16;

    printf("\n%s[EXPERIMENTO T2] Divide y Venceras (%d repeticiones)...%s\n",
           AMARILLO, repeticiones, RESET);

    FILE *f = fopen("db/tiempos_t2.csv", "w");
    if (!f) {
        f = fopen("tiempos_t2.csv", "w");
        if (!f) {
            printf("%sError: no se pudo crear tiempos_t2.csv%s\n", ROJO, RESET);
            return;
        }
        printf("%s[AVISO] Carpeta db/ no encontrada, guardando en directorio actual.%s\n", AMARILLO, RESET);
    }

    fprintf(f, "n,merge_clasico,merge_opt%d,qs_ultimo,qs_primero,qs_aleatorio,qs_mediana,qs_peor_ultimo\n", umbral_opt);

    srand((unsigned)time(NULL));

    for (int i = 0; i < num_vals; i++) {
        int n = valores_n[i];
        double t_mc = 0, t_mo = 0, t_qu = 0, t_qp = 0, t_qr = 0, t_qm = 0, t_qpeor = 0;

        Deportista *base = NULL;
        generarDatos(&base, &n);
        Deportista *copia = (Deportista *)malloc(n * sizeof(Deportista));
        if (!base || !copia) { free(base); free(copia); continue; }

        // Caso promedio: datos mezclados, 5 repeticiones 
        for (int r = 0; r < repeticiones; r++) {
            mezclarDatos(base, n);
            clock_t ini, fin_t;

            copiarArreglo(base, copia, n);
            ini = clock(); mergeSortRec(copia, 0, n-1, 4); fin_t = clock();
            t_mc += (double)(fin_t - ini) / CLOCKS_PER_SEC;

            copiarArreglo(base, copia, n);
            ini = clock(); mergeSortOptRec(copia, 0, n-1, 4, umbral_opt); fin_t = clock();
            t_mo += (double)(fin_t - ini) / CLOCKS_PER_SEC;

            copiarArreglo(base, copia, n);
            ini = clock(); quickSortRec(copia, 0, n-1, 4, 'u'); fin_t = clock();
            t_qu += (double)(fin_t - ini) / CLOCKS_PER_SEC;

            copiarArreglo(base, copia, n);
            ini = clock(); quickSortRec(copia, 0, n-1, 4, 'p'); fin_t = clock();
            t_qp += (double)(fin_t - ini) / CLOCKS_PER_SEC;

            copiarArreglo(base, copia, n);
            ini = clock(); quickSortRec(copia, 0, n-1, 4, 'r'); fin_t = clock();
            t_qr += (double)(fin_t - ini) / CLOCKS_PER_SEC;

            copiarArreglo(base, copia, n);
            ini = clock(); quickSortRec(copia, 0, n-1, 4, 'm'); fin_t = clock();
            t_qm += (double)(fin_t - ini) / CLOCKS_PER_SEC;
        }

        // Peor caso QS pivote ultimo: arreglo ya ordenado
        copiarArreglo(base, copia, n);
        mergeSortRec(copia, 0, n-1, 4);
        {
            clock_t ini = clock();
            quickSortRec(copia, 0, n-1, 4, 'u');
            clock_t fin_t = clock();
            t_qpeor = (double)(fin_t - ini) / CLOCKS_PER_SEC;
        }

        free(base);
        free(copia);

        t_mc /= repeticiones; t_mo /= repeticiones;
        t_qu /= repeticiones; t_qp /= repeticiones;
        t_qr /= repeticiones; t_qm /= repeticiones;

        fprintf(f, "%d,%f,%f,%f,%f,%f,%f,%f\n",
                n, t_mc, t_mo, t_qu, t_qp, t_qr, t_qm, t_qpeor);

        printf("  N=%-6d completado.\n", n);
    }

    fclose(f);
    printf("%s[EXPERIMENTO T2] Listo. Datos en db/tiempos_t2.csv%s\n", VERDE, RESET);
}