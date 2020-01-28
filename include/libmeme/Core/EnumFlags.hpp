#ifndef _ML_ENUM_FLAGS_HPP_
#define _ML_ENUM_FLAGS_HPP_

#include <libmeme/Common.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// operator overloads to make using enum class more hospitable
#define ML_ENUM_FLAGS(Base, Enum)												\
	/* == */																	\
	constexpr bool operator==(Enum const lhs, Base const rhs) noexcept			\
	{																			\
		return static_cast<Base>(lhs) == rhs;									\
	}																			\
	constexpr bool operator==(Base const lhs, Enum const rhs) noexcept			\
	{																			\
		return lhs == static_cast<Base>(rhs);									\
	}																			\
																				\
	/* != */																	\
	constexpr bool operator!=(Enum const lhs, Base const rhs) noexcept			\
	{																			\
		return static_cast<Base>(lhs) != rhs;									\
	}																			\
	constexpr bool operator!=(Base const lhs, Enum const rhs) noexcept			\
	{																			\
		return lhs != static_cast<Base>(rhs);									\
	}																			\
																				\
	/* ! ~ + - */																\
	constexpr decltype(auto) operator!(Enum const value) noexcept				\
	{																			\
		return !static_cast<Base>(value);										\
	}																			\
	constexpr decltype(auto) operator~(Enum const value) noexcept				\
	{																			\
		return ~static_cast<Base>(value);										\
	}																			\
	constexpr decltype(auto) operator+(Enum const value) noexcept				\
	{																			\
		return +static_cast<Base>(value);										\
	}																			\
	constexpr decltype(auto) operator-(Enum const value) noexcept				\
	{																			\
		return -static_cast<Base>(value);										\
	}																			\
																				\
	/* | */																		\
	constexpr decltype(auto) operator|(Base const lhs, Enum const rhs) noexcept	\
	{																			\
		return static_cast<Base>(lhs | static_cast<Base>(rhs));					\
	}																			\
	constexpr decltype(auto) operator|(Enum const lhs, Base const rhs) noexcept	\
	{																			\
		return static_cast<Enum>(static_cast<Base>(lhs) | rhs);					\
	}																			\
	constexpr decltype(auto) operator|=(Base & lhs, Enum const rhs) noexcept	\
	{																			\
		return lhs = lhs | rhs;													\
	}																			\
	constexpr decltype(auto) operator|=(Enum & lhs, Base const rhs) noexcept	\
	{																			\
		return lhs = lhs | rhs;													\
	}																			\
	constexpr decltype(auto) operator|(Enum const lhs, Enum const rhs) noexcept	\
	{																			\
		return lhs | static_cast<Base>(rhs);									\
	}																			\
	constexpr decltype(auto) operator|=(Enum & lhs, Enum const rhs) noexcept	\
	{																			\
		return lhs = lhs | rhs;													\
	}																			\
																				\
	/* & */																		\
	constexpr decltype(auto) operator&(Base const lhs, Enum const rhs) noexcept	\
	{																			\
		return static_cast<Base>(lhs & static_cast<Base>(rhs));					\
	}																			\
	constexpr decltype(auto) operator&(Enum const lhs, Base const rhs) noexcept	\
	{																			\
		return static_cast<Enum>(static_cast<Base>(lhs) & rhs);					\
	}																			\
	constexpr decltype(auto) operator&=(Base & lhs, Enum const rhs) noexcept	\
	{																			\
		return lhs = lhs & rhs;													\
	}																			\
	constexpr decltype(auto) operator&=(Enum & lhs, Base const rhs) noexcept	\
	{																			\
		return lhs = lhs & rhs;													\
	}																			\
	constexpr decltype(auto) operator&(Enum const lhs, Enum const rhs) noexcept	\
	{																			\
		return lhs & static_cast<Base>(rhs);									\
	}																			\
	constexpr decltype(auto) operator&=(Enum & lhs, Enum const rhs) noexcept	\
	{																			\
		return lhs = lhs & rhs;													\
	}																			\
																				\
	/* sanity check */															\
	static_assert(std::is_enum_v<Enum>)
	

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_ENUM_FLAGS_HPP_