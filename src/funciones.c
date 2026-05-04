#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "deportista.h"
#include "funciones.h"
#include "interfaz.h"

void finalizar_programa() { 
    printf("%sLimpiando cache y saliendo del programa...\n%s", AMARILLO, RESET);
    exit(0);
}

void help() {
    printf("%sComandos disponibles:%s\n", CYAN, RESET);
    printf("%s- generar <n>: %sGenera un archivo con n deportistas aleatorios.\n", VERDE, RESET);
    printf("%s- cargar <archivo>: %sCarga los datos de deportistas desde el archivo.\n", VERDE, RESET);
    printf("%s- ordenar <algoritmo>: %sOrdena los datos (bubble, insertion, selection, cocktail).\n", VERDE, RESET);
    printf("%s- busqueda <algoritmo>: %sBusca a un deportista por su ID (secuencial o binaria).\n", VERDE, RESET);
    printf("%s- ranking <n>: %sMuestra el top N de los deportistas usando el puntaje.\n", VERDE, RESET);
    printf("%s- experimento: %sMide tiempos de algoritmos y exporta CSV.\n", VERDE, RESET);
    printf("%s- guardar [archivo]: %sGuarda datos en CSV.\n", VERDE, RESET);
    printf("%s- all: %sMuestra todos los deportistas cargados.\n", VERDE, RESET);
    printf("%s- exit: %sSale del programa.\n", VERDE, RESET);
}

void generarDatos(Deportista **arreglo, int *cantidad) {
    if (*cantidad <= 0) return;

    *arreglo = (Deportista *)malloc((*cantidad) * sizeof(Deportista));
    if (*arreglo == NULL) return;

    const char *equipos_locales[] = EQUIPOS_INIT;
    srand(time(NULL)); 

    for (int i = 0; i < *cantidad; i++) {
        (*arreglo)[i].id = i + 1;
        
        int largo_nombre = 5; 
        for (int j = 0; j < largo_nombre; j++) {
            (*arreglo)[i].nombre[j] = 'A' + (rand() % 26);
        }
        (*arreglo)[i].nombre[largo_nombre] = '\0'; 

        strcpy((*arreglo)[i].equipo, equipos_locales[rand() % NUM_EQUIPOS]);
        (*arreglo)[i].puntaje = (float)(rand() % 1001) / 10.0;
        (*arreglo)[i].competencias = (rand() % 15) + 1;
    }

    printf("%sSe han generado %d deportistas con nombres aleatorios (ej: %s).%s\n", 
           VERDE, *cantidad, (*arreglo)[0].nombre, RESET);
}

void guardarCSV(Deportista *arreglo, int cantidad, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("%sNo se pudo crear el archivo %s%s\n", ROJO, nombreArchivo, RESET);
        return;
    }
    fprintf(archivo, "ID,Nombre,Equipo,Puntaje,Competencias\n");
    for (int i = 0; i < cantidad; i++) {
        fprintf(archivo, "%d,%s,%s,%.2f,%d\n", 
                arreglo[i].id, arreglo[i].nombre, arreglo[i].equipo, 
                arreglo[i].puntaje, arreglo[i].competencias);
    }
    fclose(archivo);
    printf("%sDatos guardados en %s%s\n", AMARILLO, nombreArchivo, RESET);
}

void cargarCSV(Deportista **arreglo, int *cantidad, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("%sError: No se pudo abrir el archivo %s%s\n", ROJO, nombreArchivo, RESET);
        return;
    }

    char linea[256];
    fgets(linea, sizeof(linea), archivo);

    int contador = 0;
    while (fgets(linea, sizeof(linea), archivo)) contador++;
    
    *cantidad = contador;
    rewind(archivo); 
    fgets(linea, sizeof(linea), archivo); 

    *arreglo = (Deportista *)malloc((*cantidad) * sizeof(Deportista));

    for (int i = 0; i < *cantidad; i++) {
        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "%d,%[^,],%[^,],%f,%d", 
               &(*arreglo)[i].id, (*arreglo)[i].nombre, (*arreglo)[i].equipo, 
               &(*arreglo)[i].puntaje, &(*arreglo)[i].competencias);
    }
    fclose(archivo);
    printf("%sSe cargaron %d deportistas desde %s%s\n", VERDE, *cantidad, nombreArchivo, RESET);
}

void mostrarTodo(Deportista *arreglo, int cantidad) {
    if (arreglo == NULL || cantidad <= 0) {
        printf("%sNo hay deportistas cargados. Usa 'generar' o 'cargar' primero.%s\n", AMARILLO, RESET);
        return;
    }
    printf("\n%s%-5s %-15s %-15s %-10s %-5s%s\n", CYAN, "ID", "NOMBRE", "EQUIPO", "PUNTAJE", "COMP.", RESET);
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < cantidad; i++) {
        printf("%-5d %-15s %-15s %-10.2f %-5d\n", 
               arreglo[i].id, arreglo[i].nombre, arreglo[i].equipo, 
               arreglo[i].puntaje, arreglo[i].competencias);
    }
    printf("------------------------------------------------------------\n");
    printf("%sTotal: %d deportistas.%s\n\n", VERDE, cantidad, RESET);
}

void mezclarDatos(Deportista *arreglo, int cantidad) {
    if (cantidad <= 1) return;
    for (int i = cantidad - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Deportista temp = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temp;
    }
    printf("%sLos datos han sido mezclados aleatoriamente (Fisher-Yates).%s\n", AMARILLO, RESET);
}

int comparar(Deportista a, Deportista b, int criterio) {
    switch(criterio) {
        case 1: return a.id - b.id; 
        case 2: return strcmp(a.nombre, b.nombre); 
        case 3: return strcmp(a.equipo, b.equipo); 
        case 4:
            if (a.puntaje > b.puntaje) return 1;
            if (a.puntaje < b.puntaje) return -1;
            return 0;
        case 5: return a.competencias - b.competencias; 
        default: return 0;
    }
}

void bubbleSort(Deportista *a, int n, int criterio) {
    int i, j, intercambio;
    for (i = 0; i < n - 1; i++) {
        intercambio = 0;
        for (j = 0; j < n - i - 1; j++) {
            if (comparar(a[j], a[j+1], criterio) > 0) {
                Deportista temp = a[j];
                a[j] = a[j+1];
                a[j+1] = temp;
                intercambio = 1;
            }
        }
        if (intercambio == 0) break; 
    }
    printf("%sOrdenado con Bubble Sort (Optimizado).%s\n", VERDE, RESET);
}

void selectionSort(Deportista *a, int n, int criterio) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (comparar(a[j], a[min_idx], criterio) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) { 
            Deportista temp = a[min_idx];
            a[min_idx] = a[i];
            a[i] = temp;
        }
    }
    printf("%sOrdenado con Selection Sort (Optimizado).%s\n", VERDE, RESET);
}

void insertionSort(Deportista *a, int n, int criterio) {
    for (int i = 1; i < n; i++) {
        Deportista clave = a[i];
        int j = i - 1;
        while (j >= 0 && comparar(a[j], clave, criterio) > 0) {
            a[j + 1] = a[j];
            j = j - 1;
        }
        a[j + 1] = clave;
    }
    printf("%sOrdenado con Insertion Sort.%s\n", VERDE, RESET);
}

void cocktailSort(Deportista *a, int n, int criterio) {
    int intercambio = 1;
    int inicio = 0;
    int fin = n - 1;
    while (intercambio) {
        intercambio = 0;
        for (int i = inicio; i < fin; ++i) {
            if (comparar(a[i], a[i + 1], criterio) > 0) {
                Deportista temp = a[i];
                a[i] = a[i+1];
                a[i+1] = temp;
                intercambio = 1;
            }
        }
        if (!intercambio) break;
        intercambio = 0;
        --fin;
        for (int i = fin - 1; i >= inicio; --i) {
            if (comparar(a[i], a[i + 1], criterio) > 0) {
                Deportista temp = a[i];
                a[i] = a[i+1];
                a[i+1] = temp;
                intercambio = 1;
            }
        }
        ++inicio;
    }
    printf("%sOrdenado con Cocktail Shaker Sort.%s\n", VERDE, RESET);
}

void busquedaSecuencial(Deportista *arreglo, int cantidad, int idBuscar) {
    if (arreglo == NULL || cantidad == 0) {
        printf("%sError: No hay datos cargados.%s\n", ROJO, RESET);
        return;
    }
    int encontrado = 0;
    for (int i = 0; i < cantidad; i++) {
        if (arreglo[i].id == idBuscar) {
            printf("\n%s[RESULTADO ENCONTRADO - SECUENCIAL]%s\n", VERDE, RESET);
            printf("ID: %-5d | Nombre: %-15s | Equipo: %-15s | Puntaje: %.2f | Comp: %d\n", 
                   arreglo[i].id, arreglo[i].nombre, arreglo[i].equipo, arreglo[i].puntaje, arreglo[i].competencias);
            encontrado = 1;
            break; 
        }
    }
    if (!encontrado) printf("%sNo se encontro ningun deportista con el ID '%d'.%s\n", ROJO, idBuscar, RESET);
}

void busquedaBinaria(Deportista *arreglo, int cantidad, int idBuscar) {
    if (arreglo == NULL || cantidad == 0) {
        printf("%sError: No hay datos cargados.%s\n", ROJO, RESET);
        return;
    }
    int inicio = 0, fin = cantidad - 1, encontrado = 0;
    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;
        if (arreglo[medio].id == idBuscar) {
            printf("\n%s[RESULTADO ENCONTRADO - BINARIA]%s\n", VERDE, RESET);
            printf("ID: %-5d | Nombre: %-15s | Equipo: %-15s | Puntaje: %.2f | Comp: %d\n", 
                   arreglo[medio].id, arreglo[medio].nombre, arreglo[medio].equipo, arreglo[medio].puntaje, arreglo[medio].competencias);
            encontrado = 1;
            break;
        }
        if (arreglo[medio].id < idBuscar) inicio = medio + 1; 
        else fin = medio - 1;    
    }
    if (!encontrado) printf("%sNo se encontro el ID '%d'. ¿Esta ordenado por ID?%s\n", ROJO, idBuscar, RESET);
}

// --- LOGICA DE EXPERIMENTO ---
typedef struct {
    int n;
    double time_bubble;
    double time_insertion;
    double time_selection;
    double time_cocktail;
} ExecResult;

void copiarArreglo(Deportista *origen, Deportista *destino, int n) {
    for (int i = 0; i < n; i++) destino[i] = origen[i];
}

void ejecutarExperimento() {
    int valores_n[] = {100, 500, 1000, 2000, 5000, 10000};
    int num_values = 6;
    int repeticiones = 5; 

    ExecResult resultados[6];
    
    printf("\n%sIniciando experimento de medicion de tiempos (%d repeticiones)...%s\n", AMARILLO, repeticiones, RESET);

    for (int i = 0; i < num_values; i++) {
        int n = valores_n[i];
        resultados[i].n = n;
        resultados[i].time_bubble = 0; resultados[i].time_insertion = 0;
        resultados[i].time_selection = 0; resultados[i].time_cocktail = 0;

        Deportista *datos_base = NULL;
        generarDatos(&datos_base, &n);
        Deportista *copia = (Deportista *)malloc(n * sizeof(Deportista));

        for (int r = 0; r < repeticiones; r++) {
            mezclarDatos(datos_base, n); 
            clock_t ini, fin;

            copiarArreglo(datos_base, copia, n);
            ini = clock(); bubbleSort(copia, n, 4); fin = clock();
            resultados[i].time_bubble += (double)(fin - ini) / CLOCKS_PER_SEC;

            copiarArreglo(datos_base, copia, n);
            ini = clock(); insertionSort(copia, n, 4); fin = clock();
            resultados[i].time_insertion += (double)(fin - ini) / CLOCKS_PER_SEC;

            copiarArreglo(datos_base, copia, n);
            ini = clock(); selectionSort(copia, n, 4); fin = clock();
            resultados[i].time_selection += (double)(fin - ini) / CLOCKS_PER_SEC;

            copiarArreglo(datos_base, copia, n);
            ini = clock(); cocktailSort(copia, n, 4); fin = clock();
            resultados[i].time_cocktail += (double)(fin - ini) / CLOCKS_PER_SEC;
        }

        resultados[i].time_bubble /= repeticiones;
        resultados[i].time_insertion /= repeticiones;
        resultados[i].time_selection /= repeticiones;
        resultados[i].time_cocktail /= repeticiones;

        free(datos_base);
        free(copia);
        printf("N=%-6d completado.\n", n);
    }

    FILE *f = fopen("tiempos_ordenamiento.csv", "w");
    fprintf(f, "n,bubble,insertion,selection,cocktail\n");
    for (int i = 0; i < num_values; i++) {
        fprintf(f, "%d,%f,%f,%f,%f\n", resultados[i].n, resultados[i].time_bubble, 
                resultados[i].time_insertion, resultados[i].time_selection, resultados[i].time_cocktail);
    }
    fclose(f);
    printf("%sExperimento finalizado. Datos exportados a tiempos_ordenamiento.csv%s\n", VERDE, RESET);
}