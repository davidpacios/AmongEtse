#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Mapas.h"


int mapaCargado = 0;

/**
 * FUNCIONES DEL PROGRAMA DE AMONG->MAPAS CON GRAFOS:
 */

//////////////////FUNCIONES DEVOLVER VAR GLOBALES:

/**
 * Devuele la variable global de mapa Cargado
 * @return
 */
int mCargado() {
    return mapaCargado;
}

//////////////////CREAR/MODIFICAR MAPA:

/**
 * Pregunta y devuelve nombre del grafo
 * @return
 */
char *_nombreGrafo() {
    char opc;
    char *nombre = (char *) malloc(name_length * (sizeof(char)));
    do {
        printf("Nombre del nuevo mapa(sin espacios):");
        scanf(" %s", nombre);
        if (strcmp("quit", nombre) == 0 || strcmp("QUIT", nombre) == 0 || strcmp("Quit", nombre) == 0 ||
            strcmp("q", nombre) == 0 || strcmp("Q", nombre) == 0) {
            return "quit";
        }
        printf("Nombre Nuevo Mapa:%s(S/N)", nombre);
        scanf(" %c", &opc);
    } while ((opc != 'S' && opc != 's'));

    return nombre;
}

/**
 * Vacia el Mapa Actual
 * @param G
 */
void _vaciarMapaCargado(grafo *G) {
    borrar_grafo(G);
    crear_grafo(G);
}

/**
 * Introduce una habitacion en el mapa
 * @param G
 */
void introducir_vertice(grafo *G) {
    tipovertice v1;
    printf("Introduce la habitacion a crear: ");
    scanf(" %s", v1.habitacion);
    if (existe_vertice(*G, v1)) {
        printf("Esa habitacion ya esta en el mapa\n");

    } else {
        insertar_vertice(G, v1);
        printf("La habitacion %s ha sido creada en el mapa\n", v1.habitacion);
    }

}

/**
 * Elimina una habiatcion del mapa
 * @param G
 */
void eliminar_vertice(grafo *G) {
    tipovertice v1;
    printf("Introduce la habitacion a eliminar: ");
    scanf(" %s", v1.habitacion);
    if (existe_vertice(*G, v1)) {
        borrar_vertice(G, v1);
        printf("La habitacion %s ha sido eliminada\n", v1.habitacion);

    } else {
        printf("Esa habitacion no existe en el mapa\n");
    }
}

/**
 * Crea una comunicacion entre dos habitaciones
 * @param G
 */
void nuevo_arco(grafo *G) {
    tipovertice v1, v2;
    int valor = 0;

    //Insertamos una nueva relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Nueva comunicacion HabitacionO--HabitacionD\n");

    //Vértice origen del arco
    printf("Desde la habitacion origen: ");
    scanf(" %s", v1.habitacion);
    if (!existe_vertice(*G, v1)) {
        printf("La habitacion %s no existe en el mapa\n", v1.habitacion);
        return;
    }

    //Vértice destino del arco
    printf("Hasta la habitacion destino: ");
    scanf(" %s", v2.habitacion);
    if (!existe_vertice(*G, v2)) {
        printf("La habitacion %s no existe en el mapa\n", v2.habitacion);
        return;
    }

    //Valor del Arco
    printf("Introduce el valor de la comunicacion:");
    scanf(" %d", &valor);

    //Matriz de adyacencia del Tripulante o del Impostor o Ambas
    char opc;
    do {
        printf("Donde quieres introducir la comunicacion, Tripulantes(T), Impostores(I) o Ambos(A):");
        scanf(" %c", &opc);
    } while (opc != 'i' && opc != 'I' && opc != 'T' && opc != 't' && opc != 'a' && opc != 'A');

    if ((opc == 'A' || opc == 'a') && son_adyacentes_I(*G, posicion(*G, v1), posicion(*G, v2)) == 0 &&
        son_adyacentes_T(*G, posicion(*G, v1), posicion(*G, v2)) == 0) {

        crear_arco_I(G, posicion(*G, v1), posicion(*G, v2), valor);//Creación del arco Impostores
        crear_arco_T(G, posicion(*G, v1), posicion(*G, v2), valor);//Creación del arco Tripulantes
    }
    if ((opc == 'I' || opc == 'i') && son_adyacentes_I(*G, posicion(*G, v1), posicion(*G, v2)) == 0) {

        crear_arco_I(G, posicion(*G, v1), posicion(*G, v2), valor);//Creación del arco Impostores

    }
    if ((opc == 'T' || opc == 't') && son_adyacentes_T(*G, posicion(*G, v1), posicion(*G, v2)) == 0) {

        crear_arco_T(G, posicion(*G, v1), posicion(*G, v2), valor);//Creación del arco Tripulantes
    }
    printf("Comunicacion creada %s--%s\n", v1.habitacion, v2.habitacion);

}

/**
 * Elimina una comunicacion entre dos habitaciones
 * @param G
 */
void eliminar_arco(grafo *G) {
    tipovertice v1, v2;

    //Eliminamos una relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Eliminar comunicacion HabitacionO--HabitacionD\n");

    //Vértice origen del arco
    printf("Introduce la habitacion origen: ");
    scanf(" %s", v1.habitacion);
    if (!existe_vertice(*G, v1)) {
        printf("La habitacion %s no existe en el mapa\n", v1.habitacion);
        return;
    }

    //Vértice destino del arco
    printf("Introduce la habitacion destino: ");
    scanf(" %s", v2.habitacion);
    if (!existe_vertice(*G, v2)) {
        printf("la habitacion %s no existe en el mapa\n", v2.habitacion);
        return;
    }

    char opc;
    do {
        printf("Donde quieres eliminar la comunicacion, Tripulantes(T), Impostores(I) o Ambos(A):");
        scanf(" %c", &opc);
    } while (opc != 'i' && opc != 'I' && opc != 'T' && opc != 't' && opc != 'a' && opc != 'A');

    //Matriz de adyacencia del Tripulante o del Impostor o Ambas
    if ((opc == 'A' || opc == 'a') && (son_adyacentes_I(*G, posicion(*G, v1), posicion(*G, v2)) != 0 ||
                                       son_adyacentes_T(*G, posicion(*G, v1), posicion(*G, v2)) != 0)) {

        borrar_arco_I(G, posicion(*G, v1), posicion(*G, v2));//Eliminar arco Impostores
        borrar_arco_T(G, posicion(*G, v1), posicion(*G, v2));//Eliminar arco Tripulantes
    }
    if ((opc == 'I' || opc == 'i') && son_adyacentes_I(*G, posicion(*G, v1), posicion(*G, v2)) != 0) {

        borrar_arco_I(G, posicion(*G, v1), posicion(*G, v2));//Eliminar arco Impostores
    }
    if ((opc == 'T' || opc == 't') && son_adyacentes_T(*G, posicion(*G, v1), posicion(*G, v2)) != 0) {

        borrar_arco_T(G, posicion(*G, v1), posicion(*G, v2));//Eliminar arco Tripulantes
    }
    printf("La comunicacion %s--%s ha sido eliminado\n", v1.habitacion, v2.habitacion);
}

/**
 * Imprime el mapa,num_vertices= numero de habitaciones y array_vertices = vector de habitaciones
 * @param G
 */
void _imprimir_grafo(grafo G) {
    tipovertice *VECTOR; //Para almacenar el vector de vértices del grafo
    int N; //número de vértices del grafo

    //Para recorrerla, simplemente vamos a recorrer la matriz de adyacencia
    N = num_vertices(G);
    VECTOR = array_vertices(G);

    //Imprimir
    int i, j;
    printf(BOLD AZUL_T"El mapa actual %s se compone de:\n"RESET_COLOR, recuperarNombre(G));
    for (i = 0; i < N; i++) {
        //Imprimo el vértice
        printf(BOLD MAGENTA_T "Habitacion(%d): %s\n"RESET_COLOR, i, VECTOR[i].habitacion);
        //Chequeo sus arcos
        for (j = 0; j < N; j++) {
            //Impostores
            if (son_adyacentes_I(G, i, j) != 0) {
                printf(BOLD  ROJO_T"\t%s..%s:%d\n"RESET_COLOR, VECTOR[i].habitacion, VECTOR[j].habitacion,
                       son_adyacentes_I(G, i, j));
            }
            //Tripulantes
            if (son_adyacentes_T(G, i, j) != 0) {
                printf(BOLD VERDE_T"\t%s--%-s:%d\n"RESET_COLOR, VECTOR[i].habitacion, VECTOR[j].habitacion,
                       son_adyacentes_T(G, i, j));

            }
        }
    }
}

/**
 * Guarda las habitacion del mapa en el fichero, es llamada por _guadarArchivo_Mapa
 * @param G
 * @param fichero
 */
void _guardarArchivo_Habitacion(grafo G, FILE *fichero) {
    int N = num_vertices(G);//Numero de Vertices
    tipovertice *VECTOR = array_vertices(G);//Array de los vertices del grafo
    for (int i = 0; i < N; i++) {//Guardamos Vertices
        fprintf(fichero, "%s\n", VECTOR[i].habitacion);
    }
}

/**
 * Guarda las comunicaciones del mapa en el fichero, es llamada por _guadarArchivo_Mapa
 * @param G
 * @param fichero
 */
void _guardarArchivo_Arcos(grafo G, FILE *fichero) {
    int N = num_vertices(G);//Numero de Vertices
    tipovertice *VECTOR = array_vertices(G);//Array de los vertices del grafo
    for (int i = 0; i < N; i++) {//Guardamos Arcos
        for (int j = i; j < N; j++) {//Para tomar la matriz triangular superior
            if (son_adyacentes_I(G, i, j) != 0) {//Impostores
                fprintf(fichero, "%s..%d-%s\n", VECTOR[i].habitacion,
                        son_adyacentes_I(G, i, j),
                        VECTOR[j].habitacion);
            }
            if (son_adyacentes_T(G, i, j) != 0) {//Tripulantes
                fprintf(fichero, "%s--%d-%s\n", VECTOR[i].habitacion, son_adyacentes_T(G, i, j),
                        VECTOR[j].habitacion);
            }
        }
    }
}

/**
 * Guarda las tareas de un mapa en el fichero, es llamada por _guadarArchivo_Mapa
 * @param G
 * @param fichero
 */
void _guardarArchivo_Tarea(grafo G, FILE *fichero) {
    int Ntareas = num_tareas(G);
    tipotareas *TAREAS = array_tareas(G);

    for (int i = 0; i < Ntareas; i++) {//Guardamos tareas
        fprintf(fichero, "%s:%d\n", TAREAS[i].nombreTarea, TAREAS[i].costeTarea);
    }
}

/**
 * Guardar las habitaciones, las comunicaciones y las tareas del mapa en un archivo(nombreGrafo.txt)
 * @param G
 */
void _guadarArchivo_Mapa(grafo G) {
    FILE *fichero;
    char comando[fichero_length * 3] = carpetaMapas;
    strcat(comando, recuperarNombre(G));
    strcat(comando, ".txt");
    fichero = fopen(comando, "w");

    if (fichero == NULL) {
        printf("No se ha podido guarda el fichero\n");

    } else {
        fprintf(fichero, "%s\n", recuperarNombre(G));//Guardamos el nombre
        _guardarArchivo_Habitacion(G, fichero);
        fprintf(fichero, "*\n");//Separador de vertices y arcos
        _guardarArchivo_Arcos(G, fichero);
        fprintf(fichero, "+\n");//Separador de tareas
        _guardarArchivo_Tarea(G, fichero);

        printf("Se han guardado los datos en '%s.txt'\n", recuperarNombre(G));
        fclose(fichero);
    }


}

//////////////////OPCIONES MENUPRINCIPAL:

/**
 * Lee las habitaciones de un fichero y las introduce en el mapa
 * @param G
 * @param linea
 */
void _leerArchivos_Habitaciones(grafo *G, char *linea) {
    tipovertice nuevoVertice;
    sscanf(linea, "%s", nuevoVertice.habitacion);
    insertar_vertice(G, nuevoVertice);
}

/**
 * Lee las comunicaciones de un fichero y las introduce en el mapa
 * @param G
 * @param linea
 */
void _leerArchivo_Arcos(grafo *G, char *linea) {
    char codigo[5];
    int valor;
    tipovertice verticeO, verticeD;
    sscanf(linea, "%[^-.]%[-.]%d-%s", verticeO.habitacion, codigo, &valor, verticeD.habitacion);
    if (codigo[0] == '-') {//Arco de Tripulante
        crear_arco_T(G, posicion(*G, verticeO), posicion(*G, verticeD), valor);

    } else if (codigo[0] == '.') {//Arco de Impostor
        crear_arco_I(G, posicion(*G, verticeO), posicion(*G, verticeD), valor);
    }
}

/**
 * Lee las tareas de un fichero y las introduce en el mapa
 * @param G
 * @param linea
 */
void _leerArchivo_Tareas(grafo *G, char *linea) {
    tipotareas tareas;
    sscanf(linea, "%[^:]:%d", tareas.nombreTarea, &tareas.costeTarea);
    insertar_tareas(G, tareas);
}

/**
 * Abrir el fichero de Lectura, es llamada por leerArchivo_Mapa
 * @param archivoMapa
 * @return
 */
FILE *_abrirFichero_Lectura(char *archivoMapa, int flagInicio) {
    FILE *fichero;
    int salir = 1;

    char comando[fichero_length * 3] = carpetaMapas;
    while (salir) {
        salir = 0;
        if (flagInicio) {
            strcpy(comando, carpetaMapas);
            strcat(comando, DEFAULTMAP);
        } else {
            printf("Mapas disponibles en la carpeta mapas:\n");
            archivosEn(carpetaMapas);
            printf("Escoge un mapa:");
            scanf(" %s", archivoMapa);
            strcpy(comando, carpetaMapas);
            strcat(comando, archivoMapa);
            if (strcmp("quit", archivoMapa) == 0 || strcmp("QUIT", archivoMapa) == 0 ||
                strcmp("Quit", archivoMapa) == 0 || strcmp("q", archivoMapa) == 0 || strcmp("Q", archivoMapa) == 0) {
                return NULL;
            }
        }
        fichero = fopen(comando, "r");
        if (fichero == NULL) {
            if (flagInicio) {
                return NULL;
            }
            printf(BOLD CYAN_T"ESCOGE UN MAPA CORRECTO\n\n"RESET_COLOR);
            salir = 1;

        }
    }
    return fichero;

}

/**
 * Lee las habitaciones,las comunicaciones y las tareas de un archivo e introducirlos en el mapa
 * @param G
 * @param flagInicio para cargar DEFAULTMAP
 */
void leerArchivo_Mapa(grafo *G, int flagInicio) {
    char linea[3 * location_length];
    char archivoMapa[fichero_length];
    FILE *fichero;
    fichero = _abrirFichero_Lectura(archivoMapa, flagInicio);
    //Leer
    if (fichero != NULL) {
        fscanf(fichero, "%s", linea);
        if (!feof(fichero)) {
            _vaciarMapaCargado(G);//Vaciamos el Grafo antes de leerArchivo_Jugadores
            introducirNombre_Grafo(*G, linea);
            printf(BOLD VERDE_T"Cargado el mapa:%s\n"RESET_COLOR, recuperarNombre(*G));
            mapaCargado = 1;
            fscanf(fichero, "%s", linea);
            while (linea[0] != '*') {
                _leerArchivos_Habitaciones(G, linea);
                fscanf(fichero, "%s", linea);
            }
            fscanf(fichero, "%s", linea);
            while (linea[0] != '+') {
                _leerArchivo_Arcos(G, linea);
                fscanf(fichero, "%s", linea);
            }
            int i = 0;
            fscanf(fichero, "%[\r\n]", linea);
            fscanf(fichero, "%[^\r\n]", linea);
            while (!feof(fichero)) {
                if (i > 100) {
                    printf("Error al leer el archivo\n");
                    exit(-1);
                }
                _leerArchivo_Tareas(G, linea);
                fscanf(fichero, "%[\r\n]", linea);//Limpiar el \n del ultimo fscanf
                fscanf(fichero, "%[^\r\n]", linea);
                i++;
            }
        } else {
            printf("No se pudo cargar el mapa %s\n", archivoMapa);
        }
        fclose(fichero);
    }
    if (flagInicio && fichero == NULL){
        printf(BOLD VERDE_T"No se ha encontrado el mapa default:%s\n"RESET_COLOR, DEFAULTMAP);

    }

}

/**
 * Pregunta y elimina el fichero
 */
void _eliminarMapa() {
    char archivoMapa[fichero_length];
    printf("Mapas disponibles en la carpeta mapas:\n");
    archivosEn(carpetaMapas);
    printf("Escoge un mapa:");
    scanf(" %s", archivoMapa);
    if (strcmp("quit", archivoMapa) == 0 || strcmp("QUIT", archivoMapa) == 0 || strcmp("Quit", archivoMapa) == 0 ||
        strcmp("q", archivoMapa) == 0 || strcmp("Q", archivoMapa) == 0) {
        return;
    }
    char comando[fichero_length];
    strcpy(comando, carpetaMapas);
    strcat(comando, archivoMapa);
    if (remove(comando) == 0) {
        printf("Se elimino correctamente %s", archivoMapa);
    } else {
        printf("No se pudo eliminar el archivo\n");
    }
}

//////////////////DATOS SOBRE EL MAPA:

/**
 * Imprimir la rutaOptima, es llamada por el _rutaOptima
 * @param G
 * @param P
 * @param origen
 * @param destino
 */
void imprimirRutaOptima(grafo G, struct camino P[][MAXVERTICES], int origen, int destino) {
    tipovertice *VECTOR; //Para almacenar el vector de vértices del grafo
    VECTOR = array_vertices(G);

    if (origen != destino) {
        imprimirRutaOptima(G, P, origen, P[origen][destino].valor);
        if (P[origen][destino].flagRol == 'I') {
            printf(BOLD  ROJO_T".."RESET_COLOR);
        } else if (P[origen][destino].flagRol == 'T') {
            printf(BOLD VERDE_T"--"RESET_COLOR);
        }
    }

    printf("%s", VECTOR[destino].habitacion);
}

/**
 * Calcula el Algoritmo de Floyd y Warshall para calcular la ruta optima
 * @param G
 * @param flagRol
 * @param numeroVertices
 * @param origen
 * @param destino
 */
void _rutaOptima(grafo G, int flagRol, int numeroVertices, int origen, int destino) {
    int D[MAXVERTICES][MAXVERTICES];
    struct camino P[MAXVERTICES][MAXVERTICES];

    AlgoritmoFloyd_Warshall(G, flagRol, numeroVertices, D, P);

    printf(BOLD MAGENTA_T"\nRUTA:\t "RESET_COLOR);
    //Imprimir la ruta mas corta
    imprimirRutaOptima(G, P, origen, destino);
    tipovertice *VECTOR = array_vertices(G);
    printf(BOLD MAGENTA_T"\nLA DISTANCIA ENTRE %s Y %s ES:%d\n"RESET_COLOR, VECTOR[origen].habitacion,
           VECTOR[destino].habitacion, D[origen][destino]);
}

/**
 * Calcula la ruta óptima de un grafo, pregunta Origen y Destino, llama _rutaOptima
 * @param G
 */
void rutaOptima(grafo G) {
    tipovertice v1, v2;
    char rol;

    printf("Ruta optima entre habitacion1--habitacion2\n");

    //Habitacion origen
    printf("Introduce habitacion origen: ");
    scanf(" %s", v1.habitacion);
    if (!existe_vertice(G, v1)) {
        printf("La habitacion %s no existe en el mapa\n", v1.habitacion);
        return;
    }

    //Habitacion destino
    printf("Introduce habitacion destino: ");
    scanf(" %s", v2.habitacion);
    if (!existe_vertice(G, v2)) {
        printf("La habitacion %s no existe en el mapa\n", v2.habitacion);
        return;
    }

    printf("Introduce para quien quieres calcular la ruta optima, Impostores(I) o Tripulantes(T):");
    scanf(" %c", &rol);


    if (rol == 'I' || rol == 'i') {
        //flagRol == 0 si Impostor
        _rutaOptima(G, 0, num_vertices(G), posicion(G, v1), posicion(G, v2));
    } else if (rol == 'T' || rol == 't') {
        //flagRol == 1 si Tripulante
        _rutaOptima(G, 1, num_vertices(G), posicion(G, v1), posicion(G, v2));
    }


}

/**
 * Calcula el Algoritmo de Prim, para el arbol de coste minimo
 * @param G
 * @param flagRol
 */
void _algoritmoPrim(grafo G, int flagRol) {
    int selected[num_vertices(G)];
    int numArcos = 0;
    int distanciaTotal = 0;
    int vx, vy, minimo;
    int caminoImprimir = 1;
    tipovertice *VECTOR = array_vertices(G);

    //Inicializamos a 0 el vector 'selected'
    for (int i = 0; i < num_vertices(G); i++) {
        selected[i] = 0;
    }

    printf(BOLD MAGENTA_T"ARBOL DE EXPANSION DE COSTE MINIMO:\n"RESET_COLOR);

    //El primer vertice lo marcamos a 1
    selected[0] = 1;

    while (numArcos < num_vertices(G) - 1) {
        minimo = infinito;
        vx = 0;
        vy = 0;
        for (int i = 0; i < num_vertices(G); i++) {
            if (selected[i] == 1) {
                for (int j = 0; j < num_vertices(G); j++) {
                    if (flagRol && selected[j] != 1 && son_adyacentes_T(G, i, j) != 0) {
                        if (minimo > son_adyacentes_T(G, i, j)) {
                            minimo = son_adyacentes_T(G, i, j);
                            vx = i;
                            vy = j;
                        }
                    } else if (!flagRol && selected[j] != 1 &&
                               (son_adyacentes_T(G, i, j) != 0 || son_adyacentes_I(G, i, j) != 0)) {
                        if (son_adyacentes_I(G, i, j) != 0) {
                            if (minimo > son_adyacentes_I(G, i, j)) {
                                minimo = son_adyacentes_I(G, i, j);
                                vx = i;
                                vy = j;
                                caminoImprimir = 0;
                            }

                        } else if (son_adyacentes_T(G, i, j) != 0) {
                            if (minimo > son_adyacentes_T(G, i, j)) {
                                minimo = son_adyacentes_T(G, i, j);
                                vx = i;
                                vy = j;
                                caminoImprimir = 1;
                            }
                        }
                    }
                }
            }
        }
        selected[vy] = 1;
        numArcos++;
        if (caminoImprimir) {
            printf(BOLD VERDE_T"%15s--%-15s:%-10d\n"RESET_COLOR, VECTOR[vx].habitacion, VECTOR[vy].habitacion,
                   son_adyacentes_T(G, vx, vy));
            distanciaTotal += son_adyacentes_T(G, vx, vy);

        } else {
            printf(BOLD  ROJO_T"%15s..%-15s:%-10d\n"RESET_COLOR, VECTOR[vx].habitacion, VECTOR[vy].habitacion,
                   son_adyacentes_I(G, vx, vy));
            distanciaTotal += son_adyacentes_I(G, vx, vy);

        }

    }
    printf(BOLD MAGENTA_T"DISTANCIA TOTAL:%d"RESET_COLOR, distanciaTotal);


}

/**
 * Calcula el Arbol de expansion de coste minimo, llama a _algoritmoPrim
 * @param G
 */
void arbolCosteMinimo(grafo G) {
    char rol;
    printf("Introduce para quien quieres calcular el arbol de expansion de coste minimo, Impostores(I) o Tripulantes(T):");
    scanf(" %c", &rol);

    if (rol == 'I' || rol == 'i') {
        //flagRol == 0 si Impostor
        _algoritmoPrim(G, 0);
    } else if (rol == 'T' || rol == 't') {
        //flagRol == 1 si Tripulante
        _algoritmoPrim(G, 1);

    }
}

//////////////////OPCIONES MENUTAREA:

/**
 * Imprimir las tareas Asociadas al mapa
 * @param G
 */
void _imprimirTareas_Mapa(grafo G) {
    int Ntareas = num_tareas(G);
    tipotareas *TAREAS = array_tareas(G);
    for (int i = 0; i < Ntareas; i++) {
        printf("%d)%s:%d\n", i + 1, TAREAS[i].nombreTarea, TAREAS[i].costeTarea);
    }
}

/**
 * Añade tareas a un mapa
 * @param G
 */
void _anadirTareas(grafo *G) {
    tipotareas tarea;
    printf("Introduce la tarea a añadir:");
    scanf(" %[^\n]", tarea.nombreTarea);
    printf("\nIntroduce el coste de la tarea:");
    scanf(" %d", &tarea.costeTarea);
    insertar_tareas(G, tarea);
    printf("Tarea Añadida-> %s:%d\n", tarea.nombreTarea, tarea.costeTarea);
}

/**
 * Pregunta y elimina la tarea seleccionada
 * @param G
 */
void _eliminarTarea(grafo *G) {
    char opc;
    int Ntareas = num_tareas(*G);
    tipotareas *TAREAS = array_tareas(*G);
    do {
        printf("Selecciona la tarea a eliminar:\n");
        for (int i = 0; i < Ntareas; i++) {
            printf("%d)%s\n", i + 1, TAREAS[i].nombreTarea);
        }

        printf("Opcion:");
        scanf(" %c", &opc);
    } while ((atoi(&opc) <= 1 && atoi(&opc) >= Ntareas) || opc == 'q' || opc == 'Q');

    printf("Se ha eliminado la tarea:%s", TAREAS[atoi(&opc) - 1].nombreTarea);
    eliminar_tareas(G, (atoi(&opc)));


}

//////////////////MENUS:

/**
 * Modificar Tareas de un mapa
 * @param G
 */
void _menuTareas(grafo *G) {
    char opcion;
    do {
        printf(BOLD "\n\n////////////Menu de Creacion de Mapa///////////////\n"RESET_COLOR);
        if (mapaCargado) {
            printf(BOLD "////////////Mapa Cargado: "BOLD CYAN_T "%s"RESET_COLOR  "///////////////\n"RESET_COLOR,
                   recuperarNombre(*G));
        }
        printf("a) Añadir tareas\n");
        printf("b) Elimnar tareas\n");
        printf("c) Imprimir tareas\n");
        printf("s) Salir\n");

        printf(BOLD "Opcion: "RESET_COLOR);
        scanf(" %c", &opcion);

        switch (opcion) {
            case 'a':
            case 'A':
                _anadirTareas(G);
                break;
            case 'b':
            case 'B':
                _eliminarTarea(G);
                break;
            case 'c':
            case 'C':
                _imprimirTareas_Mapa(*G);
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

/**
 * Menu Secundario dentro de crear mapa o modificar mapa
 * @param G
 */
void _menuGrafo(grafo *G, int flagCrear) {

    char opcion;
    if (flagCrear) {
        char *aux;
        aux = _nombreGrafo();
        if (strcmp("quit", aux) == 0) {
            return;
        }
        _vaciarMapaCargado(G);
        introducirNombre_Grafo(*G, aux);
        mapaCargado = 1;
    }
    do {
        printf(BOLD "\n\n////////////Menu de Creacion de Mapa///////////////\n"RESET_COLOR);
        if (mapaCargado) {
            printf(BOLD "////////////Mapa Cargado: "BOLD CYAN_T "%s"RESET_COLOR  "///////////////\n"RESET_COLOR,
                   recuperarNombre(*G));
        }
        printf("a) Añadir habitacion\n");
        printf("b) Clausular habitacion\n");
        printf("c) Crear comunicacion entre habitaciones\n");
        printf("d) Eliminar comunicacion entre habitaciones\n");
        printf("i) Imprimir mapa\n");
        printf("t) Imprimir tareas\n");
        printf("h) Modificar tareas\n");
        printf("r) Ver cual es la ruta más rápida entre dos habitaciones\n");
        printf("p) Ver el árbol de expansión de coste mínimo\n");
        printf("g) Guardar el mapa actual\n");
        printf("s) Salir\n");

        printf(BOLD "Opcion: "RESET_COLOR);
        scanf(" %c", &opcion);

        switch (opcion) {
            case 'a':
            case 'A':
                introducir_vertice(G);
                break;
            case 'b':
            case 'B':
                eliminar_vertice(G);
                break;
            case 'c':
            case 'C':
                nuevo_arco(G);
                break;
            case 'd':
            case 'D':
                eliminar_arco(G);
                break;
            case 'i':
            case 'I':
                _imprimir_grafo(*G);
                break;
            case 't':
            case 'T':
                _imprimirTareas_Mapa(*G);
                break;
            case 'h':
            case 'H':
                _menuTareas(G);
                break;
            case 'r':
            case 'R':
                rutaOptima(*G);
                break;
            case 'p':
            case 'P':
                arbolCosteMinimo(*G);
                break;
            case 'g':
            case 'G':
                _guadarArchivo_Mapa(*G);
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

/**
 * Menu Principal seccion de Mapa del menu en el main.c
 * @param G
 */
void menuMapaPrincipal(grafo *G) {

    char opcion;

    do {
        printf("\n\n////////////"BOLD"Menu Mapa"RESET_COLOR"///////////////\n");
        if (mapaCargado) {
            printf("////////////"BOLD VERDE_T"Mapa Cargado:%s"RESET_COLOR"///////////////\n",
                   recuperarNombre(*G));
        }
        printf("a) Crear Mapa\n");
        printf("m) Modificar Mapa cargado\n");
        printf("b) Leer un mapa(.txt)\n");
        printf("c) Eliminar un mapa(.txt)\n");
        printf("s) Atrás\n");

        printf(BOLD "Opcion: "RESET_COLOR);
        scanf(" %c", &opcion);

        switch (opcion) {
            case 'a':
            case 'A':
                _menuGrafo(G, 1);
                break;
            case 'b':
            case 'B':
                leerArchivo_Mapa(G, 0);
                break;
            case 'c':
            case 'C':
                _eliminarMapa();
                break;
            case 'm':
            case 'M':
                if (mapaCargado) {
                    _menuGrafo(G, 0);
                } else {
                    printf("Primero debes cargar un mapa\n");
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