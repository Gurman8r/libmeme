#ifndef _ML_ENUM_INFO_HPP_
#define _ML_ENUM_INFO_HPP_

#include <libmeme/Core/Cx.hpp>

namespace ml
{
	template <
		class _Enum,
		size_t _Cap = 32
	> struct enum_info
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static_assert(std::is_enum_v<_Enum>, "Type must be an enum");

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using enum_type = typename _Enum;
		using name_type = typename std::string_view;
		using desc_type = typename std::string_view;
		using self_type = typename enum_info<enum_type, _Cap>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Enums = typename cx::map<enum_type, size_t, _Cap>;
		using Names = typename cx::map<enum_type, name_type, _Cap>;
		using Descs = typename cx::map<enum_type, desc_type, _Cap>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Enums enums;
		Names names;
		Descs descs;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr enum_info() noexcept = default;

		constexpr explicit enum_info(
			cx::vector<enum_type, _Cap> && enums, 
			cx::vector<name_type, _Cap> && names, 
			cx::vector<desc_type, _Cap> && descs
		)	: enums{}, names{}, descs{}
		{
			for (size_t i = 0; i < enums.size(); i++)
			{
				this->enums.push_back(cx::make_pair(enums[i], i));
				this->names.push_back(cx::make_pair(enums[i], names[i]));
				this->descs.push_back(cx::make_pair(enums[i], descs[i]));
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr std::optional<enum_type> find_by_index(size_t value) const
		{
			auto const it{ this->enums.find_by_value(value) };
			return ((it != this->enums.cend())
				? std::make_optional(it->first)
				: std::nullopt
			);
		}

		constexpr std::optional<enum_type> find_by_name(name_type const & value) const
		{
			auto const it{ this->names.find_by_value(value) };
			return ((it != this->names.cend())
				? std::make_optional(it->first)
				: std::nullopt
			);
		}

		constexpr std::optional<enum_type> find_by_desc(desc_type const & value) const
		{
			auto const it{ this->descs.find_by_value(value) };
			return ((it != this->descs.cend())
				? std::make_optional(it->first)
				: std::nullopt
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr std::optional<size_t> get_index(enum_type value) const
		{
			auto const it{ this->enums.find_by_key(value) };
			return ((it != this->enums.cend()) 
				? std::make_optional(it->second) 
				: std::nullopt
			);
		}

		constexpr std::optional<name_type> get_name(enum_type value) const
		{
			auto const it{ this->names.find_by_key(value) };
			return ((it != this->names.cend())
				? std::make_optional(it->second)
				: std::nullopt
			);
		}

		constexpr std::optional<desc_type> get_desc(enum_type value) const
		{
			auto const it{ this->descs.find_by_key(value) };
			return ((it != this->descs.cend())
				? std::make_optional(it->second)
				: std::nullopt
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENUM_INFO_HPP_