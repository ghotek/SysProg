#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

const int INF_ROOTS -1;

const int NO_ROOTS 0;

int main() {

    SolverTest();
    return 0;
}

void SolverTest() {

    double a = NAN, b = NAN, c = NAN;
    double x1 = NAN, x2 = NAN;

    //Test 1

    a = 1e-8;
    b = 1e-8;
    c = 1e-8;

    if (SolveSquare(a, b, c, &x1, &x2) == INF_ROOTS)
        printf("# Test 1 :: Done\n");
    else
        printf("# Test 1 :: Fail\n");

    //Test 2

    a = 0;
    b = 1.1;
    c = 0;

    if (SolveSquare(a, b, c, &x1, &x2) == 1 && x1 == 0)
        printf("# Test 2 :: Done\n");
    else
        printf("# Test 2 :: Fail\n");

    //Test 3

    a = 0;
    b = 0;
    c = 1.1;

    if (SolveSquare(a, b, c, &x1, &x2) == NO_ROOTS)
        printf("# Test 3 :: Done\n");
    else
        printf("# Test 3 :: Fail\n");

    //Test 4

    a = 0;
    b = 2.2;
    c = 1.1;

    if (SolveSquare(a, b, c, &x1, &x2) == 1 && x1 == -0.5)
        printf("# Test 4 :: Done\n");
    else
        printf("# Test 4 :: Fail\n");

    //Test 5

    a = 1e-3;
    b = 0;
    c = 0;

    if (SolveSquare(a, b, c, &x1, &x2) == 1 && x1 == 0)
        printf("# Test 5 :: Done\n");
    else
        printf("# Test 5 :: Fail\n");

    //Test 6

    a = 1;
    b = 1;
    c = 0;

    if (SolveSquare(a, b, c, &x1, &x2) == 2 && x1 == 0 && x2 == -1)
        printf("# Test 6 :: Done\n");
    else
        printf("# Test 6 :: Fail\n");

    //Test 7

    a = 1;
    b = -5;
    c = 6;

    if (SolveSquare(a, b, c, &x1, &x2) == 2 && x1 == 3 && x2 == 2)
        printf("# Test 7 :: Done\n");
    else
        printf("# Test 7 :: Fail\n");

    //Test 8

    a = 1;
    b = 2;
    c = 1;

    if (SolveSquare(a, b, c, &x1, &x2) == 1 && x1 == -1)
        printf("# Test 8 :: Done\n");
    else
        printf("# Test 8 :: Fail\n");

    //Test 9

    a = 10;
    b = 1;
    c = 40;

    if (SolveSquare(a, b, c, &x1, &x2) == NO_ROOTS)
        printf("# Test 9 :: Done\n");
    else
        printf("# Test 9 :: Fail\n");

    //Test 10

    a = 1;
    b = 0;
    c = -1;

    if (SolveSquare(a, b, c, &x1, &x2) == 2 && x1 == 1 && x2 == -1)
        printf("# Test 10 :: Done\n");
    else
        printf("# Test 10 :: Fail\n");
}
