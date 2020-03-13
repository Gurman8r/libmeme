#ifndef _ML_COMMON_HPP_
#define _ML_COMMON_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if (ML_HAS_CXX11 && ML_HAS_CXX14)
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

#if ML_HAS_CXX17
#	include <any>
#	include <filesystem>
#	include <memory_resource>
#	include <optional>
#	include <string_view>
#	include <variant>
#endif

#if ML_HAS_CXX20
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
#define ML_addressof(ptr)	((void *)(ML_INTMAX)ptr)
#define ML_arraysize(arr)	(sizeof(arr) / sizeof(*arr))
#define ML_forward(value)	std::forward<decltype(value)>(value)
#define ML_deserialize		std::istream & operator >>
#define ML_serialize		std::ostream & operator <<

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_ALIAS	int8_t		= typename ML_INT8;
	ML_ALIAS	int16_t		= typename ML_INT16;
	ML_ALIAS	int32_t		= typename ML_INT32;
	ML_ALIAS	int64_t		= typename ML_INT64;

	ML_ALIAS	uint8_t		= typename ML_UINT8;
	ML_ALIAS	uint16_t	= typename ML_UINT16;
	ML_ALIAS	uint32_t	= typename ML_UINT32;
	ML_ALIAS	uint64_t	= typename ML_UINT64;

	ML_ALIAS	byte_t		= typename ML_BYTE;
	ML_ALIAS	intmax_t	= typename ML_INTMAX;
	ML_ALIAS	uintmax_t	= typename ML_UINTMAX;

	ML_ALIAS	float32_t	= typename ML_FLOAT32;
	ML_ALIAS	float64_t	= typename ML_FLOAT64;
	ML_ALIAS	float80_t	= typename ML_FLOAT80;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_ALIAS	float_t		= typename float32_t;
	ML_ALIAS	hash_t		= typename uint64_t;
	ML_ALIAS	intptr_t	= typename intmax_t;
	ML_ALIAS	ptrdiff_t	= typename intmax_t;
	ML_ALIAS	max_align_t = typename float64_t;
	ML_ALIAS	size_t		= typename uintmax_t;
	ML_ALIAS	ulong_t		= typename unsigned long;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_ALIAS	cstring		= typename char const *;
	ML_ALIAS	cwstring	= typename wchar_t const *;
#if ML_HAS_CXX20
	ML_ALIAS	c8string	= typename char8_t const *;
#endif
	ML_ALIAS	c16string	= typename char16_t const *;
	ML_ALIAS	c32string	= typename char32_t const *;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace std			= ::std;
	namespace chrono		= _ML std::chrono;
	namespace filesystem	= _ML std::filesystem;
	namespace pmr			= _ML std::pmr;
#if ML_HAS_CXX20
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