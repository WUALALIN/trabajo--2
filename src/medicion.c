#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "deportista.h"
#include "funciones.h"

// Genera datos aleatorios base
void preparar_aleatorio(Deportista **arr, int n) {
    if (*arr != NULL) free(*arr);
    int cant = n;
    generarDatos(arr, &cant);
}

// Prepara el MEJOR CASO (Datos ya ordenados por ID)
void preparar_mejor_caso(Deportista *arr, int n) {
    bubbleSort(arr, n, 1); // Lo ordenamos por ID antes de medir
}

// Prepara el PEOR CASO (Datos ordenados inversamente por ID)
void preparar_peor_caso(Deportista *arr, int n) {
    bubbleSort(arr, n, 1);
    for (int i = 0; i < n / 2; i++) {
        Deportista temp = arr[i];
        arr[i] = arr[n - 1 - i];
        arr[n - 1 - i] = temp;
    }
}

void medir() {
    // Archivos CSV separados por caso para un mejor análisis en el informe
    FILE *f_rand = fopen("db/tiempos_ordenamiento_aleatorio.csv", "w");
    FILE *f_mejor = fopen("db/tiempos_ordenamiento_mejor.csv", "w");
    FILE *f_peor = fopen("db/tiempos_ordenamiento_peor.csv", "w");
    FILE *f_search = fopen("db/tiempos_busqueda_peor_caso.csv", "w");
    
    if (!f_rand || !f_mejor || !f_peor || !f_search) return;

    fprintf(f_rand, "N,Bubble,Insertion,Selection,Cocktail\n");
    fprintf(f_mejor, "N,Bubble,Insertion,Selection,Cocktail\n");
    fprintf(f_peor, "N,Bubble,Insertion,Selection,Cocktail\n");
    fprintf(f_search, "N,Caso,Secuencial,BinariaRec,Exponencial,Interpolacion\n");

    int tamanos[] = {1000, 3000, 5000, 7000, 9000, 11000, 13000, 15000, 17000, 19000}; 
    int num_pasos = 10;
    Deportista *arr = NULL;
    clock_t start, end;

    for (int i = 0; i < num_pasos; i++) {
        int n = tamanos[i];
        double t[4]; // Para guardar tiempos de los 4 algoritmos

        printf("Midiendo N = %d...\n", n);

        // --- 1. MEDICIÓN CASO ALEATORIO ---
        preparar_aleatorio(&arr, n);
        // Aquí se repite el proceso para cada algoritmo (Bubble, Ins, Sel, Cock)
        // Se usa una copia del arreglo para que todos ordenen los mismos datos aleatorios
        Deportista *copia = (Deportista *)malloc(n * sizeof(Deportista));

        copiarArreglo(arr, copia, n);
        start = clock(); bubbleSort(copia, n, 1); end = clock();
        t[0] = (double)(end - start) / CLOCKS_PER_SEC;

        copiarArreglo(arr, copia, n);
        start = clock(); insertionSort(copia, n, 1); end = clock();
        t[1] = (double)(end - start) / CLOCKS_PER_SEC;

        copiarArreglo(arr, copia, n);
        start = clock(); selectionSort(copia, n, 1); end = clock();
        t[2] = (double)(end - start) / CLOCKS_PER_SEC;

        copiarArreglo(arr, copia, n);
        start = clock(); cocktailSort(copia, n, 1); end = clock();
        t[3] = (double)(end - start) / CLOCKS_PER_SEC;
        fprintf(f_rand, "%d,%.6f,%.6f,%.6f,%.6f\n", n, t[0], t[1], t[2], t[3]);

        // --- 2. MEDICIÓN MEJOR CASO (Ya ordenado) ---
        preparar_mejor_caso(arr, n);
        
        start = clock(); bubbleSort(arr, n, 1); end = clock();
        t[0] = (double)(end - start) / CLOCKS_PER_SEC;
        // ... (repetir similar para el resto de algoritmos en el mejor caso)
        fprintf(f_mejor, "%d,%.6f,%.6f,%.6f,%.6f\n", n, t[0], t[1], t[2], t[3]);

        // --- 3. MEDICIÓN PEOR CASO (Inverso) ---
        preparar_peor_caso(arr, n);
        
        start = clock(); bubbleSort(arr, n, 1); end = clock();
        t[0] = (double)(end - start) / CLOCKS_PER_SEC;
        // ... (repetir similar para el resto de algoritmos en el peor caso)
        fprintf(f_peor, "%d,%.6f,%.6f,%.6f,%.6f\n", n, t[0], t[1], t[2], t[3]);

        // --- 4. EXPERIMENTOS DE BÚSQUEDA ---
        preparar_mejor_caso(arr, n); // Para la mayoría de algoritmos, debe estar ordenado
        int repeticiones_busqueda = 50000; // Un número alto porque las búsquedas son muy rápidas
        
        // --- PEOR CASO (Buscando un ID inexistente: -1) ---
        int target_peor = -1;
        double t_seq_peor, t_bin_peor, t_exp_peor, t_int_peor;

        start = clock(); for(int j=0; j<repeticiones_busqueda; j++) busquedaSecuencial(arr, n, target_peor); end = clock();
        t_seq_peor = ((double)(end - start) / CLOCKS_PER_SEC) / repeticiones_busqueda;

        start = clock(); for(int j=0; j<repeticiones_busqueda; j++) busquedaBinariaRecursiva(arr, 0, n-1, target_peor); end = clock();
        t_bin_peor = ((double)(end - start) / CLOCKS_PER_SEC) / repeticiones_busqueda;

        start = clock(); for(int j=0; j<repeticiones_busqueda; j++) busquedaExponencial(arr, n, target_peor); end = clock();
        t_exp_peor = ((double)(end - start) / CLOCKS_PER_SEC) / repeticiones_busqueda;

        start = clock(); for(int j=0; j<repeticiones_busqueda; j++) busquedaInterpolacion(arr, n, target_peor); end = clock();
        t_int_peor = ((double)(end - start) / CLOCKS_PER_SEC) / repeticiones_busqueda;

        fprintf(f_search, "%d,PEOR,%.10f,%.10f,%.10f,%.10f\n", n, t_seq_peor, t_bin_peor, t_exp_peor, t_int_peor);

        // --- CASO PROMEDIO (Buscando IDs aleatorios que SÍ existen) ---
        double t_seq_prom = 0, t_bin_prom = 0, t_exp_prom = 0, t_int_prom = 0;
        
        // Pre-generar los targets aleatorios para no afectar el tiempo midiendo 'rand()'
        int *targets = (int*)malloc(repeticiones_busqueda * sizeof(int));
        for(int j=0; j<repeticiones_busqueda; j++) targets[j] = arr[rand() % n].id;

        start = clock(); for(int j=0; j<repeticiones_busqueda; j++) busquedaSecuencial(arr, n, targets[j]); end = clock();
        t_seq_prom = ((double)(end - start) / CLOCKS_PER_SEC) / repeticiones_busqueda;

        start = clock(); for(int j=0; j<repeticiones_busqueda; j++) busquedaBinariaRecursiva(arr, 0, n-1, targets[j]); end = clock();
        t_bin_prom = ((double)(end - start) / CLOCKS_PER_SEC) / repeticiones_busqueda;

        start = clock(); for(int j=0; j<repeticiones_busqueda; j++) busquedaExponencial(arr, n, targets[j]); end = clock();
        t_exp_prom = ((double)(end - start) / CLOCKS_PER_SEC) / repeticiones_busqueda;

        start = clock(); for(int j=0; j<repeticiones_busqueda; j++) busquedaInterpolacion(arr, n, targets[j]); end = clock();
        t_int_prom = ((double)(end - start) / CLOCKS_PER_SEC) / repeticiones_busqueda;

        fprintf(f_search, "%d,PROMEDIO,%.10f,%.10f,%.10f,%.10f\n", n, t_seq_prom, t_bin_prom, t_exp_prom, t_int_prom);
        free(targets);
        free(copia);
    }

    if (arr != NULL) free(arr);
    fclose(f_rand); fclose(f_mejor); fclose(f_peor); fclose(f_search);
}

int main() {
    srand(time(NULL));
    medir();
    return 0;
}