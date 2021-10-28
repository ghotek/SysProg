#ifndef SOLVERHEAD_H_INCLUDED
#define SOLVERHEAD_H_INCLUDED

/*!
    Solves a square equation ax^2 + bx + c = 0

    \param  [in]     a   a - Coefficient
    \param  [in]     b   b - Coefficient
    \param  [in]     c   c - Coefficient
    \param  [out]    x1  x1 - Pointer for the first root
    \param  [out]    x2  x2 - Pointer for the second root

    \return Number of roots.

    \note   In case of infinite number of roots,
            returns INF_ROOTS,
            if no roots returns NO_ROOTS.
*/

int     SolveSquare(double a, double b, double c,
                    double* x1, double* x2);

/*!
    Solves a linear equation x = -c/b

    \param  [in]     b   b - Coefficient
    \param  [in]     c   c - Coefficient
    \param  [out]    x   x - Pointer for the root

    \note   This function is used to solve
            linear equations arising in the
            first SolveSquare function
*/

int     SolveLinear(double b, double c, double* x);

/*!
    Compares the returned value with a number close to zero.
    This is necessary to avoid mistakes when working with small "doubles".
    \param [in] x   x - Value to compare with 0

    \note Close to zero value - EPS
*/

int     IsZero(double x);

/*!
    A function that allows you to scan only one entered value.
    Right example: # a: 10
    Wrong example: # a: 10 1 u c

    \param [in] x           x - Pointer for input value
    \param [in] letter_coef letter_coef - a, b or c
*/

void    SafeScan(double* x, const char* letter_coef);


#endif // SOLVERHEAD_H_INCLUDED
