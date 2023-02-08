#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "FuncionesComunesJugarYMapas.h"


/**
 * Funcion EXTRA implementa la funcionalidad mitica de "ENTER PARA CONTINUAR"
 * @return 1
 */
int presionarContinuar() {
    char aux = 'N';
    printf("\nEnter para continuar...\n");
    fflush(stdin);
    aux = getchar();
    while (aux != '\r' || aux != '\n') {
        fflush(stdin);
        aux = getchar();
        if (aux == '\n' || aux == '\r') {
            return 1;
        }
    }

    return 0;
}

/**
 * Funcion privada que posibilita introducir el nickname introducido o no
 * @param nombreJugador
 * @return 1 en caso de éxito
 */
int condicionNickname(tipoelem jugador) {
    if (jugador.nombreUsuario[0] == '@' && strlen(jugador.nombreUsuario) < name_length) {
        return 1;
    }
    return 0;
}

/**
 * Funcion privada para solicitar el Nickname y lo devuelve.
 * @return tipoelem
 */
tipoelem solicitarNickname() {
    tipoelem jugador;
    printf("Introduce el nickname del jugador(debe empezar por @ y no tener mas de 19 carácteres)\n");
    printf("Nickname:");
    scanf(" %s", jugador.nombreUsuario);

    return jugador;
}

/**
 * Función que añade un jugador al árbol(opcion A)
 * @param Usuarios
 */
void altaJugador(abb *Usuarios) {
    tipoelem jugador = solicitarNickname();
    if (condicionNickname(jugador) && es_miembro(*Usuarios, jugador) == 0) {
        jugador.rol = '\0';
        jugador.tareas = NULL;
        insertar(Usuarios, jugador);
    }
}

/**
 * Función que elimina un jugador del árbol(opcion B)
 * @param Usuarios
 */
int bajaJugador(abb *Usuarios) {
    tipoelem jugador = solicitarNickname();

    if (es_miembro(*Usuarios, jugador)) {
        suprimir(Usuarios, jugador);
        printf("El jugador ' %s ' ya no está disponible para _jugar\n", jugador.nombreUsuario);
        return 1;
    } else {
        printf("El jugador ' %s ' no está en el listado de jugadores\n", jugador.nombreUsuario);
        return 0;
    }
}

/**
 * Funcion privada para imprimir los ESTADOS del jugador
 * @param jugador
 */
void _imprimirActividad(tipoelem jugador){
    if (jugador.actividad == MOVIENDOSE) {
        printf("ACT:"ROJO_T"MOV\n"RESET_COLOR);
    } else if (jugador.actividad == TAREA) {
        printf("ACT:"ROJO_T"TAR\n"RESET_COLOR);
    } else if (jugador.actividad == ESPERANDO) {
        printf("ACT:"ROJO_T"ESP\n"RESET_COLOR);
    }
}

/**
 * Función privada que imprime los datos de un jugador
 * @param jugador
 * @param flagFinPartida imprimir resumen de la partida
 */
void _imprimirJugadorIndividual(tipoelem jugador, int flagFinPartida) {
    if (flagFinPartida) {
        if (jugador.actividad != TERMINADO) {
            tipoelemCola aux = primero(jugador.tareas);
            printf("%-20sROL:%-3cPAS:%-5dHAB:"CYAN_T"%-20s"RESET_COLOR"PROXHAB:%-20sTARHAB:"VERDE_T"%-20s"RESET_COLOR"TARNOM:%-30sPESTAR:%-5d",jugador.nombreUsuario, jugador.rol, jugador.pasosAcumulados, jugador.ubicacionActual,jugador.proximaUbicacion,aux.lugarTarea, aux.nombreTarea, jugador.pesoTareaActual);
            _imprimirActividad(jugador);
        } else if (jugador.actividad == TERMINADO) {
            printf(VERDE_T"%-20s"RESET_COLOR"ROL:%-3cPAS:%-5dHAB:"CYAN_T"%-20s"RESET_COLOR"ACT:"AMARILLO_T"TERM\n"RESET_COLOR,jugador.nombreUsuario, jugador.rol, jugador.pasosAcumulados, jugador.ubicacionActual);
        }
    } else if (jugador.rol == '\0') {
        printf("%-20s\n", jugador.nombreUsuario);
    }else if(jugador.actividad != TERMINADO){
        if(jugador.rol == 'K' || jugador.rol == 'E'){
            printf(MAGENTA_T);
        }
        tipoelemCola aux = primero(jugador.tareas);
        printf("%-20s"RESET_COLOR"PAS:%-5dHAB:"CYAN_T"%-20s"RESET_COLOR"PROXHAB:%-20sTARHAB:"VERDE_T"%-20s"RESET_COLOR"TARNOM:%-30sPESTAR:%-5d",jugador.nombreUsuario, jugador.pasosAcumulados, jugador.ubicacionActual, jugador.proximaUbicacion,aux.lugarTarea, aux.nombreTarea, jugador.pesoTareaActual);
        _imprimirActividad(jugador);

    }else if(jugador.actividad == TERMINADO){
        if(jugador.rol == 'K' || jugador.rol == 'E'){
            printf(MAGENTA_T);
        }else{
            printf(VERDE_T);
        }
        printf("%-20s"RESET_COLOR"PAS:%-5dHAB:"CYAN_T"%-20s"RESET_COLOR"ACT:"AMARILLO_T"TERM\n"RESET_COLOR,jugador.nombreUsuario, jugador.pasosAcumulados, jugador.ubicacionActual);
    }
}

/**
 * Recorrido recursivo que llama a _imprimirJugadorIndividual() para cada jugador
 * @param Usuarios
 */
void listadoJugadores(abb Usuarios, int flagFinPartida) {
    tipoelem jugador;
    if (!es_vacio(Usuarios)) {
        listadoJugadores(izq(Usuarios), flagFinPartida);

        leer(Usuarios, &jugador);
        _imprimirJugadorIndividual(jugador, flagFinPartida);

        listadoJugadores(der(Usuarios), flagFinPartida);

    }
}

/**
 * Funcion mkdir para ./Mapas y ./Listados
 * @param nombreCarpeta
 */
int crearCarpeta(char *nombreCarpeta) {
    if (strcmp(nombreCarpeta, carpetaMapas) == 0) {
        if (mkdir(carpetaMapas, 0777) == 0) {
            printf(BOLD VERDE_T"Se ha creado la carpeta Mapas en ./Mapas para almacenar todos los mapas del juego\n"RESET_COLOR);
            return 1;
        }
    } else if (strcmp(nombreCarpeta, carpetaListados) == 0) {
        if (mkdir(carpetaListados, 0777) == 0) {
            printf(BOLD VERDE_T"Se ha creado la carpeta Listados en ./Listados para almacenar todos las Listas de Jugadores del juego\n"RESET_COLOR);
            return 1;
        }
    }

    return 0;
}

/**
 * Funcion ls de shell implementada
 * @param directorio
 */
void archivosEn(char *directorio) {
    DIR *dir;
    struct dirent *ent;

    dir = opendir(directorio);
    if (dir == NULL) {
        printf("Error al buscar los archivos\n");
        exit(-1);
    }

    while ((ent = readdir(dir)) != NULL) {
        printf("%-15s  ", ent->d_name);

    }
    printf("\n");
}

/**
 * Llama a system() para limpiar la terminal
 */
void limpiarPantallaTerminal() {
    system("cls");
    system("clear");
}

/**
 * Funcion privada que calcula el Algoritmo de Floyd y Warshall
 * @param G
 * @param flagRol
 * @param numeroVertices
 * @param D
 * @param P
 */
void AlgoritmoFloyd_Warshall(grafo G, int flagRol, int numeroVertices, int D[][MAXVERTICES],struct camino P[][MAXVERTICES]) {
    int matrizP;

    //Inicializamos las matrices
    for (int i = 0; i < MAXVERTICES; i++) {
        for (int j = 0; j < MAXVERTICES; j++) {
            matrizP = 1;
            if (i != j && son_adyacentes_T(G, i, j) != 0 && flagRol) {
                D[i][j] = son_adyacentes_T(G, i, j);
                P[i][j].valor = i;
                P[i][j].flagRol = 'T';
                matrizP = 0;
            } else if (i != j && (son_adyacentes_I(G, i, j) != 0 || son_adyacentes_T(G, i, j) != 0) &&
                       flagRol == 0) {
                if (son_adyacentes_I(G, i, j) != 0) {
                    D[i][j] = son_adyacentes_I(G, i, j);
                    P[i][j].flagRol = 'I';
                } else {
                    D[i][j] = son_adyacentes_T(G, i, j);
                    P[i][j].flagRol = 'T';
                }
                P[i][j].valor = i;
                matrizP = 0;
            } else if (i == j) {
                D[i][j] = 0;
            } else {
                D[i][j] = infinito;
            }
            if (matrizP) {
                P[i][j].valor = 0;
                P[i][j].flagRol = 'T';
            }
        }
    }

    //Algoritmo Floyd-Warshall
    for (int k = 0; k < numeroVertices; k++) {
        for (int i = 0; i < numeroVertices; i++) {
            for (int j = 0; j < numeroVertices; j++) {
                if (D[i][j] > D[i][k] + D[k][j]) {
                    D[i][j] = D[i][k] + D[k][j];
                    P[i][j] = P[k][j];
                }
            }
        }
    }
}