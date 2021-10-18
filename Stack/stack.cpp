#include "stack.h"

int StackCtor(Stack* stk) {

    assert(stk);

    stk->capacity = ST_SZ;

    #ifdef CANARIES

    stk->Canary1 = CANARY;
    stk->Canary2 = CANARY;

    if(!(stk->data = (char*)calloc(ST_SZ * sizeof(int) + 2 * sizeof(size_t), sizeof(char))))
        return MEM_ERR;

    *((size_t*)(stk->data)) = CANARY;
    *((size_t*)(stk->data + sizeof(size_t) + ST_SZ * sizeof(int))) = CANARY;

    stk->data += sizeof(size_t);

        #ifdef HASH

        stk->hash = FinalHash(stk, stk->data - sizeof(size_t),
                              sizeof(stk), sizeof(int) * (stk->capacity) + 2 * sizeof(size_t));
        #endif

    #else

    if (!(stk->data = (char*)calloc(ST_SZ * sizeof(int), sizeof(char))))
        return MEM_ERR;

        #ifdef HASH

        stk->hash = FinalHash(stk, stk->data,
                              sizeof(stk), sizeof(int) * (stk->capacity));

        #endif

    #endif

    stk->sz = 0;

    #ifdef DEBUG
        StackAssertIt(stk);
    #endif

    return 1;
}

int StackPush(Stack* stk, int value) {

    #ifdef DEBUG
        StackAssertIt(stk);
    #endif

    if (stk->sz == stk->capacity) {

        if (!StackAddMem(stk))
            return NO_FREE_MEM_ERR;
    }

    *(int*)(stk->data + stk->sz * sizeof(int)) = value;
    stk->sz++;

    UpdateHash(stk);

    #ifdef DEBUG
        StackAssertIt(stk);
    #endif

    return 1;
}

static void UpdateHash(Stack* stk) {

    #ifdef CANARIES

        #ifdef HASH

        stk->hash = FinalHash(stk, stk->data - sizeof(size_t),
                              sizeof(stk), sizeof(int) * (stk->capacity) + 2 * sizeof(size_t));

        #endif

    #else

        #ifdef HASH

        stk->hash = FinalHash(stk, stk->data,
                              sizeof(stk), sizeof(int) * (stk->capacity));

        #endif

    #endif
}

int StackPop(Stack* stk) {

    #ifdef DEBUG
        StackAssertIt(stk);
    #endif

    int r_value = 0;

    stk->sz--;

    r_value = *(stk->data + stk->sz * sizeof(int));

    *(stk->data + stk->sz * sizeof(int)) = 0;

    UpdateHash(stk);

    #ifdef DEBUG
        StackAssertIt(stk);
    #endif

    return r_value;
}

int StackAddMem(Stack* stk) {

    #ifdef DEBUG
        StackAssertIt(stk);
    #endif

    stk->capacity *= RLLOC_MUL;

    #ifdef CANARIES

    char* new_data = (char*)SRecalloc(stk->data - sizeof(size_t),
                                      stk->capacity,
                                      stk->sz, sizeof(int));
    if (new_data) {

        stk->data = new_data + sizeof(size_t);
        *((size_t*)(stk->data + stk->capacity * sizeof(int))) = CANARY;
    } else
        return 0;

        #ifdef HASH

        stk->hash = FinalHash(stk, stk->data - sizeof(size_t),
                              sizeof(stk), sizeof(int) * (stk->capacity) + 2 * sizeof(size_t));

        #endif

    #else

    int* new_data = (int*)SRecalloc(stk->data, stk->capacity, stk->sz, sizeof(int));
    if (new_data)
        stk->data = (char*)new_data;
    else
        return 0;

        #ifdef HASH

        stk->hash = FinalHash(stk, stk->data,
                              sizeof(stk), sizeof(int) * (stk->capacity));

        #endif

    #endif

    #ifdef DEBUG
        StackAssertIt(stk);
    #endif

    return 1;
}

void* SRecalloc(void* r_data, size_t new_capacity, size_t sz, size_t size_of) {

    assert(r_data);

    char* rc_data = (char*)r_data;

    #ifdef CANARIES

    char* mem_data = rc_data + sz * size_of + sizeof(size_t);

    if ((r_data = realloc(r_data, new_capacity * size_of + 2*sizeof(size_t))))
        memset(mem_data, 0, sz * (RLLOC_MUL - 1) * size_of);

    #else

    char* mem_data = rc_data + sz * size_of;

    if ((r_data = realloc(r_data, new_capacity * size_of)))
        memset(mem_data, 0, sz * (RLLOC_MUL - 1) * size_of);

    #endif

    return r_data;
}

void StackClear(Stack* stk) {

    #ifdef DEBUG
        StackAssertIt(stk);
    #endif

    stk->sz = 0;
    memset(stk->data, 0, stk->capacity * sizeof(int));

    UpdateHash(stk);

    #ifdef DEBUG
        StackAssertIt(stk);
    #endif
}

int StackDtor(Stack* stk) {

    #ifdef DEBUG
        StackAssertIt(stk);
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

void StackAssert(Stack* stk, char const* func_name, char const* file_name, size_t n_line) {

    stk->last_err = StackCheck(stk);

    if (stk->last_err) {

        printf("Stack [0x%p] dump from %s (%s %d), reason %i \n",
                stk, func_name, file_name, n_line, stk->last_err);
        fflush(stdout);

        Dump(stk, func_name, file_name, n_line);
        // abort();  :)))
    }
}

int StackCheck(Stack* stk) {

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
         *(size_t*)(stk->data + stk->capacity * sizeof(int)) != CANARY )
        return ARRAY_CANARY_ERR;

        #ifdef HASH

        size_t old_hash = stk->hash;
        stk->hash = 0;

        size_t check_hash = FinalHash(stk, stk->data - sizeof(size_t),
                                      sizeof(stk), sizeof(int) * (stk->capacity) + 2 * sizeof(size_t));

        #endif

    #else

        #ifdef HASH

        size_t old_hash = stk->hash;
        stk->hash = 0;

        size_t check_hash = FinalHash(stk, stk->data,
                                      sizeof(stk), sizeof(int) * (stk->capacity));

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

void Dump(Stack* stk, char const* func_name, char const* file_name, size_t n_line) {//в логи

    printf("Stack <int> [0x%p] \"stk\" at %s at %s(%d)\n",
            stk, func_name, file_name, n_line);
    printf("{\n");
    fflush(stdout);

    for (int* i = (int*)stk->data; i < (int*)stk->data + stk->capacity; i++) {

        if (i < (int*)stk->data + stk->sz)
            printf("\t*");
        else
            printf("\t ");

        printf("[%d]\t%d\n", i - (int*)stk->data, *i);
        fflush(stdout);
    }

    #ifdef CANARIES

    printf("\n\t Array Canary1    :\t{%#x}\n", *(size_t*)(stk->data - sizeof(size_t)));
    fflush(stdout);
    printf("\t Array Canary2    :\t{%#x}\n", *(size_t*)(stk->data + stk->capacity * sizeof(int)));
    fflush(stdout);

    printf("\t Canary1          :\t{%#x}\n", stk->Canary1);
    fflush(stdout);
    printf("\t Canary2          :\t{%#x}\n", stk->Canary2);
    fflush(stdout);

    #endif

    printf("\t Capacity         :\t{%d}\n", stk->capacity);
    fflush(stdout);
    printf("\t Size             :\t{%d}\n", stk->sz);
    fflush(stdout);

    #ifdef HASH

    printf("\t Hash             :\t{%#x}\n", stk->hash);
    fflush(stdout);

    #endif

    #ifdef DEBUG
    printf("\t Last Error       :\t{%d}\n", stk->last_err);
    fflush(stdout);

    #endif

    printf("}\n");
}
