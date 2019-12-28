#ifndef _ML_TYPE_OF_HPP_
#define _ML_TYPE_OF_HPP_

#include <libmeme/Core/NameOf.hpp>
#include <libmeme/Core/Hash.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct typeof;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct typeof<T> final
	{
		using name_t = typename std::string_view;

		constexpr typeof() noexcept = default;

		static constexpr auto name() noexcept -> name_t const & { return m_name; }
		
		static constexpr auto hash() noexcept -> hash_t const & { return m_hash; }

	private:
		static constexpr name_t m_name
		{
			nameof<>::filter_signature_type(nameof<T>::value)
		};

		static constexpr hash_t m_hash
		{
			Hash{ m_name }
		};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> static constexpr auto const typeof_v{ typeof<T>{} };

	template <
		class T
	> static constexpr auto const & hashof_v{ typeof_v<T>.hash() };

	template <
		class T
	> static constexpr auto const & nameof_v{ typeof_v<T>.name() };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct typeof<> final
	{
		using name_t = typename std::string_view;

		constexpr typeof() noexcept
			: m_name{ "" }, m_hash{ 0 }
		{
		}

		template <class T> constexpr typeof(typeof<T> const & other) noexcept
			: m_name{ other.name() }, m_hash{ other.hash() }
		{
		}

		constexpr auto name() const noexcept -> name_t const & { return m_name; }

		constexpr auto hash() const noexcept -> hash_t const & { return m_hash; }

	private:
		name_t m_name;
		hash_t m_hash;
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