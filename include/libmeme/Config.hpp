#ifndef _ML_CONFIG_HPP_
#define _ML_CONFIG_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// PROJECT
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML__author              "Melody Gurman"
#define ML__name                "libmeme"
#define ML__version             0
#define ML__url                 "https://www.github.com/Gurman8r/libmeme"
#define ML__date                __DATE__
#define ML__time                __TIME__


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// LANGUAGE
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * Lang */
#if defined(__cplusplus)
#   if defined(_MSVC_LANG)
#       define ML_lang           _MSVC_LANG
#   else
#       define ML_lang           __cplusplus
#   endif

/* * * * * * * * * * * * * * * * C++20 */
#   if (ML_lang >= 201907L)
#       define ML_has_cxx20     1
#       define ML_has_cxx17     1
#       define ML_has_cxx14     1
#       define ML_has_cxx11     1

/* * * * * * * * * * * * * * * * C++17 */
#   elif (ML_lang >= 201703L)
#       define ML_has_cxx20     0
#       define ML_has_cxx17     1
#       define ML_has_cxx14     1
#       define ML_has_cxx11     1

/* * * * * * * * * * * * * * * * C++14 */
#   elif (ML_lang >= 201402L)
#       define ML_has_cxx20     0
#       define ML_has_cxx17     0
#       define ML_has_cxx14     1
#       define ML_has_cxx11     1

/* * * * * * * * * * * * * * * * C++11 */
#   elif (ML_lang >= 201103L)
#       define ML_has_cxx20     0
#       define ML_has_cxx17     0
#       define ML_has_cxx14     0
#       define ML_has_cxx11     1

/* * * * * * * * * * * * * * * * Unknown C++ */
#   else
#       error This version of C++ is not supported.
#   endif

/* * * * * * * * * * * * * * * * C++ Unsupported */
#else
#   error This system does not support C++.
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// OPERATING SYSTEM
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * Windows */
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64) || defined(__MINGW32__) || defined(__MINGW64__)
#   define ML_os_windows        1
#   define ML_os_name           "Windows"

/* * * * * * * * * * * * * * * * Apple */
#elif defined(__APPLE__) && defined(__MACH__)
#   define ML_os_apple          1
#   define ML_os_name           "Apple"

/* * * * * * * * * * * * * * * * Unix */
#elif defined(__unix__)

/* * * * * * * * * * * * * * * * Android */
#   if defined(__ANDROID__)
#       define ML_os_android    1
#       define ML_os_unix		ML_os_android
#       define ML_os_name       "Android"

/* * * * * * * * * * * * * * * * Linux */
#   elif defined(__linux__)
#       define ML_os_linux      2
#       define ML_os_unix		ML_os_linux
#       define ML_os_name       "Linux"

/* * * * * * * * * * * * * * * * FreeBSD */
#   elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#       define ML_os_freebsd    3
#       define ML_os_unix		ML_os_freebsd
#       define ML_os_name       "FreeBSD"

/* * * * * * * * * * * * * * * * Unknown Unix */
#   else
#       error This Unix system is not supported.
#   endif

/* * * * * * * * * * * * * * * * Unknown Operating System */
#else
#   error This operating system is not supported.
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// ARCHITECTURE
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * x64 */
#if defined(__x86_64__) || defined(_M_X64) || defined(_x64)
#   define ML_x64               1
#   define ML_arch              64
#   define ML_platform          "x64"

/* * * * * * * * * * * * * * * * x86 */
#elif defined(__i386__) || defined(_M_IX86)
#   define ML_x86               1
#   define ML_arch              32
#   define ML_platform          "x86"

/* * * * * * * * * * * * * * * * ARM */
#elif defined(__arm__) || defined(_M_ARM) || defined(__aarch64__)
#   if defined(__aarch64__)
#       define ML_arm64         1
#       define ML_arch          64
#       define ML_platform      "arm64"
#   else
#       define ML_arm32         1
#       define ML_arch          32
#       define ML_platform      "arm32"
#   endif

/* * * * * * * * * * * * * * * * PowerPC */
#elif defined(ppc) || defined(_M_PPC) || defined(__ppc64__)
#   if defined(__ppc64__)
#       define ML_ppc64         1
#       define ML_arch          64
#       define ML_platform      "ppc64"
#   else
#       define ML_ppc32         1
#       define ML_arch          32
#       define ML_platform      "ppc32"
#   endif

/* * * * * * * * * * * * * * * * Unknown Architecture */
#else
#   error Unable to detect platform architecture.
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// COMPILER
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * Visual Studio */
#if defined(_MSC_VER)
#   define ML_cc_msvc           _MSC_VER
#   define ML_cc_version        ML_cc_msvc
#   if (ML_cc_version >= 1920)
#       define ML_cc_name       "Visual Studio 2019"
#   elif (ML_cc_version >= 1910)
#       define ML_cc_name       "Visual Studio 2017"
#   elif (ML_cc_version >= 1900)
#       define ML_cc_name       "Visual Studio 2015"
#   elif (ML_cc_version >= 1800)
#       define ML_cc_name       "Visual Studio 2013"
#   else
#       error This version of Visual Studio is not supported.
#   endif

/* * * * * * * * * * * * * * * * Clang / LLVM */
#elif defined(__clang__)
#   define ML_cc_clang          __clang__
#   define ML_cc_version        ML_cc_clang
#   define ML_cc_name           "Clang/LLVM"

/* * * * * * * * * * * * * * * * GCC */
#elif (defined(__GNUC__) || defined(__GNUG__)) && !defined(__clang__)
#   if defined(__GNUC__)
#       define ML_cc_gcc        __GNUC__
#   else
#       define ML_cc_gcc        __GNUG__
#   endif
#   define ML_cc_version        ML_cc_gcc
#   define ML_cc_name           "GCC"

/* * * * * * * * * * * * * * * * Intel */
#elif defined(__ICC) || defined(__INTEL_COMPILER)
#   if defined(__ICC)
#       define ML_cc_intel      __ICC
#   else
#       define ML_cc_intel      __INTEL_COMPILER
#   endif
#   define ML_cc_version        ML_cc_intel
#   define ML_cc_name           "Intel"

/* * * * * * * * * * * * * * * * MinGW */
#elif defined(__MINGW32__) || defined(__MINGW64__)
#   if defined(__MINGW64__)
#       define ML_cc_mingw      __MINGW64__
#   else
#       define ML_cc_mingw      __MINGW32__
#   endif
#   define ML_cc_version        ML_cc_mingw
#   define ML_cc_name           "MinGW"

/* * * * * * * * * * * * * * * * Emscripten */
#elif defined(__EMSCRIPTEN__)
#   define ML_cc_emscripten     __EMSCRIPTEN__
#   define ML_cc_version        ML_cc_emscripten
#   define ML_cc_name           "Emscripten"

/* * * * * * * * * * * * * * * * asm.js */
#elif defined(__asmjs__)
#   define ML_cc_asmjs          __asmjs__
#   define ML_cc_version        ML_cc_asmjs
#   define ML_cc_name           "asm.js"

/* * * * * * * * * * * * * * * * WebAssembly */
#elif defined(__wasm__)
#   define ML_cc_wasm           __wasm__
#   define ML_cc_version        ML_cc_wasm
#   define ML_cc_name           "WebAssembly"

/* * * * * * * * * * * * * * * * Unknown Compiler */
#else
#   error This compiler is not supported.
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// TYPES
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// integers
#if defined(ML_cc_msvc)
#   define  ML_int8             signed __int8
#   define  ML_int16            signed __int16
#   define  ML_int32            signed __int32
#   define  ML_int64            signed __int64
#   define  ML_uint8            unsigned __int8
#   define  ML_uint16           unsigned __int16
#   define  ML_uint32           unsigned __int32
#   define  ML_uint64           unsigned __int64
#else
#   define  ML_int8             signed char
#   define  ML_int16            signed short
#   define  ML_int32            signed int
#   define  ML_int64            signed long long
#   define  ML_uint8            unsigned char
#   define  ML_uint16           unsigned short
#   define  ML_uint32           unsigned int
#   define  ML_uint64           unsigned long long
#endif

// byte
#ifndef ML_byte
#   define ML_byte              unsigned char
#endif

// char
#ifndef ML_char
#   if !ML_has_cxx20
#       define ML_char          char
#   else
#       define ML_char          char8_t
#   endif
#endif

// intmax
#if (ML_arch == 32)
#   define ML_intmax            ML_int32
#   define ML_uintmax           ML_uint32
#else
#   define ML_intmax            ML_int64
#   define ML_uintmax           ML_uint64
#endif

// ulong
#ifndef ML_ulong
#   define ML_ulong             unsigned long
#endif

// floats
#define ML_float32              float
#define ML_float64              double
#define ML_float80              long double // 8, 10, 12, or 16 bytes (cc dependant)


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// CONFIGURATION
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(_DEBUG)
/* * * * * * * * * * * * * * * * Debug */
#   define ML_is_debug          true
#   define ML_configuration     "debug"
#else
/* * * * * * * * * * * * * * * * Release */
#   define ML_is_debug          false
#   define ML_configuration     "release"
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// PREPROCESSOR
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// concat implementation
#define ML_impl_concat(a, b)    a##b

// macro concatenate
#define ML_concat(a, b)         ML_impl_concat(a, b)

// macro name to string
#define ML_to_string(expr)      #expr

// macro value to string
#define ML_stringify(expr)      ML_to_string(expr)

// anonymous variable
#if defined(__COUNTER__)
#	define ML_anon(...)         ML_concat(_ml_, ML_concat(##__VA_ARGS__, ML_concat(_, ML_concat(__COUNTER__, _))))
#elif defined(__LINE__)
#	define ML_anon(...)         ML_concat(_ml_, ML_concat(##__VA_ARGS__, ML_concat(_, ML_concat(__LINE__, _))))
#else
#   define ML_anon(expr)        expr
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// ATTRIBUTES
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// nodiscard
#if __has_cpp_attribute(nodiscard) >= 201603L
#   define ML_NODISCARD         [[nodiscard]]
#else
#   define ML_NODISCARD
#endif

// likely
#if __has_cpp_attribute(likely) >= 201907L
#   define ML_LIKELY(expr)      ((expr)) [[likely]]
#else
#   define ML_LIKELY(expr)      ((expr))
#endif

// unlikely
#if __has_cpp_attribute(unlikely) >= 201907L
#   define ML_UNLIKELY(expr)    ((expr)) [[unlikely]]
#else
#   define ML_UNLIKELY(expr)    ((expr))
#endif

// inlining
#ifdef ML_cc_msvc
#   define ML_ALWAYS_INLINE     __forceinline
#   define ML_NEVER_INLINE      __declspec(noinline)
#elif defined(ML_cc_clang) || defined(ML_cc_gcc)
#   define ML_ALWAYS_INLINE     inline __attribute__((always_inline))
#   define ML_NEVER_INLINE      __attribute__ ((noinline))
#else
#   define ML_ALWAYS_INLINE     inline
#   define ML_NEVER_INLINE
#endif

// visibility
#ifdef ML_cc_msvc
#   define ML_API_EXPORT        __declspec(dllexport)
#   define ML_API_IMPORT        __declspec(dllimport)
#elif (defined(ML_cc_clang) || defined(ML_cc_gcc)) && (ML_cc_version >= 4)
#   define ML_API_EXPORT        __attribute__ ((visibility ("default")))
#   define ML_API_IMPORT        __attribute__ ((visibility ("hidden")))
#else
#   define ML_API_EXPORT
#   define ML_API_IMPORT
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// WARNINGS
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifdef ML_cc_msvc
#   pragma warning(disable: 4031)   // second formal parameter list longer than the first list
#   pragma warning(disable: 4067)   // unexpected tokens following preprocessor directive - expected a newline
#   pragma warning(disable: 4251)   // type1 needs to have dll-interface to be used by type2
#   pragma warning(disable: 4307)   // integral constant overflow
#   pragma warning(disable: 4308)   // negative integral constant converted to unsigned type
#   pragma warning(disable: 4309)   // truncation of constant value
#   pragma warning(disable: 4723)   // potential divide by zero
#   pragma warning(disable: 6011)   // dereferencing NULL pointer
#   pragma warning(disable: 6282)   // incorrect operator
#   pragma warning(disable: 26437)  // do not slice
#   pragma warning(disable: 26444)  // avoid unnamed objecs with custom construction and destruction
#   pragma warning(disable: 26451)  // arithmetic overflow
#   pragma warning(disable: 26495)  // value may be uninitialized
#   pragma warning(disable: 26812)  // unscoped enum
#else
#endif

#endif // !_ML_CONFIG_HPP_