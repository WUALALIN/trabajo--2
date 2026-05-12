#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deportista.h"
#include "funciones.h"
#include "interfaz.h"
#include "ordenamiento.h"
#include "selection.h"

int main(void) {
    char linea[256];
    char comando[10], arg1[100];
    Deportista *mis_deportistas = NULL; // El arreglo empieza vacío
    int cantidad_actual = 0;

    printf("%sBienvenido al sistema de deportistas%s\n", VERDE, RESET);
    printf("%sPara obtener ayuda, escriba 'help'%s\n", AMARILLO, RESET);
    while (1) {
        printf("%s> %s", MORADO, RESET);

        if (fgets(linea, sizeof(linea), stdin) == NULL) {
            finalizar_programa();
            break;
        }

        linea[strcspn(linea, "\n")] = 0;  // Elimina el salto de linea
        int num_args = sscanf(linea, "%s %s", comando, arg1);

        if (strcmp(comando, "help") == 0) {
            help();

        } else if (strcmp(comando, "generar") == 0) {
            if (num_args < 2) {
                printf("%sUso: generar <n>%s\n", ROJO, RESET); 
            } else {
                cantidad_actual = atoi(arg1);
                // Liberar memoria si ya existía algo previo
                if (mis_deportistas != NULL) free(mis_deportistas); 
                
                generarDatos(&mis_deportistas, &cantidad_actual);
                mezclarDatos(mis_deportistas, cantidad_actual);
                guardarCSV(mis_deportistas, cantidad_actual, "deportistas.csv");
            }
        } 
        else if (strcmp(comando, "cargar") == 0) {
            if (num_args < 2) {
                printf("%sUso: cargar <nombre_archivo.csv>%s\n", ROJO, RESET);
            } else {
                // Liberamos si ya había algo cargado
                if (mis_deportistas != NULL) free(mis_deportistas);
                
                cargarCSV(&mis_deportistas, &cantidad_actual, arg1);
            }
        } 
        else if (strcmp(comando, "ordenar") == 0) {
            if (num_args < 2) {
                printf("%sUso: ordenar <bubble|insertion|selection|cocktail|merge|mergeopt|qs_u|qs_p|qs_r|qs_m>%s\n", ROJO, RESET);
            } else {
                int criterio;
                printf("Elija campo (1:ID, 2:Nombre, 3:Equipo, 4:Puntaje, 5:Competencias): ");
                scanf("%d", &criterio);
                getchar();

                if (strcmp(arg1, "bubble") == 0) bubbleSort(mis_deportistas, cantidad_actual, criterio);
                else if (strcmp(arg1, "insertion") == 0) insertionSort(mis_deportistas, cantidad_actual, criterio);
                else if (strcmp(arg1, "selection") == 0) selectionSort(mis_deportistas, cantidad_actual, criterio);
                else if (strcmp(arg1, "cocktail") == 0) cocktailSort(mis_deportistas, cantidad_actual, criterio);
                else if (strcmp(arg1, "merge") == 0) {
                    mergeSort(mis_deportistas, cantidad_actual, criterio);
                }
                else if (strcmp(arg1, "mergeopt") == 0) {
                    int umbral;
                    printf("Ingrese umbral (recomendado 8-32, default 16): ");
                    scanf("%d", &umbral);
                    getchar(); 
                    mergeSortOpt(mis_deportistas, cantidad_actual, criterio, umbral);
                }
                else if (strcmp(arg1, "qs_u") == 0) {
                    quickSort(mis_deportistas, cantidad_actual, criterio, 'u');
                }
                else if (strcmp(arg1, "qs_p") == 0) {
                    quickSort(mis_deportistas, cantidad_actual, criterio, 'p');
                }
                else if (strcmp(arg1, "qs_r") == 0) {
                    quickSort(mis_deportistas, cantidad_actual, criterio, 'r');
                }
                else if (strcmp(arg1, "qs_m") == 0) {
                    quickSort(mis_deportistas, cantidad_actual, criterio, 'm');
                }
                else printf("%sAlgoritmo no reconocido.%s\n", ROJO, RESET);
            }
        } 
        else if (strcmp(comando, "busqueda") == 0) {
            if (num_args < 2) {
                printf("%sUso: busqueda <secuencial|binaria|recursiva|rango|exponencial|interpolacion>%s\n", ROJO, RESET);
            } else {
                if (strcmp(arg1, "rango") == 0) {
                    float min, max;
                    printf("%s[INFO] Asegurese que los datos esten ordenados POR PUNTAJE (ordenar 4).%s\n", AMARILLO, RESET);
                    printf("%sIngrese el puntaje minimo: %s", CYAN, RESET);
                    if (scanf("%f", &min) != 1) printf("Error leyendo el valor.\n");
                    
                    printf("%sIngrese el puntaje maximo: %s", CYAN, RESET);
                    if (scanf("%f", &max) != 1) printf("Error leyendo el valor.\n");
                    getchar(); // Consumir el \n

                    busquedaBinariaRango(mis_deportistas, cantidad_actual, min, max);

                } else {
                    int valorBuscar;
                    printf("%sIngrese el ID a buscar: %s", CYAN, RESET);
                    fflush(stdin); 
                    if (scanf("%d", &valorBuscar) != 1) printf("Error leyendo el valor.\n");
                    getchar(); // Consumir el \n

                    if (strcmp(arg1, "secuencial") == 0) {
                        busquedaSecuencial(mis_deportistas, cantidad_actual, valorBuscar);
                    } else if (strcmp(arg1, "binaria") == 0) {
                        busquedaBinaria(mis_deportistas, cantidad_actual, valorBuscar);
                    } else if (strcmp(arg1, "recursiva") == 0) {
                        int pos = busquedaBinariaRecursiva(mis_deportistas, 0, cantidad_actual - 1, valorBuscar);
                        imprimirResultadoBusqueda(mis_deportistas, pos, valorBuscar, "BINARIA RECURSIVA");
                    } else if (strcmp(arg1, "exponencial") == 0) {
                        int pos = busquedaExponencial(mis_deportistas, cantidad_actual, valorBuscar);
                        imprimirResultadoBusqueda(mis_deportistas, pos, valorBuscar, "EXPONENCIAL");
                    } else if (strcmp(arg1, "interpolacion") == 0) {
                        int pos = busquedaInterpolacion(mis_deportistas, cantidad_actual, valorBuscar);
                        imprimirResultadoBusqueda(mis_deportistas, pos, valorBuscar, "INTERPOLACION");
                    } else {
                        printf("%sAlgoritmo no reconocido.%s\n", ROJO, RESET);
                    }
                }
            }
        }
        else if (strcmp(comando, "ranking") == 0 ) {
            if (num_args < 2) {
                printf("%sUso: ranking <N> (ej: ranking 5)%s\n", ROJO, RESET);
            } else {
                int n_top = atoi(arg1);
                if (n_top > cantidad_actual) n_top = cantidad_actual;
                if (n_top <= 0) n_top = 5;

                char algo[20];
                printf("%sElija el algoritmo para ordenar el ranking (bubble, insertion, selection, cocktail): %s", AMARILLO, RESET);
                scanf("%s", algo);
                getchar(); // Consumir salto de linea

                // Ordenamos por Puntaje (criterio 4)
                if (strcmp(algo, "bubble") == 0) bubbleSort(mis_deportistas, cantidad_actual, 4);
                else if (strcmp(algo, "insertion") == 0) insertionSort(mis_deportistas, cantidad_actual, 4);
                else if (strcmp(algo, "selection") == 0) selectionSort(mis_deportistas, cantidad_actual, 4);
                else if (strcmp(algo, "cocktail") == 0) cocktailSort(mis_deportistas, cantidad_actual, 4);
                else {
                    printf("%sAlgoritmo no reconocido. Usando Selection Sort por defecto.%s\n", ROJO, RESET);
                    selectionSort(mis_deportistas, cantidad_actual, 4);
                }

                printf("\n%s=== TOP %d MEJORES DEPORTISTAS ===%s\n", AMARILLO, n_top, RESET);
                printf("%s%-5s %-15s %-15s %-10s %-5s%s\n", CYAN, "ID", "NOMBRE", "EQUIPO", "PUNTAJE", "COMP.", RESET);
                printf("------------------------------------------------------------\n");
                
                // Mostrar de atrás para adelante (de mayor a menor puntaje)
                for (int i = cantidad_actual - 1; i >= cantidad_actual - n_top; i--) {
                    printf("%-5d %-15s %-15s %-10.2f %-5d\n", 
                           mis_deportistas[i].id, mis_deportistas[i].nombre, 
                           mis_deportistas[i].equipo, mis_deportistas[i].puntaje, 
                           mis_deportistas[i].competencias);
                }
                printf("------------------------------------------------------------\n");
            }
        }
        else if (strcmp(comando, "all") == 0 ) {
            mostrarTodo(mis_deportistas, cantidad_actual);

        } 
        else if (strcmp(comando, "experimento") == 0) {
            int option;
            printf("Elige que experimento realizar: \n");
            printf("1.- Experimento n°1\n");
            printf("2.- Experimento n°2\n");
            printf("3.- Quick Select\n");
            scanf("%d", &option);

            switch (option)
            {
            case 1:
                ejecutarExperimento();
                break;
            
            case 2:
                ejecutarExperimentoT2();
                break;

            case 3:
                ejecutarExperimentoT3();
                break;
            }
        }
        else if (strcmp(comando, "top_n") == 0) {
            int n, tamaño;
            printf("Ingresa cantidad de deportistas a crear: \n");
            scanf("%d", &tamaño);
            printf("Ingresa el 'n': \n");
            scanf("%d", &n);
            Deportista *arreglo = NULL;
            generarDatos(&arreglo, &n);
            top_n_deportistas(arreglo, sizeof(arreglo-1), n);
            free(arreglo);
        }
        else if (strcmp(comando, "guardar") == 0) {
            if (mis_deportistas == NULL || cantidad_actual == 0) {
                printf("%sError: No hay datos en memoria para guardar. Usa 'generar' primero.%s\n", ROJO, RESET);
            } else { // si no hay nombre de archivo escribimos 
                char *nombre_archivo = (num_args > 1) ? arg1 : "deportistas_guardados.csv";
                guardarCSV(mis_deportistas, cantidad_actual, nombre_archivo);
            }
        }
        else if (strcmp(comando, "exit") == 0) {
            finalizar_programa();
            break;

        } 
        else {
            printf("%sComando o argumento invalido. Revise la sintaxis.%s\n", ROJO, RESET);
        }
    }

    return 0;
}