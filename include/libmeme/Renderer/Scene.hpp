#ifndef _ML_SCENE_HPP_
#define _ML_SCENE_HPP_

// WIP

#include <libmeme/Renderer/Export.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	struct ML_RENDERER_API scene final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		scene(allocator_type alloc = {}) noexcept
		{
		}

		explicit scene(scene const & other, allocator_type alloc = {})
			: scene{ alloc }
		{
			this->copy(other);
		}

		explicit scene(scene && other, allocator_type alloc = {}) noexcept
			: scene{ alloc }
		{
			this->swap(std::move(other));
		}

		scene & operator=(scene const & other)
		{
			this->copy(other);
			return (*this);
		}

		scene & operator=(scene && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		void copy(scene const & other)
		{
			if (this != std::addressof(other))
			{
			}
		}

		void swap(scene & other) noexcept
		{
			if (this != std::addressof(other))
			{
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:


		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SCENE_HPP_