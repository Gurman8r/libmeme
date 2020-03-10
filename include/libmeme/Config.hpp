#ifndef _ML_CONFIG_HPP_
#define _ML_CONFIG_HPP_

// Project Information
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML__AUTHOR  "Melody Gurman"
#define ML__NAME    "libmeme"
#define ML__VERSION 0
#define ML__URL     "https://www.github.com/Gurman8r/libmeme"
#define ML__DATE    __DATE__
#define ML__TIME    __TIME__


// Language
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#if defined(__cplusplus)
#	if defined(_MSVC_LANG)
#		define ML_CC_LANG _MSVC_LANG
#	else
#		define ML_CC_LANG __cplusplus
#	endif
#	if (ML_CC_LANG >= 201907L)     // C++20
#		define ML_HAS_CXX20 1
#       define ML_HAS_CXX17 1
#       define ML_HAS_CXX14 1
#       define ML_HAS_CXX11 1
#   elif (ML_CC_LANG >= 201703L)   // C++17
#		define ML_HAS_CXX20 0
#		define ML_HAS_CXX17 1
#       define ML_HAS_CXX14 1
#       define ML_HAS_CXX11 1
#   elif (ML_CC_LANG >= 201402L)   // C++14
#		define ML_HAS_CXX20 0
#		define ML_HAS_CXX17 0
#       define ML_HAS_CXX14 1
#       define ML_HAS_CXX11 1
#   elif (ML_CC_LANG >= 201103L)   // C++11
#		define ML_HAS_CXX20 0
#		define ML_HAS_CXX17 0
#       define ML_HAS_CXX14 0
#       define ML_HAS_CXX11 1
#	endif
#else
#	error This system does not support C++.
#endif


// Operating System
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#if defined(_WIN32) || defined(_WIN64) \
  || defined(WIN32) || defined(WIN64) \
  || defined(__MINGW32__) || defined(__MINGW64__)
#	define ML_OS_WINDOWS 1
#	define ML_OS_NAME "Windows"
#elif defined(__APPLE__) && defined(__MACH__)
#	define ML_OS_APPLE 1
#	define ML_OS_NAME "Apple"
#elif defined(__unix__)
#	define ML_OS_UNIX 1
#	if defined(__ANDROID__)
#		define ML_OS_ANDROID 1
#		define ML_OS_NAME "Android"
#	elif defined(__linux__)
#		define ML_OS_LINUX 1
#		define ML_OS_NAME "Linux"
#	elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#		define ML_OS_FREEBSD 1
#		define ML_OS_NAME "FreeBSD"
#   else
#       error This Unix system is not supported.
#	endif
#else
#	error This system is not supported.
#endif


// Architecture
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#if defined(__x86_64__) || defined(_M_X64) || defined(_x64)
#	define ML_X64 1
#	define ML_ARCH 64
#	define ML_PLATFORM "x64"
#elif defined(__i386__) || defined(_M_IX86)
#	define ML_X86 1
#	define ML_ARCH 32
#	define ML_PLATFORM "x86"
#elif defined(__arm__) || defined(_M_ARM) || defined(__aarch64__)
#	if defined(__aarch64__)
#		define ML_ARM64 1
#		define ML_ARCH 64
#		define ML_PLATFORM "arm64"
#	else
#		define ML_ARM32 1
#		define ML_ARCH 32
#		define ML_PLATFORM "arm32"
#	endif
#elif defined(ppc) || defined(_M_PPC) || defined(__ppc64__)
#	if defined(__ppc64__)
#		define ML_PPC64 1
#		define ML_ARCH 64
#		define ML_PLATFORM "ppc64"
#	else
#		define ML_PPC32 1
#		define ML_ARCH 32
#		define ML_PLATFORM "ppc32"
#	endif
#else
#	error Unable to detect platform architecture.
#endif


// Compiler
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#if defined(_MSC_VER)
//  Visual Studio
#	define ML_CC_MSVC _MSC_VER
#	define ML_CC_VER ML_CC_MSVC
#	if (ML_CC_VER >= 1920)
#	    define ML_CC_NAME "Visual Studio 2019"
#	elif (ML_CC_VER >= 1910)
#	    define ML_CC_NAME "Visual Studio 2017"
#	elif (ML_CC_VER >= 1900)
#	    define ML_CC_NAME "Visual Studio 2015"
#	elif (ML_CC_VER >= 1800)
#	    define ML_CC_NAME "Visual Studio 2013"
#	else
#		error This version of Visual Studio is not supported.
#	endif
#elif defined(__clang__)
//  Clang/LLVM
#	define ML_CC_CLANG __clang__
#	define ML_CC_VER ML_CC_CLANG
#	define ML_CC_NAME "Clang/LLVM"
#elif (defined(__GNUC__) || defined(__GNUG__)) && !defined(__clang__)
//  GCC
#	if defined(__GNUC__)
#		define ML_CC_GCC __GNUC__
#	else
#		define ML_CC_GCC __GNUG__
#	endif
#	define ML_CC_VER ML_CC_GCC
#	define ML_CC_NAME "GCC"
#elif defined(__ICC) || defined(__INTEL_COMPILER)
//  Intel
#	if defined(__ICC)
#		define ML_CC_INTEL __ICC
#	else
#		define ML_CC_INTEL __INTEL_COMPILER
#	endif
#	define ML_CC_VER ML_CC_INTEL
#	define ML_CC_NAME "Intel"
#elif defined(__MINGW32__) || defined(__MINGW64__)
//  MinGW
#	if defined(__MINGW64__)
#		define ML_CC_MINGW __MINGW64__
#	else
#		define ML_CC_MINGW __MINGW32__
#	endif
#	define ML_CC_VER ML_CC_MINGW
#	define ML_CC_NAME "MinGW"
#elif defined(__EMSCRIPTEN__)
//  Emscripten
#	define ML_CC_EMSCRIPTEN
#	define ML_CC_VER ML_CC_EMSCRIPTEN
#	define ML_CC_NAME "Emscripten"
#elif defined(__asmjs__)
//  asm.js
#	define ML_CC_ASMJS
#	define ML_CC_VER ML_CC_ASMJS
#	define ML_CC_NAME "asm.js"
#elif defined(__wasm__)
//  WebAssembly
#	define ML_CC_WASM
#	define ML_CC_VER ML_CC_WASM
#	define ML_CC_NAME "WebAssembly"
#else
#	error This compiler is not supported.
#endif


// Types
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// integer
#if defined(ML_CC_MSVC)
#	define	ML_INT8             signed __int8
#	define	ML_INT16            signed __int16
#	define	ML_INT32            signed __int32
#	define	ML_INT64            signed __int64
#	define	ML_UINT8            unsigned __int8
#	define	ML_UINT16           unsigned __int16
#	define	ML_UINT32           unsigned __int32
#	define	ML_UINT64           unsigned __int64
#else
#	define	ML_INT8             signed char
#	define	ML_INT16            signed short
#	define	ML_INT32            signed int
#	define	ML_INT64            signed long long
#	define	ML_UINT8            unsigned char
#	define	ML_UINT16           unsigned short
#	define	ML_UINT32           unsigned int
#	define	ML_UINT64           unsigned long long
#endif
#define	ML_FLOAT32              float
#define	ML_FLOAT64              double
#define	ML_FLOAT80              long double // 8, 10, 12, or 16 bytes (CC Dependant)

#ifndef ML_BYTE
#   define ML_BYTE              unsigned char
#endif

#if !ML_HAS_CXX20
#   define ML_CHAR              char
#else
#   define ML_CHAR              char8_t
#endif

#if (ML_ARCH == 32)
#	define ML_INTMAX            ML_INT32
#	define ML_UINTMAX           ML_UINT32
#else
#	define ML_INTMAX            ML_INT64
#	define ML_UINTMAX           ML_UINT64
#endif


// Debug
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#if defined(_DEBUG)
#	define ML_IS_DEBUG          1
#   define ML_CONFIGURATION     "debug"
#else
#	define ML_IS_DEBUG          0
#   define ML_CONFIGURATION     "release"
#endif


// Namespace
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define _ML                     ::ml::
#define _ML_BEGIN               namespace ml {
#define _ML_END                 }


// Alias
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_ALIAS                using


// Exceptions
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_THROW                throw
#define ML_CATCH                catch
#define ML_TRY                  try


// Preprocessor
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_CONCAT_IMPL(a, b)    a##b
#define ML_CONCAT(a, b)         ML_CONCAT_IMPL(a, b)
#define ML_TOSTRING(str)        #str
#define ML_STRINGIFY(str)       ML_TOSTRING(str)


// Anonymous Variables
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#if defined(__COUNTER__)
#	define ML_ANONYMOUS(expr)   ML_CONCAT(_ML_, ML_CONCAT(expr, ML_CONCAT(_, __COUNTER__)))
#elif defined(__LINE__)
#	define ML_ANONYMOUS(expr)   ML_CONCAT(_ML_, ML_CONCAT(expr, ML_CONCAT(_, __LINE__)))
#endif

#define ML_IMPL_ONCE(once)      static bool once{ false }; if (!once && (once = true))
#define ML_ONCE_CALL            ML_IMPL_ONCE(ML_ANONYMOUS(once))


// Attributes
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// nodiscard
#if __has_cpp_attribute(nodiscard) >= 201603L
#   define ML_NODISCARD [[nodiscard]]
#else
#   define ML_NODISCARD
#endif

// likely
#if __has_cpp_attribute(likely) >= 201907L
#   define ML_LIKELY(...) (##__VA_ARGS__) [[likely]]
#else
#   define ML_LIKELY(...) (##__VA_ARGS__)
#endif

// unlikely
#if __has_cpp_attribute(unlikely) >= 201907L
#   define ML_UNLIKELY(...) (##__VA_ARGS__) [[unlikely]]
#else
#   define ML_UNLIKELY(...) (##__VA_ARGS__)
#endif

// inline
#ifdef ML_CC_MSVC
#	define ML_ALWAYS_INLINE __forceinline
#	define ML_NEVER_INLINE  __declspec(noinline)
#elif defined(ML_CC_GCC) || defined(ML_CC_CLANG)
#	define ML_ALWAYS_INLINE inline __attribute__((always_inline))
#	define ML_NEVER_INLINE  __attribute__ ((noinline))
#else
#	define ML_ALWAYS_INLINE
#	define ML_NEVER_INLINE
#endif

// visibility
#ifdef ML_CC_MSVC
#	define ML_API_EXPORT __declspec(dllexport)
#	define ML_API_IMPORT __declspec(dllimport)
#elif (defined(ML_CC_CLANG) || defined(ML_CC_GCC)) && (ML_CC_VER >= 4)
#	define ML_API_EXPORT __attribute__ ((visibility ("default")))
#	define ML_API_IMPORT __attribute__ ((visibility ("hidden")))
#else
#   define ML_API_EXPORT
#   define ML_API_IMPORT
#endif


// Disable Compiler Warnings
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifdef ML_CC_MSVC
#	pragma warning(disable: 4031)	// second formal parameter list longer than the first list
#	pragma warning(disable: 4067)	// unexpected tokens following preprocessor directive - expected a newline
#	pragma warning(disable: 4251)	// type1 needs to have dll-interface to be used by type2
#	pragma warning(disable: 4307)	// integral constant overflow
#	pragma warning(disable: 4308)	// negative integral constant converted to unsigned type
#	pragma warning(disable: 4309)	// truncation of constant value
#	pragma warning(disable: 4723)	// potential divide by zero
#	pragma warning(disable: 6282)	// incorrect operator
#	pragma warning(disable: 26437)	// do not slice
#	pragma warning(disable: 26444)	// avoid unnamed objecs with custom construction and destruction
#	pragma warning(disable: 26451)	// arithmetic overflow
#	pragma warning(disable: 26495)	// value may be uninitialized
#	pragma warning(disable: 26812)	// unscoped enum
#else
#endif

#endif // !_ML_CONFIG_HPP_