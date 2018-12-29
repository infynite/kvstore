#include <stdio.h>
#include "kvstore.h"
void write_test(Store*);
void read_test(Store*);
int main() {
    Store s;
    store_init(&s,"../data");
    int i;
    printf("Write(0) Read(1) :");
    scanf("%d",&i);
    if(i) read_test(&s);
    else write_test(&s);
    return 0;
}
void read_test(Store* s){
    Table* t = store_open_table(s,"settings");
    printf("%s",(const char*)dynamic_get(table_findv(t,dynamic_create("s1"))));
}
void write_test(Store* s){
    Table* t = store_open_table(s,"settings");
    table_addv(t,dynamic_create("s1"),dynamic_create("TEST"));
    store_save(s);
}