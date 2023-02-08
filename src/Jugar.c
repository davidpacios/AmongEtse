#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Jugar.h"

///////////////////////////////////IMPLEMENTACION DE CREACION DE MAPAS|LISTADOS/////////////////////////////////////
//todo esta implementado, pero no revisado y no terminado de corregir errores al jugar la partida, con el mapa DEFAULT:Skeld.txt no hay problemas
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////Posible Modificacion Para Aumentar La Dificultad/////////////////////////////////////
//todo se podria no imprimir la variable ProxHabitacion porque los impostores pueden tener una ProxHabitacion dada por trampillas y
//todo si sabes el mapa ya sabes que esa comunicacion no es para tripulantes, otra forma seria que al moverse para hacer tareas en
//todo _moverTripulantes jugador.actividad == MOVERSE poner que solo sea sonadyacentes_T y que los impostores asi solo usen los camino, cuando
//todo el espectador no sabes que habitaicones esta usando para matar o para escapar.
//todo añadir tiempo al skip, para que aumente la dificultad
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////VAR GLOBALES:
int numeroImpostores = 0;
int ImpostoresActuales = 0;

////////////////////////////////////////////////////FUNCIONES:

////////////////AYUDA:

/**
 * Funcion privada para generar un aleatorio
 * @param inf
 * @param sup
 * @return
 */
unsigned int _aleatorio(int inf, int sup) {
    return (rand() % (sup - inf + 1)) + inf;
}

/**
 * Funcion para cambiar una palabra de minusculas a mayusculas
 * @param palabra
 * @return
 */
char *_cambiarMayusculas(char *palabra) {
    int n = strlen(palabra);
    char *PALABRA;
    PALABRA = (char *) malloc(n + 1 * (sizeof(char)));
    for (int i = 0; i < n; i++) {
        PALABRA[i] = toupper((char) palabra[i]);
    }
    return PALABRA;
}

/**
 * Solicita entre las rondas el posibles impostor o SKIP
 * @param Jugadores
 * @param flag_skip
 * @param flag_nombre
 * @return
 */
tipoelem _solicitarNicknamePosibleImpostor(abb Jugadores, int *flag_skip, int *flag_nombre) {
    tipoelem jugador;
    do {

        printf(BOLD AMARILLO_T"¿Quien es el impostor//Skip?");
        scanf(" %s", jugador.nombreUsuario);
        printf(RESET_COLOR);

        if (strcmp(jugador.nombreUsuario, "Skip") == 0 || strcmp(jugador.nombreUsuario, "SKIP") == 0 ||
            strcmp(jugador.nombreUsuario, "skip") == 0 || strcmp(jugador.nombreUsuario, "s") == 0 || strcmp(jugador.nombreUsuario, "S") == 0 ) {
            *flag_skip = 1;
        }
        if (condicionNickname(jugador) && es_miembro(Jugadores, jugador)) {
            *flag_nombre = 1;
        }

    } while (*flag_skip == 0 && *flag_nombre == 0);


    return jugador;

}


////////////////ANTES PARTIDA:

/**
 * Funcion que devuelve el numero de jugadores por referencias de un arbol
 * @param Usuarios
 * @param numero
 * @return
 */
void _numeroJugadores(abb Usuarios, int *numero) {

    if (!es_vacio(Usuarios)) {
        _numeroJugadores(izq(Usuarios), numero);
        (*numero)++;
        _numeroJugadores(der(Usuarios), numero);
    }
}

/**
 * Controlar el inicio del Juego,Se deberia controlar que haya suficientes arcos?
 * @param G
 * @param Jugadores
 * @return 1 en Condicion Aceptada
 */
int condicionJugar(grafo G, abb Jugadores) {
    int Nvertices = num_vertices(G);
    int Ntareas = num_tareas(G);
    int Njugadores;
    _numeroJugadores(Jugadores, &Njugadores);
    if (Nvertices >= minVertices && Ntareas >= minTareas && Njugadores >= minJugadores && todosVadyacentes(G)) {
        return 1;
    }
    return 0;

}

/**
 * Funcion para dar de baja jugadores, antes de comnezar la partida, llama a bajaJugador()
 * @param Usuarios
 * @param numJugadores
 */
void _eliminarJugadores(abb *Usuarios, int *numJugadores) {
    listadoJugadores(*Usuarios, 0);
    while (*numJugadores > jugadoresXpartida) {
        printf(BOLD AMARILLO_T"Numero de Jugadores a Eliminar:%d\n", *numJugadores - jugadoresXpartida);
        if (bajaJugador(Usuarios)) {
            (*numJugadores)--;
        }
        printf(RESET_COLOR);
    }
}

/**
 * Genera un aleatorio de todos los posibles tareas del mapa y su peso por referencia
 * @param G
 * @param peso
 * @return
 */
char *_generaTarea(grafo G, int *peso) {
    unsigned int op;
    char *aux;
    int Ntareas = num_tareas(G);
    tipotareas *TAREAS = array_tareas(G);
    aux = (char *) malloc(task_length * sizeof(char));
    op = _aleatorio(1, Ntareas);
    strncpy(aux, TAREAS[op - 1].nombreTarea, task_length);
    *peso = TAREAS[op - 1].costeTarea;
    return aux;

}

/**
 * Funcion que te permite seleccionar una habitacion
 * @param flagInicio
 * @param G
 * @return
 */
char *_escogerHabitacion(int flagInicio, grafo G) {
    char op;
    char *aux;
    int N = num_vertices(G);
    tipovertice *VECTOR = array_vertices(G);
    aux = (char *) malloc(location_length * sizeof(char));
    int posi = 0;
    if (flagInicio == 1) {
        do {
            printf(BOLD AMARILLO_T"Donde quieres que comiencen los jugadores:\n"RESET_COLOR);
            for (int i = 0; i < N; i++) {
                printf("%d) %s\n", i + 1, VECTOR[i].habitacion);
            }
            printf(BOLD AMARILLO_T"OPCION:");
            scanf(" %c", &op);
            printf(RESET_COLOR);
        } while ((atoi(&op) < 1 || atoi(&op) > N));
        posi = atoi(&op) - 1;

    } else if (flagInicio == 2) {
        posi = _aleatorio(0, N - 1);
    }

    strncpy(aux, VECTOR[posi].habitacion, location_length);
    return aux;

}

/**
 * Copia el Arbol de Usuarios a Jugadores
 * @param Usuarios
 * @param Jugadores
 */
void _copiarArbol(abb Usuarios, abb *Jugadores) {
    tipoelem jugador;
    tipoelem nuevoJugador;
    if (!es_vacio(Usuarios)) {
        leer(Usuarios, &jugador);
        strncpy(nuevoJugador.nombreUsuario, jugador.nombreUsuario, strlen(jugador.nombreUsuario) + 1);
        nuevoJugador.rol = jugador.rol;
        nuevoJugador.tareas = NULL;
        insertar(Jugadores, nuevoJugador);

        _copiarArbol(izq(Usuarios), Jugadores);
        _copiarArbol(der(Usuarios), Jugadores);
    }
}

/**
 * Inicializa los valores de ROL de cada jugador
 * @param jugadores
 */
void _inicializarRol(tipoelem *jugadores) {
    int op = 0;
    if (numeroImpostores > ImpostoresActuales) {
        op = _aleatorio(0, 1);
    }
    switch (op) {
        case 0:
            jugadores->rol = 'T';
            break;
        case 1:
            jugadores->rol = 'I';
            ImpostoresActuales++;
            break;
    }
}

/**
 * Funcion para hacer un recorrido de Jugadores y asegurarnos de que numeroImpostores = ImpostoresActuales,
 * en otro caso se vuelven a reasignar roles, haciendo de nuevo el recorrido
 * @param Jugadores
 */
void _recorridoInicializar_Rol(abb Jugadores) {
    tipoelem jugador;
    if (!es_vacio(Jugadores)) {

        leer(Jugadores, &jugador);
        _inicializarRol(&jugador); //Inicializa el campo del jugador.rol en funcion de numeroImpostores
        modificar(Jugadores, jugador);

        _recorridoInicializar_Rol(izq(Jugadores));
        _recorridoInicializar_Rol(der(Jugadores));

    }
}

/**
 * Funcion que inicializa las tareas de cada jugador
 * @param jugador
 * @param G
 */
void _inicializarTareas(tipoelem *jugador, grafo G) {
    tipoelemCola tareas;
    if (jugador->rol == 'I') {//Se le da un numero diferente de tareas a los Impostores porque al tener trampillas pueden acabar antes las tareas
        for (int i = 0; i < task_numberI; i++) {
            strncpy(tareas.lugarTarea, _escogerHabitacion(2, G), location_length);
            strncpy(tareas.nombreTarea, _generaTarea(G, &tareas.pesoTarea), task_length);
            insertar_cola(&(jugador->tareas), tareas);
        }
    } else {
        for (int i = 0; i < task_numberT; i++) {
            strncpy(tareas.lugarTarea, _escogerHabitacion(2, G), location_length);
            strncpy(tareas.nombreTarea, _generaTarea(G, &tareas.pesoTarea), task_length);
            insertar_cola(&(jugador->tareas), tareas);
        }
    }


}

/**
 * Funcion para inicializar todos los jugadores, llama a _inicializarTareas.
 * @param jugador
 * @param habitacionInicio
 * @param G
 * @param Jugadores
 */
void _inicializarJugador(tipoelem *jugador, char *habitacionInicio, grafo G) {
    crear_cola(&(jugador->tareas));
    strncpy(jugador->ubicacionActual, habitacionInicio, strlen(habitacionInicio) + 1);
    jugador->pasosAcumulados = 0;
    jugador->pesoTareaActual = 0;
    jugador->actividad = NADA;
    _inicializarTareas(jugador, G);
    printf("Sale de inicializar\n");
}

/**
 * Inicializa todos los campos de todos los jugadores menos el ROL
 * @param Jugadores
 * @param habitacionInicio
 * @param G
 */
void _inicializar(abb Jugadores, char *habitacionInicio, grafo G) {
    tipoelem jugador;
    if (!es_vacio(Jugadores)) {

        leer(Jugadores, &jugador);
        _inicializarJugador(&jugador, habitacionInicio, G);
        modificar(Jugadores, jugador);

        _inicializar(izq(Jugadores), habitacionInicio, G);
        _inicializar(der(Jugadores), habitacionInicio, G);
    }
}

/**
 * Devuelve el numero de impostores y tripulantes por referencia
 * @param Jugadores
 * @param numeroImpostores
 * @param numeroT
 */
void _contadorRoles(abb Jugadores, int *numeroI, int *numeroT) {
    tipoelem jugador;
    if (!es_vacio(Jugadores)) {
        _contadorRoles(izq(Jugadores), numeroI, numeroT);

        leer(Jugadores, &jugador);

        if (jugador.rol == 'I') {
            (*numeroI)++;
        } else if (jugador.rol == 'T') {
            (*numeroT)++;
        }

        _contadorRoles(der(Jugadores), numeroI, numeroT);
    }

}

/**
 * Genera los valores Iniciales de la partida y de los jugadores,_numeroJugadores,numImpostores,Habitacion de Inicio
 * y copia los datos de Usuarios en Jugadores
 * @param Usuarios
 * @param Jugadores
 * @param numJugadores
 * @param G
 */
char *_generarPartida(abb Usuarios, abb *Jugadores, int *numJugadores, grafo G) {
    _copiarArbol(Usuarios, Jugadores);
    char *habitacionInicio;
    char opc;
    _numeroJugadores(*Jugadores, numJugadores);
    if (*numJugadores > jugadoresXpartida) {
        limpiarPantallaTerminal();
        printf(BOLD AMARILLO_T "Sois %d jugadores y como maximo podeis ser %d:\n"RESET_COLOR, *numJugadores,
               jugadoresXpartida);
        _eliminarJugadores(Jugadores, numJugadores);
    }
    if (*numJugadores > jugadoresXpartida / 2) {
        limpiarPantallaTerminal();
        do {
            printf(BOLD AMARILLO_T "Sois %d jugadores,Numero de impostores(1/2):", *numJugadores);
            scanf(" %c", &opc);
            printf(RESET_COLOR);
        } while (opc != '1' && opc != '2');
        numeroImpostores = atoi(&opc);
    } else {
        numeroImpostores = 1;
    }

    limpiarPantallaTerminal();
    habitacionInicio = _escogerHabitacion(1, G);

    ImpostoresActuales = 0;
    while (numeroImpostores > ImpostoresActuales) {
        ImpostoresActuales = 0;
        _recorridoInicializar_Rol(*Jugadores);
    }
    _inicializar(*Jugadores, habitacionInicio, G);


    return habitacionInicio;
}

////////////////HABITACIONES Y RUTAS:

/**
 * Devuelve por referencia la habitacion siguiente a la que debemos movernos para ir desde Origen a Destino.
 * @param G
 * @param P
 * @param origen
 * @param destino
 */
void _habitacionProxima(grafo G, struct camino P[][MAXVERTICES], int origen, int destino,tipovertice *proximaHabitacion, int *distanciaTotalRuta) {
    tipovertice *VECTOR; //Para almacenar el vector de vértices del grafo
    VECTOR = array_vertices(G);

    if (origen != destino) {
        if (origen == P[origen][destino].valor) {
            *proximaHabitacion = VECTOR[destino];
        }
        _habitacionProxima(G, P, origen, P[origen][destino].valor, proximaHabitacion, distanciaTotalRuta);
    }

}

/**
 * Funcion axiliar que llama a AlgoritmoFloyd_Warshall y _habitacionProxima
 * @param G
 * @param flagRol
 * @param origen
 * @param destino
 * @return la distancia total de la ruta
 */
int _proximaHabitacionAuxiliar(grafo G, int flagRol,int origen, int destino,tipovertice *proximaHabitacion,int D[][MAXVERTICES],struct camino P[][MAXVERTICES]){
    int distanciaTotalRuta;
    AlgoritmoFloyd_Warshall(G, flagRol, num_vertices(G), D, P);
    _habitacionProxima(G, P, origen, destino, proximaHabitacion, &distanciaTotalRuta);
    distanciaTotalRuta = D[origen][destino];
    return distanciaTotalRuta;
}

/**
 * Calcula la habitacion siguiente y la distancia, para llegar desde una ubiacion inicial v1 hasta una ubicacion destino v2
 * @param G
 * @param v1
 * @param v2
 * @param rol
 * @param distanciaTotalRuta
 * @return
 */
tipovertice _proximaHabitacion(grafo G, tipovertice v1, tipovertice v2, char rol, int *distanciaTotalRuta) {
    tipovertice proximaHabitacion;
    if (!existe_vertice(G, v1)) {
        printf("La habitacion %s no existe en el mapa\n", v1.habitacion);
        exit(-1);
    }

    if (!existe_vertice(G, v2)) {
        printf("La habitacion %s no existe en el mapa\n", v2.habitacion);
        exit(-1);
    }

    int D[MAXVERTICES][MAXVERTICES];
    struct camino P[MAXVERTICES][MAXVERTICES];

    if (rol == 'I' || rol == 'i' || rol == 'E') {
        //flagRol == 0 si Impostor
        (*distanciaTotalRuta) = _proximaHabitacionAuxiliar(G,0, posicion(G,v1), posicion(G,v2), &proximaHabitacion,D,P);
        return proximaHabitacion;
    } else if (rol == 'T' || rol == 't' || rol == 'K') {
        //flagRol == 1 si Tripulante
        (*distanciaTotalRuta) = _proximaHabitacionAuxiliar(G,0, posicion(G,v1), posicion(G,v2), &proximaHabitacion,D,P);
        return proximaHabitacion;
    }
    printf("Error al encontrar la _proximaHabitacion\n");
    exit(-1);
}

/**
 * Inicializa los campos para llamar a _proximahabitacion.
 * @param jugador
 * @param G
 * @param distanciaTotal
 * @return
 */
tipovertice _siguienteHabitacion(tipoelem *jugador, grafo G, int *distanciaTotal) {
    tipoelemCola aux;
    tipovertice v1, v2, sigHabitacion;
    aux = primero(jugador->tareas);
    strncpy(v1.habitacion, jugador->ubicacionActual, strlen(jugador->ubicacionActual) + 1);//Vertice Origen
    strncpy(v2.habitacion, aux.lugarTarea, strlen(aux.lugarTarea) + 1);//Vertice Destiino
    sigHabitacion = _proximaHabitacion(G, v1, v2, jugador->rol, distanciaTotal);//Vertice inmediato, siguiente al origen para llegar al destino
    return sigHabitacion;
}

/**
 * Busca la habitacion mas cercana de coste minimo, si hay algun error devuelve INFINITO,devuelve la posicion en el VECTOR de Vertices
 * @param G
 * @param v1
 * @param rol
 * @return
 */
int _buscarHabitacionCerca(grafo G, tipovertice v1, char rol, int *distancia) {
    int N = num_vertices(G);
    tipovertice *VECTOR = array_vertices(G);
    unsigned int aleatorio_habitacionAdyacente;
    *distancia = infinito;
    int posi;
    for (int i = 0; i < N; i++) {
        aleatorio_habitacionAdyacente = _aleatorio(0, 1);
        if (posicion(G, VECTOR[i]) != posicion(G, v1)) {
            if ((rol == 'I' || rol == 'E')) {
                if (son_adyacentes_I(G, posicion(G, v1), posicion(G, VECTOR[i])) != 0 && son_adyacentes_I(G, posicion(G, v1), posicion(G, VECTOR[i])) <= *distancia) {
                    *distancia = son_adyacentes_I(G, posicion(G, v1), posicion(G, VECTOR[i]));
                    posi = i;
                    if (aleatorio_habitacionAdyacente) {//Para evitar rutas repetitivas(Msuperior-Reactor...)
                        return posi;
                    }
                } else if (son_adyacentes_T(G, posicion(G, v1), posicion(G, VECTOR[i])) != 0 && son_adyacentes_T(G, posicion(G, v1), posicion(G, VECTOR[i])) <= *distancia) {
                    *distancia = son_adyacentes_T(G, posicion(G, v1), posicion(G, VECTOR[i]));
                    posi = i;
                    if (aleatorio_habitacionAdyacente) {//Para evitar rutas repetitivas(Msuperior-Reactor...)
                        return posi;
                    }
                }
            } else if ((rol == 'T' || rol == 'K') && son_adyacentes_T(G, posicion(G, v1), posicion(G, VECTOR[i]) != 0)) {
                if (son_adyacentes_T(G, posicion(G, v1), posicion(G, VECTOR[i])) <= *distancia) {
                    *distancia = son_adyacentes_T(G, posicion(G, v1), posicion(G, VECTOR[i]));
                    posi = i;
                    if (aleatorio_habitacionAdyacente) {//Para evitar rutas repetitivas(Msuperior-Reactor...)
                        return posi;
                    }
                }
            }
        }
    }
    if (*distancia != infinito) {
        return posi;
    } else {
        return infinito;
    }

}

////////////////ESTADOS DE LOS JUGADORES:

/**
 * Recalcula la ruta moviendose para seguir yendo hacia la tarea
 * @param jugador
 * @param G
 */
void _recalcularRutaMOVIENDOSE(tipoelem *jugador,grafo G){
    tipovertice sigHabitacion;
    int distanciaTotal = 0;
    sigHabitacion = _siguienteHabitacion(jugador, G, &distanciaTotal);//Calculamos la ProxHabitacion.
    strncpy(jugador->proximaUbicacion, sigHabitacion.habitacion, strlen(sigHabitacion.habitacion) + 1);
    jugador->actividad = MOVIENDOSE;
}

/**
 * Si la HabTarea es == a jugador.ubicacionActual->jugador.actividad = Tarea y devuelve 1
 * @param jugador
 * @return
 */
int _condicionEstaEnTarea(tipoelem *jugador){
    tipoelemCola aux;
    aux = primero(jugador->tareas);
    if (strcmp(jugador->ubicacionActual, aux.lugarTarea) ==0) {//Si la ubicacionActual es igual a la ubicacion de la tarea.
        strncpy(jugador->proximaUbicacion, enTarea, strlen(enTarea) + 1);
        jugador->actividad = TAREA;
        return 1;
    }
    return 0;
}

/**
 * Acciones cuando el jugador.actividad == NADA
 * @param jugador
 */
void _estadoNADA(tipoelem *jugador, grafo G) {
    tipoelemCola aux;
    aux = primero(jugador->tareas);
    jugador->pesoTareaActual = aux.pesoTarea;
    _recalcularRutaMOVIENDOSE(jugador,G);
    _condicionEstaEnTarea(jugador);
}

/**
 * Acciones comunes para todos los ROLES dentro de moviendose
 * @param jugador
 * @param G
 * @param distanciaTotal
 * @return 1 si ya esta en la habitacion de la tarea, else = 0
 */
int _estadoMOVIENDOSE_ComunROL(tipoelem *jugador, grafo G) {
    strncpy(jugador->ubicacionActual, jugador->proximaUbicacion, strlen(jugador->proximaUbicacion) + 1);
    if(_condicionEstaEnTarea(jugador)){
        return 1;
    }
    _recalcularRutaMOVIENDOSE(jugador,G);
    return 0;
}

/**
 * Acciones cuando el jugador.actividad == MOVIENDOSE
 * @param jugador
 * @param G
 * @param distanciaTotal
 */
void _estadoMOVIENDOSE(tipoelem *jugador, grafo G) {
    //Se podria interpretar que todos se mueven por caminos de tripulantes para hacer tareas, para asi evitar que la comunicacion a
    // ProxHabitacion sea una trampilla y sea obv que es el impostor, pero en el juego real depende el propio jugador.(SUBJETIVO en un juego cmd)
    tipovertice sigHabitacion;
    int distanciaTotal = 0;
    sigHabitacion = _siguienteHabitacion(jugador, G, &distanciaTotal);
    tipovertice v1;
    if (jugador->pasosAcumulados > 0) {
        strncpy(v1.habitacion, jugador->ubicacionActual,strlen(jugador->ubicacionActual) + 1); //Para la funcion posicion
        int seguir = 0;
        if (jugador->rol == 'T' || jugador->rol == 'K') {
            while (jugador->pasosAcumulados >= son_adyacentes_T(G, posicion(G, v1), posicion(G, sigHabitacion)) && seguir == 0) {
                jugador->pasosAcumulados -= son_adyacentes_T(G, posicion(G, v1), posicion(G, sigHabitacion));
                seguir = _estadoMOVIENDOSE_ComunROL(jugador,G);//Condicion de parada, porque ha llegado a la habitacion de la tarea
            }
        } else if (jugador->rol == 'E' || jugador->rol == 'I') {
            while (jugador->pasosAcumulados >= son_adyacentes_I(G, posicion(G, v1), posicion(G, sigHabitacion)) && seguir == 0) {
                jugador->pasosAcumulados -= son_adyacentes_I(G, posicion(G, v1), posicion(G, sigHabitacion));
                seguir = _estadoMOVIENDOSE_ComunROL(jugador, G);
            }
        }
    }


}

/**
 * Acciones cuando el jugador.actividad == TAREA
 * @param jugador
 * @param G
 * @param distanciaTotal
 */
void _estadoTAREA(tipoelem *jugador) {
    if (jugador->pasosAcumulados > 0) {
        if (jugador->pasosAcumulados >= jugador->pesoTareaActual) {
            int valor = jugador->pasosAcumulados;
            jugador->pasosAcumulados = jugador->pasosAcumulados - jugador->pesoTareaActual;
            jugador->pesoTareaActual = jugador->pesoTareaActual - valor;
            if (jugador->pesoTareaActual < 0) {
                jugador->pesoTareaActual = 0;
            }
            jugador->actividad = ESPERANDO;
        } else {
            jugador->pesoTareaActual = jugador->pesoTareaActual - jugador->pasosAcumulados;
            jugador->pasosAcumulados = 0;
        }

    }
}

/**
 * Accion cuando jugador.actividad == ESPERANDO
 * @param jugador
 * @param G
 */
void _estadoESPERANDO(tipoelem *jugador, grafo G) {
    tipoelemCola aux;
    suprimir_cola(&(jugador->tareas));
    if (jugador->tareas != NULL && !es_vacia_cola(jugador->tareas)) {
        aux = primero(jugador->tareas);
        jugador->pesoTareaActual = aux.pesoTarea;
        if (!_condicionEstaEnTarea(jugador)) {//Si su hab es la habitacion de la tarea devuelve 1 sino devuelve 0 y ->
            _recalcularRutaMOVIENDOSE(jugador,G);//->Sigue moviendose hacia su tarea,recalculamos ProxHabitacion
        }
    } else {
        jugador->actividad = TERMINADO;
        destruir_cola(&(jugador->tareas));
        strncpy(jugador->proximaUbicacion, tareasTerminadas, strlen(tareasTerminadas) + 1);
        jugador->pesoTareaActual = 0;
        jugador->tareas = NULL;
    }
}

/**
 * Accion cuando jugador.actividad == TERMINADO
 * @param jugador
 * @param G
 */
void _estadoTERMINADO(tipoelem *jugador, grafo G) {
    int distanciaTotal = 0;
    int posi;
    tipovertice *VECTOR = array_vertices(G);
    tipovertice v1;
    strncpy(v1.habitacion, jugador->ubicacionActual, strlen(jugador->ubicacionActual) + 1);
    posi = _buscarHabitacionCerca(G, v1, jugador->rol,&distanciaTotal);//Buscamos habitaciones cerca, con un coste bajo, tb depende de un aleatorio, para evitar rutas repetitivas
    if (jugador->pasosAcumulados >= distanciaTotal && posi != infinito) {
        jugador->pasosAcumulados -= distanciaTotal;
        strncpy(jugador->ubicacionActual, VECTOR[posi].habitacion, strlen(VECTOR[posi].habitacion) + 1);
    }

}

////////////////EJECUCION PARTIDA:

/**
 * Actualiza el numero de pasos de los jugadores
 * @param Jugadores
 */
void _siguienteRonda(abb Jugadores) {

    tipoelem jugador;

    if (!es_vacio(Jugadores)) {
        _siguienteRonda(izq(Jugadores));


        leer(Jugadores, &jugador);
        if (jugador.actividad == TERMINADO) {
            jugador.pasosAcumulados += pasosTerminado;

        } else {
            if (jugador.rol == 'T' || jugador.rol == 'K') {
                jugador.pasosAcumulados += pasosTXronda;

            }
            if (jugador.rol == 'I' || jugador.rol == 'E') {
                jugador.pasosAcumulados += pasosIXronda;
            }
        }
        modificar(Jugadores, jugador);

        _siguienteRonda(der(Jugadores));
    }

}

/**
 * Actualiza las tareas y ubicaciones de los impostores, tambien modifica sus estados
 * @param Jugadores
 * @param G
 */
void _moverTripulante(abb Jugadores, grafo G) {
    tipoelem jugador;
    if (!es_vacio(Jugadores)) {
        _moverTripulante(izq(Jugadores), G);

        leer(Jugadores, &jugador);

        if (jugador.actividad == NADA) {
            _estadoNADA(&jugador, G);
        }
        if (jugador.actividad == MOVIENDOSE) {
            _estadoMOVIENDOSE(&jugador, G);
        }
        if (jugador.actividad == TAREA) {
            _estadoTAREA(&jugador);
        }
        if (jugador.actividad == ESPERANDO) {
            _estadoESPERANDO(&jugador, G);
        }
        if (jugador.actividad == TERMINADO) {
            _estadoTERMINADO(&jugador, G);
        }

        modificar(Jugadores, jugador);
        _moverTripulante(der(Jugadores), G);


    }
}

/**
 * Devuelve el impostor 'i'
 * @param Jugadores
 * @param impostor
 * @param i
 * @param contador
 */
void _buscarImpostor(abb Jugadores, tipoelem *impostor, int i, int *contador) {

    if (!es_vacio(Jugadores)) {

        leer(Jugadores, impostor);
        if (impostor->rol == 'I') {
            (*contador)++;
        }

        if (*contador != i) {
            _buscarImpostor(izq(Jugadores), impostor, i, contador);
        }
        if (*contador != i) {
            _buscarImpostor(der(Jugadores), impostor, i, contador);
        }
    }
}

/**
 * Accion de matar del impostor,moviendolo, si es posible, para matar.
 * @param jugador
 * @param impostor
 * @param algunMuerto
 * @param distanciaMatar
 * @param aleatorio_pasosExtra
 * @return
 */
void _matar(tipoelem *jugador, tipoelem *impostor, int *algunMuerto, int distanciaMatar,unsigned int *aleatorio_pasosExtra) {
    jugador->rol = 'K';
    printf(BOLD ROJO_T"%s ha sido asesinado en %s\n"RESET_COLOR, jugador->nombreUsuario, jugador->ubicacionActual);
    strncpy(impostor->ubicacionActual, jugador->ubicacionActual, strlen(jugador->ubicacionActual) + 1);
    *aleatorio_pasosExtra -= distanciaMatar;
    *algunMuerto = 1;
}

/**
 * Actualizar los datos del impostor en funcion a su actividad, para evitar al modificar su HAB no queden datos mal como ProxHab o actividad
 * @param G
 * @param impostor
 * @param posi
 */
void _actualizarImpostorEscapar(grafo G, tipoelem *impostor, int posi, int flagEscapar) {

    tipovertice sigHabitacion;
    int distanciaTotal = 0;
    tipoelemCola aux;
    if (flagEscapar) {//Mata y escapa
        tipovertice *VECTOR = array_vertices(G);
        strncpy(impostor->ubicacionActual, VECTOR[posi].habitacion, location_length);
    }
    if (impostor->actividad == TAREA) {//Si el impostor antes de matar y escapar estaba simuladno una tarea, se le suprime y se pasa a la siguiente recalculando la ruta
        suprimir_cola(&(impostor->tareas));
        if (!es_vacia_cola(impostor->tareas)) {
            aux = primero(impostor->tareas);
            if (strcmp(impostor->ubicacionActual, aux.lugarTarea) == 0) {//Si coincide que a donde escapa es la hab de la nueva tarea
                strncpy(impostor->proximaUbicacion, enTarea, strlen(enTarea) + 1);
                impostor->actividad = TAREA;
            } else {//Sino se calcula su ProxHabitaicon y tendra una ruta nueva
                sigHabitacion = _siguienteHabitacion(impostor, G, &distanciaTotal);
                strncpy(impostor->proximaUbicacion, sigHabitacion.habitacion, location_length);
                impostor->actividad = MOVIENDOSE;
            }
        } else {//Si era su ultima tarea->jugador.actividad == TERMINADO.
            impostor->actividad = TERMINADO;
            destruir_cola(&(impostor->tareas));
            strncpy(impostor->proximaUbicacion, tareasTerminadas, strlen(tareasTerminadas) + 1);
            impostor->pesoTareaActual = 0;
            impostor->tareas = NULL;
        }

    }else if (impostor->actividad == MOVIENDOSE) {//Si antes de matar y escapar estaba moviendose
        aux = primero(impostor->tareas);
        if (strcmp(impostor->ubicacionActual, aux.lugarTarea) == 0) {//Si la tarea que tenia coincide con la habitacion a la que ha escapado
            strncpy(impostor->proximaUbicacion, enTarea, strlen(enTarea) + 1);
            impostor->actividad = TAREA;
        } else {//Sino se recalcula su ruta
            sigHabitacion = _siguienteHabitacion(impostor, G, &distanciaTotal);
            strncpy(impostor->proximaUbicacion, sigHabitacion.habitacion, location_length);
        }
    }

}

/**
 * Accion de escapar del impostor una vez ha matado
 * @param G
 * @param impostor
 * @param aleatorio_pasosExtra
 * @return
 */
int _escapar(grafo G, tipoelem *impostor, unsigned int aleatorio_pasosExtra) {
    int posi, distanciaEscapar = 0;
    tipovertice v1;
    strncpy(v1.habitacion, impostor->ubicacionActual, location_length);
    if ((posi = _buscarHabitacionCerca(G, v1, impostor->rol, &distanciaEscapar) != infinito)) {
        if (distanciaEscapar < aleatorio_pasosExtra) {
            _actualizarImpostorEscapar(G, impostor, posi, 1);
            return 1;
        }
    }
    return 0;
}

/**
 * Se lleva a cabo el asesinato de los tripulantes,los impostores tienen un aleatorio de escapar
 * @param Jugadores
 * @param impostor
 * @param algunMuerto
 */
void _recorridoMatarTripulante(abb Jugadores, grafo G, tipoelem *impostor, int *algunMuerto) {
    tipoelem jugador;
    tipovertice v1, v2;
    int distancia;
    unsigned int aleatorio_muerte = _aleatorio(0, 1);
    unsigned int aleatorio_pasosExtra = _aleatorio(2, 5);
    unsigned int aleatorio_escapa = _aleatorio(0, 2);
    unsigned int aleatorio_hacerRuido = _aleatorio(0, 1);
    int posi;

    if (!es_vacio(Jugadores)) {

        leer(Jugadores, &jugador);
        if (aleatorio_muerte && jugador.rol == 'T' && jugador.actividad != NADA && jugador.actividad != MOVIENDOSE &&
            aleatorio_pasosExtra > 0) {
            //No se usa buscarHabitacion porque, esta planteada para un unico  jugador y aqui se comparan las ubicaciones de jugador con impostor
            strncpy(v1.habitacion, impostor->ubicacionActual, strlen(impostor->ubicacionActual) + 1);
            strncpy(v2.habitacion, jugador.ubicacionActual, strlen(jugador.ubicacionActual) + 1);
            _proximaHabitacion(G, v1, v2, impostor->rol, &distancia);//Nos interesa la distancia
            if (distancia <= aleatorio_pasosExtra) {
                _matar(&jugador, impostor, algunMuerto, distancia, &aleatorio_pasosExtra);
                if (aleatorio_pasosExtra > 0 && aleatorio_escapa != 0 &&
                    _escapar(G, impostor, aleatorio_pasosExtra)) {//_escapar ya actualiza a los impostores
                    //Con aleatorio_escapa == 0, no escapa, asi aumenta la dificultad, hay mas posibilidad de escapar
                    if (aleatorio_hacerRuido) {
                        printf(BOLD AMARILLO_T"Se ha oido ruido, debe haber escapado alguien de la habitacion...\n"RESET_COLOR);
                    }
                } else {
                    strncpy(v1.habitacion, impostor->ubicacionActual, strlen(impostor->ubicacionActual) + 1);
                    if ((posi = _buscarHabitacionCerca(G, v1, impostor->rol, &distancia) != infinito)) {
                        _actualizarImpostorEscapar(G, impostor, posi, 0);
                    }
                }
            }
            modificar(Jugadores, jugador);
        }

        if (*algunMuerto == 0) {
            _recorridoMatarTripulante(izq(Jugadores), G, impostor, algunMuerto);
        }

        if (*algunMuerto == 0) {
            _recorridoMatarTripulante(der(Jugadores), G, impostor, algunMuerto);
        }
    }


}

/**
 * Funcion privada para ejecutar cada ronda
 * @param Jugadores
 * @param numeroImpostores
 * @param numeroTripulantes
 */
void _ejecutarRonda(abb Jugadores, int numeroI, int *numeroT, grafo G) {
    tipoelem jugador;
    leer(Jugadores, &jugador);
    _moverTripulante(Jugadores, G);
    int algunMuerto = 0;
    if (jugador.actividad != NADA) {//Para que no se maten en la primera ronda
        for (int i = 0; i < numeroI; i++) {
            int contador = -1;
            tipoelem impostor;
            _buscarImpostor(Jugadores, &impostor, i, &contador);//Recuperampos el impostor para posteriormente matar
            algunMuerto = 0;
            _recorridoMatarTripulante(Jugadores, G, &impostor, &algunMuerto);
            modificar(Jugadores,
                      impostor);//Debemos modificarlo aqui, porque en la recursiva de _recorridoMatarTripulante, puede pasar que el impostor no este en el subarbol de alguna llamada recursiva
            if (algunMuerto) {
                (*numeroT)--;
            }
        }
    }


}

/**
 * Imprime quien ha ganado, en funcion de las caracteristicas de la partida en curso
 * @param finPartida
 */
void _decisionGanar(int finPartida) {
    if (finPartida == 1) {
        printf(BOLD ROJO_T"\n/////////////////GANAN LOS IMPOSTORES:SON MAS QUE LOS TRIPULANTES/////////////////\n"RESET_COLOR);
    } else if (finPartida == 2) {
        printf(BOLD VERDE_T"\n/////////////////GANAN LOS TRIPULANTES: NO QUEDAN IMPOSTORES/////////////////\n"RESET_COLOR);
    } else if (finPartida == 3) {
        printf(BOLD VERDE_T"\n/////////////////GANAN LOS TRIPULANTES: ACABARON LAS TAREAS/////////////////\n"RESET_COLOR);
    }

}

/**
 * Elimina al jugador de la partida o skip.
 * @param Jugadores
 * @param jugador
 * @param numeroImpostores
 * @param numeroTripulantes
 */
void _eliminarJugador(abb Jugadores, tipoelem *jugador, int *numeroI, int *numeroT) {
    if (!es_vacio(Jugadores)) {

        buscar_nodo(Jugadores, jugador->nombreUsuario, jugador);
        if (jugador->rol == 'I') {
            (*numeroI)--;
            jugador->rol = 'E';
            modificar(Jugadores, *jugador);

        } else if (jugador->rol == 'T') {
            (*numeroT)--;
            jugador->rol = 'K';
            modificar(Jugadores, *jugador);
        }
    }
}

/**
 * Comprueba si todos los jugadores han acabado sus tareas
 * @param Jugadores
 * @param tareasTerminadas
 */
void _tareasFinalizadas(abb Jugadores, int *sabertareasTerminadas) {
    tipoelem jugador;
    if (!es_vacio(Jugadores)) {

        _tareasFinalizadas(izq(Jugadores), sabertareasTerminadas);

        leer(Jugadores, &jugador);

        if (jugador.rol != 'I' && jugador.rol != 'E' && jugador.actividad != TERMINADO) {
            *sabertareasTerminadas = 0;
        }
        _tareasFinalizadas(izq(Jugadores), sabertareasTerminadas);
    }
}

/**
 * Funcionalidad de jugar
 * @param Jugadores
 * @param G
 */
void _jugar(abb Jugadores, grafo G) {
    limpiarPantallaTerminal();
    int numeroI = 0, numeroT = 0;
    int finPartida = 0, ronda = 0;
    tipoelem jugador;
    int aux_skip;
    int aux_nombre;
    int saberTareasTerminadas = 1;
    _contadorRoles(Jugadores, &numeroI, &numeroT);
    do {
        ronda++;
        printf(BOLD AMARILLO_T"////////////////////////////////////////////////////////////////////////////////////////RONDA %d//////////////////////////////////////////////////////////////////////////////////////\n"RESET_COLOR,
               ronda);
        _ejecutarRonda(Jugadores, numeroI, &numeroT, G);
        saberTareasTerminadas = 1;
        _tareasFinalizadas(Jugadores, &saberTareasTerminadas);
        if (saberTareasTerminadas) {
            //Ganan Tripulantes
            finPartida = 3;
        }
        listadoJugadores(Jugadores, 0);
        if (finPartida == 0) {
            if (numeroT > numeroI) {
                aux_skip = 0;
                aux_nombre = 0;
                jugador = _solicitarNicknamePosibleImpostor(Jugadores, &aux_skip, &aux_nombre);
                if (aux_nombre) {
                    _eliminarJugador(Jugadores, &jugador, &numeroI, &numeroT);
                    printf(BOLD ROJO_T "Se ha decidido eliminar a %s\n"RESET_COLOR, jugador.nombreUsuario);
                }
            }
            if (numeroI == 0) {
                //Ganan Tripulantes
                finPartida = 2;

            } else if (numeroI >= numeroT) {
                // Ganan impostores
                finPartida = 1;
            }
            _siguienteRonda(Jugadores);
        }
    } while (finPartida == 0);
    _decisionGanar(finPartida);
    printf(BOLD AMARILLO_T"////////////////////////////////////////////////////////////////////////////////////////RESUMEN DE LA PARTIDA://////////////////////////////////////////////////////////////////////////////////////\n"RESET_COLOR);
    listadoJugadores(Jugadores, 1);
    presionarContinuar();


}

////////////////MENU:

/**
 * Menu de Inicializacion y prepara Jugadores
 * @param G
 * @param Usuarios
 */
void menuAntesJugar(grafo G, abb Usuarios) {
    char opcion;
    abb Jugadores;
    crear(&Jugadores);
    int numJugadores = 0;
    char *habitacionInicio;
    habitacionInicio = _generarPartida(Usuarios, &Jugadores, &numJugadores, G);
    //Menú
    do {
        limpiarPantallaTerminal();

        printf(BOLD AMARILLO_T"PARTIDA CARGADA: COMENZAREIS EN %s,NUMERO DE JUGADORES:%d,NUMERO DE IMPOSTORES %d \n"RESET_COLOR,
               _cambiarMayusculas(habitacionInicio), numJugadores, numeroImpostores);
        printf("a) Jugar\n");
        printf("s) Atrás\n\n");

        printf("Opcion:");
        scanf(" %c", &opcion);
        printf("--------------------------------------------------------------------------\n");
        printf("--------------------------------------------------------------------------\n");

        switch (opcion) {
            case 'a':
            case 'A':
                _jugar(Jugadores, G);
                opcion = 'S';
                break;
            case 's':
            case 'S':
                printf("Volviendo al menu de Inicio...\n");
                break;
            default:
                printf("Introduce una opcion valida\n");
                break;
        }

    } while (opcion != 'S' && opcion != 's');
    ImpostoresActuales = 0;
    destruir(&Jugadores);
}
