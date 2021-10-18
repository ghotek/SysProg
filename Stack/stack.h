#ifndef STACKHEAD_H_INCLUDED
#define STACKHEAD_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

typedef struct {

    #ifdef CANARIES

    size_t Canary1;

    #endif

    char* data;
    size_t capacity;
    size_t sz;

    #ifdef HASH

    size_t hash;

    #endif

    #ifdef DEBUG

    size_t last_err;

    #endif

    #ifdef CANARIES

    size_t Canary2;

    #endif

} Stack;

//----- STACK INIT CONSTANT LIST -----

typedef enum {

    RLLOC_MUL = 2,
    ST_SZ     = 4
} STK_CONST_L;

//------------------------------------

//----- MEMORY ERRORS LIST -----

typedef enum {

    MEM_ERR          = 2,
    NO_FREE_MEM_ERR  = 3,
    FATAL_MEM_ERR    = 4,
    NULL_POINTER_ERR = 5,
    STACK_UNDERFLOW  = 6
} MEM_ERR_L;

//------------------------------

int StackCtor(Stack* stk);
int StackPush(Stack* stk, int value);
int StackAddMem(Stack* stk);
int StackPop(Stack* stk);
void StackClear(Stack* stk);
int StackDtor(Stack* stk);

void* SRecalloc(void* r_data, size_t new_capacity, size_t sz, size_t size_of);
void Dump(Stack* stk, char const* func_name, char const* file_name, size_t n_line);
static void UpdateHash(Stack* stc);

#define DumpIt(stc) Dump(stc, __PRETTY_FUNCTION__, __FILE__, __LINE__)

//----- Debug -----

#ifdef HASH

#include "hash.h"
const int INVALID_HASH = 61;

#endif

#ifdef DEBUG

void StackAssert(Stack* stk, char const* func_name, char const* file_name, size_t n_line);

int StackCheck(Stack* stk);

#define StackAssertIt(stc) StackAssert(stc, __PRETTY_FUNCTION__, __FILE__, __LINE__)

#endif

#ifdef CANARIES

const size_t CANARY = 0xBAD0DED0;

typedef enum {

    STRUCT_CANARY_ERR = 51,
    ARRAY_CANARY_ERR = 52
} CANARY_ERR;

#endif

//-----------------

#endif // STACKHEAD_H_INCLUDED
