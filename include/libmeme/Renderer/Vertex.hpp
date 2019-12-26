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

		constexpr Vertex() noexcept = default;

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

	template <class ... Args> static constexpr auto make_vertex(Args && ... args)
	{
		return Vertex{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_HPP_