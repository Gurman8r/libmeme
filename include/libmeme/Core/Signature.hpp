#ifndef _ML_SIGNATURE_HPP_
#define _ML_SIGNATURE_HPP_

#include <libmeme/Common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Based on CTTI
// https://github.com/Manu343726/ctti
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/pretty_function.hpp

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_SIGNATURE_VIEW "std::basic_string_view<char,struct std::char_traits<char> >"

#if defined(ML_CC_MSVC)
#	define ML_SIGNATURE					__FUNCSIG__
#	define ML_TYPE_SIGNATURE_PREFIX		"class " ML_SIGNATURE_VIEW " __cdecl ml::signature::type<"
#	define ML_TYPE_SIGNATURE_SUFFIX		">(void)"
#	define ML_VALUE_SIGNATURE_PREFIX	"class " ML_SIGNATURE_VIEW " __cdecl ml::signature::value<"
#	define ML_VALUE_SIGNATURE_SEPARATOR	"; T Value = "
#	define ML_VALUE_SIGNATURE_SUFFIX	">(void)"
#elif defined(ML_CC_GCC)
#	define ML_SIGNATURE					__PRETTY_FUNCTION__
#	define ML_TYPE_SIGNATURE_PREFIX		"constexpr " ML_SIGNATURE_VIEW " ml::signature::type() [with T = "
#	define ML_TYPE_SIGNATURE_SUFFIX		"]"
#	define ML_VALUE_SIGNATURE_PREFIX	"constexpr " ML_SIGNATURE_VIEW " ml::signature::value() [with T = "
#	define ML_VALUE_SIGNATURE_SEPARATOR	"; Value = "
#	define ML_VALUE_SIGNATURE_SUFFIX	"]"
#elif defined(ML_CC_CLANG)
#	define ML_SIGNATURE					__PRETTY_FUNCTION__
#	define ML_TYPE_SIGNATURE_PREFIX		"" ML_SIGNATURE_VIEW " ml::signature::type() [T = "
#	define ML_TYPE_SIGNATURE_SUFFIX		"]"
#	define ML_VALUE_SIGNATURE_PREFIX	"" ML_SIGNATURE_VIEW " ml::signature::value() [T = "
#	define ML_VALUE_SIGNATUTE_SEPARATOR	"; Value = "
#	define ML_VALUE_SIGNATURE_SUFFIX	"]"
#else
#	error Type information is not available.
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Type Signature
namespace ml::signature
{
	template <class T
	> ML_NODISCARD static constexpr std::string_view type()
	{
		return { ML_SIGNATURE };
	}

	namespace detail
	{
		static constexpr std::tuple<std::string_view, std::string_view> type
		{
			ML_TYPE_SIGNATURE_PREFIX,
			ML_TYPE_SIGNATURE_SUFFIX
		};
	}
}

// Value Signature
namespace ml::signature
{
	template <class T, T Value
	> ML_NODISCARD static constexpr std::string_view value()
	{
		return { ML_SIGNATURE };
	}

	namespace detail
	{
		static constexpr std::tuple<std::string_view, std::string_view, std::string_view> value
		{
			ML_VALUE_SIGNATURE_PREFIX,
			ML_VALUE_SIGNATURE_SEPARATOR,
			ML_VALUE_SIGNATURE_SUFFIX
		};
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_SIGNATURE_HPP_