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
  long posicion = hash(key, map->capacity);
  long posicionOriginal = posicion;

  while(map->buckets[posicion]!= NULL && map->buckets[posicion]->key != NULL)
  {
    if(strcmp(map->buckets[posicion]->key, key) == 0)
    {
      return;
    }
    posicion = (posicion + 1) % map->capacity;
    if(posicion == posicionOriginal)
    {
      return;
    } 
  }
  map->buckets[posicion] = createPair(key,value);
  map->current = posicion;

  map->size++;
}

/*
Implemente la función void enlarge(HashMap * map). Esta función agranda la capacidad del arreglo buckets y reubica todos sus elementos. Para hacerlo es recomendable mantener referenciado el arreglo actual/antiguo de la tabla con un puntero auxiliar. Luego, los valores de la tabla se reinicializan con un nuevo arreglo con el doble de capacidad. Por último los elementos del arreglo antiguo se insertan en el mapa vacío con el método insertMap. Puede seguir los siguientes pasos:

a - Cree una variable auxiliar de tipo Pair** para matener el arreglo map->buckets (old_buckets);

b - Duplique el valor de la variable capacity.

c - Asigne a map->buckets un nuevo arreglo con la nueva capacidad.

d - Inicialice size a 0.

e - Inserte los elementos del arreglo old_buckets en el mapa (use la función insertMap que ya implementó).
*/

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair** old = map->buckets;

  map->capacity *= 2;
  map ->buckets = (Pair**)malloc(map->capacity * sizeof(Pair*));
  map->size = 0;

  for(long i = 0; i < map->capacity / 2; i++)
  {
    if(old[i] != NULL)
    {
      insertMap(map, old[i]->key, old[i]->value);
      free(old[i]);
    }
  }
  free(old);
}

/*
Implemente la función createMap en el archivo hashmap.c. Esta función crea una variable de tipo HashMap, inicializa el arreglo de buckets con casillas nulas, inicializa el resto de variables y retorna el mapa. Inicialice el índice current a -1.
*/


HashMap * createMap(long capacity){
  HashMap *mapa = (HashMap*)malloc(sizeof(HashMap)); //Asignar memoria al mapa
  if(mapa == NULL)
  {
    free(mapa);
    return NULL;
  }
  mapa->buckets = (Pair**)calloc(capacity, sizeof(Pair*));
  if(mapa->buckets == NULL)
  {
    free(mapa);
    return NULL;
  }
  mapa->size = 0;
  mapa->current = -1;
  mapa->capacity = capacity;

  return mapa;
}

/*
Implemente la función void eraseMap(HashMap * map, char * key). Está función elimina el dato correspondiente a la clave key. Para hacerlo debe buscar el dato y luego marcarlo para que no sea válido. No elimine el par, sólo invalídelo asignando NULL a la clave (pair->key=NULL). Recuerde actualizar la variable size. 
*/
// size -1?

/*
Implemente la función void eraseMap(HashMap * map, char * key). Está función elimina el dato correspondiente a la clave key. Para hacerlo debe buscar el dato y luego marcarlo para que no sea válido. No elimine el par, sólo invalídelo asignando NULL a la clave (pair->key=NULL). Recuerde actualizar la variable size.
*/

void eraseMap(HashMap * map,  char * key){
  long posicion = hash(key, map->capacity);
  long posicionOriginal = posicion;

  while(map->buckets[posicion]!=NULL && map->buckets[posicion]->key != NULL)
  {
    if(strcmp(map->buckets[posicion]->key, key) == 0)
    {
      map->buckets[posicion]->key = NULL;
      map->size--;
    }
    posicion = (posicion +1) % map->capacity;
    if(posicion == posicionOriginal)
    {
      return;
    }
  }
  return;
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

/*
 Implemente las funciones para recorrer la estructura: Pair * firstMap(HashMap * map) retorna el primer Pair válido del arreglo buckets. Pair * nextMap(HashMap * map) retorna el siguiente Pair del arreglo buckets a partir índice current. Recuerde actualizar el índice.
*/
Pair * firstMap(HashMap * map){
  long i;
  for(i = 0; i <map->capacity; i++)
  {
    if(map->buckets[i] != NULL && map->buckets[i]->key != NULL)
    {
      map->current = i;
      return map->buckets[i];
    }    
  }
  return NULL;
}

Pair * nextMap(HashMap * map){
  long i;
  for(i = map->current + 1; i < map->capacity; i++) // sig del current
  {
    if(map->buckets[i] != NULL && map->buckets[i]->key != NULL)
    {
      map->current = i;
      return map->buckets[i];
    }
  }
  return NULL;
}
