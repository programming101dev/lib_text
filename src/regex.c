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

#include <p101_c/p101_stdlib.h>

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
