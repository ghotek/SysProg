#ifndef HASHHEAD_H_INCLUDED
#define HASHHEAD_H_INCLUDED

#include <stddef.h>
#include <stdint.h>

size_t GnuHash(void const* data, size_t sz);
size_t FinalHash(void const* data1, void const* data2,
                 size_t size1, size_t size2);

#endif // HASHHEAD_H_INCLUDED
