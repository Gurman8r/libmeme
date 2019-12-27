#ifndef _ML_VERTEX_HPP_
#define _ML_VERTEX_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	struct Vertex final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t Size{ 8 };

		using storage_t = typename Array<float_t, Size>;

		storage_t m_storage{ 0 };

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

		constexpr Vertex(Vertex const & other)
			: m_storage{ other.m_storage }
		{
		}

		constexpr Vertex(storage_t const & storage)
			: m_storage{ storage }
		{
		}

		constexpr Vertex(storage_t && storage) noexcept
			: m_storage{ std::move(storage) }
		{
		}

		constexpr Vertex() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr decltype(auto) operator[](size_t const i) { return m_storage[i]; }
		
		constexpr decltype(auto) operator[](size_t const i) const { return m_storage[i]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr vec3 position() const noexcept
		{
			return { m_storage[0], m_storage[1], m_storage[2] };
		}

		constexpr vec3 normal() const noexcept
		{
			return { m_storage[3], m_storage[4], m_storage[5] };
		}

		constexpr vec2 texcoord() const noexcept
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

	static constexpr auto make_vertex(vec3 && p, vec3 && n, vec2 && t)
	{
		return Vertex{ std::move(p), std::move(n), std::move(t) };
	}

	static constexpr auto make_vertex(Vertex::storage_t && s)
	{
		return Vertex{ std::move(s) };
	}

	template <class ... Args> static constexpr auto make_vertex(Args && ... args)
	{
		return Vertex{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace alg
	{
		static inline std::vector<float_t> const & contiguous(std::vector<Vertex> const & vertices)
		{
			static std::vector<float_t> temp;
			if (const size_t imax{ vertices.size() * Vertex::Size })
			{
				temp.resize(imax);

				for (size_t i = 0; i < imax; i++)
				{
					temp[i] = vertices[i / Vertex::Size][i % Vertex::Size];
				}
			}
			else if (!temp.empty())
			{
				temp.clear();
			}
			return temp;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_HPP_