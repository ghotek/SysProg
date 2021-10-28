#ifdef UNIVERSAL
    #include "universal_stack.h"
#else
    #include "stack.h"
#endif

int main() {

    Stack st;

    #ifdef UNIVERSAL

    StackUniversalCtor(&st);

    UniversalDumpIt(&st);

    int f = 11, n = 156, s = 3311, k = -100, t = 1;

    StackUniversalPush(&st, &f, sizeof(int));

    StackUniversalPush(&st, &n, sizeof(int));

    StackUniversalPush(&st, &s, sizeof(int));

    StackUniversalPush(&st, &k, sizeof(int));

    StackUniversalPush(&st, &t, sizeof(int));

    UniversalDumpIt(&st);

    StackUniversalClear(&st);

    UniversalDumpIt(&st);

    StackUniversalDtor(&st);

    #else

    StackCtor(&st);

    DumpIt(&st);

    StackPush(&st, 10);
    StackPush(&st, 20);
    StackPush(&st, 212);
    StackPush(&st, 300);
    StackPush(&st, 1929);
    StackPush(&st, -29142);
    StackPush(&st, 1);
    StackPush(&st, 2);
    StackPush(&st, 3);
    StackPush(&st, 400);
    StackPush(&st, 2929);
    StackPush(&st, -1);
    StackPush(&st, 10);
    StackPush(&st, 20);
    StackPush(&st, 212);
    StackPush(&st, 300);
    StackPush(&st, 1929);
    StackPush(&st, -29142);
    StackPush(&st, 1);
    StackPush(&st, 2);
    StackPush(&st, 3);
    StackPush(&st, 400);
    StackPush(&st, 2929);
    StackPush(&st, -1);
    StackPush(&st, 10);
    StackPush(&st, 20);
    StackPush(&st, 212);
    StackPush(&st, 300);
    StackPush(&st, 1929);
    StackPush(&st, -29142);
    StackPush(&st, 1);
    StackPush(&st, 2);
    StackPush(&st, 3);
    StackPush(&st, 400);
    StackPush(&st, 2929);
    StackPush(&st, -1);
    StackPush(&st, 0);

    DumpIt(&st);

    StackPop(&st);
    StackPop(&st);

    StackClear(&st);

    DumpIt(&st);

    StackDtor(&st);

    #endif

    return 0;
}
