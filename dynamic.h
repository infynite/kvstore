//
// Created by Amin  Rezaei on 12/25/18.
//

#ifndef KVSTORE_DYNAMIC_H
#define KVSTORE_DYNAMIC_H

#include <stdio.h>
#include <wchar.h>
#include "str.h"

struct Dynamic_t{
    unsigned char type;
    union {
        char* s;
        wchar_t* w;
        int i;
        float f;
        double d;
        char c;
        void* p;
        long l;
        long double ld;
    };
};
typedef struct Dynamic_t Dynamic;
enum{
    STRING,INT,FLOAT,DOUBLE,CHARACTER,POINTER,LONG,LONG_DOUBLE,WIDE_STRING
};

#define defineh_dcreator(T,FT) Dynamic dynamic_create##FT(T);
#define define_dcreator(T,FT,TP) \
Dynamic dynamic_create##FT(T val){ \
    Dynamic d; \
    d.type = TP; \
    d.FT = val; \
    return d; \
}
#define defineh_dsetter(T,FT) void dynamic_set##FT(Dynamic*,T);
#define define_dsetter(T,FT,TP) \
void dynamic_set##FT(Dynamic* d,T val){ \
    d->type = TP; \
    d->FT = val; \
}

/*
 * Creates dynamic with value by generics and auto-type detection
 */
#define dynamic_create(T) _Generic((T), \
                char*: dynamic_creates, \
                wchar_t *: dynamic_createw, \
                const wchar_t *: dynamic_createw, \
                const char*: dynamic_creates, \
                int: dynamic_createi,  \
                float: dynamic_createf,  \
                double: dynamic_created,  \
                char: dynamic_createc,  \
                void*: dynamic_createp,  \
                long: dynamic_createl,  \
                long double: dynamic_createld  \
)(T)
/*
 * Sets value to dynamic by generics and auto-type detection
 */
#define dynamic_set(D,T) _Generic((T), \
                const char*: dynamic_sets, \
                const wchar_t*: dynamic_setw, \
                int: dynamic_seti,  \
                float: dynamic_setf,  \
                double: dynamic_setd,  \
                char: dynamic_setc,  \
                void*: dynamic_setp,  \
                long: dynamic_setl,  \
                long double: dynamic_setld  \
)(D,T)

defineh_dcreator(const char*,s)
defineh_dcreator(const wchar_t*,w)
defineh_dcreator(int,i)
defineh_dcreator(float,f)
defineh_dcreator(double,d)
defineh_dcreator(char,c)
defineh_dcreator(void*,p)
defineh_dcreator(long,l)
defineh_dcreator(long double,ld)

defineh_dsetter(const char*,s)
defineh_dsetter(const wchar_t*,w)
defineh_dsetter(int,i)
defineh_dsetter(float,f)
defineh_dsetter(double,d)
defineh_dsetter(char,c)
defineh_dsetter(void*,p)
defineh_dsetter(long,l)
defineh_dsetter(long double,ld)
/*
 * Returns pointer to dynamics value
 */
void* dynamic_get(Dynamic*);
/*
 * Returns 1 if dynamics are from same type Otherwise 0
 */
int dynamic_comparable(Dynamic*,Dynamic*);
/*
 * Compares Two Dynamic Values
 *  Returns : integer
 *      - (-2) if dynamics are not comparable
 *      - (0) if dynamic values are equal
 *      - (1) if first value greater than second
 *      - (-1) if first value smaller than second
 *      - (2) if no implementation available
 */
int dynamic_compare(Dynamic*,Dynamic*);
/*
 * Size totally dynamic and its value occupies
 */
size_t dynamic_sizeof(Dynamic*);
/*
 * Writes dynamic into file
 * @returns 0 -> failure , 1 -> success
 */
int dynamic_write(Dynamic*,FILE*);
int dynamic_writev(Dynamic,FILE*);
/*
 * Reads file into dynamic
 * @returns 0 -> failure , 1 -> success
 */
int dynamic_read(Dynamic*,FILE*);

define_dcreator(const char*,s,STRING)
define_dcreator(const wchar_t*,w,WIDE_STRING)
define_dcreator(int,i,INT)
define_dcreator(float,f,FLOAT)
define_dcreator(double,d,DOUBLE)
define_dcreator(char,c,CHARACTER)
define_dcreator(void*,p,POINTER)
define_dcreator(long,l,LONG)
define_dcreator(long double,ld,LONG_DOUBLE)

define_dsetter(const char*,s,STRING)
define_dsetter(const wchar_t*,w,WIDE_STRING)
define_dsetter(int,i,INT)
define_dsetter(float,f,FLOAT)
define_dsetter(double,d,DOUBLE)
define_dsetter(char,c,CHARACTER)
define_dsetter(void*,p,POINTER)
define_dsetter(long,l,LONG)
define_dsetter(long double,ld,LONG_DOUBLE)

void* dynamic_get(Dynamic* d){
    switch(d->type){
        case INT:return &d->i;
        case STRING:return d->s;
        case WIDE_STRING:return d->w;
        case LONG_DOUBLE:return &d->ld;
        case LONG:return &d->l;
        case FLOAT:return &d->f;
        case DOUBLE:return &d->d;
        case POINTER:return d->p;
        case CHARACTER:return &d->c;
        default:return NULL;
    }
}
int dynamic_comparable(Dynamic* f,Dynamic* s){
    return f->type == s->type;
}
int dynamic_compare(Dynamic* f,Dynamic* s){
    if(!dynamic_comparable(f,s)) return -2;
    switch(f->type) {
        case INT:return f->i == s->i ? 0 : f->i < s->i ? -1 : 1;
        case DOUBLE:return f->d == s->d ? 0 : f->d < s->d ? -1 : 1;
        case LONG_DOUBLE:return f->ld == s->ld ? 0 : f->ld < s->ld ? -1 : 1;
        case LONG:return f->l == s->l ? 0 : f->l < s->l ? -1 : 1;
        case FLOAT:return f->f == s->f ? 0 : f->f < s->f ? -1 : 1;
        case STRING:return streq(f->s,s->s) ? 0 : 1;
        case WIDE_STRING:return wstreq(f->w,s->w) ? 0 : 1;
        case CHARACTER:return f->c == s->c ? 0 : f->c < s->c ? -1 : 1;
        default:return 2;
    }
}

size_t dynamic_sizeof(Dynamic* d){
    size_t size = 0;
    size += sizeof(char);
    switch(d->type) {
        case INT:size+= sizeof(int);break;
        case DOUBLE:size+= sizeof(double);break;
        case LONG_DOUBLE:size+= sizeof(long double);break;
        case LONG:size+= sizeof(long);break;
        case FLOAT:size+= sizeof(float);break;
        case CHARACTER:size+= sizeof(char);break;
        case STRING:size+= str_len(d->s) + sizeof(int);break;
        case WIDE_STRING:size+= sizeof(wchar_t)*wstr_len(d->w) + sizeof(int);break;
        default:break;
    }
    return size;
}
int dynamic_write(Dynamic* d,FILE* file){
    nreturnv(fwrite(&d->type, sizeof(char),1,file),1,0)
    if(d->type == STRING){
        int len[1] = {str_len(d->s)};
        nreturnv(fwrite(len, sizeof(int),1,file),1,0)
        nreturnv(fwrite(d->s, sizeof(char),(size_t)len[0],file),len[0],0)
    }else if(d->type == WIDE_STRING){
        int len[1] = {wstr_len(d->w)};
        nreturnv(fwrite(len, sizeof(int),1,file),1,0)
        nreturnv(fwrite(d->s, sizeof(wchar_t),(size_t)len[0],file),len[0],0)
    }else{
        nreturnv(fwrite(dynamic_get(d),dynamic_sizeof(d)- sizeof(char),1,file),1,0)
    }
    return 1;
}
int dynamic_writev(Dynamic d,FILE* file) {
    return dynamic_write(&d,file);
}
int dynamic_read(Dynamic* d,FILE* file){
    nreturnv(fread(&d->type, sizeof(char),1,file),1,0)
    if(d->type == STRING){
        int len[1];
        nreturnv(fread(len, sizeof(int),1,file),1,0)
        d->s = (char*) malloc(sizeof(char) * (len[0]+1));
        nreturnv(fread(d->s, sizeof(char),(size_t)len[0],file),len[0],0)
        d->s[len[0]]=0;
    }else if(d->type == WIDE_STRING){
        int len[1];
        nreturnv(fread(len, sizeof(int),1,file),1,0)
        d->w = (wchar_t*) malloc(sizeof(wchar_t) * (len[0]+1));
        nreturnv(fread(d->w, sizeof(wchar_t),(size_t)len[0],file),len[0],0)
        d->w[len[0]]=0;
    }else{
        nreturnv(fread(dynamic_get(d),dynamic_sizeof(d)- sizeof(char),1,file),1,0)
    }
    return 1;
}

#endif
