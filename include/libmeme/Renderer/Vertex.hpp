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

		constexpr vertex(std::initializer_list<float_t> init)
			: m_data{}
		{
			for (auto it{ init.begin() }; it != init.end(); ++it)
			{
				m_data[std::distance(init.begin(), it)] = (*it);
			}
		}

		constexpr vertex(storage_type const & storage)
			: m_data{ storage }
		{
		}

		constexpr vertex(storage_type && storage) noexcept
			: m_data{ std::move(storage) }
		{
		}

		constexpr vertex(vertex const & other)
			: m_data{ other.m_data }
		{
		}

		constexpr vertex(vertex && other) noexcept
			: m_data{}
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
				util::swap(m_data, other.m_data);
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
		storage_type m_data{ 0 };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static constexpr auto make_vertex(vec3 && p, vec3 && n, vec2 && t)
	{
		return vertex{ ML_FWD(p), ML_FWD(n), ML_FWD(t) };
	}

	ML_NODISCARD static constexpr auto make_vertex(vertex::storage_type && s)
	{
		return vertex{ ML_FWD(s) };
	}

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_vertex(Args && ... args)
	{
		return vertex{ ML_FWD(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline auto const & contiguous(pmr::vector<vertex> const & v)
	{
		static pmr::vector<float_t> temp{};

		if (size_t const imax{ v.size() * vertex::size })
		{
			temp.resize(imax);

			for (size_t i = 0; i < imax; i++)
			{
				temp[i] = v[i / vertex::size][i % vertex::size];
			}
		}
		else if (!temp.empty())
		{
			temp.clear();
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_HPP_