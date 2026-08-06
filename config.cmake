# Project metadata
set(PROJECT_NAME "p101_text")
set(PROJECT_VERSION "0.0.1")
set(PROJECT_DESCRIPTION "Strings, wide text, patterns, regular expressions, and word expansion")
set(PROJECT_LANGUAGE "C")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(STANDARD_FLAGS
        -D_POSIX_C_SOURCE=200809L
        -D_XOPEN_SOURCE=700
        -Werror
)
set(DARWIN_STANDARD_FLAGS -D_DARWIN_C_SOURCE)
set(LINUX_STANDARD_FLAGS -D_GNU_SOURCE)
set(BSD_STANDARD_FLAGS -D_BSD_SOURCE -D__BSD_VISIBLE)

set(LIBRARY_TARGETS p101_text)
set(p101_text_SOURCES
        src/ctype.c
        src/regex.c
        src/stdlib.c
        src/string.c
        src/strings.c
        src/unistd.c
        src/wchar.c
        src/wctype.c
        src/wordexp.c
)
set(p101_text_HEADERS
        include/p101_text/p101_ctype.h
        include/p101_text/p101_regex.h
        include/p101_text/p101_stdlib.h
        include/p101_text/p101_string.h
        include/p101_text/p101_strings.h
        include/p101_text/p101_unistd.h
        include/p101_text/p101_wchar.h
        include/p101_text/p101_wctype.h
        include/p101_text/p101_wordexp.h
)
set(p101_text_LINK_LIBRARIES
        p101_error
        p101_env
        p101_c
)

