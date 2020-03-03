#ifndef _ML_NAMEOF_HPP_
#define _ML_NAMEOF_HPP_

#include <libmeme/Core/PrettyFunction.hpp>

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

		using name_type = typename pretty_function::detail::name;

		using guid_type = typename decltype(util::hash(name_type{}));

		nameof() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static constexpr auto filter_prefix(name_type const & s, name_type const & pre)
		{
			bool const match{
				(s.size() >= pre.size() && (s.substr(0, pre.size()) == pre))
			};
			return match ? s.substr(pre.size()) : s;
		}

		ML_NODISCARD static constexpr auto filter_suffix(name_type const & s, name_type const & suf)
		{
			bool const match{
				(s.size() >= suf.size()) && ((s.substr(s.size() - suf.size(), suf.size()) == suf))
			};
			return match ? s.substr(0, (s.size() - suf.size())) : s;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static constexpr auto filter_type(name_type const & s) noexcept
		{
			auto const
				& a{ std::get<0>(pretty_function::detail::type) },
				& b{ std::get<1>(pretty_function::detail::type) };
			return filter_suffix(filter_prefix(s, a), b);
		}

		ML_NODISCARD static constexpr auto filter_value(name_type const & s) noexcept
		{
			auto const
				& a{ std::get<0>(pretty_function::detail::value) },
				& b{ std::get<1>(pretty_function::detail::value) },
				& c{ std::get<2>(pretty_function::detail::value) };
			return s; // NYI
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static constexpr auto filter_template(name_type const & s) noexcept
		{
			size_t const t{ s.find_first_of('<') };
			return (t != s.npos) ? s.substr(0, t) : s;
		}

		ML_NODISCARD static constexpr auto filter_namespace(name_type const & s) noexcept
		{
			size_t const t{ s.find_first_of('<') }; // check for template
			return (t != s.npos)
				? s.substr(s.substr(0, t).find_last_of(':') + 1)
				: s.substr(s.find_last_of(':') + 1);
		}

		ML_NODISCARD static constexpr auto filter_struct(name_type const & s) noexcept
		{
			return filter_prefix(s, "struct ");
		}

		ML_NODISCARD static constexpr auto filter_class(name_type const & s) noexcept
		{
			return filter_prefix(s, "class ");
		}

		ML_NODISCARD static constexpr auto filter_union(name_type const & s) noexcept
		{
			return filter_prefix(s, "union ");
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static constexpr auto filter_all(name_type const & s) noexcept
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
		&& nameof_v<cstring>		== "const char*"
		&& nameof_v<cwstring>		== "const wchar_t*"
		&& nameof_v<c16string>		== "const char16_t*"
		&& nameof_v<c32string>		== "const char32_t*"
		&& nameof_v<std::string>	== "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >"
		&& nameof_v<std::wstring>	== "class std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >"
		&& nameof_v<std::u16string> == "class std::basic_string<char16_t,struct std::char_traits<char16_t>,class std::allocator<char16_t> >"
		&& nameof_v<std::u32string> == "class std::basic_string<char32_t,struct std::char_traits<char32_t>,class std::allocator<char32_t> >"
	
		, "intellisense often struggles with nameof");

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NAMEOF_HPP_