#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "deportista.h"
#include "funciones.h"
#include "interfaz.h"
#include "ordenamiento.h"

// AUXILIARES

void swap(Deportista *a, int i, int j) {
    Deportista tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

// Fusiona dos mitades ordenadas usando buffers temporales
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

// EXPERIMENTO DE TIEMPOS T2

void ejecutarExperimentoT2() {
    int valores_n[]  = {1000, 3000, 5000, 7000, 9000, 11000, 13000, 15000, 17000, 19000};
    int num_vals     = 10;
    int repeticiones = 10;

    // Umbrales a comparar para Merge Sort optimizado
    int umbrales[]   = {8, 16, 32};
    int num_umbrales = 3;

    printf("\n%s[EXPERIMENTO T2] Divide y Venceras (%d repeticiones)...%s\n",
           AMARILLO, repeticiones, RESET);

    // CSV 1: caso promedio de todos los algoritmos 
    FILE *f1 = fopen("db/tiempos_t2_promedio.csv", "w");
    if (!f1) f1 = fopen("tiempos_t2_promedio.csv", "w");
    if (!f1) { printf("%sError creando CSV promedio%s\n", ROJO, RESET); return; }

    fprintf(f1, "n,merge_clasico,merge_opt8,merge_opt16,merge_opt32,"
                "qs_ultimo,qs_primero,qs_aleatorio,qs_mediana\n");

    // CSV 2: peor caso de cada variante de Quick Sort (arreglo ordenado) 
    FILE *f2 = fopen("db/tiempos_t2_peor.csv", "w");
    if (!f2) f2 = fopen("tiempos_t2_peor.csv", "w");
    if (!f2) { printf("%sError creando CSV peor caso%s\n", ROJO, RESET); fclose(f1); return; }

    fprintf(f2, "n,qs_peor_ultimo,qs_peor_primero,qs_peor_aleatorio,qs_peor_mediana\n");

    // CSV 3: mejor caso de Quick Sort (arreglo ordenado inversamente para mediana, aleatorio para otros) 
    FILE *f3 = fopen("db/tiempos_t2_mejor.csv", "w");
    if (!f3) f3 = fopen("tiempos_t2_mejor.csv", "w");
    if (!f3) { printf("%sError creando CSV mejor caso%s\n", ROJO, RESET); fclose(f1); fclose(f2); return; }

    fprintf(f3, "n,merge_clasico,qs_ultimo,qs_primero,qs_aleatorio,qs_mediana\n");

    srand((unsigned)time(NULL));

    for (int i = 0; i < num_vals; i++) {
        int n = valores_n[i];

        double t_mc = 0;
        double t_mo[3] = {0, 0, 0}; // umbrales 8, 16, 32
        double t_qu = 0, t_qp = 0, t_qr = 0, t_qm = 0;
        double t_peor_u = 0, t_peor_p = 0, t_peor_r = 0, t_peor_m = 0;
        double t_mejor_mc = 0, t_mejor_u = 0, t_mejor_p = 0, t_mejor_r = 0, t_mejor_m = 0;

        Deportista *base = NULL;
        generarDatos(&base, &n);
        Deportista *copia = (Deportista *)malloc(n * sizeof(Deportista));
        if (!base || !copia) { free(base); free(copia); continue; }

        // CASO PROMEDIO: datos mezclados, 5 repeticiones 
        for (int r = 0; r < repeticiones; r++) {
            mezclarDatos(base, n);
            clock_t ini, fin_t;

            copiarArreglo(base, copia, n);
            ini = clock(); mergeSortRec(copia, 0, n-1, 4); fin_t = clock();
            t_mc += (double)(fin_t - ini) / CLOCKS_PER_SEC;

            for (int u = 0; u < num_umbrales; u++) {
                copiarArreglo(base, copia, n);
                ini = clock(); mergeSortOptRec(copia, 0, n-1, 4, umbrales[u]); fin_t = clock();
                t_mo[u] += (double)(fin_t - ini) / CLOCKS_PER_SEC;
            }

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

        // PEOR CASO QS: arreglo ya ordenado ascendente 
        // Para 'u' y 'p' el peor caso es arreglo ordenado
        // Para 'r' y 'm' igual se mide para comparar
        copiarArreglo(base, copia, n);
        mergeSortRec(copia, 0, n-1, 4); // ordenar ascendente

        Deportista *ordenado = (Deportista *)malloc(n * sizeof(Deportista));
        copiarArreglo(copia, ordenado, n);

        clock_t ini, fin_t;

        copiarArreglo(ordenado, copia, n);
        ini = clock(); quickSortRec(copia, 0, n-1, 4, 'u'); fin_t = clock();
        t_peor_u = (double)(fin_t - ini) / CLOCKS_PER_SEC;

        copiarArreglo(ordenado, copia, n);
        ini = clock(); quickSortRec(copia, 0, n-1, 4, 'p'); fin_t = clock();
        t_peor_p = (double)(fin_t - ini) / CLOCKS_PER_SEC;

        copiarArreglo(ordenado, copia, n);
        ini = clock(); quickSortRec(copia, 0, n-1, 4, 'r'); fin_t = clock();
        t_peor_r = (double)(fin_t - ini) / CLOCKS_PER_SEC;

        copiarArreglo(ordenado, copia, n);
        ini = clock(); quickSortRec(copia, 0, n-1, 4, 'm'); fin_t = clock();
        t_peor_m = (double)(fin_t - ini) / CLOCKS_PER_SEC;

        // MEJOR CASO: arreglo ya ordenado (Merge Sort siempre O(n log n)) 
        // Para QS el mejor caso practico es datos aleatorios con buena particion
        // Usamos arreglo ordenado para Merge Sort (no cambia) y aleatorio para QS
        copiarArreglo(ordenado, copia, n);
        ini = clock(); mergeSortRec(copia, 0, n-1, 4); fin_t = clock();
        t_mejor_mc = (double)(fin_t - ini) / CLOCKS_PER_SEC;

        // Para QS mejor caso usamos datos mezclados una sola vez
        mezclarDatos(base, n);

        copiarArreglo(base, copia, n);
        ini = clock(); quickSortRec(copia, 0, n-1, 4, 'u'); fin_t = clock();
        t_mejor_u = (double)(fin_t - ini) / CLOCKS_PER_SEC;

        copiarArreglo(base, copia, n);
        ini = clock(); quickSortRec(copia, 0, n-1, 4, 'p'); fin_t = clock();
        t_mejor_p = (double)(fin_t - ini) / CLOCKS_PER_SEC;

        copiarArreglo(base, copia, n);
        ini = clock(); quickSortRec(copia, 0, n-1, 4, 'r'); fin_t = clock();
        t_mejor_r = (double)(fin_t - ini) / CLOCKS_PER_SEC;

        copiarArreglo(base, copia, n);
        ini = clock(); quickSortRec(copia, 0, n-1, 4, 'm'); fin_t = clock();
        t_mejor_m = (double)(fin_t - ini) / CLOCKS_PER_SEC;

        free(base);
        free(copia);
        free(ordenado);

        // Promediar caso promedio
        t_mc /= repeticiones;
        for (int u = 0; u < num_umbrales; u++) t_mo[u] /= repeticiones;
        t_qu /= repeticiones; t_qp /= repeticiones;
        t_qr /= repeticiones; t_qm /= repeticiones;

        fprintf(f1, "%d,%f,%f,%f,%f,%f,%f,%f,%f\n",
                n, t_mc, t_mo[0], t_mo[1], t_mo[2],
                t_qu, t_qp, t_qr, t_qm);

        fprintf(f2, "%d,%f,%f,%f,%f\n",
                n, t_peor_u, t_peor_p, t_peor_r, t_peor_m);

        fprintf(f3, "%d,%f,%f,%f,%f,%f\n",
                n, t_mejor_mc, t_mejor_u, t_mejor_p, t_mejor_r, t_mejor_m);

        printf("  N=%-6d completado.\n", n);
    }

    fclose(f1);
    fclose(f2);
    fclose(f3);

    printf("%s[EXPERIMENTO T2] Listo. Archivos generados:%s\n", VERDE, RESET);
    printf("  db/tiempos_t2_promedio.csv\n");
    printf("  db/tiempos_t2_peor.csv\n");
    printf("  db/tiempos_t2_mejor.csv\n");
}
