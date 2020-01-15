#ifndef _ML_ENUM_INFO_HPP_
#define _ML_ENUM_INFO_HPP_

#include <libmeme/Core/Cx.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	template <class E, size_t Size = 32
	> struct enum_info
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static_assert(std::is_enum_v<E>, "Type must be an enum");

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using enum_type = typename E;
		using name_type = typename std::string_view;
		using desc_type = typename std::string_view;
		using self_type = typename enum_info<enum_type, Size>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Enums = typename cx::map<enum_type, size_t, Size>;
		using Names = typename cx::map<enum_type, name_type, Size>;
		using Descs = typename cx::map<enum_type, desc_type, Size>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Enums enums;
		Names names;
		Descs descs;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr enum_info() noexcept = default;

		constexpr explicit enum_info(
			cx::vector<enum_type, Size> && enums, 
			cx::vector<name_type, Size> && names, 
			cx::vector<desc_type, Size> && descs
		)	: enums{}, names{}, descs{}
		{
			for (size_t i = 0; i < enums.size(); ++i)
			{
				this->enums.push_back(cx::make_pair(enums[i], i));
				this->names.push_back(cx::make_pair(enums[i], names[i]));
				this->descs.push_back(cx::make_pair(enums[i], descs[i]));
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr std::optional<enum_type> find_by_index(size_t value) const
		{
			auto const it{ this->enums.find_by_value(value) };
			return ((it != this->enums.cend())
				? std::make_optional(it->first)
				: std::nullopt
			);
		}

		ML_NODISCARD constexpr std::optional<enum_type> find_by_name(name_type const & value) const
		{
			auto const it{ this->names.find_by_value(value) };
			return ((it != this->names.cend())
				? std::make_optional(it->first)
				: std::nullopt
			);
		}

		ML_NODISCARD constexpr std::optional<enum_type> find_by_desc(desc_type const & value) const
		{
			auto const it{ this->descs.find_by_value(value) };
			return ((it != this->descs.cend())
				? std::make_optional(it->first)
				: std::nullopt
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr std::optional<size_t> get_index(enum_type value) const
		{
			auto const it{ this->enums.find_by_key(value) };
			return ((it != this->enums.cend()) 
				? std::make_optional(it->second) 
				: std::nullopt
			);
		}

		ML_NODISCARD constexpr std::optional<name_type> get_name(enum_type value) const
		{
			auto const it{ this->names.find_by_key(value) };
			return ((it != this->names.cend())
				? std::make_optional(it->second)
				: std::nullopt
			);
		}

		ML_NODISCARD constexpr std::optional<desc_type> get_desc(enum_type value) const
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Macro Version
#define ML_GEN_ENUM_INFO(TYPE)														\
static constexpr bool value_at(int32_t i, TYPE & value)								\
{																					\
	return alg::value_at(i, value, TYPE##_values);									\
}																					\
static constexpr int32_t index_of(TYPE value)										\
{																					\
	return alg::index_of(value, TYPE##_values);										\
}																					\
static constexpr C_String name_of(TYPE value)										\
{																					\
	return alg::at_index(index_of(value), TYPE##_names);							\
}																					\
static constexpr C_String raw_name_of(TYPE value)									\
{																					\
	return alg::at_index(index_of(value), TYPE##_raw_names);						\
}																					\
static constexpr C_String desc_of(TYPE value)										\
{																					\
	return alg::at_index(index_of(value), TYPE##_descriptions);						\
}																					\
static constexpr TYPE find_by_name(C_String value, TYPE dv = TYPE##_values[0])		\
{																					\
	int32_t i = alg::index_of(value, TYPE##_names);									\
	return ((i != -1) ? TYPE##_values[i] : dv);										\
}																					\
static constexpr TYPE find_by_raw_name(C_String value, TYPE dv = TYPE##_values[0])	\
{																					\
	int32_t i = alg::index_of(value, TYPE##_raw_names);								\
	return ((i != -1) ? TYPE##_values[i] : dv);										\
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_ENUM_INFO_HPP_