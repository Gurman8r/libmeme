#ifndef _ML_PRETTY_FUNCTION_HPP_
#define _ML_PRETTY_FUNCTION_HPP_

#include <libmeme/Core/Utility.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Based on CTTI
// https://github.com/Manu343726/ctti
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/pretty_function.hpp

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_SIGNATURE_T "std::basic_string_view<char,struct std::char_traits<char> >"

#if defined(ML_CC_MSVC)
#	define ML_PRETTY_FUNCTION		__FUNCSIG__
#	define ML_PRETTY_TYPE_PREFIX	"class " ML_SIGNATURE_T " __cdecl ml::pretty_function::type<"
#	define ML_PRETTY_TYPE_SUFFIX	">(void)"
#	define ML_PRETTY_VALUE_PREFIX	"class " ML_SIGNATURE_T " __cdecl ml::pretty_function::value<"
#	define ML_PRETTY_VALUE_DELIM	"; T Value = "
#	define ML_PRETTY_VALUE_SUFFIX	">(void)"
#elif defined(ML_CC_GCC)
#	define ML_PRETTY_FUNCTION			__PRETTY_FUNCTION__
#	define ML_PRETTY_TYPE_PREFIX	"constexpr " ML_SIGNATURE_T " ml::pretty_function::type() [with T = "
#	define ML_PRETTY_TYPE_SUFFIX	"]"
#	define ML_PRETTY_VALUE_PREFIX	"constexpr " ML_SIGNATURE_T " ml::pretty_function::value() [with T = "
#	define ML_PRETTY_VALUE_DELIM	"; Value = "
#	define ML_PRETTY_VALUE_SUFFIX	"]"
#elif defined(ML_CC_CLANG)
#	define ML_PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	define ML_PRETTY_TYPE_PREFIX	ML_SIGNATURE_T " ml::pretty_function::type() [T = "
#	define ML_PRETTY_TYPE_SUFFIX	"]"
#	define ML_PRETTY_VALUE_PREFIX	ML_SIGNATURE_T " ml::pretty_function::value() [T = "
#	define ML_PRETTY_VALUE_DELIM	"; Value = "
#	define ML_PRETTY_VALUE_SUFFIX	"]"
#else
#	error Type information is not available.
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Type Signature
namespace ml::pretty_function
{
	template <class T
	> ML_NODISCARD static constexpr std::string_view type()
	{
		return { ML_PRETTY_FUNCTION };
	}

	namespace detail
	{
		static constexpr std::tuple<std::string_view, std::string_view> type
		{
			ML_PRETTY_TYPE_PREFIX,
			ML_PRETTY_TYPE_SUFFIX
		};
	}
}

// Value Signature
namespace ml::pretty_function
{
	template <class T, T Value
	> ML_NODISCARD static constexpr std::string_view value()
	{
		return { ML_PRETTY_FUNCTION };
	}

	namespace detail
	{
		static constexpr std::tuple<std::string_view, std::string_view, std::string_view> value
		{
			ML_PRETTY_VALUE_PREFIX,
			ML_PRETTY_VALUE_DELIM,
			ML_PRETTY_VALUE_SUFFIX
		};
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PRETTY_FUNCTION_HPP_