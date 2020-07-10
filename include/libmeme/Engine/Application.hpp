#ifndef _ML_APPLICATION_HPP_
#define _ML_APPLICATION_HPP_

// WIP

#include <libmeme/Core/BatchVector.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/Performance.hpp>
#include <libmeme/Engine/ImGui.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>
#include <libmeme/Platform/SharedLibrary.hpp>

namespace ml
{
	struct ML_ENGINE_API application : render_window, event_listener
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		application(allocator_type alloc = {}) noexcept;

		application(window_settings const & ws, allocator_type alloc = {}) noexcept;

		virtual ~application() noexcept override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void on_event(event const & ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool open(window_settings const & ws) override;

		virtual void close() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool free_plugin(plugin_handle value);

		plugin_handle load_plugin(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		struct // performance
		{
			using fps_times_t = pmr::vector<float_t>;

			timer		loop_timer	{ false };
			duration	delta_time	{};
			uint64_t	frame_count	{};
			float_t		frame_rate	{};
			float_t		fps_accum	{};
			size_t		fps_index	{};
			fps_times_t	fps_times	{ 120, allocator_type{} };
		}
		m_perf;
		
		ImGuiContext * m_imgui{};

		ds::batch_vector<
			hash_t,			// file hash
			fs::path,		// file path
			shared_library,	// library instance
			unique<plugin>	// plugin instance
		> m_plugins{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_APPLICATION_HPP_