#ifndef _ML_CLIENT_CONTEXT_HPP_
#define _ML_CLIENT_CONTEXT_HPP_

#include <libmeme/Client/Export.hpp>
#include <libmeme/Core/Events.hpp>
#include <libmeme/Core/Timer.hpp>
#include <libmeme/Core/Matrix.hpp>
#include <libmeme/Window/Input.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct blackboard;
	struct imgui_context;
	struct render_window;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// client io
	struct ML_NODISCARD client_io final : trackable, non_copyable
	{
		using allocator_type = pmr::polymorphic_allocator<byte_t>;

		client_io(int32_t argc, char ** argv, json prefs, allocator_type alloc)
			: argc{ argc }, argv{ argv }, prefs{ json{ prefs } }, alloc{ alloc }
		{
		}

		int32_t const			argc						; // 
		char ** const			argv						; // 
		json					prefs						; // 
		allocator_type			alloc						; // 

		fs::path const
			program_name{ argv[0] }							, // 
			program_path{ fs::current_path() }				, // 
			content_path{ prefs["path"].get<fs::path>() }	; // 

		timer const				main_timer	{}				; // 
		timer					loop_timer	{ false }		; // 
		duration				frame_time	{}				; // 
		uint64_t				frame_count	{}				; // 
		float_t					fps_rate	{}				; // 
		float_t					fps_accum	{}				; // 
		size_t					fps_index	{}				; // 
		pmr::vector<float_t>	fps_times	{ 120, alloc }	; // 

		vec2d					cursor		{}				; // 
		mouse_state				mouse		{}				; // 
		keyboard_state			keyboard	{}				; // 

		ML_NODISCARD fs::path path2(fs::path const & path) const noexcept
		{
			return content_path.native() + path.native();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// client context
	struct ML_NODISCARD client_context final
	{
		memory			* const mem		; // memory
		client_io		* const io		; // io
		blackboard		* const vars	; // vars
		event_bus		* const bus		; // bus
		render_window	* const window	; // window
		imgui_context	* const imgui	; // imgui
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// client object
	template <class Derived
	> struct client_object : trackable, non_copyable, event_listener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit client_object(client_context * context) noexcept
			: event_listener{ ML_check(context)->bus }
			, m_context		{ context }
		{
			ML_assert_msg(get_bus() == m_context->bus, "BUS MISMATCH");
		}

		virtual ~client_object() noexcept override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void on_event(event &&) override = 0;

		using event_listener::get_bus;

		ML_NODISCARD auto get_context	() const noexcept -> client_context	* { return m_context; }
		ML_NODISCARD auto get_imgui		() const noexcept -> imgui_context	* { return m_context->imgui; }
		ML_NODISCARD auto get_io		() const noexcept -> client_io		* { return m_context->io; }
		ML_NODISCARD auto get_memory	() const noexcept -> memory			* { return m_context->mem; }
		ML_NODISCARD auto get_window	() const noexcept -> render_window	* { return m_context->window; }
		ML_NODISCARD auto get_vars		() const noexcept -> blackboard		* { return m_context->vars; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		client_context * const m_context; // context

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CLIENT_API_HPP_