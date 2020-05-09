#ifndef _ML_PRETTY_FUNCTION_HPP_
#define _ML_PRETTY_FUNCTION_HPP_

// Sources:
// https://github.com/Manu343726/ctti
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/pretty_function.hpp

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ML_CTTI_STR_TYPE
#define ML_CTTI_STR_TYPE std::basic_string_view<char,struct std::char_traits<char> >
#endif
#define ML_CTTI_STR_IMPL ML_stringify(ML_CTTI_STR_TYPE)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_cc_msvc)
#	define ML_PRETTY_FUNCTION		__FUNCSIG__
#	define ML_PRETTY_TYPE_PREFIX	"class " ML_CTTI_STR_IMPL " __cdecl ml::pretty_function::type<"
#	define ML_PRETTY_TYPE_SUFFIX	">(void)"
#	define ML_PRETTY_VALUE_PREFIX	"class " ML_CTTI_STR_IMPL " __cdecl ml::pretty_function::value<"
#	define ML_PRETTY_VALUE_DELIM	"; T Value = "
#	define ML_PRETTY_VALUE_SUFFIX	">(void)"
#elif defined(ML_cc_clang)
#	define ML_PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	define ML_PRETTY_TYPE_PREFIX	ML_CTTI_STR_IMPL " ml::pretty_function::type() [T = "
#	define ML_PRETTY_TYPE_SUFFIX	"]"
#	define ML_PRETTY_VALUE_PREFIX	ML_CTTI_STR_IMPL " ml::pretty_function::value() [T = "
#	define ML_PRETTY_VALUE_DELIM	"; Value = "
#	define ML_PRETTY_VALUE_SUFFIX	"]"
#elif defined(ML_cc_gcc)
#	define ML_PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	define ML_PRETTY_TYPE_PREFIX	"constexpr " ML_CTTI_STR_IMPL " ml::pretty_function::type() [with T = "
#	define ML_PRETTY_TYPE_SUFFIX	"]"
#	define ML_PRETTY_VALUE_PREFIX	"constexpr " ML_CTTI_STR_IMPL " ml::pretty_function::value() [with T = "
#	define ML_PRETTY_VALUE_DELIM	"; Value = "
#	define ML_PRETTY_VALUE_SUFFIX	"]"
#else
#	error Type information is not available.
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::pretty_function
{
	ML_alias string_type = typename ML_CTTI_STR_TYPE;

	template <class T
	> ML_NODISCARD static constexpr string_type type()
	{
		return { ML_PRETTY_FUNCTION };
	}

	template <class T, T Value
	> ML_NODISCARD static constexpr string_type value()
	{
		return { ML_PRETTY_FUNCTION };
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PRETTY_FUNCTION_HPP_