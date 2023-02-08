#include "cola.h"
#include <stdlib.h>
#include <stdio.h>

//////////////////////////////// ESTRUCTURAS DE DATOS:

struct celdaCola {
    tipoelemCola elemento;
    struct celdaCola * sig;
};

typedef struct celdaCola * puntero;

struct tipocola {
    puntero frente;
    puntero final;
};

//////////////////////////////// FUNCIONES:

/**
 * Fija a null el puntero
 * @param C
 */
void crear_cola(cola *C) {
    *C = (cola) malloc(sizeof (struct tipocola));
    (*C)->frente = (puntero) malloc(sizeof (struct celdaCola));
    (*C)->final = (*C)->frente;
    (*C)->frente->sig = NULL;
}

/**
 * Libera la memoria de cada celda de la cola
 * @param C
 */
void destruir_cola(cola *C) {
    puntero p, r;
    p = (*C)->frente;
    while (p != NULL) {
        r = p;
        p = p->sig;
        free(r);
    }
    free(*C);
}

/**
 * Comprueba si la cola está vacía
 * @return 1 si la cola está vacía y 0 en otro caso
 */
unsigned es_vacia_cola(cola C) {
    return (C->frente == C->final);
}

/**
 * Devuelve el primer elemento de la cola. No comprueba
 * si la cola está vacía, es posible que haya que llamar
 * a es_vacia_cola antes de llamar a estar función.
 * @param C
 * @return Primer elemento de la cola
 */
tipoelemCola primero(cola C) {
    return (C->frente)->sig->elemento;
}

/**
 * Introduce un nuevo elemento al final de la cola.
 * @param C
 * @param E
 */
void insertar_cola(cola *C, tipoelemCola E) {
    ((*C)->final)->sig = (puntero) malloc(sizeof (struct celdaCola));
    (*C)->final = ((*C)->final)->sig;
    ((*C)->final)->elemento = E;
    ((*C)->final)->sig = NULL;
}

/**
 * Elimina de la cola el primer elemento. No comprueba
 * si la cola está vacía, es posible que haya que llamar
 * a es_vacia_cola antes de llamar a estar función.
 * @param C
 */
void suprimir_cola(cola *C) {
    puntero p;
    p = (*C)->frente;
    (*C)->frente = (*C)->frente->sig;
    free(p);
}
