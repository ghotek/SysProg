#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "solverHead.h"

/*!
    If the square equation has an infinite number of solutions,
    then SolveSquare will return this value.
*/
const int INF_ROOTS = -1;

/*!
    If the square equation has no roots, then the SolveSquare will return this value.
*/
const int NO_ROOTS = 0;

/*!
    A value close to zero, comparison with which
    makes it clear how close the "double" value is to zero.
*/
const double EPS = 1e-7;


int main() {

    printf ("# SQUARE EQUATION SOLVER\n"
            "# 11.09.2021\n\n");

    double a = NAN, b = NAN, c = NAN;

    printf("# Enter values of a, b, c\n");

    SafeScan(&a, "a");
    SafeScan(&b, "b");
    SafeScan(&c, "c");

    printf("# Your square equation: (%lg)x^2 + (%lg)x + (%lg) = 0\n", a, b, c);

    double x1 = NAN, x2 = NAN;
    int keyRoots = SolveSquare(a, b, c, &x1, &x2);

    switch(keyRoots) {

        case 1:
            printf("# Root: x1 = %lg", x1);
            break;

        case 2:
            printf("# Roots: x1 = %lg\n#\t x2 = %lg", x1, x2);
            break;

        case NO_ROOTS:
            printf("# No roots\n");
            break;

        case INF_ROOTS:
            printf("# Infinite number of roots\n");
            break;

        default:
            printf("SolveEquation: unknown error\n");
            return 1;
            break;
    }
    return 0;
}


void SafeScan(double* x, const char* letter_coef) {

    char buf[128] = {};
    int flag = 0, l = 0;

    printf("# %c : ", *letter_coef);
    do {
        gets(buf);
        l = strlen(buf);

        for (int i = 0; i < l; i++) {
            flag = 0;

            if (!isdigit(buf[i]) && (buf[i] != '.') && (buf[i]!= '-')) {
                printf("# Wrong input format, please enter correct\n"
                       "# Coefficient: {your value}\n");
                flag = 1;
                break;
            }
        }
        if(flag) {
            printf("# %c : ", *letter_coef);
        } else sscanf(buf, "%lg", x);

    }  while(flag);
}


int IsZero(double x) {
    assert(isfinite(x));
    return (fabs(x) < EPS) ? 1 : 0;
}

int SolveSquare(double a, double b, double c,
                double* x1, double* x2)
{
    assert(isfinite(a));
    assert(isfinite(b));
    assert(isfinite(c));

    assert(x1 != NULL);
    assert(x2 != NULL);
    assert(x1 != x2);

    if (IsZero(a)) {
        if (IsZero(b)) {
            return (IsZero(c)) ? INF_ROOTS : NO_ROOTS;
        }

        return SolveLinear(b, c, x1);
    }

    double D = b*b - 4*a*c;

    if (IsZero(D))
        return SolveLinear(2*a, b, x1);

    if (D > 0) {
        *x1 = (-b + sqrt(D))/(2*a);
        *x2 = (-b - sqrt(D))/(2*a);
        return 2;
    }
    return NO_ROOTS;
}

int SolveLinear(double b, double c, double* x) {

    *x = -c/b;
    return 1;
}
