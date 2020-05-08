#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <libmeme/Engine/FileManager.hpp>
#include <libmeme/Engine/GuiManager.hpp>
#include <libmeme/Engine/PluginManager.hpp>
#include <libmeme/Engine/ScriptManager.hpp>
#include <libmeme/Engine/TimeManager.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API ML_NODISCARD engine final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		class engine_context;

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool is_initialized() noexcept;

		static bool create_context(json const & j, allocator_type const & alloc = {}) noexcept;

		static bool destroy_context() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool startup() noexcept;

		static bool shutdown() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static json				& config() noexcept;

		static file_manager		& fs() noexcept;

		static gui_manager		& gui() noexcept;

		static plugin_manager	& plugins() noexcept;

		static script_manager	& scripts() noexcept;

		static time_manager		& time() noexcept;

		static render_window	& window() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENGINE_HPP_