#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deportista.h"
#include "funciones.h"
#include "interfaz.h"

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
                printf("%sUso: ordenar <bubble|insertion|selection|cocktail>%s\n", ROJO, RESET);
            } else {
                int criterio;
                printf("Elija campo (1:ID, 2:Nombre, 3:Equipo, 4:Puntaje, 5:Competencias): ");
                scanf("%d", &criterio);
                getchar(); // Consumir salto de linea

                if (strcmp(arg1, "bubble") == 0) bubbleSort(mis_deportistas, cantidad_actual, criterio);
                else if (strcmp(arg1, "insertion") == 0) insertionSort(mis_deportistas, cantidad_actual, criterio);
                else if (strcmp(arg1, "selection") == 0) selectionSort(mis_deportistas, cantidad_actual, criterio);
                else if (strcmp(arg1, "cocktail") == 0) cocktailSort(mis_deportistas, cantidad_actual, criterio);
                else printf("%sAlgoritmo no reconocido.%s\n", ROJO, RESET);
            }
        } 
        else if (strcmp(comando, "busqueda") == 0) {
            if (num_args < 2) {
                printf("%sUso: busqueda <secuencial|binaria>%s\n", ROJO, RESET);
            } else {
                int idBuscar;
                printf("%sIngrese el ID a buscar: %s", CYAN, RESET);
                
                // Limpiamos el buffer y leemos un número
                fflush(stdin); 
                if (scanf("%d", &idBuscar) != 1) {
                    printf("Error leyendo el ID.\n");
                }
                getchar(); // Consumir el \n que deja scanf

                if (strcmp(arg1, "secuencial") == 0) {
                    busquedaSecuencial(mis_deportistas, cantidad_actual, idBuscar);
                } else if (strcmp(arg1, "binaria") == 0) {
                    printf("%s[INFO] Asegurese que los datos esten ordenados POR ID.%s\n", AMARILLO, RESET);
                    busquedaBinaria(mis_deportistas, cantidad_actual, idBuscar);
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
            ejecutarExperimento();
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
