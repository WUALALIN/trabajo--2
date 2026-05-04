#ifndef DEPORTISTA_H
#define DEPORTISTA_H    

#define MAX_NOMBRE 50
#define EQUIPOS_INIT {"Equipo1", "Equipo2", "Equipo3", "Equipo4", "Equipo5"}
#define NUM_EQUIPOS 5

typedef struct {
    int id;
    char nombre[MAX_NOMBRE];
    char equipo[MAX_NOMBRE];
    float puntaje;
    int competencias;
} Deportista;


void generarDatos(Deportista **arreglo, int *cantidad);
void guardarCSV(Deportista *arreglo, int cantidad, const char *nombreArchivo);

#endif