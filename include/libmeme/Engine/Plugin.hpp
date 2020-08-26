#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <libmeme/Core/EventBus.hpp>
#include <libmeme/Core/Performance.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>
#include <libmeme/Engine/GuiManager.hpp>

#ifndef ML_PLUGIN_API
#define ML_PLUGIN_API	ML_API_EXPORT
#endif

#ifndef ML_PLUGIN_MAIN
#define ML_PLUGIN_MAIN	"ml_plugin_main"
#endif

namespace ml
{
	struct plugin;

	ML_decl_handle(plugin_id);

	struct file_context final
	{
		fs::path const program_path, content_path;

		fs::path const & path() const & noexcept { return program_path; }

		fs::path path2(fs::path const & path = {}) const noexcept
		{
			return content_path.native() + path.native();
		}
	};

	struct timer_context final
	{
		using fps_times_t = pmr::vector<float_t>;

		timer		main		{};
		timer		loop		{ false };
		duration	delta_time	{};
		uint64_t	frame_count	{};
		float_t		frame_rate	{};
		float_t		fps_accum	{};
		size_t		fps_index	{};
		fps_times_t	fps_times	{ 120, fps_times_t::allocator_type{} };
	};

	struct engine_context final
	{
		event_bus		* const bus	; // bus
		json			* const cfg	; // config
		file_context	* const file; // files
		gui_manager		* const gui	; // gui
		memory			* const mem	; // memory
		timer_context	* const time; // timers
		render_window	* const win	; // window
	};

	struct plugin : non_copyable, trackable, event_listener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit plugin(engine_context * ctx) : event_listener{ ctx->bus }, m_ctx{ ctx }
		{
			ML_assert("PLUGIN BUS MISMATCH" && (m_ctx->bus == event_listener::m_event_bus));
		}

		virtual ~plugin() noexcept override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void on_event(event const &) override = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto getbus() const noexcept -> event_bus & { return *m_ctx->bus; }
		
		auto getcfg() const noexcept -> json & { return *m_ctx->cfg; }

		auto getfs() const noexcept -> file_context & { return *m_ctx->file; }

		auto getgui() const noexcept -> gui_manager & { return *m_ctx->gui; }
		
		auto getmem() const noexcept -> memory & { return *m_ctx->mem; }

		auto gettime() const noexcept -> timer_context & { return *m_ctx->time; }
		
		auto getwin() const noexcept -> render_window & { return *m_ctx->win; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		engine_context * const m_ctx;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PLUGIN_HPP_