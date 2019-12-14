#ifndef _ML_BUFFERS_HPP_
#define _ML_BUFFERS_HPP_

#include <libmeme/Renderer/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/Handle.hpp>
#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template<class T> struct GraphicsBuffer
	{
		using self_type = typename T;

		using buffer_t = typename void const *;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct VAO final : public GraphicsBuffer<VAO>
	{
		union
		{
			uint32_t handle;
			uint32_t mode;
		};

		constexpr explicit VAO(uint32_t handle, uint32_t mode)
			: handle{ handle }
			, mode{ mode }
		{
		}

		constexpr VAO(uint32_t mode)
			: self_type{ NULL, mode }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct VBO final : public GraphicsBuffer<VBO>
	{
		union
		{
			uint32_t handle;
			uint32_t usage;
			buffer_t data;
			uint32_t size;
			uint32_t count;
		};

		constexpr explicit VBO(uint32_t handle, uint32_t usage, buffer_t data, uint32_t size, uint32_t count)
			: handle{ handle }
			, usage{ usage }
			, data{ data }
			, size{ size }
			, count{ count }
		{
		}

		constexpr VBO(uint32_t usage, void * data, uint32_t size, uint32_t count)
			: self_type{ NULL, usage, data, size, count }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct IBO final : public GraphicsBuffer<IBO>
	{
		union
		{
			uint32_t handle;
			uint32_t usage;
			uint32_t type;
			buffer_t data;
			uint32_t count;
		};

		constexpr explicit IBO(uint32_t handle, uint32_t usage, uint32_t type, buffer_t data, uint32_t count)
			: handle{ handle }
			, usage{ usage }
			, type{ type }
			, data{ data }
			, count{ count }
		{
		}

		constexpr IBO(uint32_t usage, uint32_t type, buffer_t data, uint32_t count)
			: self_type{ NULL, usage, type, data, count }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct FBO final : public GraphicsBuffer<FBO>
	{
		union
		{
			uint32_t handle;
			vec2 size;
		};

		constexpr explicit FBO(uint32_t handle, vec2 const & size)
			: handle{ handle }
			, size{ size }
		{
		}

		constexpr FBO(vec2 const & size)
			: self_type{ NULL, size }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct RBO final : public GraphicsBuffer<RBO>
	{
		union
		{
			uint32_t handle;
			vec2 size;
			uint32_t storage;
		};

		constexpr explicit RBO(uint32_t handle, vec2 const & size, uint32_t storage)
			: handle{ handle }
			, size{ size }
			, storage{ storage }
		{
		}

		constexpr RBO(vec2 const & size, uint32_t storage)
			: self_type{ NULL, size, storage }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFERS_HPP_