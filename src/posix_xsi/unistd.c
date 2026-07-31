#include "p101_text/text.h"
#include <p101_env/wrapper.h>
#ifdef __linux__
    #include <crypt.h>
#endif
#include <unistd.h>

void p101_swab(const struct p101_env *env, const void *restrict src, void *restrict dest, ssize_t nbytes)
{
    P101_TRACE(env);
    errno = 0;
    swab(src, dest, nbytes);
    P101_TRACE_EXIT(env);
}
