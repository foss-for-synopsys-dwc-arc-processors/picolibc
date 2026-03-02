#include "arc_semihost.h"
#include <sys/time.h>
#include <sys/types.h>

int
gettimeofday(struct timeval *tv, void *tz)
{
    int ret = arc_semihost2(SYS_SEMIHOST_gettimeofday, (uintptr_t)tv, (uintptr_t)tz);
    if (ret == -1)
        arc_semihost_errno(EFAULT);
    return ret;
}
