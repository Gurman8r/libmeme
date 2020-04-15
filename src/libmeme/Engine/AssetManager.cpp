#include <libmeme/Engine/AssetManager.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	asset_manager::asset_manager(json const & j, allocator_type const & alloc) noexcept
		: m_data{ alloc }
	{
	}

	asset_manager::~asset_manager() noexcept
	{
		clear();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}