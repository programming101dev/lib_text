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
