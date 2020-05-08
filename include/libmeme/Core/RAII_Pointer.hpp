#ifndef _ML_RAII_POINTER_HPP_
#define _ML_RAII_POINTER_HPP_

#include <libmeme/Core/Memory.hpp>

namespace ml
{
	// similar to wheat_ptr, but with less gluten
	template <class T
	> struct raii_ptr final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type			= typename raii_ptr<T>;
		using value_type		= typename T;
		using pointer			= typename value_type *;
		using const_pointer		= typename value_type const *;
		using reference			= typename value_type &;
		using const_reference	= typename value_type const &;
		using rvalue			= typename value_type &&;
		using const_rvalue		= typename value_type const &&;
		using ctor_type			= typename std::function<pointer(void)>;
		using dtor_type			= typename std::function<void(pointer)>;
		using func_data			= typename std::pair<ctor_type, dtor_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> raii_ptr(Args && ... args) noexcept : m_func{ ML_forward(args)... }
		{
			this->create();
		}

		raii_ptr(self_type && other) noexcept : self_type{}
		{
			this->swap(std::move(other));
		}

		~raii_ptr() noexcept
		{
			this->destroy();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		self_type & operator=(self_type && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_func.swap(other.m_func);
				std::swap(m_data, other.m_data);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		self_type & create() noexcept
		{
			if (m_func.first) { m_data = std::invoke(m_func.first); }
			return (*this);
		}

		self_type & destroy() noexcept
		{
			if (m_func.second) { std::invoke(m_func.second, m_data); }
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD operator bool() const noexcept { return m_data; }

		ML_NODISCARD operator pointer &() & noexcept { return m_data; }

		ML_NODISCARD operator const_pointer const &() const & noexcept { return m_data; }

		ML_NODISCARD auto operator *() & noexcept -> reference { return (*m_data); }

		ML_NODISCARD auto operator *() const & noexcept -> const_reference { return (*m_data); }

		ML_NODISCARD auto operator *() && noexcept -> rvalue { return std::move(*m_data); }

		ML_NODISCARD auto operator *() const && noexcept -> const_rvalue { return std::move(*m_data); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		pointer		m_data{}; // pointer value
		func_data	m_func{}; // constructor / destructor

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RAII_POINTER_HPP_