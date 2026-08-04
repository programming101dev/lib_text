#include <arpa/inet.h>
#include <errno.h>
#include <fmtmsg.h>
#include <fnmatch.h>
#include <math.h>
#include <p101_env/env.h>
#include <p101_error/error.h>
#include <p101_text/text.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int    failures;
static size_t fault_resource_events;
static FILE  *outcome_stream;

#define P101_TEST_ERRNO_SENTINEL 0x5A5A

#ifdef __linux__
    #define P101_TEST_PLATFORM "linux"
#elif defined(__APPLE__)
    #define P101_TEST_PLATFORM "macos"
#elif defined(__FreeBSD__)
    #define P101_TEST_PLATFORM "freebsd"
#else
    #define P101_TEST_PLATFORM "posix"
#endif

#define EXPECT(condition)                                                                                                                                                                                                                                          \
    do                                                                                                                                                                                                                                                             \
    {                                                                                                                                                                                                                                                              \
        if(!(condition))                                                                                                                                                                                                                                           \
        {                                                                                                                                                                                                                                                          \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition);                                                                                                                                                                                   \
            failures++;                                                                                                                                                                                                                                            \
        }                                                                                                                                                                                                                                                          \
    } while(0)

struct fault_state
{
    int checks;
    int code;
};

static void write_outcome(const char *wrapper, const char *domain, const char *symbol, int code, int passed)
{
    int written;

    if(outcome_stream == NULL)
    {
        return;
    }
    written = fprintf(outcome_stream, "P101WRAPPER\t1\tFAULT\t%s\tlib_text\t%s\t%s\t%s\t%d\t%s\n", P101_TEST_PLATFORM, wrapper, domain, symbol, code, passed ? "PASS" : "FAIL");
    if(written < 0 || fflush(outcome_stream) != 0)
    {
        fprintf(stderr, "FAIL: cannot write wrapper outcome receipt\n");
        failures++;
    }
}

static int fail_next_call(const struct p101_env *env, const char *call_name, void *user_data)
{
    struct fault_state *state;

    (void)env;
    (void)call_name;
    state = user_data;
    state->checks++;
    return state->code;
}

static void count_fd_event(const struct p101_env *env, p101_env_fd_event event, int fd, const char *file_name, const char *function_name, int line_number, void *user_data)
{
    (void)env;
    (void)event;
    (void)fd;
    (void)file_name;
    (void)function_name;
    (void)line_number;
    (void)user_data;
    fault_resource_events++;
}

static void count_alloc_event(const struct p101_env *env, p101_env_alloc_event event, const void *ptr, const void *new_ptr, size_t size, const char *file_name, const char *function_name, int line_number, void *user_data)
{
    (void)env;
    (void)event;
    (void)ptr;
    (void)new_ptr;
    (void)size;
    (void)file_name;
    (void)function_name;
    (void)line_number;
    (void)user_data;
    fault_resource_events++;
}

static void count_resource_event(const struct p101_env *env, p101_env_resource_kind event, const char *resource_class, const char *resource_id, const char *related_id, size_t size, const char *metadata, const char *file_name, const char *function_name,
                                 int line_number, void *user_data)
{
    (void)env;
    (void)event;
    (void)resource_class;
    (void)resource_id;
    (void)related_id;
    (void)size;
    (void)metadata;
    (void)file_name;
    (void)function_name;
    (void)line_number;
    (void)user_data;
    fault_resource_events++;
}

/* P101_TEST_CASE(p101_mbsnrtowcs) */
static void test_p101_mbsnrtowcs(struct p101_env *env, struct p101_error *err)
{
    wchar_t       argument_2[4];
    unsigned char argument_2_before[sizeof(argument_2)];
    memset(argument_2, 0xA5, sizeof(argument_2));
    memcpy(argument_2_before, argument_2, sizeof(argument_2));
    mbstate_t     argument_6[4];
    unsigned char argument_6_before[sizeof(argument_6)];
    memset(argument_6, 0xA5, sizeof(argument_6));
    memcpy(argument_6_before, argument_6, sizeof(argument_6));
#ifdef __linux__
    static const int         errors[]      = {EILSEQ, EINVAL};
    static const char *const error_names[] = {"EILSEQ", "EINVAL"};
#elif defined(__APPLE__)
    static const int         errors[]      = {EILSEQ, EINVAL};
    static const char *const error_names[] = {"EILSEQ", "EINVAL"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {EILSEQ, EINVAL};
    static const char *const error_names[] = {"EILSEQ", "EINVAL"};
#else
    static const int         errors[]      = {EILSEQ, EINVAL};
    static const char *const error_names[] = {"EILSEQ", "EINVAL"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        size_t result = p101_mbsnrtowcs(env, err, argument_2, NULL, 0, 0, argument_6);
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_errno(err, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == ((size_t)-1));
        EXPECT(memcmp(argument_2, argument_2_before, sizeof(argument_2)) == 0);
        EXPECT(memcmp(argument_6, argument_6_before, sizeof(argument_6)) == 0);
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_mbsnrtowcs", "errno", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_open_wmemstream) */
static void test_p101_open_wmemstream(struct p101_env *env, struct p101_error *err)
{
    wchar_t      *argument_2[4];
    unsigned char argument_2_before[sizeof(argument_2)];
    memset(argument_2, 0xA5, sizeof(argument_2));
    memcpy(argument_2_before, argument_2, sizeof(argument_2));
    size_t        argument_3[4];
    unsigned char argument_3_before[sizeof(argument_3)];
    memset(argument_3, 0xA5, sizeof(argument_3));
    memcpy(argument_3_before, argument_3, sizeof(argument_3));
#ifdef __linux__
    static const int         errors[]      = {EINVAL, EMFILE, ENOMEM};
    static const char *const error_names[] = {"EINVAL", "EMFILE", "ENOMEM"};
#elif defined(__APPLE__)
    static const int         errors[]      = {EINVAL, ENOMEM};
    static const char *const error_names[] = {"EINVAL", "ENOMEM"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {EINVAL, ENOMEM};
    static const char *const error_names[] = {"EINVAL", "ENOMEM"};
#else
    static const int         errors[]      = {EINVAL, EMFILE, ENOMEM};
    static const char *const error_names[] = {"EINVAL", "EMFILE", "ENOMEM"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        FILE *result = p101_open_wmemstream(env, err, argument_2, argument_3);
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_errno(err, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == (NULL));
        EXPECT(memcmp(argument_2, argument_2_before, sizeof(argument_2)) == 0);
        EXPECT(memcmp(argument_3, argument_3_before, sizeof(argument_3)) == 0);
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_open_wmemstream", "errno", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_regcomp) */
static void test_p101_regcomp(struct p101_env *env, struct p101_error *err)
{
    regex_t       argument_2[4];
    unsigned char argument_2_before[sizeof(argument_2)];
    memset(argument_2, 0xA5, sizeof(argument_2));
    memcpy(argument_2_before, argument_2, sizeof(argument_2));
#ifdef __linux__
    static const int         errors[]      = {REG_BADBR, REG_BADPAT, REG_BADRPT, REG_EBRACE, REG_EBRACK, REG_ECOLLATE, REG_ECTYPE, REG_EEND, REG_EESCAPE, REG_EPAREN, REG_ERANGE, REG_ESIZE, REG_ESPACE, REG_ESUBREG};
    static const char *const error_names[] = {"REG_BADBR", "REG_BADPAT", "REG_BADRPT", "REG_EBRACE", "REG_EBRACK", "REG_ECOLLATE", "REG_ECTYPE", "REG_EEND", "REG_EESCAPE", "REG_EPAREN", "REG_ERANGE", "REG_ESIZE", "REG_ESPACE", "REG_ESUBREG"};
#elif defined(__APPLE__)
    static const int         errors[] = {REG_ASSERT, REG_BADBR, REG_BADMAX, REG_BADPAT, REG_BADRPT, REG_EBRACE, REG_EBRACK, REG_ECOLLATE, REG_ECTYPE, REG_EESCAPE, REG_EMPTY, REG_EPAREN, REG_ERANGE, REG_ESPACE, REG_ESUBREG, REG_ILLSEQ, REG_INVARG};
    static const char *const error_names[] =
        {"REG_ASSERT", "REG_BADBR", "REG_BADMAX", "REG_BADPAT", "REG_BADRPT", "REG_EBRACE", "REG_EBRACK", "REG_ECOLLATE", "REG_ECTYPE", "REG_EESCAPE", "REG_EMPTY", "REG_EPAREN", "REG_ERANGE", "REG_ESPACE", "REG_ESUBREG", "REG_ILLSEQ", "REG_INVARG"};
#elif defined(__FreeBSD__)
    static const int         errors[] = {REG_ASSERT, REG_BADBR, REG_BADPAT, REG_BADRPT, REG_EBRACE, REG_EBRACK, REG_ECOLLATE, REG_ECTYPE, REG_EESCAPE, REG_EMPTY, REG_EPAREN, REG_ERANGE, REG_ESPACE, REG_ESUBREG, REG_ILLSEQ, REG_INVARG};
    static const char *const error_names[] =
        {"REG_ASSERT", "REG_BADBR", "REG_BADPAT", "REG_BADRPT", "REG_EBRACE", "REG_EBRACK", "REG_ECOLLATE", "REG_ECTYPE", "REG_EESCAPE", "REG_EMPTY", "REG_EPAREN", "REG_ERANGE", "REG_ESPACE", "REG_ESUBREG", "REG_ILLSEQ", "REG_INVARG"};
#else
    static const int         errors[]      = {REG_BADBR, REG_BADPAT, REG_BADRPT, REG_EBRACE, REG_EBRACK, REG_ECOLLATE, REG_ECTYPE, REG_EESCAPE, REG_EPAREN, REG_ERANGE, REG_ESPACE, REG_ESUBREG};
    static const char *const error_names[] = {"REG_BADBR", "REG_BADPAT", "REG_BADRPT", "REG_EBRACE", "REG_EBRACK", "REG_ECOLLATE", "REG_ECTYPE", "REG_EESCAPE", "REG_EPAREN", "REG_ERANGE", "REG_ESPACE", "REG_ESUBREG"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        int result = p101_regcomp(env, err, argument_2, NULL, 0);
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_error(err, P101_ERROR_SYSTEM, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == state.code);
        EXPECT(memcmp(argument_2, argument_2_before, sizeof(argument_2)) == 0);
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_regcomp", "system", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_regexec) */
static void test_p101_regexec(struct p101_env *env, struct p101_error *err)
{
    regmatch_t    argument_5[4];
    unsigned char argument_5_before[sizeof(argument_5)];
    memset(argument_5, 0xA5, sizeof(argument_5));
    memcpy(argument_5_before, argument_5, sizeof(argument_5));
#ifdef __linux__
    static const int         errors[]      = {EIO};
    static const char *const error_names[] = {"EIO"};
#elif defined(__APPLE__)
    static const int         errors[] = {REG_ASSERT, REG_BADBR, REG_BADMAX, REG_BADPAT, REG_BADRPT, REG_EBRACE, REG_EBRACK, REG_ECOLLATE, REG_ECTYPE, REG_EESCAPE, REG_EMPTY, REG_EPAREN, REG_ERANGE, REG_ESPACE, REG_ESUBREG, REG_ILLSEQ, REG_INVARG};
    static const char *const error_names[] =
        {"REG_ASSERT", "REG_BADBR", "REG_BADMAX", "REG_BADPAT", "REG_BADRPT", "REG_EBRACE", "REG_EBRACK", "REG_ECOLLATE", "REG_ECTYPE", "REG_EESCAPE", "REG_EMPTY", "REG_EPAREN", "REG_ERANGE", "REG_ESPACE", "REG_ESUBREG", "REG_ILLSEQ", "REG_INVARG"};
#elif defined(__FreeBSD__)
    static const int         errors[] = {REG_ASSERT, REG_BADBR, REG_BADPAT, REG_BADRPT, REG_EBRACE, REG_EBRACK, REG_ECOLLATE, REG_ECTYPE, REG_EESCAPE, REG_EMPTY, REG_EPAREN, REG_ERANGE, REG_ESPACE, REG_ESUBREG, REG_ILLSEQ, REG_INVARG};
    static const char *const error_names[] =
        {"REG_ASSERT", "REG_BADBR", "REG_BADPAT", "REG_BADRPT", "REG_EBRACE", "REG_EBRACK", "REG_ECOLLATE", "REG_ECTYPE", "REG_EESCAPE", "REG_EMPTY", "REG_EPAREN", "REG_ERANGE", "REG_ESPACE", "REG_ESUBREG", "REG_ILLSEQ", "REG_INVARG"};
#else
    static const int         errors[]      = {EIO};
    static const char *const error_names[] = {"EIO"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        int result = p101_regexec(env, err, NULL, NULL, 0, argument_5, 0);
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_error(err, P101_ERROR_SYSTEM, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == state.code);
        EXPECT(memcmp(argument_5, argument_5_before, sizeof(argument_5)) == 0);
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_regexec", "system", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_strcoll_l) */
static void test_p101_strcoll_l(struct p101_env *env, struct p101_error *err)
{
#ifdef __linux__
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#elif defined(__APPLE__)
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#else
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        int result = p101_strcoll_l(env, err, NULL, NULL, (locale_t){0});
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_errno(err, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == (0));
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_strcoll_l", "errno", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_strdup) */
static void test_p101_strdup(struct p101_env *env, struct p101_error *err)
{
#ifdef __linux__
    static const int         errors[]      = {ENOMEM};
    static const char *const error_names[] = {"ENOMEM"};
#elif defined(__APPLE__)
    static const int         errors[]      = {ENOMEM};
    static const char *const error_names[] = {"ENOMEM"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {ENOMEM};
    static const char *const error_names[] = {"ENOMEM"};
#else
    static const int         errors[]      = {ENOMEM};
    static const char *const error_names[] = {"ENOMEM"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        char *result = p101_strdup(env, err, NULL);
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_errno(err, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == (NULL));
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_strdup", "errno", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_strerror_r) */
static void test_p101_strerror_r(struct p101_env *env, struct p101_error *err)
{
    char          argument_3[4];
    unsigned char argument_3_before[sizeof(argument_3)];
    memset(argument_3, 0xA5, sizeof(argument_3));
    memcpy(argument_3_before, argument_3, sizeof(argument_3));
#ifdef __linux__
    static const int         errors[]      = {EINVAL, ERANGE};
    static const char *const error_names[] = {"EINVAL", "ERANGE"};
#elif defined(__APPLE__)
    static const int         errors[]      = {EINVAL, ERANGE};
    static const char *const error_names[] = {"EINVAL", "ERANGE"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {EINVAL, ERANGE};
    static const char *const error_names[] = {"EINVAL", "ERANGE"};
#else
    static const int         errors[]      = {EINVAL, ERANGE};
    static const char *const error_names[] = {"EINVAL", "ERANGE"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        int result = p101_strerror_r(env, err, 0, argument_3, 0);
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_errno(err, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == (-1));
        EXPECT(memcmp(argument_3, argument_3_before, sizeof(argument_3)) == 0);
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_strerror_r", "errno", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_strndup) */
static void test_p101_strndup(struct p101_env *env, struct p101_error *err)
{
#ifdef __linux__
    static const int         errors[]      = {ENOMEM};
    static const char *const error_names[] = {"ENOMEM"};
#elif defined(__APPLE__)
    static const int         errors[]      = {ENOMEM};
    static const char *const error_names[] = {"ENOMEM"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {ENOMEM};
    static const char *const error_names[] = {"ENOMEM"};
#else
    static const int         errors[]      = {ENOMEM};
    static const char *const error_names[] = {"ENOMEM"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        char *result = p101_strndup(env, err, NULL, 0);
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_errno(err, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == (NULL));
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_strndup", "errno", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_strsignal) */
static void test_p101_strsignal(struct p101_env *env, struct p101_error *err)
{
#ifdef __linux__
    static const int         errors[]      = {EIO};
    static const char *const error_names[] = {"EIO"};
#elif defined(__APPLE__)
    static const int         errors[]      = {EINVAL, ENOMEM};
    static const char *const error_names[] = {"EINVAL", "ENOMEM"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {EIO};
    static const char *const error_names[] = {"EIO"};
#else
    static const int         errors[]      = {EIO};
    static const char *const error_names[] = {"EIO"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        char *result = p101_strsignal(env, err, 0);
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_errno(err, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == (NULL));
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_strsignal", "errno", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_strxfrm_l) */
static void test_p101_strxfrm_l(struct p101_env *env, struct p101_error *err)
{
    char          argument_2[4];
    unsigned char argument_2_before[sizeof(argument_2)];
    memset(argument_2, 0xA5, sizeof(argument_2));
    memcpy(argument_2_before, argument_2, sizeof(argument_2));
#ifdef __linux__
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#elif defined(__APPLE__)
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#else
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        size_t result = p101_strxfrm_l(env, err, argument_2, NULL, 0, (locale_t){0});
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_errno(err, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == (0));
        EXPECT(memcmp(argument_2, argument_2_before, sizeof(argument_2)) == 0);
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_strxfrm_l", "errno", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_towctrans_l) */
static void test_p101_towctrans_l(struct p101_env *env, struct p101_error *err)
{
#ifdef __linux__
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#elif defined(__APPLE__)
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#else
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        wint_t result = p101_towctrans_l(env, err, 0, 0, (locale_t){0});
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_errno(err, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == (WEOF));
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_towctrans_l", "errno", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_wcscoll_l) */
static void test_p101_wcscoll_l(struct p101_env *env, struct p101_error *err)
{
#ifdef __linux__
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#elif defined(__APPLE__)
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#else
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        int result = p101_wcscoll_l(env, err, NULL, NULL, (locale_t){0});
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_errno(err, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == (0));
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_wcscoll_l", "errno", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_wcsdup) */
static void test_p101_wcsdup(struct p101_env *env, struct p101_error *err)
{
#ifdef __linux__
    static const int         errors[]      = {ENOMEM};
    static const char *const error_names[] = {"ENOMEM"};
#elif defined(__APPLE__)
    static const int         errors[]      = {ENOMEM};
    static const char *const error_names[] = {"ENOMEM"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {ENOMEM};
    static const char *const error_names[] = {"ENOMEM"};
#else
    static const int         errors[]      = {ENOMEM};
    static const char *const error_names[] = {"ENOMEM"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        wchar_t *result = p101_wcsdup(env, err, NULL);
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_errno(err, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == (NULL));
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_wcsdup", "errno", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_wcsnrtombs) */
static void test_p101_wcsnrtombs(struct p101_env *env, struct p101_error *err)
{
    char          argument_2[4];
    unsigned char argument_2_before[sizeof(argument_2)];
    memset(argument_2, 0xA5, sizeof(argument_2));
    memcpy(argument_2_before, argument_2, sizeof(argument_2));
    mbstate_t     argument_6[4];
    unsigned char argument_6_before[sizeof(argument_6)];
    memset(argument_6, 0xA5, sizeof(argument_6));
    memcpy(argument_6_before, argument_6, sizeof(argument_6));
#ifdef __linux__
    static const int         errors[]      = {EILSEQ, EINVAL};
    static const char *const error_names[] = {"EILSEQ", "EINVAL"};
#elif defined(__APPLE__)
    static const int         errors[]      = {EILSEQ, EINVAL};
    static const char *const error_names[] = {"EILSEQ", "EINVAL"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {EILSEQ, EINVAL};
    static const char *const error_names[] = {"EILSEQ", "EINVAL"};
#else
    static const int         errors[]      = {EILSEQ, EINVAL};
    static const char *const error_names[] = {"EILSEQ", "EINVAL"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        size_t result = p101_wcsnrtombs(env, err, argument_2, NULL, 0, 0, argument_6);
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_errno(err, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == ((size_t)-1));
        EXPECT(memcmp(argument_2, argument_2_before, sizeof(argument_2)) == 0);
        EXPECT(memcmp(argument_6, argument_6_before, sizeof(argument_6)) == 0);
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_wcsnrtombs", "errno", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_wcsxfrm_l) */
static void test_p101_wcsxfrm_l(struct p101_env *env, struct p101_error *err)
{
    wchar_t       argument_2[4];
    unsigned char argument_2_before[sizeof(argument_2)];
    memset(argument_2, 0xA5, sizeof(argument_2));
    memcpy(argument_2_before, argument_2, sizeof(argument_2));
#ifdef __linux__
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#elif defined(__APPLE__)
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#else
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        size_t result = p101_wcsxfrm_l(env, err, argument_2, NULL, 0, (locale_t){0});
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_errno(err, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == (0));
        EXPECT(memcmp(argument_2, argument_2_before, sizeof(argument_2)) == 0);
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_wcsxfrm_l", "errno", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_wctrans_l) */
static void test_p101_wctrans_l(struct p101_env *env, struct p101_error *err)
{
#ifdef __linux__
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#elif defined(__APPLE__)
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#else
    static const int         errors[]      = {EINVAL};
    static const char *const error_names[] = {"EINVAL"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        wctrans_t result = p101_wctrans_l(env, err, NULL, (locale_t){0});
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_errno(err, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == (0));
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_wctrans_l", "errno", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_wctype_l) */
static void test_p101_wctype_l(struct p101_env *env, struct p101_error *err)
{
#ifdef __linux__
    static const int         errors[]      = {EIO};
    static const char *const error_names[] = {"EIO"};
#elif defined(__APPLE__)
    static const int         errors[]      = {EIO};
    static const char *const error_names[] = {"EIO"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {EIO};
    static const char *const error_names[] = {"EIO"};
#else
    static const int         errors[]      = {EIO};
    static const char *const error_names[] = {"EIO"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        wctype_t result = p101_wctype_l(env, err, NULL, (locale_t){0});
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_errno(err, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == (0));
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_wctype_l", "errno", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

/* P101_TEST_CASE(p101_wordexp) */
static void test_p101_wordexp(struct p101_env *env, struct p101_error *err)
{
    wordexp_t     argument_3[4];
    unsigned char argument_3_before[sizeof(argument_3)];
    memset(argument_3, 0xA5, sizeof(argument_3));
    memcpy(argument_3_before, argument_3, sizeof(argument_3));
#ifdef __linux__
    static const int         errors[]      = {WRDE_BADCHAR, WRDE_BADVAL, WRDE_CMDSUB, WRDE_NOSPACE, WRDE_SYNTAX};
    static const char *const error_names[] = {"WRDE_BADCHAR", "WRDE_BADVAL", "WRDE_CMDSUB", "WRDE_NOSPACE", "WRDE_SYNTAX"};
#elif defined(__APPLE__)
    static const int         errors[]      = {WRDE_BADCHAR, WRDE_BADVAL, WRDE_CMDSUB, WRDE_NOSPACE, WRDE_SYNTAX};
    static const char *const error_names[] = {"WRDE_BADCHAR", "WRDE_BADVAL", "WRDE_CMDSUB", "WRDE_NOSPACE", "WRDE_SYNTAX"};
#elif defined(__FreeBSD__)
    static const int         errors[]      = {WRDE_BADCHAR, WRDE_BADVAL, WRDE_CMDSUB, WRDE_NOSPACE, WRDE_SYNTAX};
    static const char *const error_names[] = {"WRDE_BADCHAR", "WRDE_BADVAL", "WRDE_CMDSUB", "WRDE_NOSPACE", "WRDE_SYNTAX"};
#else
    static const int         errors[]      = {WRDE_BADCHAR, WRDE_BADVAL, WRDE_CMDSUB, WRDE_NOSPACE, WRDE_SYNTAX};
    static const char *const error_names[] = {"WRDE_BADCHAR", "WRDE_BADVAL", "WRDE_CMDSUB", "WRDE_NOSPACE", "WRDE_SYNTAX"};
#endif

    for(size_t index = 0U; index < sizeof(errors) / sizeof(errors[0]); index++)
    {
        struct fault_state state = {0, errors[index]};
        int                failures_before;

        failures_before = failures;
        EXPECT(p101_error_has_no_error(err));
        fault_resource_events = 0U;
        errno                 = P101_TEST_ERRNO_SENTINEL;
        p101_env_set_fault_injector(env, fail_next_call, &state);
        int result = p101_wordexp(env, err, NULL, argument_3, 0);
        (void)result;
        EXPECT(state.checks == 1);
        EXPECT(p101_error_is_error(err, P101_ERROR_SYSTEM, state.code));
        EXPECT(errno == P101_TEST_ERRNO_SENTINEL);
        EXPECT(result == state.code);
        EXPECT(memcmp(argument_3, argument_3_before, sizeof(argument_3)) == 0);
        EXPECT(fault_resource_events == 0U);
        write_outcome("p101_wordexp", "system", error_names[index], state.code, failures == failures_before);
        p101_error_reset(err);
    }
    p101_env_set_fault_injector(env, NULL, NULL);
}

int main(void)
{
    const char        *outcome_path;
    struct p101_error *err;
    struct p101_env   *env;

    outcome_path = getenv("P101_WRAPPER_OUTCOME_LOG");
    if(outcome_path != NULL && outcome_path[0] != '\0')
    {
        outcome_stream = fopen(outcome_path, "a");
        if(outcome_stream == NULL)
        {
            fprintf(stderr, "FAIL: cannot open wrapper outcome receipt\n");
            return EXIT_FAILURE;
        }
    }
    err = p101_error_create(false);
    if(err == NULL)
    {
        if(outcome_stream != NULL)
        {
            (void)fclose(outcome_stream);
        }
        return EXIT_FAILURE;
    }
    env = p101_env_create(err, NULL);
    if(env == NULL)
    {
        p101_error_destroy(err);
        if(outcome_stream != NULL)
        {
            (void)fclose(outcome_stream);
        }
        return EXIT_FAILURE;
    }
    p101_env_set_fd_observer(env, count_fd_event, NULL);
    p101_env_set_alloc_observer(env, count_alloc_event, NULL);
    p101_env_set_resource_observer(env, count_resource_event, NULL);
    test_p101_mbsnrtowcs(env, err);
    test_p101_open_wmemstream(env, err);
    test_p101_regcomp(env, err);
    test_p101_regexec(env, err);
    test_p101_strcoll_l(env, err);
    test_p101_strdup(env, err);
    test_p101_strerror_r(env, err);
    test_p101_strndup(env, err);
    test_p101_strsignal(env, err);
    test_p101_strxfrm_l(env, err);
    test_p101_towctrans_l(env, err);
    test_p101_wcscoll_l(env, err);
    test_p101_wcsdup(env, err);
    test_p101_wcsnrtombs(env, err);
    test_p101_wcsxfrm_l(env, err);
    test_p101_wctrans_l(env, err);
    test_p101_wctype_l(env, err);
    test_p101_wordexp(env, err);
    p101_env_destroy(env);
    p101_error_destroy(err);
    if(outcome_stream != NULL && fclose(outcome_stream) != 0)
    {
        fprintf(stderr, "FAIL: cannot close wrapper outcome receipt\n");
        failures++;
    }
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
