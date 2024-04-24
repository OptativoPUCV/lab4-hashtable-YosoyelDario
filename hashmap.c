#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

/*
 Implemente la función void insertMap(HashMap * map, char * key, void * value). Esta función inserta un nuevo dato (key,value) en el mapa y actualiza el índice current a esa posición. Recuerde que para insertar un par (clave,valor) debe:

a - Aplicar la función hash a la clave para obtener la posición donde debería insertar el nuevo par

b - Si la casilla se encuentra ocupada, avance hasta una casilla disponible (método de resolución de colisiones). Una casilla disponible es una casilla nula, pero también una que tenga un par inválido (key==NULL).

c - Ingrese el par en la casilla que encontró.

No inserte claves repetidas. Recuerde que el arreglo es circular. Recuerde actualizar la variable size.
*/

void insertMap(HashMap * map, char * key, void * value){
  long posicion = hash(key, map->capacity);     //Valores long porque en el struct los valores de la capacity y size son long.
  long posicionOriginal = posicion;

  while(map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL)
  {
    if(strcmp(map->buckets[posicion]->key, key) == 0)
    {
      return;
    }
    
    posicion = (posicion +1) % map->capacity; // arreglo circula volver al inicio si llega al final.

    if(posicion == posicionOriginal) // Si se volvio significa que no habian casillas disponibles para el dato.
      return;
  }

  map->buckets[posicion] = createPair(key,value);
  map->current = posicion;
  

  map->size++;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity){
  HashMap* mapa = (HashMap*)malloc(sizeof(HashMap));
  if(mapa == NULL)
  {
    return NULL;
  }
  mapa->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
  if(mapa->buckets == NULL){
    free(mapa);
    return NULL;
  }

  mapa->size = 0;
  mapa->capacity = capacity;
  mapa->current = -1;
  return mapa;
}

void eraseMap(HashMap * map,  char * key) {    


}

/*
 Implemente la función Pair * searchMap(HashMap * map, char * key), la cual retorna el Pair asociado a la clave ingresada. Recuerde que para buscar el par debe:

a - Usar la función hash para obtener la posición donde puede encontrarse el par con la clave

b - Si la clave no se encuentra avance hasta encontrarla (método de resolución de colisiones)

c - Si llega a una casilla nula, retorne NULL inmediatamente (no siga avanzando, la clave no está)

Recuerde actualizar el índice current a la posición encontrada. Recuerde que el arreglo es circular.
*/

Pair * searchMap(HashMap * map,  char * key){
  long posicion = hash(key, map->capacity);
  long posicionOriginal = posicion;

  while(map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL)
  {
    if(strcmp(map->buckets[posicion]->key, key) == 0)
    {
      map->current = posicion;
      return map->buckets[posicion];
    }
    posicion = (posicion +1) % map->capacity;
    if(posicion == posicionOriginal)
      return NULL;
  }
  return NULL;
  
}

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
