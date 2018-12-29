//
// Created by Amin  Rezaei on 12/26/18.
//

#ifndef KVSTORE_STR_H
#define KVSTORE_STR_H

#include <stdlib.h>
#include <wchar.h>
#include "syntax.h"

char* str_concat(const char* f,const char* s){
    int f_len = 0,s_len = 0;
    irange(i,f[$i]) f_len++;
    irange(i,s[$i]) s_len++;
    char* r = (char*) malloc((f_len + s_len + 1)* sizeof(char));
    int i = 0;
    irange(i,f[$i]){
        r[$i] = f[$i];
        i++;
    }
    irange(i,s[$i]) r[$i+i] = s[$i];
    r[f_len + s_len] = 0;
    return r;
}
int str_len(const char* s){
    irange2(i,s[$i]);
    return $i;
}
int streq(const char* f,const char* s){
    if(str_len(f)!=str_len(s))
        return 0;
    range(i,0,str_len(f)){
        if(f[$i]!=s[$i]) return 0;
    }
    return 1;
}
wchar_t* wstr_concat(const wchar_t* f,const wchar_t* s){
    int f_len = 0,s_len = 0;
    irange(i,f[$i]) f_len++;
    irange(i,s[$i]) s_len++;
    wchar_t* r = (wchar_t*) malloc((f_len + s_len + 1)* sizeof(wchar_t));
    int i = 0;
    irange(i,f[$i]){
        r[$i] = f[$i];
        i++;
    }
    irange(i,s[$i]) r[$i+i] = s[$i];
    r[f_len + s_len] = 0;
    return r;
}
int wstr_len(const wchar_t* s){
    irange2(i,s[$i]);
    return $i;
}
int wstreq(const wchar_t* f,const wchar_t* s){
    if(wstr_len(f)!=wstr_len(s))
        return 0;
    range(i,0,wstr_len(f)){
        if(f[$i]!=s[$i]) return 0;
    }
    return 1;
}
#endif //KVSTORE_STR_H
