#include "arc_semihost.h"
#include <sys/time.h>
#include <sys/types.h>

int
gettimeofday(struct timeval * restrict tv, void * restrict tz)
{
    int ret = arc_semihost2(SYS_SEMIHOST_gettimeofday, tv, tz);
    if (ret == -1)
        arc_semihost_errno(EFAULT);
    return ret;
}
