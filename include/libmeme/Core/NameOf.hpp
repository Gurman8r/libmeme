#ifndef _ML_NAMEOF_HPP_
#define _ML_NAMEOF_HPP_

#include <libmeme/Core/Signature.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Based on CTTI
// https://github.com/Manu343726/ctti
// https://github.com/Manu343726/ctti/blob/master/include/ctti/nameof.hpp
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/name_filters.hpp

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct nameof;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct nameof<> final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		nameof() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static constexpr std::string_view filter_prefix(std::string_view const & s, std::string_view const & pre)
		{
			return (s.size() >= pre.size() && (s.substr(0, pre.size()) == pre))
				? s.substr(pre.size())
				: s;
		}

		ML_NODISCARD static constexpr std::string_view filter_suffix(std::string_view const & s, std::string_view const & suf)
		{
			return (s.size() >= suf.size()) && ((s.substr(s.size() - suf.size(), suf.size()) == suf))
				? s.substr(0, (s.size() - suf.size()))
				: s;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static constexpr std::string_view filter_type(std::string_view const & s)
		{
			return filter_suffix(filter_prefix(s,
				std::get<0>(signature::detail::type)),
				std::get<1>(signature::detail::type)
			);
		}

		ML_NODISCARD static constexpr std::string_view filter_value(std::string_view const & s)
		{
			return s; // NYI - currently no need for this functionality
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static constexpr std::string_view filter_namespace(std::string_view const & s)
		{
			return s.substr(s.find_first_of(':') + 2);
		}

		ML_NODISCARD static constexpr std::string_view filter_struct(std::string_view const & s)
		{
			return filter_prefix(s, "struct ");
		}

		ML_NODISCARD static constexpr std::string_view filter_class(std::string_view const & s)
		{
			return filter_prefix(s, "class ");
		}

		ML_NODISCARD static constexpr std::string_view filter_constexpr(std::string_view const & s)
		{
			return filter_prefix(s, "constexpr ");
		}

		ML_NODISCARD static constexpr std::string_view filter_const(std::string_view const & s)
		{
			return filter_prefix(s, "const ");
		}

		ML_NODISCARD static constexpr std::string_view filter_union(std::string_view const & s)
		{
			return filter_prefix(s, "union ");
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> struct nameof<T> final
	{
		static constexpr auto value{ nameof<>::filter_type(signature::type<T>()) };
	};

	template <class T
	> static constexpr auto nameof_v{ nameof<T>::value };

	template <class T
	> static constexpr auto const hashof_v{ util::hash(nameof_v<T>) };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_CC_MSVC
	template <> struct nameof<int64_t> final
	{
		static constexpr auto value{ "long long"sv }; // __int64
	};

	template <> struct nameof<uint64_t> final
	{
		static constexpr auto value{ "unsigned long long"sv }; // unsigned __int64
	};
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Tests
namespace ml::tests
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static_assert("Test Static Type Info"
		&& nameof_v<bool>			== "bool"
		&& nameof_v<int8_t>			== "signed char"
		&& nameof_v<int16_t>		== "short"
		&& nameof_v<int32_t>		== "int"
		&& nameof_v<int64_t>		== "long long"
		&& nameof_v<uint8_t>		== "unsigned char"
		&& nameof_v<uint16_t>		== "unsigned short"
		&& nameof_v<uint32_t>		== "unsigned int"
		&& nameof_v<uint64_t>		== "unsigned long long"
		&& nameof_v<float32_t>		== "float"
		&& nameof_v<float64_t>		== "double"
		&& nameof_v<float80_t>		== "long double"
		&& nameof_v<char>			== "char"
		&& nameof_v<wchar_t>		== "wchar_t"
		&& nameof_v<char16_t>		== "char16_t"
		&& nameof_v<char32_t>		== "char32_t"
		&& nameof_v<C_string>		== "const char*"
		&& nameof_v<C_wstring>		== "const wchar_t*"
		&& nameof_v<C_u16string>	== "const char16_t*"
		&& nameof_v<C_u32string>	== "const char32_t*"
		&& nameof_v<std::string>	== "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >"
		&& nameof_v<std::wstring>	== "class std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >"
		&& nameof_v<std::u16string> == "class std::basic_string<char16_t,struct std::char_traits<char16_t>,class std::allocator<char16_t> >"
		&& nameof_v<std::u32string> == "class std::basic_string<char32_t,struct std::char_traits<char32_t>,class std::allocator<char32_t> >"
	);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NAMEOF_HPP_