#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Listado.h"

char nombreLista[name_length];
int listaCargada = 0;

/**
 * FUNCIONES DEL PROGRAMA DE AMONG->LISTADOS CON ARBOLES ABB:
 */

/**
 * FUNCIONES DEL PROGRAMA DE AMONG->MAPAS CON GRAFOS:
 */


//////////////////FUNCIONES DEVOLVER VAR GLOBALES:

/**
 * Devuelve la variable global Lista Cargada
 * @return
 */
int lCargada() {
    return listaCargada;
}

/**
 * Devuelve la variable global NombreLista
 * @return
 */
char *nombreListCargada() {
    return nombreLista;
}

//////////////////OPCIONES MENU:

/**
 * Funcion que elimina un fichero
 */
void _eliminarLista() {
    char archivo[fichero_length];
    printf("Lista de Jugadores disponibles en la carpeta Listados:\n");
    archivosEn(carpetaListados);
    printf("Escoge una Lista de Jugadores:");
    scanf(" %s", archivo);
    if (strcmp("quit", archivo) == 0 || strcmp("QUIT", archivo) == 0 || strcmp("Quit", archivo) == 0 ||
        strcmp("q", archivo) == 0 || strcmp("Q", archivo) == 0) {
        return;
    }
    char comando[fichero_length];
    strcpy(comando, carpetaListados);
    strcat(comando, archivo);
    if (remove(comando) == 0) {
        printf("Se elimino correctamente %s", archivo);
    } else {
        printf("No se pudo eliminar el archivo\n");
    }
}

/**
 * Funcion para vaciar el arbol Cargado
 * @param Usuarios
 */
void _vaciarArbolCargado(abb *Usuarios) {
    destruir(Usuarios);
    crear(Usuarios);

}

/**
 * Función para leer el archivo de disco(Siempre se ejecuta para cargar los jugadores)
 * @param Usuarios
 * @param flagInicio para cargar DEFAULTLIST
 */
void leerArchivo_Jugadores(abb *Usuarios, int flagInicio) {
    tipoelem jugador;
    FILE *fichero;
    char archivo[fichero_length];
    int salir = 1;
    char nombre[name_length];
    char comando[fichero_length * 3] = "./Listados/";


    while (salir) {
        salir = 0;
        if (flagInicio) {
            strcpy(comando, carpetaListados);
            strcat(comando, DEFAULTLIST);
        } else {
            printf("Listas de Jugadores disponibles en la carpeta Listados:\n");
            archivosEn(carpetaListados);
            printf("Escoge una Lista de Jugadores:");
            scanf(" %s", archivo);
            strcpy(comando, carpetaListados);
            strcat(comando, archivo);
            if (strcmp("quit", archivo) == 0 || strcmp("QUIT", archivo) == 0 || strcmp("Quit", archivo) == 0 ||
                strcmp("q", archivo) == 0 || strcmp("Q", archivo) == 0) {
                return;
            }
        }
        fichero = fopen(comando, "r");
        if (fichero == NULL) {
            if (flagInicio) {
                printf(BOLD VERDE_T"No se ha encontrado la lista default:%s\n"RESET_COLOR, DEFAULTLIST);
                return;
            }
            printf(BOLD CYAN_T"ESCOGE UN FICHERO CORRECTO\n\n"RESET_COLOR);

            salir = 1;

        }
    }

    fscanf(fichero, " %s", nombre);//Combrobamos si esta vacio el .txt
    if (!feof(fichero)) {
        _vaciarArbolCargado(Usuarios);//Vaciamos el Arbol antes de leerArchivo_Jugadores
        strncpy(nombreLista, nombre, strlen(nombre) + 1);
        fscanf(fichero, " %s", jugador.nombreUsuario);
        jugador.rol = '\0';
        jugador.tareas = NULL;
        while (!feof(fichero)) {
            if (condicionNickname(jugador) && es_miembro(*Usuarios, jugador) == 0) {
                insertar(Usuarios, jugador);
            }
            fscanf(fichero, " %s", jugador.nombreUsuario);
        }

        listaCargada = 1;
        printf(BOLD VERDE_T"Cargada la Lista:%s"RESET_COLOR"\n", nombreLista);
    } else {
        printf("No se ha podido leer nada de %s\n", archivo);

    }
    fclose(fichero);

}

/**
 * Funcion privada para hacer recorrido inorden del arbol y copiar los jugadores en el fichero
 * @param fichero
 * @param Usuarios
 */
void _guardarDatos(FILE *fichero, abb Usuarios) {
    tipoelem jugador;
    if (!es_vacio(Usuarios)) {
        //Recorrido postOrden
        _guardarDatos(fichero, izq(Usuarios));
        _guardarDatos(fichero, der(Usuarios));

        leer(Usuarios, &jugador);
        fprintf(fichero, "%s \n", jugador.nombreUsuario);
    }
}

/**
 * Funcion para guardar el listado de jugadores en el fichero
 * @param Usuarios
 */
void guardarArchivo_Jugadores(abb Usuarios) {
    FILE *fichero;
    char comando[fichero_length * 3] = "./Listados/";
    strcat(comando, nombreLista);
    strcat(comando, ".txt");
    fichero = fopen(comando, "w");

    if (fichero == NULL) {
        printf("Error al abrir el fichero\n");
        exit(-1);
    } else {
        fprintf(fichero, "%s\n", nombreLista);
        _guardarDatos(fichero, Usuarios);
    }

    printf("Se han guardado los datos correctamente en '%s.txt'\n", nombreLista);
    fclose(fichero);

}

/**
 * Funcion pregunta y devuelve nombre del arbol
 * @return
 */
char *_nombreArbol() {
    char opc;
    char *nombre = (char *) malloc(name_length * (sizeof(char)));
    do {
        printf("Nombre del nuevo Listado(sin espacios):");
        scanf(" %s", nombre);
        if (strcmp("quit", nombre) == 0 || strcmp("QUIT", nombre) == 0 || strcmp("Quit", nombre) == 0 ||
            strcmp("q", nombre) == 0 || strcmp("Q", nombre) == 0) {
            return "quit";
        }
        printf("Nombre Nuevo Listado:%s(S/N)", nombre);
        scanf(" %c", &opc);
    } while ((opc != 'S' && opc != 's'));

    return nombre;
}

//////////////////MENU:

/**
 * Menu Crear o Modificar Listado de Jugadores
 * @param Usuarios
 */
void _menuJugadores(abb *Usuarios, int flagCrear) {
    char opcion;
    if (flagCrear) {
        char *aux;
        aux = _nombreArbol();
        if (strcmp("quit", aux) == 0) {
            return;
        }
        _vaciarArbolCargado(Usuarios);
        strncpy(nombreLista, aux, name_length);
        listaCargada = 1;
    }
    //Menú
    do {
        printf(BOLD "\n\n////////////Menu: Crear Listado de Jugadores///////////////\n"RESET_COLOR);
        if (listaCargada) {
            printf(BOLD "////////////Lista Cargada: "BOLD CYAN_T "%s"RESET_COLOR  "///////////////\n"RESET_COLOR,
                   nombreLista);
        }
        printf("a) Alta de un jugador\n");
        printf("b) Baja de un jugador\n");
        printf("c) Listado de jugadores\n");
        printf("d) Guardar archivo\n");
        printf("s) Salir\n\n");

        printf("Opcion:");
        scanf(" %c", &opcion);
        printf("--------------------------------------------------------------------------\n");
        printf("--------------------------------------------------------------------------\n");

        switch (opcion) {
            case 'a':
            case 'A':
                altaJugador(Usuarios);
                //presionarContinuar();
                break;
            case 'b':
            case 'B':
                bajaJugador(Usuarios);
                //presionarContinuar();
                break;
            case 'c':
            case 'C':
                printf("Lista de Jugadores:\n");
                listadoJugadores(*Usuarios, 0);
                //presionarContinuar();
                break;
            case 'd':
            case 'D':
                guardarArchivo_Jugadores(*Usuarios);
                //presionarContinuar();
                break;
            case 's':
            case 'S':
                break;
        }

    } while (opcion != 'S' && opcion != 's');


}

/**
 * Menu Listado Jugadores
 * @param Usuarios
 */
void menuJugadoresPrincipal(abb *Usuarios) {
    char opcion;

    do {
        printf("\n\n////////////"BOLD"Menu Jugadores"RESET_COLOR"///////////////\n");
        if (listaCargada) {
            printf(BOLD "////////////"BOLD VERDE_T"Lista Cargada:%s"RESET_COLOR  "///////////////\n"RESET_COLOR,
                   nombreLista);
        }
        printf("a) Crear Lista Jugadores\n");
        printf("m) Modificar Lista de Jugadores Cargada\n");
        printf("b) Leer un Lista(.txt)\n");
        printf("c) Eliminar un Lista(.txt)\n");
        printf("s) Atrás\n");

        printf(BOLD "Opcion: "RESET_COLOR);
        scanf(" %c", &opcion);

        switch (opcion) {
            case 'a':
            case 'A':
                _menuJugadores(Usuarios, 1);
                break;
            case 'b':
            case 'B':
                leerArchivo_Jugadores(Usuarios, 0);
                break;
            case 'c':
            case 'C':
                _eliminarLista();
                break;
            case 'm':
            case 'M':
                if (listaCargada) {
                    _menuJugadores(Usuarios, 0);
                } else {
                    printf("Primero debes cargar una lista de jugadores\n");
                    presionarContinuar();
                }
                break;
            case 's':
            case 'S':
                opcion = 's';
                break;
            default:
                printf(BOLD"Opción equivocada\n"RESET_COLOR);
        }
    } while (opcion != 's');

}
