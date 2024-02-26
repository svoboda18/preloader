#include "typedefs.h"
#include "sec_hrid.h"
#include "sec_devinfo.h"

u32 get_hrid_size(void)
{
    return HRID_SIZE;
}

u32 get_hrid(unsigned char *rid, unsigned char *rid_sz)
{
    u32 ret = E_SUCCESS;
    u32 i, j;

    if (rid_sz == NULL) {
        return E_BUF_SIZE_ZERO_OR_NULL;
    }

    if (rid == NULL) {
        return E_BUF_ZERO_OR_NULL;
    }

    if (*rid_sz < (HRID_SIZE * 4)) {
        return E_BUF_NOT_ENOUGH;
    }

    for (i = 0; i < HRID_SIZE; i++) {
        u32 reg_val = 0;
        reg_val = seclib_get_devinfo_with_index(12 + i);
        for (j = 0; j < 4; j++) {
            *(rid + i * 4 + j) = (reg_val & (0xff << (8 * j))) >> (8 * j);
        }
    }

    *rid_sz = HRID_SIZE * 4;

    return ret;
}
