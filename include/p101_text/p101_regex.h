#ifndef LIBP101_TEXT_P101_REGEX_H
#define LIBP101_TEXT_P101_REGEX_H

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

    int    p101_regcomp(const struct p101_env *env, struct p101_error *err, regex_t *restrict preg, const char *restrict pattern, int cflags);
    size_t p101_regerror(const struct p101_env *env, int errcode, const regex_t *restrict preg, char *restrict errbuf, size_t errbuf_size);
    int    p101_regexec(const struct p101_env *env, struct p101_error *err, const regex_t *restrict preg, const char *restrict string, size_t nmatch, regmatch_t pmatch[restrict], int eflags);
    void   p101_regfree(const struct p101_env *env, regex_t *preg);

#ifdef __cplusplus
}
#endif

#endif    // LIBP101_TEXT_P101_REGEX_H
