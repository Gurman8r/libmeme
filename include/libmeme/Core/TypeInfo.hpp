#ifndef _ML_TYPE_OF_HPP_
#define _ML_TYPE_OF_HPP_

#include <libmeme/Core/PrettyFunction.hpp>
#include <libmeme/Core/Hash.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct nameof;
	template <class ... T> struct typeof;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
				filter_template(s)))));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct nameof<T> final
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

	// name of type
	template <class T
	> static constexpr auto nameof_v{ nameof<T>::value };

	// hash of type
	template <class T
	> static constexpr auto hashof_v{ util::hash(nameof_v<T>) };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct ML_NODISCARD typeof<T> final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using view_type = typename nameof<>::view_type;

		using guid_type = typename nameof<>::guid_type;

		constexpr typeof() noexcept = default;

		static constexpr view_type const & name() noexcept { return m_name; }

		static constexpr guid_type const & guid() noexcept { return m_guid; }

		template <class ... U
		> constexpr auto compare(typeof<U...> const & value) noexcept
		{
			return ML_compare(m_guid, value.guid());
		}

	private:
		static constexpr view_type m_name{ nameof_v<T> };

		static constexpr guid_type m_guid{ hashof_v<T> };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct ML_NODISCARD typeof<> final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using view_type = typename nameof<>::view_type;

		using guid_type = typename nameof<>::guid_type;

		constexpr typeof() noexcept
			: m_name{}, m_guid{}
		{
		}

		template <class T
		> constexpr typeof(typeof<T> const & value) noexcept
			: m_name{ value.name() }, m_guid{ value.guid() }
		{
		}

		constexpr view_type const & name() const noexcept { return m_name; }

		constexpr guid_type const & guid() const noexcept { return m_guid; }

		template <class ... U
		> constexpr auto compare(typeof<U...> const & value) const noexcept
		{
			return ML_compare(m_guid, value.guid());
		}

	private:
		view_type m_name; guid_type m_guid;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T
	> static constexpr auto typeof_v{ typeof<T...>{} };
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T
	> ML_NODISCARD constexpr bool operator==(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return lhs.compare(rhs) == 0;
	}

	template <class ... T
	> ML_NODISCARD constexpr bool operator!=(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return lhs.compare(rhs) != 0;
	}

	template <class ... T
	> ML_NODISCARD constexpr bool operator<(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return lhs.compare(rhs) < 0;
	}

	template <class ... T
	> ML_NODISCARD constexpr bool operator>(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return lhs.compare(rhs) > 0;
	}

	template <class ... T
	> ML_NODISCARD constexpr bool operator<=(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return lhs.compare(rhs) <= 0;
	}

	template <class ... T
	> ML_NODISCARD constexpr bool operator>=(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return lhs.compare(rhs) >= 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class X, class ... Y
	> ML_NODISCARD constexpr bool operator==(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return lhs.compare(rhs) == 0;
	}

	template <class X, class ... Y
	> ML_NODISCARD constexpr bool operator!=(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return lhs.compare(rhs) != 0;
	}

	template <class X, class ... Y
	> ML_NODISCARD constexpr bool operator<(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return lhs.compare(rhs) < 0;
	}

	template <class X, class ... Y
	> ML_NODISCARD constexpr bool operator>(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return lhs.compare(rhs) > 0;
	}

	template <class X, class ... Y
	> ML_NODISCARD constexpr bool operator<=(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return lhs.compare(rhs) <= 0;
	}

	template <class X, class ... Y
	> ML_NODISCARD constexpr bool operator>=(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return lhs.compare(rhs) >= 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static_assert("tests"
		&& ml::nameof_v<bool>			== "bool"
		&& ml::nameof_v<int8_t>			== "signed char"
		&& ml::nameof_v<int16_t>		== "short"
		&& ml::nameof_v<int32_t>		== "int"
		&& ml::nameof_v<int64_t>		== "long long"
		&& ml::nameof_v<uint8_t>		== "unsigned char"
		&& ml::nameof_v<uint16_t>		== "unsigned short"
		&& ml::nameof_v<uint32_t>		== "unsigned int"
		&& ml::nameof_v<uint64_t>		== "unsigned long long"
		&& ml::nameof_v<float32_t>		== "float"
		&& ml::nameof_v<float64_t>		== "double"
		&& ml::nameof_v<float80_t>		== "long double"
		&& ml::nameof_v<char>			== "char"
		&& ml::nameof_v<wchar_t>		== "wchar_t"
		&& ml::nameof_v<char16_t>		== "char16_t"
		&& ml::nameof_v<char32_t>		== "char32_t"
		&& ml::nameof_v<cstring>		== "const char*"
		&& ml::nameof_v<cwstring>		== "const wchar_t*"
		&& ml::nameof_v<c16string>		== "const char16_t*"
		&& ml::nameof_v<c32string>		== "const char32_t*"
		&& ml::nameof_v<std::string>	== "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >"
		&& ml::nameof_v<std::wstring>	== "class std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >"
		&& ml::nameof_v<std::u16string> == "class std::basic_string<char16_t,struct std::char_traits<char16_t>,class std::allocator<char16_t> >"
		&& ml::nameof_v<std::u32string> == "class std::basic_string<char32_t,struct std::char_traits<char32_t>,class std::allocator<char32_t> >"
	
		, "test nameof<>");

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TYPE_OF_HPP_