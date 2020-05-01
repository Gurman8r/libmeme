#ifndef _ML_RAII_POINTER_HPP_
#define _ML_RAII_POINTER_HPP_

// testing

#include <libmeme/Core/Memory.hpp>

namespace ml
{
	template <class T
	> struct ML_NODISCARD raii_ptr final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type			= typename raii_ptr<T>;
		using value_type		= typename T;
		using pointer			= typename T *;
		using const_pointer		= typename T const *;
		using reference			= typename T &;
		using const_reference	= typename T const &;
		using rvalue			= typename T &&;
		using const_rvalue		= typename T const &&;
		using ctor_type			= typename std::function<void *(void)>;
		using dtor_type			= typename std::function<void(void *)>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ctor, class Dtor
		> raii_ptr(Ctor && ctor, Dtor && dtor) noexcept
			: m_ctor{ ML_forward((ctor_type &&)ctor) }
			, m_dtor{ ML_forward((dtor_type &&)dtor) }
			, m_data{}
		{
			this->ctor();
		}

		raii_ptr() noexcept
			: self_type{ &memory_manager::allocate<value_type>, &memory_manager::deallocate }
		{
		}

		raii_ptr(self_type && other) noexcept
			: self_type{}
		{
			this->swap(std::move(other));
		}

		~raii_ptr() noexcept
		{
			this->dtor();
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
				m_ctor.swap(other.m_ctor);
				m_dtor.swap(other.m_dtor);
				std::swap(m_data, other.m_data);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		self_type & ctor() noexcept
		{
			if (!m_data && m_ctor)
			{
				m_data = (pointer)std::invoke(m_ctor);
			}
			return (*this);
		}

		self_type & dtor() noexcept
		{
			if (m_data && m_dtor)
			{
				std::invoke(m_dtor, m_data);
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool has_value() const noexcept { return m_data; }

		auto data() noexcept -> pointer const { return m_data; }

		auto data() const noexcept -> const_pointer const { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		operator bool() const noexcept { return m_data; }

		operator pointer const() noexcept { return m_data; }

		operator const_pointer const() const noexcept { return m_data; }

		auto operator->() noexcept -> pointer const { return m_data; }

		auto operator->() const noexcept -> const_pointer const { return m_data; }

		auto operator *() & noexcept -> reference { return *m_data; }

		auto operator *() const & noexcept -> const_reference { return *m_data; }

		auto operator *() && noexcept -> rvalue { return std::move(*m_data); }

		auto operator *() const && noexcept -> const_rvalue { return std::move(*m_data); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ctor_type	m_ctor;
		dtor_type	m_dtor;
		pointer		m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RAII_POINTER_HPP_