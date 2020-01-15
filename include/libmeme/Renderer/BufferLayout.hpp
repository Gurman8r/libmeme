#ifndef _ML_BUFFER_LAYOUT_HPP_
#define _ML_BUFFER_LAYOUT_HPP_

#include <libmeme/Renderer/Vertex.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	struct ML_RENDERER_API BufferLayout final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Attribute final
		{
			/* * * * * * * * * * * * * * * * * * * * */

			uint32_t	index		{ 0 };
			uint32_t	size		{ 0 };
			uint32_t	type		{ 0 };
			bool		normalize	{ false };
			uint32_t	stride		{ 0 };
			uint32_t	offset		{ 0 };
			uint32_t	width		{ 0 };

			constexpr Attribute() noexcept = default;
			constexpr Attribute(Attribute const &) noexcept = default;
			constexpr Attribute(Attribute &&) noexcept = default;

			Attribute const & operator()() const;

			/* * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename Attribute;
		using self_type			= typename BufferLayout;
		using initializer_type	= typename std::initializer_list<value_type>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename value_type const *;
		using const_reference	= typename value_type const &;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr const value_type Default[] = {
			{ 0, 3, GL::Float, false, Vertex::Size, 0, sizeof(float_t) },
			{ 1, 3, GL::Float, false, Vertex::Size, 3, sizeof(float_t) },
			{ 2, 2, GL::Float, false, Vertex::Size, 6, sizeof(float_t) },
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr BufferLayout() noexcept
			: self_type{ Default }
		{
		}

		template <size_t N>
		constexpr BufferLayout(const value_type(&data)[N]) noexcept
			: self_type{ &data[0], N }
		{
		}

		constexpr BufferLayout(const_iterator first, const_iterator last) noexcept
			: self_type{ first, (size_t)(last - first) }
		{
		}

		constexpr BufferLayout(self_type const & copy) noexcept
			: self_type{ copy.m_data, copy.m_size }
		{
		}

		constexpr BufferLayout(const_pointer data, size_t size) noexcept
			: m_data{ data }
			, m_size{ size }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type const & bind() const
		{
			for (auto const & elem : (*this))
			{
				elem();
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto at(size_t i) const -> const_reference	{ return *(cbegin() + i); }
		constexpr auto back()		const -> const_reference	{ return (*(cend() - 1)); }
		constexpr auto begin()		const -> const_iterator		{ return m_data; }
		constexpr auto cbegin()		const -> const_iterator		{ return m_data; }
		constexpr auto cend()		const -> const_iterator		{ return m_data + m_size; }
		constexpr auto data()		const -> const_pointer		{ return m_data; }
		constexpr auto empty()		const -> bool				{ return (m_size == 0); }
		constexpr auto end()		const -> const_iterator		{ return m_data + m_size; }
		constexpr auto front()		const -> const_reference	{ return (*cbegin()); }
		constexpr auto size()		const -> size_t				{ return m_size; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator bool() const
		{
			return !empty();
		}

		constexpr const_reference operator[](size_t i) const
		{
			return at(i);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const_pointer m_data;
		size_t m_size;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args> static constexpr auto make_buffer_layout(Args && ... args)
	{
		return BufferLayout{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFER_LAYOUT_HPP_