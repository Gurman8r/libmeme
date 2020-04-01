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
		using name_type = typename nameof<>::name_type;

		using guid_type = typename nameof<>::guid_type;

		constexpr typeof() noexcept = default;

		ML_NODISCARD static constexpr name_type const & name() noexcept
		{
			return m_name;
		}

		ML_NODISCARD static constexpr guid_type const & guid() noexcept
		{
			return m_guid;
		}

		template <class ... U
		> ML_NODISCARD constexpr auto compare(typeof<U...> const & other) noexcept
		{
			return (m_guid != other.guid()) ? (m_guid < other.guid()) ? -1 : 1 : 0;
		}

	private:
		static constexpr name_type m_name{ nameof_v<T> };

		static constexpr guid_type m_guid{ hashof_v<T> };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct typeof<> final
	{
		using name_type = typename nameof<>::name_type;

		using guid_type = typename nameof<>::guid_type;

		constexpr typeof() noexcept
			: m_name{}, m_guid{}
		{
		}

		template <class T
		> constexpr typeof(typeof<T> const & other) noexcept
			: m_name{ other.name() }, m_guid{ other.guid() }
		{
		}

		ML_NODISCARD constexpr name_type const & name() const noexcept
		{
			return m_name;
		}

		ML_NODISCARD constexpr guid_type const & guid() const noexcept
		{
			return m_guid;
		}

		template <class ... U
		> ML_NODISCARD constexpr auto compare(typeof<U...> const & other) const noexcept
		{
			return (m_guid != other.guid()) ? (m_guid < other.guid()) ? -1 : 1 : 0;
		}

	private:
		name_type m_name;
		guid_type m_guid;
	};
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T
	> static constexpr auto typeof_v{ typeof<T...>{} };
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T
	> ML_NODISCARD constexpr bool operator==(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return lhs.compare(rhs) == 0;
	}

	template <class ... T
	> ML_NODISCARD constexpr bool operator!=(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return lhs.compare(rhs) != 0;
	}

	template <class ... T
	> ML_NODISCARD constexpr bool operator<(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return lhs.compare(rhs) < 0;
	}

	template <class ... T
	> ML_NODISCARD constexpr bool operator>(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return lhs.compare(rhs) > 0;
	}

	template <class ... T
	> ML_NODISCARD constexpr bool operator<=(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return lhs.compare(rhs) <= 0;
	}

	template <class ... T
	> ML_NODISCARD constexpr bool operator>=(typeof<> const & lhs, typeof<T...> const & rhs) noexcept
	{
		return lhs.compare(rhs) >= 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class X, class ... Y
	> ML_NODISCARD constexpr bool operator==(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return lhs.compare(rhs) == 0;
	}

	template <class X, class ... Y
	> ML_NODISCARD constexpr bool operator!=(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return lhs.compare(rhs) != 0;
	}

	template <class X, class ... Y
	> ML_NODISCARD constexpr bool operator<(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return lhs.compare(rhs) < 0;
	}

	template <class X, class ... Y
	> ML_NODISCARD constexpr bool operator>(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return lhs.compare(rhs) > 0;
	}

	template <class X, class ... Y
	> ML_NODISCARD constexpr bool operator<=(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return lhs.compare(rhs) <= 0;
	}

	template <class X, class ... Y
	> ML_NODISCARD constexpr bool operator>=(typeof<X> const & lhs, typeof<Y...> const & rhs) noexcept
	{
		return lhs.compare(rhs) >= 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TYPE_OF_HPP_