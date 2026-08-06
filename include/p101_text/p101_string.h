#ifndef LIBP101_TEXT_P101_STRING_H
#define LIBP101_TEXT_P101_STRING_H

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

#ifndef LIBP101_TEXT_SHARED_DECLARATIONS
    #define LIBP101_TEXT_SHARED_DECLARATIONS
    #include <ctype.h>
    #include <inttypes.h>
    #include <locale.h>
    #include <p101_env/env.h>
    #include <p101_error/attributes.h>
    #include <regex.h>
    #include <stddef.h>
    #include <string.h>
    #include <sys/types.h>
    #include <wchar.h>
    #include <wctype.h>
    #include <wordexp.h>
    #if defined(__APPLE__) || defined(__FreeBSD__)
        #include <xlocale.h>
    #endif
#endif    // LIBP101_TEXT_SHARED_DECLARATIONS

#ifdef __cplusplus
extern "C"
{
#endif

    void  *p101_memccpy(const struct p101_env *env, void *restrict s1, const void *restrict s2, int c, size_t n);
    char  *p101_stpcpy(const struct p101_env *env, char *restrict s1, const char *restrict s2);
    char  *p101_stpncpy(const struct p101_env *env, char *restrict s1, const char *restrict s2, size_t n);
    char  *p101_strcasestr(const struct p101_env *env, const char *big, const char *little);
    char  *p101_strchrnul(const struct p101_env *env, const char *s, int c);
    int    p101_strcoll_l(const struct p101_env *env, struct p101_error *err, const char *s1, const char *s2, locale_t locale);
    char  *p101_strdup(const struct p101_env *env, struct p101_error *err, const char *s) P101_ATTR_MALLOC P101_ATTR_WARN_UNUSED_RESULT;
    int    p101_strerror_r(const struct p101_env *env, struct p101_error *err, int errnum, char *strerrbuf, size_t buflen);
    char  *p101_strndup(const struct p101_env *env, struct p101_error *err, const char *s, size_t size) P101_ATTR_MALLOC P101_ATTR_ALLOC_SIZE(4) P101_ATTR_WARN_UNUSED_RESULT;
    size_t p101_strnlen(const struct p101_env *env, const char *s, size_t maxlen);
    char  *p101_strsep(const struct p101_env *env, char **stringp, const char *delim);
    char  *p101_strsignal(const struct p101_env *env, struct p101_error *err, int signum);
    char  *p101_strtok_r(const struct p101_env *env, char *restrict s, const char *restrict sep, char **restrict state);
    size_t p101_strxfrm_l(const struct p101_env *env, struct p101_error *err, char *restrict s1, const char *restrict s2, size_t n, locale_t locale);

#ifdef __cplusplus
}
#endif

#endif    // LIBP101_TEXT_P101_STRING_H
