#include "hash.h"
#include <assert.h>

size_t GnuHash(void const* data, size_t sz) {

    assert(data);

	char const* chdata = (char const*)data;

	size_t h = 5381;//посмотреть другие hash функции

	for(char const* ch = chdata; ch < chdata + sz; ++ch) {
		h = h * 33 + (size_t)*ch;
	}

	return h;
}

size_t FinalHash(void const* data1, void const* data2,
                 size_t size1, size_t size2)
{

    size_t hs_data1 = GnuHash(data1, size1);
    size_t hs_data2 = GnuHash(data2, size2);

    size_t f_hs = 33 * hs_data1 + hs_data2;

    return f_hs;
}
