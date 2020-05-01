#ifndef _ML_GAME_OBJECT_MANAGER_HPP_
#define _ML_GAME_OBJECT_MANAGER_HPP_

// WIP

#include <libmeme/Engine/GameObject.hpp>

namespace ml
{
	struct object_manager final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		object_manager(json const & j, allocator_type const & alloc = {}) noexcept;

		~object_manager() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_GAME_OBJECT_MANAGER_HPP_