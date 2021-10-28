#ifndef CORRECTIONHEADER_H_INCLUDED
#define CORRECTIONHEADER_H_INCLUDED

/*!
    Finding file size on bytes

    \param  [in]    f   f - file with unknown size.

    \return size of input file.
*/
unsigned int FileSize(FILE* f);

/*!
    File correction function

    \param  [in]    unc_f   unc_f - uncorrect file.
    \param  [in]    c_f     c_f   - corrected file.
    \param  [in]    sz_f    sz_f  - size of input uncorrect file.

    \return 1   - if program ran successful.
            etd - error with suitable code.
*/
int FileCorrection(FILE* unc_f, FILE* c_f, unsigned int sz_f);

#endif // CORRECTIONHEADER_H_INCLUDED
