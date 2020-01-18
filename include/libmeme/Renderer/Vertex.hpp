#ifndef _ML_VERTEX_HPP_
#define _ML_VERTEX_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	struct vertex final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t size{ 8 };

		using storage_type = typename ds::array<float_t, size>;

		storage_type m_storage{ 0 };

		constexpr explicit vertex(vec3 const & p, vec3 const & n, vec2 const & t) : m_storage{
			p[0], p[1], p[2], n[0], n[1], n[2], t[0], t[1]
		}
		{
		}

		constexpr vertex(std::initializer_list<float_t> init)
			: m_storage{}
		{
			for (auto it{ init.begin() }; it != init.end(); ++it)
			{
				m_storage[std::distance(init.begin(), it)] = (*it);
			}
		}

		constexpr vertex(storage_type const & storage)
			: m_storage{ storage }
		{
		}

		constexpr vertex(storage_type && storage) noexcept
			: m_storage{ std::move(storage) }
		{
		}

		constexpr vertex(vertex const & other)
			: m_storage{ other.m_storage }
		{
		}

		constexpr vertex(vertex && other) noexcept
			: m_storage{}
		{
			swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vertex & operator=(vertex const & other)
		{
			vertex temp{ other };
			swap(temp);
			return (*this);
		}

		vertex & operator=(vertex && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		constexpr void swap(vertex & other) noexcept
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

		constexpr vertex & position(vec3 const & value) noexcept
		{
			m_storage[0] = value[0];
			m_storage[1] = value[1];
			m_storage[2] = value[2];
			return (*this);
		}

		constexpr vertex & normal(vec3 const & value) noexcept
		{
			m_storage[3] = value[0];
			m_storage[4] = value[1];
			m_storage[5] = value[2];
			return (*this);
		}

		constexpr vertex & texcoord(vec2 const & value) noexcept
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
		return vertex{ std::move(p), std::move(n), std::move(t) };
	}

	ML_NODISCARD static constexpr auto make_vertex(vertex::storage_type && s)
	{
		return vertex{ std::move(s) };
	}

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_vertex(Args && ... args)
	{
		return vertex{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline pmr::vector<float_t> contiguous(pmr::vector<vertex> const & vertices)
	{
		pmr::vector<float_t> temp;
		temp.resize(vertices.size() * vertex::size);
		for (size_t i = 0, imax = temp.size(); i < imax; ++i)
		{
			temp[i] = vertices[i / vertex::size][i % vertex::size];
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_HPP_