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

// assert
#ifndef ML_assert
#define ML_assert(expr) assert(expr)
#endif

// environment
#define ML_argc		__argc
#define ML_argv		__argv
#define ML_wargv	__wargv
#define ML_envp		_environ
#define ML_wenvp	_wenviron

// utils
#define ML_addressof(ptr)	((void *)(ML_intmax)ptr)
#define ML_arraysize(arr)	(sizeof(arr) / sizeof(*arr))
#define ML_forward(value)	std::forward<decltype(value)>(value)
#define ML_deserialize		std::istream & operator >>
#define ML_serialize		std::ostream & operator <<

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

	namespace std			= ::std;
	namespace chrono		= _ML std::chrono;
	namespace filesystem	= _ML std::filesystem;
	namespace pmr			= _ML std::pmr;
#if ML_has_cxx20
	namespace ranges		= _ML std::ranges;
	namespace views			= _ML std::ranges::views;
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace literals
	{
		using namespace _ML std::chrono_literals;
		using namespace _ML std::string_literals;
		using namespace _ML std::string_view_literals;
	}
	using namespace literals;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_COMMON_HPP_