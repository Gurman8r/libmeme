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

		~application() noexcept override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool open(window_settings const & ws) override;

		virtual void close() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void on_event(event const & ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		plugin_handle load_plugin(fs::path const & path);

		bool free_plugin(plugin_handle value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_frame_count() const noexcept -> uint64_t { return m_frame_count; }

		ML_NODISCARD auto get_frame_rate() const noexcept -> float_t { return m_frame_rate; }

		ML_NODISCARD auto get_frame_time() const noexcept -> duration { return m_frame_time; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		timer					m_loop_timer	{ false };
		duration				m_frame_time	{};
		uint64_t				m_frame_count	{};
		float_t					m_frame_rate	{};
		float_t					m_fps_accum		{};
		size_t					m_fps_index		{};
		pmr::vector<float_t>	m_fps_times		{ 120, allocator_type{} };
		
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