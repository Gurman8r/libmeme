#ifndef _ML_NAMEOF_HPP_
#define _ML_NAMEOF_HPP_

// Based on CTTI
// https://github.com/Manu343726/ctti
// https://github.com/Manu343726/ctti/blob/master/include/ctti/nameof.hpp
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/name_filters.hpp

#include <libmeme/Core/PrettyFunction.hpp>
#include <libmeme/Core/Hash.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct nameof;

	template <> struct nameof<> final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		nameof() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using view_type = typename pretty_function::view_type;

		using guid_type = typename decltype(util::hash(view_type{}));

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static constexpr auto filter_prefix(view_type s, view_type v)
		{
			bool const match
			{
				(s.size() >= v.size() && (s.substr(0, v.size()) == v))
			};
			return match ? s.substr(v.size()) : s;
		}

		ML_NODISCARD static constexpr auto filter_suffix(view_type s, view_type v)
		{
			bool const match
			{
				(s.size() >= v.size()) && (s.substr(s.size() - v.size(), v.size()) == v)
			};
			return match ? s.substr(0, (s.size() - v.size())) : s;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static constexpr auto filter_type(view_type s) noexcept
		{
			constexpr auto const
				pre{ ML_PRETTY_TYPE_PREFIX },
				suf{ ML_PRETTY_TYPE_SUFFIX };
			return filter_suffix(filter_prefix(s, pre), suf);
		}

		ML_NODISCARD static constexpr auto filter_value(view_type s) noexcept
		{
			constexpr auto const
				pre{ ML_PRETTY_VALUE_PREFIX },
				dlm{ ML_PRETTY_VALUE_DELIM },
				suf{ ML_PRETTY_VALUE_SUFFIX };
			return s; // NYI
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static constexpr auto filter_template(view_type s) noexcept
		{
			size_t const t{ s.find_first_of('<') };
			return (t != s.npos) ? s.substr(0, t) : s;
		}

		ML_NODISCARD static constexpr auto filter_namespace(view_type s) noexcept
		{
			size_t const t{ s.find_first_of('<') }; // check for template
			return (t != s.npos)
				? s.substr(s.substr(0, t).find_last_of(':') + 1)
				: s.substr(s.find_last_of(':') + 1);
		}

		ML_NODISCARD static constexpr auto filter_struct(view_type s) noexcept
		{
			return filter_prefix(s, "struct ");
		}

		ML_NODISCARD static constexpr auto filter_class(view_type s) noexcept
		{
			return filter_prefix(s, "class ");
		}

		ML_NODISCARD static constexpr auto filter_union(view_type s) noexcept
		{
			return filter_prefix(s, "union ");
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static constexpr auto filter_all(view_type s) noexcept
		{
			return
				filter_class(
				filter_struct(
				filter_union(
				filter_namespace(
				filter_template(
				s)))));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> struct nameof<T> final
	{
		static constexpr auto value{ nameof<>::filter_type(pretty_function::type<T>()) };
	};

#ifdef ML_cc_msvc
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

	template <class T
	> static constexpr auto nameof_v{ nameof<T>::value };

	template <class T
	> static constexpr auto hashof_v{ util::hash(nameof_v<T>) };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::tests
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static_assert("tests"
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
		&& nameof_v<cstring>		== "const char*"
		&& nameof_v<cwstring>		== "const wchar_t*"
		&& nameof_v<c16string>		== "const char16_t*"
		&& nameof_v<c32string>		== "const char32_t*"
		&& nameof_v<std::string>	== "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >"
		&& nameof_v<std::wstring>	== "class std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >"
		&& nameof_v<std::u16string> == "class std::basic_string<char16_t,struct std::char_traits<char16_t>,class std::allocator<char16_t> >"
		&& nameof_v<std::u32string> == "class std::basic_string<char32_t,struct std::char_traits<char32_t>,class std::allocator<char32_t> >"
	
		, "test nameof<>");

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NAMEOF_HPP_