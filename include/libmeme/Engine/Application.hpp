#ifndef _ML_APPLICATION_HPP_
#define _ML_APPLICATION_HPP_

#include <libmeme/Core/BatchVector.hpp>
#include <libmeme/System/EventBus.hpp>
#include <libmeme/System/Performance.hpp>
#include <libmeme/System/SharedLibrary.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>
#include <libmeme/Engine/GuiManager.hpp>
#include <libmeme/Engine/Plugin.hpp>

namespace ml
{
	struct ML_ENGINE_API application final : trackable, event_listener
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		application(json const & j, allocator_type alloc = {}) noexcept;

		~application() noexcept override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_event(event const & ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		plugin_handle install_plugin(fs::path const & path);

		bool uninstall_plugin(plugin_handle value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t execute_file(fs::path const & value);

		int32_t execute_string(pmr::string const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto total_time() const noexcept -> duration { return m_time.main_timer.elapsed(); }

		auto delta_time() const noexcept -> duration { return m_time.delta_time; }

		auto frame_rate() const noexcept -> float_t { return m_time.frame_rate; }

		auto frame_count() const noexcept -> uint64_t { return m_time.frame_count; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto config() const & noexcept -> json const & { return m_config; }

		auto path() -> fs::path const & { return m_path.program; }

		auto path2(fs::path const & value = {}) -> fs::path { return m_path.content.native() + value.native(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto gui() & noexcept -> gui_manager & { return m_gui; }

		auto window() & noexcept -> render_window & { return m_window; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		struct // system
		{
			memory & mem;
			event_bus & bus;
			performance & perf;
		}
		m_sys;

		struct // time
		{
			using fps_times_t = pmr::vector<float_t>;

			timer		main_timer{ true };
			timer		loop_timer{ false };
			duration	delta_time{};
			uint64_t	frame_count{};
			float_t		frame_rate{};
			float_t		fps_accum{};
			size_t		fps_index{};
			fps_times_t	fps_times{ 120, allocator_type{} };
		}
		m_time;

		json m_config; // config

		struct // paths
		{
			fs::path program, content;
		}
		m_path;

		render_window	m_window; // window

		gui_manager		m_gui	; // imgui

		ds::batch_vector
		<
			hash_t,			// hash
			fs::path,		// path
			shared_library,	// library
			scoped<plugin>	// plugin
		>
		m_plugins;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_APPLICATION_HPP_