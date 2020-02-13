#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Timer.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>

namespace ml
{
	class ML_ENGINE_API engine final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			
		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using path_storage		= typename ds::flat_set<fs::path>;
		using plugin_storage	= typename ds::flat_map<struct shared_library, struct plugin *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// startup settings
		struct config final
		{
			fs::path					program_name;	// program name
			pmr::vector<pmr::string>	command_line;	// command line arguments
			fs::path					library_path;	// library path
			fs::path					content_path;	// content path
			pmr::vector<fs::path>		script_list;	// scripts to run on start
			pmr::vector<fs::path>		plugin_list;	// plugins to load on start
			pmr::string					window_title;	// window title
			video_mode					window_video;	// window video mode
			context_settings			window_context;	// window context settings
			int32_t						window_flags;	// window flags
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// runtime settings
		struct runtime final
		{
			float_t delta_time; // 
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// engine context
		struct context final : trackable
		{
			config			config			{};			// 
			runtime			runtime			{};			// 
			render_window	window			{};			// 
			timer			main_timer		{ true };	// 
			timer			loop_timer		{ false };	// 
			path_storage	lib_filenames	{};			// 
			plugin_storage	lib_instances	{};			// 
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static engine::context const * create_context();

		static bool startup(bool install_callbacks);

		static void shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static bool running();

		static void begin_loop();

		static void begin_draw();

		static void end_loop();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool load_plugin(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool initialized() noexcept;

		ML_NODISCARD static engine::context const * get_context() noexcept;

		ML_NODISCARD static engine::config & get_config() noexcept;

		ML_NODISCARD static engine::runtime & get_runtime() noexcept;

		ML_NODISCARD static duration const & get_time() noexcept;

		ML_NODISCARD static render_window & get_window() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_