#include <ctype.h>
#include <locale.h>
#include <p101_env/env.h>
#include <p101_error/error.h>
#include <p101_text/text.h>
#include <regex.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <wchar.h>
#include <wctype.h>
#include <wordexp.h>

static int failures;

#define EXPECT(condition)                                                                                                                                                                                                                                          \
    do                                                                                                                                                                                                                                                             \
    {                                                                                                                                                                                                                                                              \
        if(!(condition))                                                                                                                                                                                                                                           \
        {                                                                                                                                                                                                                                                          \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition);                                                                                                                                                                                   \
            failures++;                                                                                                                                                                                                                                            \
        }                                                                                                                                                                                                                                                          \
    } while(0)

static void test_ctype(const struct p101_env *env, locale_t locale)
{
    /* P101_TEST_CASE(p101_isalnum_l) */
    EXPECT(p101_isalnum_l(env, 'A', locale) == isalnum_l('A', locale));
    /* P101_TEST_CASE(p101_isalpha_l) */
    EXPECT(p101_isalpha_l(env, 'A', locale) == isalpha_l('A', locale));
    /* P101_TEST_CASE(p101_isblank_l) */
    EXPECT(p101_isblank_l(env, ' ', locale) == isblank_l(' ', locale));
    /* P101_TEST_CASE(p101_iscntrl_l) */
    EXPECT(p101_iscntrl_l(env, '\n', locale) == iscntrl_l('\n', locale));
    /* P101_TEST_CASE(p101_isdigit_l) */
    EXPECT(p101_isdigit_l(env, '7', locale) == isdigit_l('7', locale));
    /* P101_TEST_CASE(p101_isgraph_l) */
    EXPECT(p101_isgraph_l(env, 'A', locale) == isgraph_l('A', locale));
    /* P101_TEST_CASE(p101_islower_l) */
    EXPECT(p101_islower_l(env, 'a', locale) == islower_l('a', locale));
    /* P101_TEST_CASE(p101_isprint_l) */
    EXPECT(p101_isprint_l(env, 'A', locale) == isprint_l('A', locale));
    /* P101_TEST_CASE(p101_ispunct_l) */
    EXPECT(p101_ispunct_l(env, '!', locale) == ispunct_l('!', locale));
    /* P101_TEST_CASE(p101_isspace_l) */
    EXPECT(p101_isspace_l(env, ' ', locale) == isspace_l(' ', locale));
    /* P101_TEST_CASE(p101_isupper_l) */
    EXPECT(p101_isupper_l(env, 'A', locale) == isupper_l('A', locale));
    /* P101_TEST_CASE(p101_isxdigit_l) */
    EXPECT(p101_isxdigit_l(env, 'f', locale) == isxdigit_l('f', locale));
    /* P101_TEST_CASE(p101_tolower_l) */
    EXPECT(p101_tolower_l(env, 'A', locale) == tolower_l('A', locale));
    /* P101_TEST_CASE(p101_toupper_l) */
    EXPECT(p101_toupper_l(env, 'a', locale) == toupper_l('a', locale));
}

static void test_wctype(const struct p101_env *env, locale_t locale)
{
    wctype_t  character_class = wctype("alpha");
    wctrans_t transform       = wctrans("tolower");

    /* P101_TEST_CASE(p101_iswalnum_l) */
    EXPECT(p101_iswalnum_l(env, L'A', locale) == iswalnum_l(L'A', locale));
    /* P101_TEST_CASE(p101_iswalpha_l) */
    EXPECT(p101_iswalpha_l(env, L'A', locale) == iswalpha_l(L'A', locale));
    /* P101_TEST_CASE(p101_iswblank_l) */
    EXPECT(p101_iswblank_l(env, L' ', locale) == iswblank_l(L' ', locale));
    /* P101_TEST_CASE(p101_iswcntrl_l) */
    EXPECT(p101_iswcntrl_l(env, L'\n', locale) == iswcntrl_l(L'\n', locale));
    /* P101_TEST_CASE(p101_iswctype_l) */
    EXPECT(p101_iswctype_l(env, L'A', character_class, locale) == iswctype_l(L'A', character_class, locale));
    /* P101_TEST_CASE(p101_iswdigit_l) */
    EXPECT(p101_iswdigit_l(env, L'7', locale) == iswdigit_l(L'7', locale));
    /* P101_TEST_CASE(p101_iswgraph_l) */
    EXPECT(p101_iswgraph_l(env, L'A', locale) == iswgraph_l(L'A', locale));
    /* P101_TEST_CASE(p101_iswlower_l) */
    EXPECT(p101_iswlower_l(env, L'a', locale) == iswlower_l(L'a', locale));
    /* P101_TEST_CASE(p101_iswprint_l) */
    EXPECT(p101_iswprint_l(env, L'A', locale) == iswprint_l(L'A', locale));
    /* P101_TEST_CASE(p101_iswpunct_l) */
    EXPECT(p101_iswpunct_l(env, L'!', locale) == iswpunct_l(L'!', locale));
    /* P101_TEST_CASE(p101_iswspace_l) */
    EXPECT(p101_iswspace_l(env, L' ', locale) == iswspace_l(L' ', locale));
    /* P101_TEST_CASE(p101_iswupper_l) */
    EXPECT(p101_iswupper_l(env, L'A', locale) == iswupper_l(L'A', locale));
    /* P101_TEST_CASE(p101_iswxdigit_l) */
    EXPECT(p101_iswxdigit_l(env, L'f', locale) == iswxdigit_l(L'f', locale));
    /* P101_TEST_CASE(p101_towctrans_l) */
    EXPECT(p101_towctrans_l(env, L'A', transform, locale) == towctrans_l(L'A', transform, locale));
    /* P101_TEST_CASE(p101_towlower_l) */
    EXPECT(p101_towlower_l(env, L'A', locale) == towlower_l(L'A', locale));
    /* P101_TEST_CASE(p101_towupper_l) */
    EXPECT(p101_towupper_l(env, L'a', locale) == towupper_l(L'a', locale));
}

static void test_narrow_strings(const struct p101_env *env, locale_t locale)
{
    char          destination[32];
    char          second[32];
    char          token_text[]    = "alpha,beta";
    char          separate_text[] = "left:right";
    char         *state           = NULL;
    char         *cursor          = separate_text;
    char         *result;
    unsigned char source_bytes[] = {1, 2, 3, 4};
    unsigned char swapped[4]     = {0};

    /* P101_TEST_CASE(p101_a64l) */
    EXPECT(p101_a64l(env, "./") == a64l("./"));
    /* P101_TEST_CASE(p101_l64a) */
    EXPECT(p101_l64a(env, 1234L) != NULL);
    /* P101_TEST_CASE(p101_ffs) */
    EXPECT(p101_ffs(env, 8) == 4);

    (void)memset(destination, 0, sizeof(destination));
    /* P101_TEST_CASE(p101_memccpy) */
    result = p101_memccpy(env, destination, "abc:def", ':', 7);
    EXPECT(result == destination + 4);
    EXPECT(memcmp(destination, "abc:", 4) == 0);

    /* P101_TEST_CASE(p101_stpcpy) */
    result = p101_stpcpy(env, destination, "alpha");
    EXPECT(result == destination + 5 && strcmp(destination, "alpha") == 0);
    /* P101_TEST_CASE(p101_stpncpy) */
    result = p101_stpncpy(env, second, "beta", sizeof(second));
    EXPECT(result == second + 4 && strcmp(second, "beta") == 0);
    /* P101_TEST_CASE(p101_strcasecmp) */
    EXPECT(p101_strcasecmp(env, "Alpha", "alpha") == 0);
    /* P101_TEST_CASE(p101_strcasecmp_l) */
    EXPECT(p101_strcasecmp_l(env, "Alpha", "alpha", locale) == 0);
    /* P101_TEST_CASE(p101_strcasestr) */
    EXPECT(p101_strcasestr(env, "Alpha Beta", "beta") != NULL);
    /* P101_TEST_CASE(p101_strchrnul) */
    EXPECT(*p101_strchrnul(env, "abc", 'z') == '\0');
    /* P101_TEST_CASE(p101_strncasecmp) */
    EXPECT(p101_strncasecmp(env, "Alpha", "ALP", 3) == 0);
    /* P101_TEST_CASE(p101_strncasecmp_l) */
    EXPECT(p101_strncasecmp_l(env, "Alpha", "ALP", 3, locale) == 0);
    /* P101_TEST_CASE(p101_strnlen) */
    EXPECT(p101_strnlen(env, "abcdef", 3) == 3);
    /* P101_TEST_CASE(p101_strsep) */
    EXPECT(strcmp(p101_strsep(env, &cursor, ":"), "left") == 0);
    /* P101_TEST_CASE(p101_strsignal) */
    EXPECT(p101_strsignal(env, SIGTERM) != NULL);
    /* P101_TEST_CASE(p101_strtok_r) */
    EXPECT(strcmp(p101_strtok_r(env, token_text, ",", &state), "alpha") == 0);
    /* P101_TEST_CASE(p101_swab) */
    p101_swab(env, source_bytes, swapped, 4);
    EXPECT(swapped[0] == 2 && swapped[1] == 1 && swapped[2] == 4 && swapped[3] == 3);
    /* P101_TEST_CASE(p101_rpmatch) */
    EXPECT(p101_rpmatch(env, "yes") == rpmatch("yes"));
}

static void test_wide_strings(const struct p101_env *env)
{
    wchar_t destination[32];
    wchar_t second[32];

    /* P101_TEST_CASE(p101_wcpcpy) */
    EXPECT(p101_wcpcpy(env, destination, L"alpha") == destination + 5);
    EXPECT(wcscmp(destination, L"alpha") == 0);
    /* P101_TEST_CASE(p101_wcpncpy) */
    EXPECT(p101_wcpncpy(env, second, L"beta", 8) == second + 4);
    EXPECT(wcscmp(second, L"beta") == 0);
    /* P101_TEST_CASE(p101_wcscasecmp) */
    EXPECT(p101_wcscasecmp(env, L"Alpha", L"alpha") == 0);
    /* P101_TEST_CASE(p101_wcsncasecmp) */
    EXPECT(p101_wcsncasecmp(env, L"Alpha", L"ALP", 3) == 0);
    /* P101_TEST_CASE(p101_wcsnlen) */
    EXPECT(p101_wcsnlen(env, L"abcdef", 3) == 3);
    /* P101_TEST_CASE(p101_wcswidth) */
    EXPECT(p101_wcswidth(env, L"abc", 3) == 3);
    /* P101_TEST_CASE(p101_wcwidth) */
    EXPECT(p101_wcwidth(env, L'A') == 1);
}

static void test_regex_and_wordexp(const struct p101_env *env, struct p101_error *err)
{
    regex_t    expression;
    regmatch_t match;
    wordexp_t  words = {0};
    char       message[128];
    int        result;
    size_t     length;

    result = p101_regcomp(env, err, &expression, "^a.*c$", REG_EXTENDED);
    EXPECT(result == 0);
    EXPECT(p101_error_has_no_error(err));
    if(result == 0)
    {
        /* P101_TEST_CASE(p101_regexec) */
        EXPECT(p101_regexec(env, &expression, "abc", 1, &match, 0) == 0);
        /* P101_TEST_CASE(p101_regerror) */
        length = p101_regerror(env, REG_NOMATCH, &expression, message, sizeof(message));
        EXPECT(length > 0);
        /* P101_TEST_CASE(p101_regfree) */
        p101_regfree(env, &expression);
    }

    result = wordexp("alpha beta", &words, 0);
    EXPECT(result == 0);
    if(result == 0)
    {
        /* P101_TEST_CASE(p101_wordfree) */
        p101_wordfree(env, &words);
    }
}

int main(void)
{
    struct p101_error *err;
    struct p101_env   *env;
    locale_t           locale;

    err = p101_error_create(false);
    if(err == NULL)
    {
        return EXIT_FAILURE;
    }
    env = p101_env_create(err, NULL);
    if(env == NULL)
    {
        p101_error_destroy(err);
        return EXIT_FAILURE;
    }
    locale = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    EXPECT(locale != (locale_t)0);
    if(locale != (locale_t)0)
    {
        test_ctype(env, locale);
        test_wctype(env, locale);
        test_narrow_strings(env, locale);
        freelocale(locale);
    }
    test_wide_strings(env);
    test_regex_and_wordexp(env, err);
    p101_env_destroy(env);
    p101_error_destroy(err);
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
