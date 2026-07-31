#include "p101_text/text.h"
#include <p101_env/wrapper.h>
#include <strings.h>

int p101_ffs(const struct p101_env *env, int i)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = ffs(i);

    P101_TRACE_EXIT(env);
    return ret_val;
}
