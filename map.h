//
// Created by Amin  Rezaei on 12/25/18.
//

#ifndef KVSTORE_MAP_H
#define KVSTORE_MAP_H

#include <stdlib.h>
#include "dynamic.h"
#include "syntax.h"

struct Map_t{
    Dynamic* keys;
    Dynamic* vals;
    int size;
};
typedef struct Map_t Map;
Map map_create();
void map_append(Map*,Dynamic*,Dynamic*);
void map_appendv(Map*,Dynamic,Dynamic);
int map_remove(Map*,Dynamic*);
int map_removev(Map*,Dynamic);
Dynamic* map_get(Map*,Dynamic*);
Dynamic* map_get2(Map*,Dynamic*,int*);
Dynamic* map_index(Map*,int);
Dynamic* map_indexv(Map*,int);
Dynamic* map_getv(Map*,Dynamic);
int map_size(Map*);
void map_dispose(Map*);
Map map_create(){
    Map m = {};
    return m;
}
void map_append(Map* map,Dynamic* key,Dynamic* val){
    if(map->size == 0){
        map->size = 1;
        map->keys = (Dynamic*) malloc(map->size * sizeof(Dynamic));
        map->vals = (Dynamic*) malloc(map->size * sizeof(Dynamic));
        map->keys[0] = *key;
        map->vals[0] = *val;
        return;
    }
    int idx;
    Dynamic* t = map_get2(map,key,&idx);
    if(t){
        map->vals[idx] = *val;
        return;
    }
    map->size += 1;
    map->keys = (Dynamic*) realloc(map->keys,(size_t) map->size * sizeof(Dynamic));
    map->vals = (Dynamic*) realloc(map->vals,(size_t) map->size * sizeof(Dynamic));
    map->keys[map->size-1] = *key;
    map->vals[map->size-1] = *val;
}
void map_appendv(Map* map,Dynamic key,Dynamic val){
    map_append(map,&key,&val);
}
int map_remove(Map* map,Dynamic* key){
    range(i,0,map_size(map)){
        if(dynamic_compare(&map->keys[$i],key)==0){
            range(j,$i,map_size(map)-1){
                map->keys[$j] = map->keys[$j+1];
                map->vals[$j] = map->vals[$j+1];
                map->size--;
                map->keys = (Dynamic*) realloc(map->keys,(size_t) map->size * sizeof(Dynamic));
                map->vals = (Dynamic*) realloc(map->vals,(size_t) map->size * sizeof(Dynamic));
                return 1;
            }
        }
    }
    return 0;
}
int map_removev(Map* map,Dynamic key){
    return map_remove(map,&key);
}
Dynamic* map_get(Map* m, Dynamic* d){
    return map_get2(m,d,NULL);
}
Dynamic* map_get2(Map* m, Dynamic* d,int* a){
    range(i,0,map_size(m)){
        Dynamic x = m->keys[$i];
        if(x.type == d->type){
           if(dynamic_compare(&x,d)==0){
               if(a) *a = $i;
               return &m->vals[$i];
           }
        }
    }
    return NULL;
}
Dynamic* map_getv(Map* m, Dynamic d) {
    return map_get(m,&d);
}
Dynamic* map_index(Map* m, int idx) {
    return &m->keys[idx];
}
Dynamic* map_indexv(Map* m, int idx) {
    return &m->vals[idx];
}
void map_dispose(Map* m){
    free(m->vals);
    free(m->keys);
    free(m);
}
int map_size(Map* m){
    return m->size;
}
#endif //KVSTORE_MAP_H
