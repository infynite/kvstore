//
// Created by Amin Rezaei on 12/25/18.
//

#ifndef KVSTORE_KVSTORE_H
#define KVSTORE_KVSTORE_H

#include <stdio.h>
#include <string.h>
#include "map.h"
#include "list.h"
#include "syntax.h"
#include "str.h"

typedef struct Store_t{
    const char* file_name;
    FILE* file;
    Map tables_head;
    List tables_data;
} Store;
typedef struct Table_t{
    const char* table_name;
    Map data;
} Table;

void store_init(Store*,const char*);
List store_tables(Store*);
int store_save(Store*);
Table* store_create_table(Store*,const char *);
Table* store_open_table(Store*,const char *);
int store_isopen_table(Store*,const char *);
int store_find_table(Store*,const char *);
int store_close_table(Store*,const char *);
size_t store_header_sizeof(Store*);

void table_add(Table*,Dynamic*,Dynamic*);
void table_addv(Table*,Dynamic,Dynamic);
void table_remove(Table*,Dynamic*);
void table_removev(Table*,Dynamic);
Dynamic* table_get(Table*,int);
Dynamic* table_find(Table*,Dynamic*);
Dynamic* table_findv(Table*,Dynamic);
Dynamic* table_getv(Table*,int);
size_t table_sizeof(Table*);

void store_init(Store* store,const char* file_name){
    store->file_name = file_name;
    store->tables_data = list_init();
    store->tables_head = map_create();
    FILE* file;
    file = fopen(store->file_name,"rb+");
    if(!file) {
        file = fopen(store->file_name, "wb+");
    }else{
        unsigned short table_count;
        size_t res = fread(&table_count, sizeof(unsigned short),1,file);
        if(res == 1){
            range(i,0,table_count){
                unsigned char s;
                size_t r = fread(&s, sizeof(unsigned char),1,file);
                if(r != 1) break;
                char* n = (char*) malloc((s+1) * sizeof(char));
                r = fread(n, sizeof(char), (size_t) s, file);
                n[s] = 0;
                if(r != s) break;
                int pos;
                r = fread(&pos, sizeof(int),1,file);
                if(r != 1) break;
                map_appendv(&store->tables_head,dynamic_create(n),dynamic_create(pos));
            }
        }
    }
    store->file = file;

}
List store_tables(Store* store){
    List l = list_init();
    range(i,0,map_size(&store->tables_head)){
        list_append(&l,map_index(&store->tables_head,$i));
    }
    return l;
}
Table* store_create_table(Store* store,const char * table){
    Dynamic name = dynamic_create(table);
    map_appendv(&store->tables_head,name,dynamic_create(-1));
    Table* t = (Table*) malloc(sizeof(Table));
    t->table_name = table;
    t->data = map_create();
    list_appendv(&store->tables_data,dynamic_create((void*)t));
    return t;
}
Table* store_open_table(Store* store,const char * table_name){
    int t_idx = store_isopen_table(store,table_name);
    if(t_idx)
        return (Table*) dynamic_get(list_get(&store->tables_data,t_idx-1));
    Table* table = malloc(sizeof(Table));
    table->table_name = table_name;
    table->data = map_create();
    int table_offset = store_find_table(store,table_name);
    if(table_offset == -1) {
        return store_create_table(store, table_name);
    }
    nreturnv(fseek(store->file,table_offset,SEEK_SET),0,NULL);
    int table_size[1];
    fread(table_size, sizeof(int),1,store->file);
    int x = 0;
    range(i,0,table_size[0]){
        Dynamic *key,*value;
        key = malloc(sizeof(Dynamic));
        value = malloc(sizeof(Dynamic));
        nreturnv(dynamic_read(key,store->file),1,NULL)
        nreturnv(dynamic_read(value,store->file),1,NULL)
        table_add(table,key,value);
    }
    list_appendv(&store->tables_data,dynamic_create((void*)table));
    return table;
}
int store_find_table(Store* store,const char * table_name){
    range(i,0,store->tables_head.size){
        Dynamic d = dynamic_create(table_name);
        if(dynamic_compare(map_index(&store->tables_head,$i),&d)==0){
            return *(int*)dynamic_get(map_indexv(&store->tables_head,$i));
        }
    }
    return -1;
}
int store_isopen_table(Store* store,const char * table_name){
    range(i,0,store->tables_data.size){
        if(streq(((Table*)dynamic_get(list_get(&store->tables_data,$i)))->table_name,table_name)){
            return $i+1;
        }
    }
    return 0;
}
int store_close_table(Store* store,const char * table_name){
    int idx = store_isopen_table(store,table_name);
    if(!idx) return 1;
    list_remove_index(&store->tables_data,idx-1);
    return 1;
}
int store_save(Store* store){
    char* name = str_concat(store->file_name, "__new");
    FILE* bak = fopen(name,"wb+");
    if(!bak) return -1;
    int proceed = 0;
    int table_count[1] = {map_size(&store->tables_head)};
    nreturnv(fwrite(table_count, sizeof(unsigned short),1,bak),1,-2)
    proceed += sizeof(unsigned short);
    proceed += store_header_sizeof(store);
    int l = 0;
    range(i,0,map_size(&store->tables_head)){
        Dynamic* key = map_index(&store->tables_head,$i);
        Dynamic* val = map_indexv(&store->tables_head,$i);
        const char* tname = dynamic_get(key);
        Table* t = store_open_table(store,tname);
        fseek(bak,proceed,SEEK_SET);
        int table_size[1] = {t->data.size};
        nreturnv(fwrite(table_size, sizeof(int),1,bak),1,-3)
        dynamic_set(val,proceed);
        proceed += table_sizeof(t) + sizeof(int);
        range(i,0,t->data.size){
            Dynamic *k,*v;
            k = table_get(t,$i);
            v = table_getv(t,$i);
            nreturnv(dynamic_write(k,bak),1,-4)
            nreturnv(dynamic_write(v,bak),1,-5)
        }
    }
    nreturnv(fseek(bak, sizeof(unsigned short),SEEK_SET),0,-10)
    range(i,0,map_size(&store->tables_head)){
        Dynamic* key = map_index(&store->tables_head,$i);
        Dynamic* val = map_indexv(&store->tables_head,$i);
        unsigned char k[1] = {(unsigned char)str_len(key->s)};
        nreturnv(fwrite(k, sizeof(unsigned char),1,bak),1,-11)
        nreturnv(fwrite(dynamic_get(key), sizeof(char),(size_t)k[0],bak),k[0],-12)
        nreturnv(fwrite(dynamic_get(val), sizeof(int),1,bak),1,-13)
    }
    nreturnv(fclose(store->file),0,-6)
    nreturnv(fclose(bak),0,-7)
    nreturnv(remove(store->file_name),0,-8)
    nreturnv(rename(name,store->file_name),0,-9)
    store->file = fopen(store->file_name,"r+");
    return 0;
}
size_t store_header_sizeof(Store* s){
    size_t size = 0;
    range(i,0,map_size(&s->tables_head)){
        Dynamic* k = map_index(&s->tables_head,$i);
        size += str_len(k->s) + sizeof(int) + sizeof(unsigned char);
    }
    return size;
}

void table_add(Table* table,Dynamic* k,Dynamic* v){
    map_append(&table->data,k,v);
}
void table_addv(Table* table,Dynamic k,Dynamic v){
    map_append(&table->data,&k,&v);
}
void table_remove(Table* table,Dynamic* d){
    map_remove(&table->data,d);
}
void table_removev(Table* table,Dynamic d){
    map_remove(&table->data,&d);
}
Dynamic* table_find(Table* table,Dynamic* d){
    return map_get(&table->data,d);
}
Dynamic* table_findv(Table* table,Dynamic d){
    return map_get(&table->data,&d);
}
Dynamic* table_get(Table* table,int index){
    return map_index(&table->data,index);
}
Dynamic* table_getv(Table* table,int index){
    return map_indexv(&table->data,index);
}
size_t table_sizeof(Table* table){
    size_t table_size = 0;
    table_size += sizeof(int);
    range(i,0,map_size(&table->data)){
        Dynamic* k = map_index(&table->data,$i);
        Dynamic* v = map_indexv(&table->data,$i);
        table_size += dynamic_sizeof(k) + dynamic_sizeof(v);
    }
    return table_size;
}
#endif //KVSTORE_KVSTORE_H
