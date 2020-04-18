#ifndef _ML_WRAPPER_HPP_
#define _ML_WRAPPER_HPP_

#include <libmeme/Config.hpp>

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// value wrapper
	template <class T> struct ML_NODISCARD wrapper
	{
		using value_type		= typename T;
		using pointer			= typename value_type *;
		using const_pointer		= typename value_type const *;
		using reference			= typename value_type &;
		using const_reference	= typename value_type const &;
		using rvalue			= typename value_type &&;
		using const_rvalue		= typename value_type const &&;
		
		value_type m_value;

		constexpr auto operator->() noexcept -> pointer const { return &m_value; }
		
		constexpr auto operator->() const noexcept -> const_pointer const { return &m_value; }

		constexpr auto operator *() & noexcept -> reference { return m_value; }
		
		constexpr auto operator *() const & noexcept -> const_reference { return m_value; }

		constexpr auto operator *() && noexcept -> rvalue { return std::move(m_value); }
		
		constexpr auto operator *() const && noexcept -> const_rvalue { return std::move(m_value); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WRAPPER_HPP_