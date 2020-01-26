#ifndef _ML_STRONG_TYPEDEF_HPP_
#define _ML_STRONG_TYPEDEF_HPP_

// Source:
// https://github.com/SuperV1234/cppcon2015/blob/master/Other/ecs/Utils/StrongTypedef.hpp

#define ML_STRONG_TYPEDEF(Base, Name)                                           \
    class Name final                                                            \
    {                                                                           \
    private:                                                                    \
        Base value;                                                             \
                                                                                \
    public:                                                                     \
        inline Name() = default;                                                \
        inline Name(Name const & mX) = default;                                 \
        inline Name(Name && mX) = default;                                      \
        inline Name & operator=(Name const & rhs) = default;                    \
        inline Name & operator=(Name && rhs) = default;                         \
        inline constexpr explicit Name(Base mX) noexcept : value{mX} {}         \
        inline constexpr Name & operator=(Base rhs) noexcept                    \
        {                                                                       \
            value = rhs;                                                        \
            return *this;                                                       \
        }                                                                       \
        inline constexpr operator Base const &() const noexcept                 \
        {                                                                       \
            return value;                                                       \
        }                                                                       \
        inline constexpr operator Base &() noexcept                             \
        {                                                                       \
            return value;                                                       \
        }                                                                       \
        inline constexpr decltype(auto) operator==(Name const & rhs) noexcept   \
        {                                                                       \
            return value == rhs.value;                                          \
        }                                                                       \
        inline constexpr decltype(auto) operator!=(Name const & rhs) noexcept   \
        {                                                                       \
            return value != rhs.value;                                          \
        }                                                                       \
        inline constexpr decltype(auto) operator<(Name const & rhs) noexcept    \
        {                                                                       \
            return value < rhs.value;                                           \
        }                                                                       \
        inline constexpr decltype(auto) operator>(Name const & rhs) noexcept    \
        {                                                                       \
            return value > rhs.value;                                           \
        }                                                                       \
        inline constexpr decltype(auto) operator<=(Name const & rhs) noexcept   \
        {                                                                       \
            return value <= rhs.value;                                          \
        }                                                                       \
        inline constexpr decltype(auto) operator>=(Name const & rhs) noexcept   \
        {                                                                       \
            return value >= rhs.value;                                          \
        }                                                                       \
    };                                                                          \
                                                                                \
    static_assert(std::is_literal_type<Name>{}, "?")

#endif // !_ML_STRONG_TYPEDEF_HPP_