#ifndef _ML_VERTEX_HPP_
#define _ML_VERTEX_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	struct Vertex final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t Size{ 8 };

		using storage_type = typename array<float_t, Size>;

		storage_type m_storage{ 0 };

		constexpr explicit Vertex(vec3 const & p, vec3 const & n, vec2 const & t) : m_storage{
			p[0], p[1], p[2], n[0], n[1], n[2], t[0], t[1]
		}
		{
		}

		constexpr Vertex(std::initializer_list<float_t> init)
			: m_storage{}
		{
			for (auto it{ init.begin() }; it != init.end(); ++it)
			{
				m_storage[std::distance(init.begin(), it)] = (*it);
			}
		}

		constexpr Vertex(storage_type const & storage)
			: m_storage{ storage }
		{
		}

		constexpr Vertex(storage_type && storage) noexcept
			: m_storage{ std::move(storage) }
		{
		}

		constexpr Vertex(Vertex const & other)
			: m_storage{ other.m_storage }
		{
		}

		constexpr Vertex(Vertex && other) noexcept
			: m_storage{}
		{
			swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Vertex & operator=(Vertex const & other)
		{
			Vertex temp{ other };
			swap(temp);
			return (*this);
		}

		Vertex & operator=(Vertex && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		constexpr void swap(Vertex & other) noexcept
		{
			if (this != std::addressof(other))
			{
				util::swap(m_storage, other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) operator[](size_t const i) { return m_storage[i]; }
		
		constexpr decltype(auto) operator[](size_t const i) const { return m_storage[i]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr vec3 position() const noexcept
		{
			return { m_storage[0], m_storage[1], m_storage[2] };
		}

		ML_NODISCARD constexpr vec3 normal() const noexcept
		{
			return { m_storage[3], m_storage[4], m_storage[5] };
		}

		ML_NODISCARD constexpr vec2 texcoord() const noexcept
		{
			return { m_storage[6], m_storage[7] };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Vertex & position(vec3 const & value) noexcept
		{
			m_storage[0] = value[0];
			m_storage[1] = value[1];
			m_storage[2] = value[2];
			return (*this);
		}

		constexpr Vertex & normal(vec3 const & value) noexcept
		{
			m_storage[3] = value[0];
			m_storage[4] = value[1];
			m_storage[5] = value[2];
			return (*this);
		}

		constexpr Vertex & texcoord(vec2 const & value) noexcept
		{
			m_storage[6] = value[0];
			m_storage[7] = value[1];
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static constexpr auto make_vertex(vec3 && p, vec3 && n, vec2 && t)
	{
		return Vertex{ std::move(p), std::move(n), std::move(t) };
	}

	ML_NODISCARD static constexpr auto make_vertex(Vertex::storage_type && s)
	{
		return Vertex{ std::move(s) };
	}

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_vertex(Args && ... args)
	{
		return Vertex{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline std::vector<float_t> contiguous(std::vector<Vertex> const & vertices)
	{
		std::vector<float_t> temp;
		temp.resize(vertices.size() * Vertex::Size);
		for (size_t i = 0, imax = temp.size(); i < imax; ++i)
		{
			temp[i] = vertices[i / Vertex::Size][i % Vertex::Size];
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_HPP_