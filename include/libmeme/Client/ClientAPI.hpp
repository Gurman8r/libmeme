#ifndef _ML_CLIENT_CONTEXT_HPP_
#define _ML_CLIENT_CONTEXT_HPP_

#include <libmeme/Core/Events.hpp>
#include <libmeme/Core/Blackboard.hpp>
#include <libmeme/Client/GuiManager.hpp>
#include <libmeme/Client/LoopSystem.hpp>
#include <libmeme/Client/Python.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// client io
	struct ML_NODISCARD client_io final
	{
		using allocator_type = pmr::polymorphic_allocator<byte_t>;

		int32_t const	argc	; // 
		char ** const	argv	; // 
		allocator_type	alloc	; // 
		json			prefs	; // 

		// paths
		fs::path const
			program_name{ argv[0] }							, // 
			program_path{ fs::current_path() }				, // 
			content_path{ prefs["path"].get<fs::path>() }	; // 

		ML_NODISCARD fs::path path2(fs::path const & path) const noexcept
		{
			return content_path.native() + path.native();
		}

		// timing
		timer const	main_timer	{}			; // 
		timer		loop_timer	{ false }	; // 
		duration	delta_time	{}			; // 
		uint64_t	frame_index	{}			; // 

		struct ML_NODISCARD // fps
		{
			float_t					rate	{}; // 
			float_t					accum	{}; // 
			size_t					index	{}; // 
			pmr::vector<float_t>	times	{ 120, allocator_type{} }; // 
		}
		fps;

		struct ML_NODISCARD // input
		{
			vec2d									cursor	{}	; // 
			ds::array<int32_t, mouse_button_MAX>	mouse	{}	; // 
			ds::array<int32_t, key_code_MAX>		keys	{}	; // 
		}
		input;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// client context
	struct ML_NODISCARD client_context final
	{
		memory			* const mem		; // memory
		client_io		* const io		; // io
		blackboard		* const data	; // content
		event_bus		* const bus		; // bus
		render_window	* const window	; // window
		gui_manager		* const imgui	; // imgui
		py_interpreter	* const python	; // python
		loop_system		* const loop	; // loop
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
			ML_assert_msg(m_context->bus == get_bus(), "BUS MISMATCH");
		}

		virtual ~client_object() noexcept override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void on_event(event const &) override = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using event_listener::get_bus;

		ML_NODISCARD auto get_context	() const noexcept -> client_context	* { return m_context; }
		ML_NODISCARD auto get_imgui		() const noexcept -> gui_manager	* { return m_context->imgui; }
		ML_NODISCARD auto get_io		() const noexcept -> client_io		* { return m_context->io; }
		ML_NODISCARD auto get_loop		() const noexcept -> loop_system	* { return m_context->loop; }
		ML_NODISCARD auto get_memory	() const noexcept -> memory			* { return m_context->mem; }
		ML_NODISCARD auto get_python	() const noexcept -> py_interpreter * { return m_context->python; }
		ML_NODISCARD auto get_window	() const noexcept -> render_window	* { return m_context->window; }
		ML_NODISCARD auto get_vars		() const noexcept -> blackboard		* { return m_context->data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		client_context * const m_context;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CLIENT_API_HPP_