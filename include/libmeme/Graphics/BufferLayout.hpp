#ifndef _ML_BUFFER_LAYOUT_HPP_
#define _ML_BUFFER_LAYOUT_HPP_

#include <libmeme/Graphics/Vertex.hpp>
#include <libmeme/Graphics/GL.hpp>

namespace ml
{
	struct ML_GRAPHICS_API buffer_layout final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_GRAPHICS_API attribute final
		{
			uint32_t	index		{}; // 
			uint32_t	size		{}; // 
			uint32_t	type		{}; // 
			bool		normalize	{}; // 
			uint32_t	stride		{}; // 
			uint32_t	offset		{}; // 
			uint32_t	width		{}; // 

			void apply() const noexcept;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename buffer_layout;
		using value_type				= typename attribute;
		using pointer					= typename value_type *;
		using reference					= typename value_type &;
		using const_pointer				= typename value_type const *;
		using const_reference			= typename value_type const &;
		using iterator					= typename pointer;
		using const_iterator			= typename const_pointer;
		using reverse_iterator			= typename std::reverse_iterator<iterator>;
		using const_reverse_iterator	= typename std::reverse_iterator<const_iterator>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr value_type default_3d[] =
		{
			{ 0, 3, GL::Float, false, vertex::size, 0, sizeof(float_t) },
			{ 1, 3, GL::Float, false, vertex::size, 3, sizeof(float_t) },
			{ 2, 2, GL::Float, false, vertex::size, 6, sizeof(float_t) },
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit constexpr buffer_layout(const_pointer data, size_t size) noexcept
			: m_data{ data }, m_size{ size }
		{
		}

		template <size_t N
		> constexpr buffer_layout(const value_type(&data)[N])
			: self_type{ &data[0], N }
		{
		}

		constexpr buffer_layout() noexcept
			: self_type{ default_3d }
		{
		}

		constexpr buffer_layout(self_type const & copy)
			: self_type{ copy.m_data, copy.m_size }
		{
		}

		constexpr buffer_layout(self_type && copy) noexcept
			: self_type{}
		{
			swap(std::move(copy));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr self_type & operator=(self_type const & value)
		{
			self_type temp{ value };
			swap(temp);
			return (*this);
		}

		constexpr self_type & operator=(self_type && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		constexpr void swap(buffer_layout & value) noexcept
		{
			if (this != std::addressof(value))
			{
				util::swap(m_data, value.m_data);
				util::swap(m_size, value.m_size);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		buffer_layout const & apply() const noexcept
		{
			for (auto const & e : (*this))
			{
				e.apply();
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator bool() const { return !empty(); }

		ML_NODISCARD constexpr auto operator[](size_t i) const  noexcept-> const_reference { return at(i); }

		ML_NODISCARD constexpr auto at(size_t i) const -> const_reference { return *(cbegin() + i); }

		ML_NODISCARD constexpr auto back() const noexcept -> const_reference { return (*(cend() - 1)); }

		ML_NODISCARD constexpr auto begin() const noexcept -> const_iterator { return m_data; }

		ML_NODISCARD constexpr auto cbegin() const noexcept -> const_iterator { return m_data; }

		ML_NODISCARD constexpr auto cend() const noexcept -> const_iterator { return m_data + m_size; }

		ML_NODISCARD constexpr auto crbegin() const noexcept -> const_reverse_iterator { return rbegin(); }

		ML_NODISCARD constexpr auto crend() const noexcept -> const_reverse_iterator { return rend(); }

		ML_NODISCARD constexpr auto data() const noexcept -> const_pointer { return m_data; }

		ML_NODISCARD constexpr bool empty() const noexcept { return (m_size == 0); }

		ML_NODISCARD constexpr auto end() const noexcept -> const_iterator { return m_data + m_size; }

		ML_NODISCARD constexpr auto front() const noexcept -> const_reference { return (*cbegin()); }

		ML_NODISCARD constexpr auto rbegin() const noexcept -> const_reverse_iterator { return std::make_reverse_iterator(cend()); }

		ML_NODISCARD constexpr auto rend() const noexcept -> const_reverse_iterator { return std::make_reverse_iterator(cbegin()); }

		ML_NODISCARD constexpr auto size() const noexcept -> size_t { return m_size; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const_pointer	m_data; // layout data
		size_t			m_size; // layout size

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_buffer_layout(Args && ... args)
	{
		return buffer_layout{ ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFER_LAYOUT_HPP_