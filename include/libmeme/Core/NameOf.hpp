#ifndef _ML_NAMEOF_HPP_
#define _ML_NAMEOF_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Based on CTTI
// https://github.com/Manu343726/ctti
// https://github.com/Manu343726/ctti/blob/master/include/ctti/nameof.hpp
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/name_filters.hpp

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/Signature.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct nameof;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct nameof<T> final
	{
		nameof() = delete;

		static constexpr std::string_view value{ signature::type<T>() };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct nameof<> final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		nameof() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static constexpr std::string_view filter_prefix(std::string_view const & s, std::string_view const & pre)
		{
			return ((s.size() >= pre.size() && (s.substr(0, pre.size()) == pre))
				? s.substr(pre.size())
				: s
			);
		}

		ML_NODISCARD static constexpr std::string_view filter_suffix(std::string_view const & s, std::string_view const & suf)
		{
			return ((s.size() >= suf.size()) && ((s.substr(s.size() - suf.size(), suf.size()) == suf))
				? s.substr(0, (s.size() - suf.size()))
				: s
			);
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
			return s; // NYI
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_CC_MSVC
	template <> struct nameof<int64_t> final
	{
		static constexpr auto value{ "long long"sv };
	};

	template <> struct nameof<uint64_t> final
	{
		static constexpr auto value{ "unsigned long long"sv };
	};
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NAMEOF_HPP_