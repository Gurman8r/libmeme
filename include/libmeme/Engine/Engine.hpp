#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <libmeme/Engine/AssetManager.hpp>
#include <libmeme/Engine/FileManager.hpp>
#include <libmeme/Engine/GuiManager.hpp>
#include <libmeme/Engine/GameObjectManager.hpp>
#include <libmeme/Engine/PluginManager.hpp>
#include <libmeme/Engine/ScriptManager.hpp>
#include <libmeme/Engine/TimeManager.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API engine final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		class engine_context;

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool is_initialized() noexcept;

		ML_NODISCARD static bool create_context(json const & j, allocator_type const & alloc = {}) noexcept;

		ML_NODISCARD static bool destroy_context() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool startup() noexcept;

		ML_NODISCARD static bool shutdown() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static asset_manager	& assets() noexcept;

		ML_NODISCARD static file_manager	& fs() noexcept;

		ML_NODISCARD static gui_manager		& gui() noexcept;

		ML_NODISCARD static gameobj_manager	& gameobj() noexcept;

		ML_NODISCARD static plugin_manager	& plugins() noexcept;

		ML_NODISCARD static script_manager	& scripts() noexcept;

		ML_NODISCARD static time_manager	& time() noexcept;

		ML_NODISCARD static render_window	& window() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_