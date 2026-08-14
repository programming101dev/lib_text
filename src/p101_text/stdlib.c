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

#include "p101_text/p101_stdlib.h"
#include <p101_env/wrapper.h>
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
