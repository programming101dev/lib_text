#include "p101_text/text.h"
#include <p101_c/p101_string.h>
#include <p101_env/wrapper.h>
#include <stdlib.h>
#include <string.h>

long p101_a64l(const struct p101_env *env, const char *s)
{
    long ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = a64l(s);

    P101_TRACE_EXIT(env);
    return ret_val;
}

char *p101_l64a(const struct p101_env *env, long value)
{
    char *ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = l64a(value);

    P101_TRACE_EXIT(env);
    return ret_val;
}
