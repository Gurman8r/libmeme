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
		using view_type = typename nameof<>::view_type;

		using guid_type = typename nameof<>::guid_type;

		constexpr typeof() noexcept = default;

		ML_NODISCARD static constexpr view_type const & name() noexcept
		{
			return m_name;
		}

		ML_NODISCARD static constexpr guid_type const & guid() noexcept
		{
			return m_guid;
		}

		template <class ... U
		> ML_NODISCARD constexpr auto compare(typeof<U...> const & value) noexcept
		{
			return (m_guid != value.guid()) ? (m_guid < value.guid()) ? -1 : 1 : 0;
		}

	private:
		static constexpr view_type m_name{ nameof_v<T> };

		static constexpr guid_type m_guid{ hashof_v<T> };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct typeof<> final
	{
		using view_type = typename nameof<>::view_type;

		using guid_type = typename nameof<>::guid_type;

		constexpr typeof() noexcept
			: m_name{}, m_guid{}
		{
		}

		template <class T
		> constexpr typeof(typeof<T> const & value) noexcept
			: m_name{ value.name() }, m_guid{ value.guid() }
		{
		}

		ML_NODISCARD constexpr view_type const & name() const noexcept
		{
			return m_name;
		}

		ML_NODISCARD constexpr guid_type const & guid() const noexcept
		{
			return m_guid;
		}

		template <class ... U
		> ML_NODISCARD constexpr auto compare(typeof<U...> const & value) const noexcept
		{
			return util::compare(m_guid, value.guid());
		}

	private:
		view_type m_name;
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