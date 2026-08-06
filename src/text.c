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

#include "p101_text/text.h"

int p101_isalnum_l(const struct p101_env *env, int c, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = isalnum_l(c, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_isalpha_l(const struct p101_env *env, int c, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = isalpha_l(c, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_isblank_l(const struct p101_env *env, int c, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = isblank_l(c, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_iscntrl_l(const struct p101_env *env, int c, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = iscntrl_l(c, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_isdigit_l(const struct p101_env *env, int c, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = isdigit_l(c, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_isgraph_l(const struct p101_env *env, int c, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = isgraph_l(c, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_islower_l(const struct p101_env *env, int c, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = islower_l(c, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_isprint_l(const struct p101_env *env, int c, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = isprint_l(c, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_ispunct_l(const struct p101_env *env, int c, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = ispunct_l(c, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_isspace_l(const struct p101_env *env, int c, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = isspace_l(c, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_isupper_l(const struct p101_env *env, int c, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = isupper_l(c, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_isxdigit_l(const struct p101_env *env, int c, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = isxdigit_l(c, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_tolower_l(const struct p101_env *env, int c, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = tolower_l(c, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_toupper_l(const struct p101_env *env, int c, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = toupper_l(c, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

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

#include <p101_c/p101_stdlib.h>
#include <p101_env/wrapper.h>

int p101_regcomp(const struct p101_env *env, struct p101_error *err, regex_t *restrict preg, const char *restrict pattern, int cflags)
{
    int ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN_SYSTEM_CODE(env, err, ret_val);
    errno   = 0;
    ret_val = regcomp(preg, pattern, cflags);

    if(ret_val != 0)
    {
        bool   has_error;
        size_t len;
        size_t reported_len;
        char  *msg;
        void  *memory;

        len       = p101_regerror(env, ret_val, preg, NULL, 0);
        memory    = p101_malloc(env, err, len);
        msg       = (char *)memory;
        has_error = p101_error_has_error(err);

        if(msg == NULL || has_error)
        {
            p101_free(env, msg);
            goto done;
        }

        reported_len = p101_regerror(env, ret_val, preg, msg, len);
        if(reported_len == 0U || reported_len > len)
        {
            P101_ERROR_RAISE_SYSTEM(err, "Unable to format the regular expression error", ret_val);
        }
        else
        {
            P101_ERROR_RAISE_SYSTEM(err, msg, ret_val);
        }
        p101_free(env, msg);
    }
    else
    {
        P101_TRACK_POINTER_RESOURCE_ACQUIRE(env, "compiled-regex", preg, 0U, NULL);
    }

done:
    P101_WRAPPER_DONE(env);
    return ret_val;
}

size_t p101_regerror(const struct p101_env *env, int errcode, const regex_t *restrict preg, char *restrict errbuf, size_t errbuf_size)
{
    size_t ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = regerror(errcode, preg, errbuf, errbuf_size);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_regexec(const struct p101_env *env, struct p101_error *err, const regex_t *restrict preg, const char *restrict string, size_t nmatch, regmatch_t pmatch[restrict], int eflags)
{
    int ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN_SYSTEM_CODE(env, err, ret_val);
    errno   = 0;
    ret_val = regexec(preg, string, nmatch, pmatch, eflags);

    if(ret_val != 0 && ret_val != REG_NOMATCH)
    {
        P101_ERROR_RAISE_SYSTEM(err, "Regular expression execution failed", ret_val);
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

void p101_regfree(const struct p101_env *env, regex_t *preg)
{
    char resource_id[P101_ENV_POINTER_RESOURCE_ID_SIZE];

    P101_TRACE(env);
    p101_env_pointer_resource_id(resource_id, sizeof(resource_id), preg);
    errno = 0;
    regfree(preg);
    P101_TRACK_RESOURCE_RELEASE(env, "compiled-regex", resource_id, NULL);
    P101_TRACE_EXIT(env);
}

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

#include <strings.h>

int p101_strcasecmp(const struct p101_env *env, const char *s1, const char *s2)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = strcasecmp(s1, s2);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_strcasecmp_l(const struct p101_env *env, const char *s1, const char *s2, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = strcasecmp_l(s1, s2, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_strncasecmp(const struct p101_env *env, const char *s1, const char *s2, size_t n)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = strncasecmp(s1, s2, n);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_strncasecmp_l(const struct p101_env *env, const char *s1, const char *s2, size_t n, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = strncasecmp_l(s1, s2, n, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

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

#include <p101_c/p101_wchar.h>

size_t p101_mbsnrtowcs(const struct p101_env *env, struct p101_error *err, wchar_t *restrict dst, const char **restrict src, size_t nmc, size_t len, mbstate_t *restrict ps)
{
    int    actual_error;
    size_t ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN(env, err, ret_val, (size_t)-1);
    errno        = 0;
    ret_val      = mbsnrtowcs(dst, src, nmc, len, ps);
    actual_error = errno;

    if(ret_val == (size_t)-1)
    {
        P101_ERROR_RAISE_ERRNO(err, actual_error == 0 ? EILSEQ : actual_error);
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

FILE *p101_open_wmemstream(const struct p101_env *env, struct p101_error *err, wchar_t **bufp, size_t *sizep)
{
    FILE *ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN(env, err, ret_val, NULL);
    errno   = 0;
    ret_val = open_wmemstream(bufp, sizep);

    if(ret_val == NULL)
    {
        P101_ERROR_RAISE_ERRNO(err, errno);
    }
    else
    {
        P101_TRACK_POINTER_RESOURCE_ACQUIRE(env, "stdio-stream", ret_val, 0U, "open_wmemstream");
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

wchar_t *p101_wcpcpy(const struct p101_env *env, wchar_t *restrict ws1, const wchar_t *restrict ws2)
{
    wchar_t *ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = wcpcpy(ws1, ws2);

    P101_TRACE_EXIT(env);
    return ret_val;
}

wchar_t *p101_wcpncpy(const struct p101_env *env, wchar_t *restrict ws1, const wchar_t *restrict ws2, size_t n)
{
    wchar_t *ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = wcpncpy(ws1, ws2, n);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_wcscasecmp(const struct p101_env *env, const wchar_t *ws1, const wchar_t *ws2)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = wcscasecmp(ws1, ws2);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_wcscoll_l(const struct p101_env *env, struct p101_error *err, const wchar_t *ws1, const wchar_t *ws2, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN(env, err, ret_val, 0);
    errno   = 0;
    ret_val = wcscoll_l(ws1, ws2, locale);

    if(errno != 0)
    {
        P101_ERROR_RAISE_ERRNO(err, errno);
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

wchar_t *p101_wcsdup(const struct p101_env *env, struct p101_error *err, const wchar_t *string)
{
    wchar_t *ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN(env, err, ret_val, NULL);
    errno   = 0;
    ret_val = wcsdup(string);

    if(ret_val == NULL)
    {
        P101_ERROR_RAISE_ERRNO(err, errno == 0 ? ENOMEM : errno);
    }
    else
    {
        P101_TRACK_ALLOC(env, ret_val, (p101_wcslen(env, string) + 1U) * sizeof(*ret_val));
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

int p101_wcsncasecmp(const struct p101_env *env, const wchar_t *ws1, const wchar_t *ws2, size_t n)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = wcsncasecmp(ws1, ws2, n);

    P101_TRACE_EXIT(env);
    return ret_val;
}

size_t p101_wcsnlen(const struct p101_env *env, const wchar_t *ws, size_t maxlen)
{
    size_t ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = wcsnlen(ws, maxlen);

    P101_TRACE_EXIT(env);
    return ret_val;
}

size_t p101_wcsnrtombs(const struct p101_env *env, struct p101_error *err, char *restrict dst, const wchar_t **restrict src, size_t nwc, size_t len, mbstate_t *restrict ps)
{
    int    actual_error;
    size_t ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN(env, err, ret_val, (size_t)-1);
    errno        = 0;
    ret_val      = wcsnrtombs(dst, src, nwc, len, ps);
    actual_error = errno;

    if(ret_val == (size_t)-1)
    {
        P101_ERROR_RAISE_ERRNO(err, actual_error == 0 ? EILSEQ : actual_error);
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

size_t p101_wcsxfrm_l(const struct p101_env *env, struct p101_error *err, wchar_t *restrict ws1, const wchar_t *restrict ws2, size_t n, locale_t locale)
{
    size_t ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN(env, err, ret_val, 0);
    errno   = 0;
    ret_val = wcsxfrm_l(ws1, ws2, n, locale);

    if(errno != 0)
    {
        P101_ERROR_RAISE_ERRNO(err, errno);
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

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

int p101_iswalnum_l(const struct p101_env *env, wint_t wc, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = iswalnum_l(wc, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_iswalpha_l(const struct p101_env *env, wint_t wc, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = iswalpha_l(wc, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_iswblank_l(const struct p101_env *env, wint_t wc, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = iswblank_l(wc, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_iswcntrl_l(const struct p101_env *env, wint_t wc, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = iswcntrl_l(wc, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_iswctype_l(const struct p101_env *env, wint_t wc, wctype_t charclass, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = iswctype_l(wc, charclass, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_iswdigit_l(const struct p101_env *env, wint_t wc, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = iswdigit_l(wc, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_iswgraph_l(const struct p101_env *env, wint_t wc, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = iswgraph_l(wc, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_iswlower_l(const struct p101_env *env, wint_t wc, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = iswlower_l(wc, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_iswprint_l(const struct p101_env *env, wint_t wc, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = iswprint_l(wc, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_iswpunct_l(const struct p101_env *env, wint_t wc, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = iswpunct_l(wc, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_iswspace_l(const struct p101_env *env, wint_t wc, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = iswspace_l(wc, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_iswupper_l(const struct p101_env *env, wint_t wc, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = iswupper_l(wc, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_iswxdigit_l(const struct p101_env *env, wint_t wc, locale_t locale)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = iswxdigit_l(wc, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

wint_t p101_towctrans_l(const struct p101_env *env, struct p101_error *err, wint_t wc, wctrans_t desc, locale_t locale)
{
    wint_t ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN(env, err, ret_val, WEOF);
    errno   = 0;
    ret_val = towctrans_l(wc, desc, locale);

    if(errno != 0)
    {
        P101_ERROR_RAISE_ERRNO(err, errno);
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

wint_t p101_towlower_l(const struct p101_env *env, wint_t wc, locale_t locale)
{
    wint_t ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = towlower_l(wc, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

wint_t p101_towupper_l(const struct p101_env *env, wint_t wc, locale_t locale)
{
    wint_t ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = towupper_l(wc, locale);

    P101_TRACE_EXIT(env);
    return ret_val;
}

wctrans_t p101_wctrans_l(const struct p101_env *env, struct p101_error *err, const char *charclass, locale_t locale)
{
    wctrans_t ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN(env, err, ret_val, 0);
    errno   = 0;
    ret_val = wctrans_l(charclass, locale);

    if(!ret_val)
    {
        P101_ERROR_RAISE_ERRNO(err, errno == 0 ? EINVAL : errno);
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

wctype_t p101_wctype_l(const struct p101_env *env, struct p101_error *err, const char *property, locale_t locale)
{
    wctype_t ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN(env, err, ret_val, 0);
    errno   = 0;
    ret_val = wctype_l(property, locale);

    if(!ret_val)
    {
        P101_ERROR_RAISE_ERRNO(err, errno == 0 ? EINVAL : errno);
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

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

static const char *wordexp_error_message(int err_code);

static const char *wordexp_error_message(int err_code)
{
    const char *msg;

    switch(err_code)
    {
        case WRDE_BADCHAR:
            msg = "Illegal character in word expansion";
            break;
        case WRDE_BADVAL:
            msg = "Undefined shell variable in word expansion";
            break;
        case WRDE_CMDSUB:
            msg = "Command substitution requested in word expansion";
            break;
        case WRDE_NOSPACE:
            msg = "Out of memory during word expansion";
            break;
        case WRDE_SYNTAX:
            msg = "Shell syntax error in word expansion";
            break;
        default:
            msg = "Unknown word expansion error";
            break;
    }

    return msg;
}

int p101_wordexp(const struct p101_env *env, struct p101_error *err, const char *restrict words, wordexp_t *restrict pwordexp, int flags)
{
    int ret_val;

    P101_TRACE(env);
    P101_WRAPPER_FAULT_RETURN_SYSTEM_CODE(env, err, ret_val);
    errno   = 0;
    ret_val = wordexp(words, pwordexp, flags);

    if(ret_val != 0)
    {
        P101_ERROR_RAISE_SYSTEM(err, wordexp_error_message(ret_val), ret_val);
    }
    else
    {
        P101_TRACK_POINTER_RESOURCE_ACQUIRE(env, "wordexp-result", pwordexp, 0U, NULL);
    }

    P101_WRAPPER_DONE(env);
    return ret_val;
}

void p101_wordfree(const struct p101_env *env, wordexp_t *pwordexp)
{
    char resource_id[P101_ENV_POINTER_RESOURCE_ID_SIZE];

    P101_TRACE(env);
    p101_env_pointer_resource_id(resource_id, sizeof(resource_id), pwordexp);
    errno = 0;
    wordfree(pwordexp);
    P101_TRACK_RESOURCE_RELEASE(env, "wordexp-result", resource_id, NULL);
    P101_TRACE_EXIT(env);
}

#include <stdlib.h>

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

void *p101_memccpy(const struct p101_env *env, void *restrict s1, const void *restrict s2, int c, size_t n)
{
    void *ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = memccpy(s1, s2, c, n);

    P101_TRACE_EXIT(env);
    return ret_val;
}

int p101_ffs(const struct p101_env *env, int i)
{
    int ret_val;

    P101_TRACE(env);
    errno   = 0;
    ret_val = ffs(i);

    P101_TRACE_EXIT(env);
    return ret_val;
}

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

#ifdef __has_include
    #if __has_include("bsd/stdlib.h")
        #include <bsd/stdlib.h>
    #endif
#endif
int p101_rpmatch(const struct p101_env *env, const char *response)
{
    int ret_val;

    P101_TRACE(env);
    ret_val = rpmatch(response);

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
