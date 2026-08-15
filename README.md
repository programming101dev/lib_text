# lib_text

The repository owns two narrow targets. `p101_text` mirrors text, pattern,
regular-expression, and wide-character APIs. `p101_locale` mirrors locale,
message-catalog, language-information, and character-conversion APIs. The
targets remain separate so plain text consumers do not acquire `iconv`.

Strings, wide text, patterns, regular expressions, and word expansion for the p101 portable systems subset.

The public API is the intersection implemented on Linux, macOS, and FreeBSD.
POSIX, XSI, optional-POSIX, and common-Unix origins are recorded in
`api-manifest.tsv`; provenance does not determine ownership.

Public headers mirror their native C/POSIX/Unix header names, and implementation
sources mirror those public headers. Standards origin is metadata, not a
source-tree boundary.

## Build and verification

```sh
cmake -S . -B build -DCMAKE_C_COMPILER=clang -DP101_BUILD_LEVEL=1
cmake -S . -B build -DP101_BUILD_LEVEL=3 && cmake --build build
```

Every public wrapper is listed in `test/unit-test-manifest.tsv` and
invoked by either a deterministic injected-failure test or a handwritten
behavior test.

Instrumentation sees only calls routed through `p101_*` wrappers. It does
not observe direct libc calls or work performed inside third-party code.
