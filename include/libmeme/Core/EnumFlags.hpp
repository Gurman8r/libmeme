#ifndef _ML_ENUM_FLAGS_HPP_
#define _ML_ENUM_FLAGS_HPP_

#include <libmeme/Common.hpp>

//using enum_base = int;
//enum class enum_flags : enum_base {};

#define ML_ENUM_FLAGS(enum_base, enum_flags) \
constexpr bool operator==(enum_flags const lhs, enum_base const rhs) noexcept					\
{																								\
	return static_cast<enum_base>(lhs) == rhs;													\
}																								\
constexpr bool operator==(enum_base const lhs, enum_flags const rhs) noexcept					\
{																								\
	return lhs == static_cast<enum_base>(rhs);													\
}																								\
																								\
constexpr bool operator!=(enum_flags const lhs, enum_base const rhs) noexcept					\
{																								\
	return static_cast<enum_base>(lhs) != rhs;													\
}																								\
constexpr bool operator!=(enum_base const lhs, enum_flags const rhs) noexcept					\
{																								\
	return lhs != static_cast<enum_base>(rhs);													\
}																								\
																								\
constexpr decltype(auto) operator~(enum_flags const value) noexcept								\
{																								\
	return ~static_cast<enum_base>(value);														\
}																								\
																								\
constexpr decltype(auto) operator|(enum_flags const lhs, enum_base const rhs) noexcept			\
{																								\
	return static_cast<enum_flags>(static_cast<enum_base>(lhs) | rhs);							\
}																								\
constexpr decltype(auto) operator&(enum_flags const lhs, enum_base const rhs) noexcept			\
{																								\
	return static_cast<enum_flags>(static_cast<enum_base>(lhs) & rhs);							\
}																								\
constexpr decltype(auto) operator|=(enum_flags & lhs, enum_base const rhs) noexcept				\
{																								\
	return lhs = lhs | rhs;																		\
}																								\
constexpr decltype(auto) operator&=(enum_flags & lhs, enum_base const rhs) noexcept				\
{																								\
	return lhs = lhs & rhs;																		\
}																								\
																								\
constexpr decltype(auto) operator|(enum_flags const lhs, enum_flags const rhs) noexcept			\
{																								\
	return lhs | static_cast<enum_base>(rhs);													\
}																								\
constexpr decltype(auto) operator&(enum_flags const lhs, enum_flags const rhs) noexcept			\
{																								\
	return lhs & static_cast<enum_base>(rhs);													\
}																								\
constexpr decltype(auto) operator|=(enum_flags & lhs, enum_flags const rhs) noexcept			\
{																								\
	return lhs = lhs | rhs;																		\
}																								\
constexpr decltype(auto) operator&=(enum_flags & lhs, enum_flags const rhs) noexcept			\
{																								\
	return lhs = lhs & rhs;																		\
}																								\
																								\
static_assert(std::is_enum_v<enum_flags>)

#endif // !_ML_ENUM_FLAGS_HPP_