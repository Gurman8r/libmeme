#ifndef _ML_VERTEX_HPP_
#define _ML_VERTEX_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	struct Vertex final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_t = typename Array<float_t, 8>;

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
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args> static constexpr auto make_vertex(Args && ... args)
	{
		return Vertex{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_HPP_