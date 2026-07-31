#include "p101_text/text.h"
#include <p101_env/wrapper.h>
#include <string.h>

void *p101_memccpy(const struct p101_env *env, void *restrict s1, const void *restrict s2, int c, size_t n)
{
    void *ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = memccpy(s1, s2, c, n);

    P101_TRACE_EXIT(env);
    return ret_val;
}
