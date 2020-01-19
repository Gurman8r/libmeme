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
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			uint32_t	index		{ 0 };
			uint32_t	size		{ 0 };
			uint32_t	type		{ 0 };
			bool		normalize	{ false };
			uint32_t	stride		{ 0 };
			uint32_t	offset		{ 0 };
			uint32_t	width		{ 0 };

			constexpr Attribute() noexcept = default;
			constexpr Attribute(Attribute const &) = default;
			constexpr Attribute(Attribute &&) noexcept = default;
			constexpr Attribute & operator=(Attribute const &) = default;
			constexpr Attribute & operator=(Attribute &&) noexcept = default;

			Attribute const & operator()() const noexcept;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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
			{ 0, 3, GL::Float, false, vertex::size, 0, sizeof(float_t) },
			{ 1, 3, GL::Float, false, vertex::size, 3, sizeof(float_t) },
			{ 2, 2, GL::Float, false, vertex::size, 6, sizeof(float_t) },
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr BufferLayout() noexcept
			: self_type{ Default }
		{
		}

		template <size_t N
		> constexpr BufferLayout(const value_type(&data)[N]) noexcept
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

		inline self_type const & apply() const noexcept
		{
			for (auto const & elem : (*this))
			{
				elem();
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator bool() const { return !empty(); }

		ML_NODISCARD constexpr auto operator[](size_t i) const -> const_reference { return at(i); }

		ML_NODISCARD constexpr auto at(size_t i) const -> const_reference { return *(cbegin() + i); }

		ML_NODISCARD constexpr auto back() const noexcept -> const_reference { return (*(cend() - 1)); }

		ML_NODISCARD constexpr auto begin() const noexcept -> const_iterator { return m_data; }

		ML_NODISCARD constexpr auto cbegin() const noexcept -> const_iterator { return m_data; }

		ML_NODISCARD constexpr auto cend() const noexcept -> const_iterator { return m_data + m_size; }

		ML_NODISCARD constexpr auto data() const noexcept -> const_pointer { return m_data; }

		ML_NODISCARD constexpr bool empty() const noexcept { return (m_size == 0); }

		ML_NODISCARD constexpr auto end() const noexcept -> const_iterator { return m_data + m_size; }

		ML_NODISCARD constexpr auto front() const noexcept -> const_reference { return (*cbegin()); }

		ML_NODISCARD constexpr auto size() const noexcept -> size_t { return m_size; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const_pointer m_data;
		size_t m_size;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_buffer_layout(Args && ... args)
	{
		return BufferLayout{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFER_LAYOUT_HPP_