#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include "kvstore.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
void read_test(Store*);
void write_test(Store*);
int main() {
    Store s;
    setlocale(LC_ALL,"");
    store_init(&s,"../data");
    int i;
    printf("Write(0) Read(1) :");
    scanf("%d",&i);
    if(i) read_test(&s);
    else write_test(&s);
    return 0;
}
void read_test(Store* s){
    Table* t = store_open_table(s,"table1");
    wprintf (L"%ls\n", CCHAIN(wchar_t*, dynamic_get, table_findv, t, dynamic_create("wide_string_test")));
    printf("%s\n",CCHAIN(char*,dynamic_get,table_findv,t,dynamic_create("string_test")));
    printf("%d\n",*CCHAIN(int*,dynamic_get,table_findv,t,dynamic_create("int_test")));
    printf("%f\n",*CCHAIN(float*,dynamic_get,table_findv,t,dynamic_create("float_test")));
    printf("%c\n",*CCHAIN(char*,dynamic_get,table_findv,t,dynamic_create("char_test")));
}
void write_test(Store* s){
    Table* t = store_open_table(s,"table1");
    table_addv(t,dynamic_create("wide_string_test"),dynamic_createw(L"تست متن غیر ASCII"));
    table_addv(t,dynamic_create("string_test"),dynamic_create("string value"));
    table_addv(t,dynamic_create("int_test"),dynamic_create(1010));
    table_addv(t,dynamic_create("float_test"),dynamic_create(4.0f));
    table_addv(t,dynamic_create("char_test"),dynamic_create('c'));
    store_save(s);
}
#pragma clang diagnostic pop