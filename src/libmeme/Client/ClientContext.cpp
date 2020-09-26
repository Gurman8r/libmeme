#include <libmeme/Client/ClientContext.hpp>
#include <libmeme/Client/ClientEvents.hpp>
#include <libmeme/Window/WindowEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	client_loop * client_loop::g_loop{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	client_loop::client_loop(client_context * client) noexcept
		: client_object	{ client }
		, m_running		{}
	{
		ML_assert(!g_loop && (g_loop = this));

		subscribe<	window_key_event				>();
		subscribe<	window_mouse_event				>();
		subscribe<	window_cursor_position_event	>();
	}

	client_loop::~client_loop() noexcept
	{
		ML_assert(g_loop == this && !(g_loop = nullptr));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void client_loop::on_event(event const & value)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t client_loop::run()
	{
		if (m_running) { return EXIT_FAILURE; }
		else { m_running = true; } ML_defer(&) { m_running = false; };

		auto const context	{ ML_check(get_context()) };
		auto const io		{ ML_check(get_io()) };
		auto const bus		{ ML_check(get_bus()) };
		auto const layers	{ ML_check(get_layers()) };
		auto const win		{ ML_check(get_window()) };
		auto const imgui	{ ML_check(get_imgui()) };

		bus->fire<client_enter_event>(context);
		ML_defer(&) { bus->fire<client_exit_event>(context); };

		if (!win->is_open()) { return EXIT_FAILURE; }
		do
		{
			io->loop_timer.restart();
			auto const dt{ (float_t)io->delta_time.count() };
			io->fps.accum += dt - io->fps.times[io->fps.index];
			io->fps.times[io->fps.index] = dt;
			io->fps.index = (io->fps.index + 1) % io->fps.times.size();
			io->fps.rate = (0.f < io->fps.accum) ? 1.f / (io->fps.accum / (float_t)io->fps.times.size()) : FLT_MAX;
			ML_defer(&io) {
				io->frame_index++;
				io->delta_time = io->loop_timer.elapsed();
			};

			window::poll_events();
			bus->fire<client_update_event>(context);
			for (layer * l : *layers) { l->on_update(io->delta_time); }

			imgui->new_frame();
			for (layer * l : *layers) { l->on_imgui(); }
			imgui->render_frame();

			if (win->has_hints(window_hints_doublebuffer))
			{
				window::swap_buffers(win->get_handle());
			}
		}
		while (win->is_open());
		return EXIT_SUCCESS;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}