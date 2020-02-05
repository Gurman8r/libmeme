#ifndef _ML_BUFFER_LAYOUT_HPP_
#define _ML_BUFFER_LAYOUT_HPP_

#include <libmeme/Renderer/Vertex.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	struct ML_RENDERER_API buffer_layout final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct attribute final
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			uint32_t	index		{ 0 };
			uint32_t	size		{ 0 };
			uint32_t	type		{ 0 };
			bool		normalize	{ false };
			uint32_t	stride		{ 0 };
			uint32_t	offset		{ 0 };
			uint32_t	width		{ 0 };

			constexpr attribute() noexcept = default;
			constexpr attribute(attribute const &) = default;
			constexpr attribute(attribute &&) noexcept = default;
			constexpr attribute & operator=(attribute const &) = default;
			constexpr attribute & operator=(attribute &&) noexcept = default;

			attribute const & operator()() const noexcept;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename attribute;
		using self_type			= typename buffer_layout;
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

		constexpr buffer_layout() noexcept
			: self_type{ Default }
		{
		}

		template <size_t N
		> constexpr buffer_layout(const value_type(&data)[N]) noexcept
			: self_type{ &data[0], N }
		{
		}

		constexpr buffer_layout(const_iterator first, const_iterator last) noexcept
			: self_type{ first, (size_t)(last - first) }
		{
		}

		constexpr buffer_layout(self_type const & copy) noexcept
			: self_type{ copy.m_data, copy.m_size }
		{
		}

		constexpr buffer_layout(const_pointer data, size_t size) noexcept
			: m_data{ data }
			, m_size{ size }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr void swap(buffer_layout & other) noexcept
		{
			if (this != std::addressof(other))
			{
				util::swap(m_data, other.m_data);
				util::swap(m_size, other.m_size);
			}
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
		return buffer_layout{ ML_FWD(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFER_LAYOUT_HPP_