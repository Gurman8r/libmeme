#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <libmeme/Engine/AssetManager.hpp>
#include <libmeme/Engine/ObjectManager.hpp>
#include <libmeme/Engine/TimeManager.hpp>
#include <libmeme/Engine/GuiManager.hpp>
#include <libmeme/Engine/PluginManager.hpp>
#include <libmeme/Engine/ScriptManager.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD engine_config final : non_copyable, trackable
	{
		using arguments_t = pmr::vector<pmr::string>;

		arguments_t		command_line		{}; // command line
		fs::path		content_home		{}; // content home
		fs::path		library_home		{}; // library home
		fs::path		program_path		{}; // program path
		fs::path		program_name		{}; // program name
		fs::path		startup_script		{}; // setup script
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API engine final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		class engine_context;

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool is_initialized() noexcept;

		ML_NODISCARD static bool create_context(json const & j, allocator_type const & alloc = {});

		ML_NODISCARD static bool destroy_context();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool startup();

		ML_NODISCARD static bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void begin_loop();

		static void pre_render();

		static void post_render();

		static void end_loop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static asset_manager & assets() noexcept;

		ML_NODISCARD static engine_config & config() noexcept;

		ML_NODISCARD static object_manager & objects() noexcept;

		ML_NODISCARD static gui_manager & gui() noexcept;

		ML_NODISCARD static plugin_manager & plugins() noexcept;

		ML_NODISCARD static script_manager & scripts() noexcept;

		ML_NODISCARD static time_manager & time() noexcept;

		ML_NODISCARD static render_window & window() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static fs::path path_to(fs::path const & value = {}) noexcept
		{
			return config().content_home.native() + value.native();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_