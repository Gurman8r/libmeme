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

		constexpr explicit vertex(vec3 const & p, vec3 const & n, vec2 const & t) : m_data{
			p[0], p[1], p[2], n[0], n[1], n[2], t[0], t[1]
		}
		{
		}

		constexpr vertex(std::initializer_list<float_t> init) : m_data{}
		{
			for (auto it{ init.begin() }; it != init.end(); ++it)
			{
				if (auto const i{ (size_t)std::distance(init.begin(), it) }; i < size)
				{
					m_data[i] = (*it);
				}
			}
		}

		constexpr vertex(storage_type const & storage) : m_data{ storage }
		{
		}

		constexpr vertex(storage_type && storage) noexcept : m_data{ std::move(storage) }
		{
		}

		constexpr vertex(vertex const & value) : m_data{ value.m_data }
		{
		}

		constexpr vertex(vertex && value) noexcept : m_data{}
		{
			swap(std::move(value));
		}

		constexpr vertex() noexcept : m_data{}
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vertex & operator=(vertex const & value)
		{
			vertex temp{ value };
			swap(temp);
			return (*this);
		}

		vertex & operator=(vertex && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		constexpr void swap(vertex & value) noexcept
		{
			if (this != std::addressof(value))
			{
				util::swap(m_data, value.m_data);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) operator[](size_t const i) { return m_data[i]; }
		
		constexpr decltype(auto) operator[](size_t const i) const { return m_data[i]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr vec3 position() const noexcept
		{
			return { m_data[0], m_data[1], m_data[2] };
		}

		ML_NODISCARD constexpr vec3 normal() const noexcept
		{
			return { m_data[3], m_data[4], m_data[5] };
		}

		ML_NODISCARD constexpr vec2 texcoord() const noexcept
		{
			return { m_data[6], m_data[7] };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr vertex & position(vec3 const & value) noexcept
		{
			m_data[0] = value[0];
			m_data[1] = value[1];
			m_data[2] = value[2];
			return (*this);
		}

		constexpr vertex & normal(vec3 const & value) noexcept
		{
			m_data[3] = value[0];
			m_data[4] = value[1];
			m_data[5] = value[2];
			return (*this);
		}

		constexpr vertex & texcoord(vec2 const & value) noexcept
		{
			m_data[6] = value[0];
			m_data[7] = value[1];
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD inline auto contiguous(pmr::vector<vertex> const & v) noexcept
	{
		pmr::vector<float_t> temp{};
		if (size_t const imax{ v.size() * vertex::size })
		{
			temp.reserve(imax);
			for (size_t i = 0; i < imax; ++i)
			{
				temp.push_back(v[i / vertex::size][i % vertex::size]);
			}
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_HPP_