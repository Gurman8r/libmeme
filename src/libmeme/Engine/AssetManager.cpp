#include <libmeme/Engine/AssetManager.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	asset_manager::asset_manager() noexcept : m_data{} {}

	asset_manager::~asset_manager() noexcept { clear(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}