#ifndef _ML_TYPE_OF_HPP_
#define _ML_TYPE_OF_HPP_

#include <libmeme/Core/NameOf.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct typeof;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct typeof<T> final
	{
		constexpr typeof() noexcept = default;

		static constexpr auto name() noexcept -> std::string_view const &
		{
			return m_name;
		}

		static constexpr auto hash() noexcept -> size_t const &
		{
			return m_hash;
		}

	private:
		static constexpr std::string_view m_name
		{
			nameof<>::filter_type(nameof<T>::value)
		};

		static constexpr size_t m_hash
		{
			hashof(m_name)
		};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> static constexpr auto const nameof_t{ typeof<T>{}.name() };

	template <class T
	> static constexpr auto const hashof_t{ typeof<T>{}.hash() };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct typeof<> final
	{
		constexpr typeof() noexcept
			: m_name{ "" }, m_hash{ 0 }
		{
		}

		template <class T> constexpr typeof(typeof<T> const & other) noexcept
			: m_name{ other.name() }, m_hash{ other.hash() }
		{
		}

		constexpr auto name() const noexcept -> std::string_view const &
		{
			return m_name;
		}

		constexpr auto hash() const noexcept -> size_t const &
		{
			return m_hash;
		}

	private:
		std::string_view m_name;
		size_t m_hash;
	};
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class ... T
	> inline ML_SERIALIZE(std::ostream & out, typeof<T...> const & value)
	{
		return out << value.name();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class ... T
	> constexpr bool operator==(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return !(lhs < rhs) && !(rhs < lhs);
	}

	template <
		class ... T
	> constexpr bool operator!=(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return !(lhs == rhs);
	}

	template <
		class ... T
	> constexpr bool operator<(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return (lhs.hash() < rhs.hash());
	}

	template <
		class ... T
	> constexpr bool operator>(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return !(lhs < rhs);
	}

	template <
		class ... T
	> constexpr bool operator<=(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <
		class ... T
	> constexpr bool operator>=(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class X, class ... Y
	> constexpr bool operator==(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return !(lhs < rhs) && !(rhs < lhs);
	}

	template <
		class X, class ... Y
	> constexpr bool operator!=(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return !(lhs == rhs);
	}

	template <
		class X, class ... Y
	> constexpr bool operator<(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return (lhs.hash() < rhs.hash());
	}

	template <
		class X, class ... Y
	> constexpr bool operator>(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return !(lhs < rhs);
	}

	template <
		class X, class ... Y
	> constexpr bool operator<=(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <
		class X, class ... Y
	> constexpr bool operator>=(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TYPE_OF_HPP_