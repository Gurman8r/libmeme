#ifndef _ML_COMMON_HPP_
#define _ML_COMMON_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if (ML_has_cxx11 && ML_has_cxx14)
#	include <cassert>
#	include <chrono>
#	include <cstdarg>
#	include <fstream>
#	include <functional>
#	include <iostream>
#	include <locale>
#	include <sstream>
#	include <type_traits>
#	include <utility>
#	include <vector>
#endif

#if ML_has_cxx17
#	include <any>
#	include <filesystem>
#	include <memory_resource>
#	include <optional>
#	include <string_view>
#	include <variant>
#endif

#if ML_has_cxx20
#	include <compare>
#	include <concepts>
#	include <format>
#	include <ranges>
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// macro concat impl
#define ML_impl_concat(a, b)	a##b

// macro concat
#define ML_concat(a, b)			ML_impl_concat(a, b)

// macro name to string
#define ML_to_string(expr)		#expr

// macro value to string
#define ML_stringify(expr)		ML_to_string(expr)

// anonymous variables
#if defined(__COUNTER__)
#	define ML_anon(expr)		ML_concat(_ml_, ML_concat(expr, ML_concat(_, ML_concat(__COUNTER__, _))))
#elif defined(__LINE__)
#	define ML_anon(expr)		ML_concat(_ml_, ML_concat(expr, ML_concat(_, ML_concat(__LINE__, _))))
#else
#   define ML_anon(expr)		expr
#endif
#define ML_anon_v(type)			auto ML_anon(anon) = type

// assert
#ifndef ML_assert
#   define ML_assert(expr)		assert(expr)
#endif

// breakpoint
#if (!ML_is_debug)
#	define ML_breakpoint()		((void)0)
#elif defined(ML_cc_msvc)
#	define ML_breakpoint()		_CSTD __debugbreak()
#else
#	define ML_breakpoint()		_CSTD raise(SIGTRAP)
#endif

// environment
#ifndef ML_DISABLE_ENV
#	define ML_argc				__argc
#	ifndef ML_WIDE_ENV
#		define ML_argv			__argv
#		define ML_envp			_environ
#	else
#		define ML_argv			__wargv
#		define ML_envp			_wenviron
#	endif
#else
#	define ML_argc
#	define ML_argv
#	define ML_envp
#endif

// miscellaneous
#define _ML						::ml::
#define ML_addressof(ptr)		((void *)(_ML intmax_t)ptr)
#define ML_alias				using
#define ML_arraysize(arr)		(sizeof(arr) / sizeof(*arr))
#define ML_forward(var)			_ML std::forward<decltype(var)>(var)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias	int8_t		= typename ML_int8;
	ML_alias	int16_t		= typename ML_int16;
	ML_alias	int32_t		= typename ML_int32;
	ML_alias	int64_t		= typename ML_int64;

	ML_alias	uint8_t		= typename ML_uint8;
	ML_alias	uint16_t	= typename ML_uint16;
	ML_alias	uint32_t	= typename ML_uint32;
	ML_alias	uint64_t	= typename ML_uint64;

	ML_alias	byte_t		= typename ML_byte;
	ML_alias	char_t		= typename ML_char;
	ML_alias	intmax_t	= typename ML_intmax;
	ML_alias	uintmax_t	= typename ML_uintmax;
	ML_alias	ulong_t		= typename ML_ulong;

	ML_alias	float32_t	= typename ML_float32;
	ML_alias	float64_t	= typename ML_float64;
	ML_alias	float80_t	= typename ML_float80;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias	float_t		= typename float32_t;
	ML_alias	hash_t		= typename uint64_t;
	ML_alias	intptr_t	= typename intmax_t;
	ML_alias	ptrdiff_t	= typename intmax_t;
	ML_alias	max_align_t = typename float64_t;
	ML_alias	size_t		= typename uintmax_t;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias	cstring		= typename char const *;
	ML_alias	cwstring	= typename wchar_t const *;
#if ML_has_cxx20
	ML_alias	c8string	= typename char8_t const *;
#endif
	ML_alias	c16string	= typename char16_t const *;
	ML_alias	c32string	= typename char32_t const *;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace std		= ::std;
	namespace chrono	= _ML std::chrono;
	namespace fs		= _ML std::filesystem;
	namespace pmr		= _ML std::pmr;
#if ML_has_cxx20
	namespace ranges	= _ML std::ranges;
	namespace views		= _ML std::ranges::views;
#endif

	namespace literals
	{
		using namespace _ML std::chrono_literals;
		using namespace _ML std::string_view_literals;
	}
	using namespace literals;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_COMMON_HPP_