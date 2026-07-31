#ifndef LIBP101_TEXT_TEXT_H
#define LIBP101_TEXT_TEXT_H

/*
 * Copyright 2026 D'Arcy Smith.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 */

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

#ifdef __cplusplus
extern "C"
{
#endif

    long      p101_a64l(const struct p101_env *env, const char *s);
    int       p101_ffs(const struct p101_env *env, int i);
    int       p101_isalnum_l(const struct p101_env *env, int c, locale_t locale);
    int       p101_isalpha_l(const struct p101_env *env, int c, locale_t locale);
    int       p101_isblank_l(const struct p101_env *env, int c, locale_t locale);
    int       p101_iscntrl_l(const struct p101_env *env, int c, locale_t locale);
    int       p101_isdigit_l(const struct p101_env *env, int c, locale_t locale);
    int       p101_isgraph_l(const struct p101_env *env, int c, locale_t locale);
    int       p101_islower_l(const struct p101_env *env, int c, locale_t locale);
    int       p101_isprint_l(const struct p101_env *env, int c, locale_t locale);
    int       p101_ispunct_l(const struct p101_env *env, int c, locale_t locale);
    int       p101_isspace_l(const struct p101_env *env, int c, locale_t locale);
    int       p101_isupper_l(const struct p101_env *env, int c, locale_t locale);
    int       p101_iswalnum_l(const struct p101_env *env, wint_t wc, locale_t locale);
    int       p101_iswalpha_l(const struct p101_env *env, wint_t wc, locale_t locale);
    int       p101_iswblank_l(const struct p101_env *env, wint_t wc, locale_t locale);
    int       p101_iswcntrl_l(const struct p101_env *env, wint_t wc, locale_t locale);
    int       p101_iswctype_l(const struct p101_env *env, wint_t wc, wctype_t charclass, locale_t locale);
    int       p101_iswdigit_l(const struct p101_env *env, wint_t wc, locale_t locale);
    int       p101_iswgraph_l(const struct p101_env *env, wint_t wc, locale_t locale);
    int       p101_iswlower_l(const struct p101_env *env, wint_t wc, locale_t locale);
    int       p101_iswprint_l(const struct p101_env *env, wint_t wc, locale_t locale);
    int       p101_iswpunct_l(const struct p101_env *env, wint_t wc, locale_t locale);
    int       p101_iswspace_l(const struct p101_env *env, wint_t wc, locale_t locale);
    int       p101_iswupper_l(const struct p101_env *env, wint_t wc, locale_t locale);
    int       p101_iswxdigit_l(const struct p101_env *env, wint_t wc, locale_t locale);
    int       p101_isxdigit_l(const struct p101_env *env, int c, locale_t locale);
    char     *p101_l64a(const struct p101_env *env, long value);
    size_t    p101_mbsnrtowcs(const struct p101_env *env, struct p101_error *err, wchar_t *restrict dst, const char **restrict src, size_t nmc, size_t len, mbstate_t *restrict ps);
    void     *p101_memccpy(const struct p101_env *env, void *restrict s1, const void *restrict s2, int c, size_t n);
    FILE     *p101_open_wmemstream(const struct p101_env *env, struct p101_error *err, wchar_t **bufp, size_t *sizep) P101_ATTR_WARN_UNUSED_RESULT;
    int       p101_regcomp(const struct p101_env *env, struct p101_error *err, regex_t *restrict preg, const char *restrict pattern, int cflags);
    size_t    p101_regerror(const struct p101_env *env, int errcode, const regex_t *restrict preg, char *restrict errbuf, size_t errbuf_size);
    int       p101_regexec(const struct p101_env *env, const regex_t *restrict preg, const char *restrict string, size_t nmatch, regmatch_t pmatch[restrict], int eflags);
    void      p101_regfree(const struct p101_env *env, regex_t *preg);
    int       p101_rpmatch(const struct p101_env *env, const char *response);
    char     *p101_stpcpy(const struct p101_env *env, char *restrict s1, const char *restrict s2);
    char     *p101_stpncpy(const struct p101_env *env, char *restrict s1, const char *restrict s2, size_t n);
    int       p101_strcasecmp(const struct p101_env *env, const char *s1, const char *s2);
    int       p101_strcasecmp_l(const struct p101_env *env, const char *s1, const char *s2, locale_t locale);
    char     *p101_strcasestr(const struct p101_env *env, const char *big, const char *little);
    char     *p101_strchrnul(const struct p101_env *env, const char *s, int c);
    int       p101_strcoll_l(const struct p101_env *env, struct p101_error *err, const char *s1, const char *s2, locale_t locale);
    char     *p101_strdup(const struct p101_env *env, struct p101_error *err, const char *s) P101_ATTR_MALLOC P101_ATTR_WARN_UNUSED_RESULT;
    int       p101_strerror_r(const struct p101_env *env, struct p101_error *err, int errnum, char *strerrbuf, size_t buflen);
    int       p101_strncasecmp(const struct p101_env *env, const char *s1, const char *s2, size_t n);
    int       p101_strncasecmp_l(const struct p101_env *env, const char *s1, const char *s2, size_t n, locale_t locale);
    char     *p101_strndup(const struct p101_env *env, struct p101_error *err, const char *s, size_t size) P101_ATTR_MALLOC P101_ATTR_ALLOC_SIZE(4) P101_ATTR_WARN_UNUSED_RESULT;
    size_t    p101_strnlen(const struct p101_env *env, const char *s, size_t maxlen);
    char     *p101_strsep(const struct p101_env *env, char **stringp, const char *delim);
    char     *p101_strsignal(const struct p101_env *env, int signum);
    char     *p101_strtok_r(const struct p101_env *env, char *restrict s, const char *restrict sep, char **restrict state);
    size_t    p101_strxfrm_l(const struct p101_env *env, struct p101_error *err, char *restrict s1, const char *restrict s2, size_t n, locale_t locale);
    void      p101_swab(const struct p101_env *env, const void *restrict src, void *restrict dest, ssize_t nbytes);
    int       p101_tolower_l(const struct p101_env *env, int c, locale_t locale);
    int       p101_toupper_l(const struct p101_env *env, int c, locale_t locale);
    wint_t    p101_towctrans_l(const struct p101_env *env, wint_t wc, wctrans_t desc, locale_t locale);
    wint_t    p101_towlower_l(const struct p101_env *env, wint_t wc, locale_t locale);
    wint_t    p101_towupper_l(const struct p101_env *env, wint_t wc, locale_t locale);
    wchar_t  *p101_wcpcpy(const struct p101_env *env, wchar_t *restrict ws1, const wchar_t *restrict ws2);
    wchar_t  *p101_wcpncpy(const struct p101_env *env, wchar_t *restrict ws1, const wchar_t *restrict ws2, size_t n);
    int       p101_wcscasecmp(const struct p101_env *env, const wchar_t *ws1, const wchar_t *ws2);
    int       p101_wcscoll_l(const struct p101_env *env, struct p101_error *err, const wchar_t *ws1, const wchar_t *ws2, locale_t locale);
    wchar_t  *p101_wcsdup(const struct p101_env *env, struct p101_error *err, const wchar_t *string) P101_ATTR_MALLOC P101_ATTR_WARN_UNUSED_RESULT;
    int       p101_wcsncasecmp(const struct p101_env *env, const wchar_t *ws1, const wchar_t *ws2, size_t n);
    size_t    p101_wcsnlen(const struct p101_env *env, const wchar_t *ws, size_t maxlen);
    size_t    p101_wcsnrtombs(const struct p101_env *env, struct p101_error *err, char *restrict dst, const wchar_t **restrict src, size_t nwc, size_t len, mbstate_t *restrict ps);
    int       p101_wcswidth(const struct p101_env *env, const wchar_t *pwcs, size_t n);
    size_t    p101_wcsxfrm_l(const struct p101_env *env, struct p101_error *err, wchar_t *restrict ws1, const wchar_t *restrict ws2, size_t n, locale_t locale);
    wctrans_t p101_wctrans_l(const struct p101_env *env, struct p101_error *err, const char *charclass, locale_t locale);
    wctype_t  p101_wctype_l(const struct p101_env *env, struct p101_error *err, const char *property, locale_t locale);
    int       p101_wcwidth(const struct p101_env *env, wchar_t wc);
    int       p101_wordexp(const struct p101_env *env, struct p101_error *err, const char *restrict words, wordexp_t *restrict pwordexp, int flags);
    void      p101_wordfree(const struct p101_env *env, wordexp_t *pwordexp);

#ifdef __cplusplus
}
#endif

#endif    // LIBP101_TEXT_TEXT_H
