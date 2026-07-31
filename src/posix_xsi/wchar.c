#include "p101_text/text.h"
#include <p101_env/wrapper.h>
#include <wchar.h>

int p101_wcswidth(const struct p101_env *env, const wchar_t *pwcs, size_t n)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = wcswidth(pwcs, n);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_wcwidth(const struct p101_env *env, wchar_t wc)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = wcwidth(wc);

    P101_TRACE_EXIT(env);
    return ret_val;
}
