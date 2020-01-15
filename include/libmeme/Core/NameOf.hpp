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

		ML_NODISCARD static constexpr std::string_view trim_front(std::string_view const & value)
		{
			return ((!value.empty() && (value.front() == ' ' || value.front() == '\t'))
				? trim_front(value.substr(1))
				: value
			);
		}

		ML_NODISCARD static constexpr std::string_view trim_back(std::string_view const & value)
		{
			return ((value.size() && (value.back() == ' ' || value.back() == '\t'))
				? trim_back(value.substr(0, value.size() - 1))
				: value
			);
		}

		ML_NODISCARD static constexpr std::string_view trim(std::string_view const & value)
		{
			return trim_front(trim_back(value));
		}

		ML_NODISCARD static constexpr std::string_view filter_prefix(std::string_view const & value, std::string_view const & pre)
		{
			return ((value.size() >= pre.size() && (value.substr(0, pre.size()) == pre))
				? value.substr(pre.size())
				: value
			);
		}

		ML_NODISCARD static constexpr std::string_view filter_suffix(std::string_view const & value, std::string_view const & suf)
		{
			return ((value.size() >= suf.size()) && ((value.substr(value.size() - suf.size(), suf.size()) == suf))
				? value.substr(0, (value.size() - suf.size()))
				: value
			);
		}

		ML_NODISCARD static constexpr std::string_view filter_signature_type(std::string_view const & value)
		{
			return filter_suffix(filter_prefix(value, 
				std::get<0>(signature::detail::type)),
				std::get<1>(signature::detail::type)
			);
		}

		ML_NODISCARD static constexpr std::string_view filter_namespace(std::string_view const & value)
		{
			return value.substr(value.find_first_of(':') + 2);
		}

		ML_NODISCARD static constexpr std::string_view filter_struct(std::string_view const & value)
		{
			return filter_prefix(value, "struct ");
		}

		ML_NODISCARD static constexpr std::string_view filter_class(std::string_view const & value)
		{
			return filter_prefix(value, "class ");
		}

		ML_NODISCARD static constexpr std::string_view filter_constexpr(std::string_view const & value)
		{
			return filter_prefix(value, "constexpr ");
		}

		ML_NODISCARD static constexpr std::string_view filter_const(std::string_view const & value)
		{
			return filter_prefix(value, "const ");
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct nameof<int64_t>
	{
		static constexpr auto value{ "long long" }; // __int64
	};

	template <> struct nameof<uint64_t>
	{
		static constexpr auto value{ "unsigned long long" }; // unsigned __int64
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NAMEOF_HPP_