/*
 * Copyright 2026 D'Arcy Smith.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "p101_text/p101_ctype.h"
#include "p101_text/p101_regex.h"
#include "p101_text/p101_stdlib.h"
#include "p101_text/p101_string.h"
#include "p101_text/p101_strings.h"
#include "p101_text/p101_unistd.h"
#include "p101_text/p101_wchar.h"
#include "p101_text/p101_wctype.h"
#include "p101_text/p101_wordexp.h"
#include <p101_env/wrapper.h>

/*
 * Copyright 2021-2024 D'Arcy Smith.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <p101_c/p101_string.h>
#include <string.h>

#if defined(__APPLE__) || defined(__FreeBSD__)
    #include <xlocale.h>
#endif

char *p101_stpcpy(const struct p101_env *env, char *restrict s1, const char *restrict s2)
{
    char *ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = stpcpy(s1, s2);

    P101_TRACE_EXIT(env);
    return ret_val;
}

char *p101_stpncpy(const struct p101_env *env, char *restrict s1, const char *restrict s2, size_t n)
{
    char *ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = stpncpy(s1, s2, n);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_strcoll_l(const struct p101_env *env, struct p101_error *err, const char *s1, const char *s2, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN(env, err, ret_val, 0);
    errno   = 0;
    ret_val = strcoll_l(s1, s2, locale);

    if(errno != 0)
    {
        P101_ERROR_RAISE_ERRNO(err, errno);
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

char *p101_strdup(const struct p101_env *env, struct p101_error *err, const char *s)
{
    char *ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN(env, err, ret_val, NULL);
    errno   = 0;
    ret_val = strdup(s);

    if(ret_val == NULL)
    {
        P101_ERROR_RAISE_ERRNO(err, errno);
    }
    else
    {
        P101_TRACK_ALLOC(env, ret_val, p101_strlen(env, s) + 1U);
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

int p101_strerror_r(const struct p101_env *env, struct p101_error *err, int errnum, char *strerrbuf, size_t buflen)
{
    int ret_val = 0;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN(env, err, ret_val, -1);
    errno = 0;

#if defined(__GLIBC__) && defined(_GNU_SOURCE)
    /* GNU variant: returns char* (may be strerrbuf or static storage). */
    {
        const char *res = strerror_r(errnum, strerrbuf, buflen);

        if(res == NULL)
        {
            ret_val = (errno != 0) ? errno : EINVAL;
        }
        else if(res != strerrbuf)
        {
            /* Ensure result is in caller buffer for consistent behavior. */
            if(buflen == 0)
            {
                ret_val = ERANGE;
            }
            else
            {
                size_t n       = p101_strlen(env, res);
                size_t maxcopy = buflen - 1;
                size_t copy    = n;

                if(copy > maxcopy)
                {
                    copy = maxcopy;
                }
                if(copy > 0)
                {
                    p101_memcpy(env, strerrbuf, res, copy);
                }
                strerrbuf[copy] = '\0';
            }
        }
    }
#else
    /* POSIX variant: returns int (0 on success, error number on failure). */
    ret_val = strerror_r(errnum, strerrbuf, buflen);
#endif

    if(ret_val != 0)
    {
        if(errno == 0)
        {
            errno = ret_val;
        }

        P101_ERROR_RAISE_ERRNO(err, errno);
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

char *p101_strndup(const struct p101_env *env, struct p101_error *err, const char *s, size_t size)
{
    char *ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN(env, err, ret_val, NULL);
    errno   = 0;
    ret_val = strndup(s, size);

    if(ret_val == NULL)
    {
        P101_ERROR_RAISE_ERRNO(err, errno);
    }
    else
    {
        P101_TRACK_ALLOC(env, ret_val, p101_strnlen(env, s, size) + 1U);
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

size_t p101_strnlen(const struct p101_env *env, const char *s, size_t maxlen)
{
    size_t ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = strnlen(s, maxlen);

    P101_TRACE_EXIT(env);
    return ret_val;
}

char *p101_strsignal(const struct p101_env *env, struct p101_error *err, int signum)
{
    char *ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN(env, err, ret_val, NULL);
    errno   = 0;
    ret_val = strsignal(signum);

    if(ret_val == NULL && errno != 0)
    {
        P101_ERROR_RAISE_ERRNO(err, errno);
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

char *p101_strtok_r(const struct p101_env *env, char *restrict s, const char *restrict sep, char **restrict state)
{
    char *ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = strtok_r(s, sep, state);

    P101_TRACE_EXIT(env);
    return ret_val;
}

size_t p101_strxfrm_l(const struct p101_env *env, struct p101_error *err, char *restrict s1, const char *restrict s2, size_t n, locale_t locale)
{
    size_t ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN(env, err, ret_val, 0);
    errno   = 0;
    ret_val = strxfrm_l(s1, s2, n, locale);

    if(errno != 0)
    {
        P101_ERROR_RAISE_ERRNO(err, errno);
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

void *p101_memccpy(const struct p101_env *env, void *restrict s1, const void *restrict s2, int c, size_t n)
{
    void *ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = memccpy(s1, s2, c, n);

    P101_TRACE_EXIT(env);
    return ret_val;
}

/*
 * Copyright 2022-2024 D'Arcy Smith.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

char *p101_strcasestr(const struct p101_env *env, const char *big, const char *little)
{
    char *ret_val;

    P101_TRACE(env);
    ret_val = strcasestr(big, little);

    P101_TRACE_EXIT(env);
    return ret_val;
}

char *p101_strchrnul(const struct p101_env *env, const char *s, int c)
{
    char *ret_val;

    P101_TRACE(env);
    ret_val = strchrnul(s, c);

    P101_TRACE_EXIT(env);
    return ret_val;
}

char *p101_strsep(const struct p101_env *env, char **stringp, const char *delim)
{
    char *ret_val;

    P101_TRACE(env);
    ret_val = strsep(stringp, delim);

    P101_TRACE_EXIT(env);
    return ret_val;
}
