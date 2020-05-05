#ifndef _ML_GAMEOBJ_MANAGER_HPP_
#define _ML_GAMEOBJ_MANAGER_HPP_

// WIP

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	struct gameobj final : trackable
	{
	};

	struct gameobj_manager final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		gameobj_manager(json const & j, allocator_type const & alloc = {}) noexcept;

		~gameobj_manager() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_GAMEOBJ_MANAGER_HPP_