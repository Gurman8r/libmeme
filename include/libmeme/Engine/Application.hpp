#ifndef _ML_APPLICATION_HPP_
#define _ML_APPLICATION_HPP_

#include <libmeme/Core/BatchVector.hpp>
#include <libmeme/System/EventBus.hpp>
#include <libmeme/System/Performance.hpp>
#include <libmeme/System/SharedLibrary.hpp>
#include <libmeme/System/Registry.hpp>
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

		int32_t run(std::function<void()> const & callback);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_event(event const & ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t do_file(fs::path const & value);

		int32_t do_string(pmr::string const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		plugin_handle install_plugin(fs::path const & path);

		bool uninstall_plugin(plugin_handle value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static application * const get() noexcept { return g_app; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD decltype(auto) bus() const noexcept { return m_sys.bus; }

		ML_NODISCARD decltype(auto) mem() const noexcept { return m_sys.mem; }

		ML_NODISCARD decltype(auto) perf() const noexcept { return m_sys.perf; }

		ML_NODISCARD decltype(auto) reg() const noexcept { return m_sys.reg; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto config() & noexcept -> json & { return m_config; }

		ML_NODISCARD auto config() const & noexcept -> json const & { return m_config; }

		template <class Key
		> ML_NODISCARD auto config(Key && key) & noexcept -> json & { return m_config[ML_forward(key)]; }

		template <class Key
		> ML_NODISCARD auto config(Key && key) const & noexcept -> json const & { return m_config[ML_forward(key)]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto path() const & noexcept -> fs::path const & { return m_path.program; }

		ML_NODISCARD auto path2(fs::path const & value = {}) const -> fs::path { return m_path.content.native() + value.native(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto gui() & noexcept -> gui_manager & { return m_gui; }

		ML_NODISCARD auto window() & noexcept -> render_window & { return m_window; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto total_time() const noexcept -> duration { return m_time.main_timer.elapsed(); }

		ML_NODISCARD auto delta_time() const noexcept -> duration { return m_time.delta_time; }

		ML_NODISCARD auto frame_rate() const noexcept -> float_t { return m_time.frame_rate; }

		ML_NODISCARD auto frame_count() const noexcept -> uint64_t { return m_time.frame_count; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static application * g_app;

		bool m_running; // running

		struct // system
		{
			event_bus	& bus	{ event_bus::get_singleton() };
			memory		& mem	{ memory::get_singleton() };
			performance & perf	{ performance::get_singleton() };
			registry	& reg	{ registry::get_singleton() };
		}
		m_sys;

		struct // time
		{
			using fps_times_t = pmr::vector<float_t>;

			timer		main_timer	{};
			timer		loop_timer	{ false };
			duration	delta_time	{};
			uint64_t	frame_count	{};
			float_t		frame_rate	{};
			float_t		fps_accum	{};
			size_t		fps_index	{};
			fps_times_t	fps_times	{ 120, allocator_type{} };
		}
		m_time;

		json m_config; // config

		struct // paths
		{
			fs::path program, content;
		}
		m_path;

		render_window m_window; // window

		gui_manager m_gui; // gui

		struct // input
		{
			vec2d cursor{};
			ds::array<int32_t, mouse_button_MAX> mouse{};
			ds::array<int32_t, key_code_MAX> keyboard{};
		}
		m_input;

		ds::batch_vector // plugins
		<
			hash_t,			// file hash
			fs::path,		// file path
			shared_library,	// library instance
			scoped<plugin>	// plugin instance
		>
		m_plugins;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_APPLICATION_HPP_