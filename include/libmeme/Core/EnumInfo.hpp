#ifndef _ML_ENUM_INFO_HPP_
#define _ML_ENUM_INFO_HPP_

#include <libmeme/Core/Utility.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_GEN_ENUM_INFO(TYPE)														\
static constexpr bool value_at(int32_t i, TYPE & value)								\
{																					\
	return util::value_at(i, value, TYPE##_values);									\
}																					\
static constexpr int32_t index_of(TYPE value)										\
{																					\
	return util::index_of(value, TYPE##_values);										\
}																					\
static constexpr C_string name_of(TYPE value)										\
{																					\
	return util::at_index(index_of(value), TYPE##_names);							\
}																					\
static constexpr C_string raw_name_of(TYPE value)									\
{																					\
	return util::at_index(index_of(value), TYPE##_raw_names);						\
}																					\
static constexpr C_string desc_of(TYPE value)										\
{																					\
	return util::at_index(index_of(value), TYPE##_descriptions);						\
}																					\
static constexpr TYPE find_by_name(C_string value, TYPE dv = TYPE##_values[0])		\
{																					\
	int32_t i = util::index_of(value, TYPE##_names);									\
	return ((i != -1) ? TYPE##_values[i] : dv);										\
}																					\
static constexpr TYPE find_by_raw_name(C_string value, TYPE dv = TYPE##_values[0])	\
{																					\
	int32_t i = util::index_of(value, TYPE##_raw_names);								\
	return ((i != -1) ? TYPE##_values[i] : dv);										\
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_ENUM_INFO_HPP_