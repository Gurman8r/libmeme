#ifndef _ML_BINDER_HPP_
#define _ML_BINDER_HPP_

#include <libmeme/Common.hpp>
#include <libmeme/Core/NonCopyable.hpp>

// Named Binder
#define ML_BIND_EX(T, name, value, ...) \
	auto name{ Binder<T> { value, ##__VA_ARGS__ } }

// Anonymous Binder
#define ML_BIND(T, value, ...) \
	ML_BIND_EX(T, ML_ANONYMOUS(ML_CONCAT(Binder, T)), value, ##__VA_ARGS__)

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct Binder;

	template <> struct Binder<> final { Binder() = delete; };

	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Binder<T> final : public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using pointer			= typename T *;
		using reference			= typename T &;
		using const_pointer		= typename T const *;
		using const_reference	= typename T const &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> explicit Binder(const_pointer value, Args && ... args)
			: m_value{ value }
		{
			T::bind(m_value, std::forward<Args>(args)...);
		}

		template <class ... Args
		> explicit Binder(const_reference value, Args && ... args)
			: Binder{ value ? &value : nullptr, std::forward<Args>(args)... }
		{
		}

		~Binder()
		{
			T::bind(nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline operator bool() const noexcept
		{
			return m_value;
		}

		ML_NODISCARD inline operator pointer() noexcept
		{
			return const_cast<pointer>(m_value);
		}

		ML_NODISCARD inline operator const_pointer() const noexcept
		{
			return m_value;
		}

		ML_NODISCARD inline pointer operator->() noexcept
		{
			return const_cast<pointer>(m_value);
		}

		ML_NODISCARD inline const_pointer operator->() const noexcept
		{
			return m_value;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const_pointer m_value;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BINDER_HPP_