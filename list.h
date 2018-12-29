//
// Created by Amin  Rezaei on 12/26/18.
//

#ifndef KVSTORE_LIST_H
#define KVSTORE_LIST_H

#include "dynamic.h"
#include "syntax.h"
#include <stdlib.h>

typedef struct List_t{
    Dynamic* data;
    int size;
} List;

List list_init();
void list_append(List*,Dynamic*);
void list_appendv(List*,Dynamic);
int list_remove(List*,Dynamic*);
int list_remove_index(List*,int);
int list_removev(List*,Dynamic);
int list_count(List*,Dynamic*);
int list_countv(List*,Dynamic);
Dynamic* list_get(List*,int);

List list_init(){
    List l = {};
    return l;
}
void list_append(List* list,Dynamic* d){
    if(list->size == 0){
        list->data = (Dynamic*) malloc(sizeof(Dynamic));
        list->size = 1;
        list->data[0] = *d;
        return;
    }
    list->size++;
    list->data = (Dynamic*) realloc(list->data,list->size * sizeof(Dynamic));
    list->data[list->size-1] = *d;
}
void list_appendv(List* list,Dynamic d){
    list_append(list,&d);
}
int list_remove_index(List* list,int idx){
    range(j,idx,list->size-1){
        list->data[$j] = list->data[$j+1];
    }
    list->size -= 1;
    list->data = realloc(list->data,list->size * sizeof(Dynamic));
    return 1;
}
int list_remove(List* list,Dynamic* d){
    int x = 0;
    range(i,0,list->size){
        if(dynamic_compare(&list->data[$i],d)==0){
            range(j,$i,list->size-1){
                list->data[$j] = list->data[$j+1];
            }
            x++;
        }
    }
    list->size -= x;
    list->data = realloc(list->data,list->size * sizeof(Dynamic));
    return x;
}
int list_removev(List* list,Dynamic d){
    return list_remove(list,&d);
}
int list_count(List* list,Dynamic* d) {
    int x = 0;
    range(i, 0, list->size) {
        if (dynamic_compare(&list->data[$i], d) == 0) {
            x++;
        }
    }
    return x;
}
int list_countv(List* list,Dynamic d) {
    return list_count(list,&d);
}
Dynamic* list_get(List* list,int index) {
    return &list->data[index];
}
#endif //KVSTORE_LIST_H
