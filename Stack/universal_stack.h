#ifndef UNIVERSAL_STACK_H_INCLUDED
#define UNIVERSAL_STACK_H_INCLUDED

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

int StackUniversalCtor(Stack* stk);
int StackUniversalPush(Stack* stk, void const* inp_data, size_t inp_data_sz);
int StackUniversalAddMem(Stack* stk, size_t inp_data_sz);
int StackUniversalPop(Stack* stk, void* inp_data, size_t inp_data_sz);
void StackUniversalClear(Stack* stk);
int StackUniversalDtor(Stack* stk);

void* UniversalSRecalloc(void* r_data, size_t new_capacity, size_t sz);
void UniversalDump(Stack* stk, char const* func_name, char const* file_name, size_t n_line);
static void UniversalUpdateHash(Stack* stc);

#define UniversalDumpIt(stc) UniversalDump(stc, __PRETTY_FUNCTION__, __FILE__, __LINE__)

//----- Debug -----

#ifdef HASH

#include "hash.h"
const int INVALID_HASH = 61;

#endif

#ifdef DEBUG

void StackUniversalAssert(Stack* stk, char const* func_name, char const* file_name, size_t n_line);

int StackUniversalCheck(Stack* stk);

#define StackUniversalAssertIt(stc) StackUniversalAssert(stc, __PRETTY_FUNCTION__, __FILE__, __LINE__)

#endif

#ifdef CANARIES

const size_t CANARY = 0xBAD0DED0;

typedef enum {

    STRUCT_CANARY_ERR = 51,
    ARRAY_CANARY_ERR = 52
} CANARY_ERR;

#endif

//-----------------

#endif // UNIVERSAL_STACK_H_INCLUDED
