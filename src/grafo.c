#include "grafo.h"
#include <string.h>
#include <stdlib.h>

/**
 * En esta implementacion guardo en la Matriz de Tripulantes todos los arcos unicamente de Tripulantes
 * y en la Matriz de Impostores unicamente los arcos de Impostores, por lo tanto si es impostoror y son_adyacentes() == 0
 * se recurre a la Matriz de Tripulantes, ya que los impostores pueden usar ambos caminos.
 *
 * Esto facilita incorporar nuevos arcos ya que si son solo para Impostores se añaden a la Matriz de Impostores
 * y si pueden usar por ambos(T y I) en la Matriz de Tripulantes, así evito estar modificando siempre la Matriz de Impostores
 * al incorporar arcos para ambos.
 *
 * Podria crearse otra matriz para los muertos ROL = 'K' && ROL = 'E'
 */


/////////////////////////////////////////////////////////// TIPOS DE DATOS:

// Estructura privada
struct tipografo {
    char nombre[name_length];//Nommbre del grafo
    int N; //número de vértices del grafo
    tipovertice VERTICES[MAXVERTICES]; //vector de vértices
    int Ntareas;//Numero de tareas
    tipotareas TAREAS[MAXTAREAS]; //Vector de tareas
    int AI[MAXVERTICES][MAXVERTICES]; //matriz de adyacencia de los Impostores
    int AT[MAXVERTICES][MAXVERTICES]; //matriz de adyacencia de los Tripulantes
};

//////////////////////////////////////////////////////////////// FUNCIONES:

/**
 * Cambiado para comparar char con strcmp
 * @param V1
 * @param V2
 * @return
 */
int _comparar_vertices(tipovertice V1, tipovertice V2){
	return strcmp(V1.habitacion,V2.habitacion) == 0 ? 0 : strcmp(V1.habitacion,V2.habitacion) > 0 ? 1 : -1;
}

/**
 * Creación del grafo con 0 nodos
 * @param G
 */
void crear_grafo(grafo *G) {
    *G = (struct tipografo*) malloc(sizeof (struct tipografo));
    (*G)->N = 0;
    (*G)->Ntareas = 0;
    char aux[] = "\0";
    strncpy((*G)->nombre,aux,strlen(aux));
}

/**
 * Devuelve la posición del vértice Vert en el vector VERTICES del grafo G,Si devuelve -1 es porque no encontró el vértice
 * @param G
 * @param V
 * @return
 */
int posicion(grafo G, tipovertice V) {
    int contador = 0;
    //comparo V con todos los vertices almacenados en VERTICES 
    while (contador < G->N) {
        //if (G->VERTICES[contador]==V)  //encontré la posicion de V
		if (_comparar_vertices(G->VERTICES[contador], V) == 0){
            return contador; 
        }
        contador++;
    }
    return -1;
}

/**
 * Devuelve 1 si el grafo G existe y 0 en caso contrario
 * @param G
 * @return
 */
int existe(grafo G) {
    return (G != NULL);
}

/**
 * Devuelve 1 si el vértice Vert existe en el grafo G
 * @param G
 * @param V
 * @return
 */
int existe_vertice(grafo G, tipovertice V) {
    return (posicion(G, V) >= 0);
}

/**
 * Inserta un vértice en el grafo, devuelve -1 si no ha podido insertarlo por estar el grafo lleno
 * @param G
 * @param Vert
 * @return
 */
int insertar_vertice(grafo *G, tipovertice Vert) {
    int i;
    if ((*G)->N == MAXVERTICES) {
    	// Se ha llegado al maximo numero de vertices
    	return -1;
    }
   
    (*G)->N++;
    (*G)->VERTICES[((*G)->N) - 1] = Vert;
    for (i = 0; i < (*G)->N; i++) {
        //Impostores
        (*G)->AI[i][((*G)->N) - 1] = 0;
        (*G)->AI[((*G)->N) - 1][i] = 0;
        //Tripulantes
        (*G)->AT[i][((*G)->N) - 1] = 0;
        (*G)->AT[((*G)->N) - 1][i] = 0;
    }
	return (*G)->N-1;
}

/**
 * Borra un vertice del grafo
 * @param G
 * @param Vert
 */
void borrar_vertice(grafo *G, tipovertice Vert) {
    int F, C, P, N = (*G)->N;
    P = posicion(*G, Vert);
    if(P == -1){
    	return;
    }
    //if (P >= 0 && P < (*G)->N) {
    for (F = P; F < N - 1; F++){
        (*G)->VERTICES[F] = (*G)->VERTICES[F + 1];
	}
    for (C = P; C < N - 1; C++){
        for (F = 0; F < N; F++){
            //Impostores
            (*G)->AI[F][C] = (*G)->AI[F][C + 1];
            //Tripulantes
            (*G)->AT[F][C] = (*G)->AT[F][C + 1];
        }
	}
    for (F = P; F < N - 1; F++){
        for (C = 0; C < N; C++){
            //Impostores
            (*G)->AI[F][C] = (*G)->AI[F + 1][C];
            //Tripulantes
            (*G)->AT[F][C] = (*G)->AT[F + 1][C];
        }
	}
    (*G)->N--;    
}

/**
 * Crea el arco de relación entre VERTICES(pos1) y VERTICES(pos2) de los Tripulantes
 * @param G
 * @param pos1
 * @param pos2
 * @param valor
 */
void crear_arco_T(grafo *G, int pos1, int pos2, int valor) {
    (*G)->AT[pos1][pos2] = valor;
    (*G)->AT[pos2][pos1] = valor;

}

/**
 * Crea el arco de relación entre VERTICES(pos1) y VERTICES(pos2) de los Impostores
 * @param G
 * @param pos1
 * @param pos2
 * @param valor
 */
void crear_arco_I(grafo *G, int pos1, int pos2, int valor) {
    (*G)->AI[pos1][pos2] = valor;
    (*G)->AI[pos2][pos1] = valor;
}

/**
 * Borra el arco de relación entre VERTICES(pos1) y VERTICES(pos2) de los Tripulantes
 * @param G
 * @param pos1
 * @param pos2
 */
void borrar_arco_T(grafo *G, int pos1, int pos2) {
    (*G)->AT[pos1][pos2] = 0;
    (*G)->AT[pos2][pos1] = 0;
}

/**
 * Borra el arco de relación entre VERTICES(pos1) y VERTICES(pos2) de los Impostores
 * @param G
 * @param pos1
 * @param pos2
 */
void borrar_arco_I(grafo *G, int pos1, int pos2) {
    (*G)->AI[pos1][pos2] = 0;
    (*G)->AI[pos2][pos1] = 0;
}

/**
 * Devuelve valor si VERTICES(pos1) y VERTICES(pos2) son vértices adyacentes de los Tripulantes
 * @param G
 * @param pos1
 * @param pos2
 * @return
 */
int son_adyacentes_T(grafo G, int pos1, int pos2) {
    return (G->AT[pos1][pos2]);
}

/**
 * Devuelve valor si VERTICES(pos1) y VERTICES(pos2) son vértices adyacentes de los Impostores
 * @param G
 * @param pos1
 * @param pos2
 * @return
 */
int son_adyacentes_I(grafo G, int pos1, int pos2) {
    return (G->AI[pos1][pos2]);
}

/**
 * Destruye el grafo
 * @param G
 */
void borrar_grafo(grafo *G) {
    free(*G);
    *G = NULL;
}

/**
 * Devuelve el número de vértices del grafo G
 * @param G
 * @return
 */
int num_vertices(grafo G) {
    return G->N;
}

/**
 * Devuelve el vector de vértices VERTICES del grafo G
 * @param G
 * @return
 */
tipovertice* array_vertices(grafo G) {
    return G->VERTICES;
}

/**
 * Devuelve el numero de propiedades del grafo
 * @param G
 * @return
 */
int num_tareas(grafo G) {
    return G->Ntareas;
}

/**
 * Devuelve el array de propiedades del grafo
 * @param G
 * @return
 */
tipotareas * array_tareas(grafo G) {
    return G->TAREAS;
}

/**
 * Funcion para Insertar las tareas asignadas al grafo, devuelve -1 si esta lleno el grafo
 * @param G
 * @param tarea
 * @return
 */
int insertar_tareas(grafo *G, tipotareas tarea){
    if ((*G)->Ntareas == MAXTAREAS) {
        // Se ha llegado al maximo numero de vertices
        return -1;
    }
    (*G)->Ntareas++;
    (*G)->TAREAS[((*G)->Ntareas) - 1] = tarea;
    return (*G)->Ntareas-1;
}

/**
 * Eliminar las tareas del grafo, devuelve -1 si esta vacio
 * @param G
 * @param posicion
 * @return
 */
int eliminar_tareas(grafo *G, int posicion){
    if ((*G)->Ntareas == 0) {
        // No hay tareas
        return -1;
    }

    for(int i = posicion - 1; i < (*G)->Ntareas;i++){
        (*G)->TAREAS[i] = (*G)->TAREAS[i+1];
    }

    (*G)->Ntareas--;
    return (*G)->Ntareas - 1;
}

/**
 * Funcion para introducir nombre a un grafo
 * @param G
 * @param nombre
 */
void introducirNombre_Grafo(grafo G, char* nombre){
    strncpy((G)->nombre,nombre, strlen(nombre) + 1);
}

/**
 * Recupera el Nombre del Grafo
 * @param G
 * @return
 */
char* recuperarNombre(grafo G){
    return G->nombre;
}

/**
 * Comprueba que todos los vertices tienen algun arco, que no sea un loop
 * @param G
 * @return
 */
int todosVadyacentes(grafo G){
    int verticeConAdyacentes = 0;
    for(int i = 0; i < (*G).N; i++){
        verticeConAdyacentes = 0;
        for(int j = 0; j < G->N; j++){
            if(i != j){
                if(son_adyacentes_T(G,i,j) != 0){
                    verticeConAdyacentes = 1;
                    break;
                }
            }
        }
        if(verticeConAdyacentes == 0){
            return 0;
        }
    }
    return 1;

}