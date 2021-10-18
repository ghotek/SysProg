#include "universal_stack.h"

int StackUniversalCtor(Stack* stk) {

    assert(stk);

    stk->capacity = ST_SZ;

    #ifdef CANARIES

    stk->Canary1 = CANARY;
    stk->Canary2 = CANARY;

    if(!(stk->data = (char*)calloc(ST_SZ + 2 * sizeof(size_t), sizeof(char))))
        return MEM_ERR;

    *((size_t*)(stk->data)) = CANARY;
    *((size_t*)(stk->data + sizeof(size_t) + ST_SZ)) = CANARY;

    stk->data += sizeof(size_t);

        #ifdef HASH

        stk->hash = FinalHash(stk, stk->data - sizeof(size_t),
                              sizeof(stk), (stk->capacity) + 2 * sizeof(size_t));
        #endif

    #else

    if (!(stk->data = (char*)calloc(ST_SZ, sizeof(char))))
        return MEM_ERR;

        #ifdef HASH

        stk->hash = FinalHash(stk, stk->data,
                              sizeof(stk), stk->capacity);

        #endif

    #endif

    stk->sz = 0;

    #ifdef DEBUG
        StackUniversalAssertIt(stk);
    #endif

    return 1;
}


int StackUniversalPush(Stack* stk, void const* inp_data, size_t inp_data_sz) {

    #ifdef DEBUG
        StackUniversalAssertIt(stk);
    #endif

    if (inp_data == NULL || !inp_data_sz)
        return NULL_POINTER_ERR;

    if (stk->sz + inp_data_sz > stk->capacity) {

        if (!StackUniversalAddMem(stk, inp_data_sz))
            return NO_FREE_MEM_ERR;
    }

    memcpy(stk->data + stk->sz, inp_data, inp_data_sz);
    stk->sz += inp_data_sz;

    UniversalUpdateHash(stk);

    #ifdef DEBUG
        StackUniversalAssertIt(stk);
    #endif

    return 1;
}

static void UniversalUpdateHash(Stack* stk) {

    #ifdef CANARIES

        #ifdef HASH

        stk->hash = FinalHash(stk, stk->data - sizeof(size_t),
                              sizeof(stk), (stk->capacity) + 2 * sizeof(size_t));

        #endif

    #else

        #ifdef HASH

        stk->hash = FinalHash(stk, stk->data,
                              sizeof(stk), stk->capacity);

        #endif

    #endif
}

int StackUniversalPop(Stack* stk, void* inp_data, size_t inp_data_sz) {

    #ifdef DEBUG
        StackUniversalAssertIt(stk);
    #endif

    if (inp_data == NULL || !inp_data_sz)
        return NULL_POINTER_ERR;

    if (stk->sz < inp_data_sz)
        return STACK_UNDERFLOW;

    stk->sz -= inp_data_sz;
    memcpy(inp_data, stk->data + stk->sz, inp_data_sz);
    memset(stk->data + stk->sz, 0, inp_data_sz);

    UniversalUpdateHash(stk);

    #ifdef DEBUG
        StackUniversalAssertIt(stk);
    #endif

    return 1;
}

int StackUniversalAddMem(Stack* stk, size_t inp_data_sz) {

    #ifdef DEBUG
        StackUniversalAssertIt(stk);
    #endif

    if (stk->sz + inp_data_sz > stk->capacity * RLLOC_MUL)
        stk->capacity = stk->sz + inp_data_sz;
    else
        stk->capacity *= RLLOC_MUL;

    #ifdef CANARIES

    char* new_data = (char*)UniversalSRecalloc(stk->data - sizeof(size_t),
                                               stk->capacity,
                                               stk->sz);
    if (new_data) {

        stk->data = new_data + sizeof(size_t);
        *((size_t*)(stk->data + stk->capacity)) = CANARY;
    } else
        return 0;

        #ifdef HASH

        stk->hash = FinalHash(stk, stk->data - sizeof(size_t),
                              sizeof(stk), (stk->capacity) + 2 * sizeof(size_t));

        #endif

    #else

    char* new_data = (char*)UniversalSRecalloc(stk->data, stk->capacity, stk->sz);
    if (new_data)
        stk->data = new_data;
    else
        return 0;

        #ifdef HASH

        stk->hash = FinalHash(stk, stk->data,
                              sizeof(stk), stk->capacity);

        #endif

    #endif

    #ifdef DEBUG
        StackUniversalAssertIt(stk);
    #endif

    return 1;
}

void* UniversalSRecalloc(void* r_data, size_t new_capacity, size_t sz) {

    assert(r_data);

    char* rc_data = (char*)r_data;

    #ifdef CANARIES

    char* mem_data = rc_data + sz + sizeof(size_t);

    if ((r_data = realloc(r_data, new_capacity + 2 * sizeof(size_t))))
        memset(mem_data, 0, new_capacity - sz);

    #else

    char* mem_data = rc_data + sz;

    if ((r_data = realloc(r_data, new_capacity)))
        memset(mem_data, 0, new_capacity - sz);

    #endif

    return r_data;
}

void StackUniversalClear(Stack* stk) {

    #ifdef DEBUG
        StackUniversalAssertIt(stk);
    #endif

    stk->sz = 0;
    memset(stk->data, 0, stk->capacity);

    UniversalUpdateHash(stk);

    #ifdef DEBUG
        StackUniversalAssertIt(stk);
    #endif
}

int StackUniversalDtor(Stack* stk) {

    #ifdef DEBUG
        StackUniversalAssertIt(stk);
    #endif

    #ifdef CANARIES

    char* dtor_data = stk->data - sizeof(size_t);

    free(dtor_data);
    stk->capacity = -1;
    dtor_data = NULL;

    #else

    free(stk->data);
    stk->capacity = -1;
    stk->data = NULL;

    #endif

    return 1;
}

#ifdef DEBUG

void StackUniversalAssert(Stack* stk, char const* func_name, char const* file_name, size_t n_line) {

    stk->last_err = StackUniversalCheck(stk);

    if (stk->last_err) {

        printf("Stack [0x%p] dump from %s (%s %u), reason %i \n",
                stk, func_name, file_name, n_line, stk->last_err);
        fflush(stdout);

        UniversalDump(stk, func_name, file_name, n_line);
        abort();
    }
}

int StackUniversalCheck(Stack* stk) {

    if (stk == NULL)
        return NULL_POINTER_ERR;

    if (stk->data == NULL)
        return NULL_POINTER_ERR;

    if (stk->sz > stk->capacity)
        return FATAL_MEM_ERR;

    #ifdef CANARIES

    if (stk->Canary1 != CANARY || stk->Canary2 != CANARY)
        return STRUCT_CANARY_ERR;

    if ( *(size_t*)(stk->data - sizeof(size_t)) != CANARY ||
         *(size_t*)(stk->data + stk->capacity) != CANARY )
        return ARRAY_CANARY_ERR;

        #ifdef HASH

        size_t old_hash = stk->hash;
        stk->hash = 0;

        size_t check_hash = FinalHash(stk, stk->data - sizeof(size_t),
                                      sizeof(stk), (stk->capacity) + 2 * sizeof(size_t));

        #endif

    #else

        #ifdef HASH

        size_t old_hash = stk->hash;
        stk->hash = 0;

        size_t check_hash = FinalHash(stk, stk->data,
                                      sizeof(stk), stk->capacity);

        #endif

    #endif

    #ifdef HASH

    stk->hash = old_hash;

    if (check_hash - old_hash)
        return INVALID_HASH;

    #endif

    return 0;
}

#endif

void UniversalDump(Stack* stk, char const* func_name, char const* file_name, size_t n_line) {

    printf("Stack <heterogeneous> [0x%p] \"stk\" at %s at %s(%u)\n",
            stk, func_name, file_name, n_line);
    printf("{\n");
    fflush(stdout);

    for (char* i = stk->data; i < stk->data + stk->capacity; i++) {

        if (i < stk->data + stk->sz)
            printf("\t*");
        else
            printf("\t ");

        printf("[%u]\t%#x\n", i - stk->data, *(unsigned char*)i);
        fflush(stdout);
    }

    #ifdef CANARIES

    printf("\n\t Array Canary1    :\t{%#x}\n", *(size_t*)(stk->data - sizeof(size_t)));
    fflush(stdout);
    printf("\t Array Canary2    :\t{%#x}\n", *(size_t*)(stk->data + stk->capacity));
    fflush(stdout);

    printf("\t Canary1          :\t{%#x}\n", stk->Canary1);
    fflush(stdout);
    printf("\t Canary2          :\t{%#x}\n", stk->Canary2);
    fflush(stdout);

    #endif

    printf("\t Capacity         :\t{%u}\n", stk->capacity);
    fflush(stdout);
    printf("\t Size             :\t{%u}\n", stk->sz);
    fflush(stdout);

    #ifdef HASH

    printf("\t Hash             :\t{%#x}\n", stk->hash);
    fflush(stdout);

    #endif

    #ifdef DEBUG
    printf("\t Last Error       :\t{%u}\n", stk->last_err);
    fflush(stdout);

    #endif

    printf("}\n");
}
