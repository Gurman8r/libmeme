/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/Cx.hpp>
#include <libmeme/Window/WindowEvents.hpp>
#include <libmeme/Graphics/Color.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>
#include <libmeme/Graphics/OpenGL.hpp>
#include <libmeme/Graphics/Buffers.hpp>
#include <libmeme/Editor/Editor.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#ifdef ML_CC_MSC
#	pragma comment(lib, "imgui.lib")
#endif


// Testing
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
namespace ml::testing
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static_assert("Unit Tests"

		&& sizeof(mat4b) == (sizeof(unsigned char) * 16)
		&& sizeof(mat4u) == (sizeof(unsigned int) * 16)
		&& sizeof(mat4i) == (sizeof(int) * 16)
		&& sizeof(mat4f) == (sizeof(float) * 16)
		&& sizeof(mat4d) == (sizeof(double) * 16)

		&& nameof_v<bool>			== "bool"
		&& nameof_v<char>			== "char"
		&& nameof_v<wchar_t>		== "wchar_t"
		&& nameof_v<char16_t>		== "char16_t"
		&& nameof_v<char32_t>		== "char32_t"
		&& nameof_v<C_String>		== "const char*"
		&& nameof_v<int8_t>			== "signed char"
		&& nameof_v<int16_t>		== "short"
		&& nameof_v<int32_t>		== "int"
#ifdef ML_CC_MSC
		&& nameof_v<int64_t>		== "__int64"
#else
		&& nameof_v<int64_t>		== "long long"
#endif
		&& nameof_v<uint8_t>		== "unsigned char"
		&& nameof_v<uint16_t>		== "unsigned short"
		&& nameof_v<uint32_t>		== "unsigned int"
#ifdef ML_CC_MSC
		&& nameof_v<uint64_t>		== "unsigned __int64"
#else
		&& nameof_v<uint64_t>		== "unsigned long long"
#endif
		&& nameof_v<float32_t>		== "float"
		&& nameof_v<float64_t>		== "double"
		&& nameof_v<float80_t>		== "long double"
#ifdef ML_HAS_CONSTEXPR_17
		&& nameof_v<std::string>	== "std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >"
		&& nameof_v<std::wstring>	== "std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >"
		&& nameof_v<std::u16string>	== "std::basic_string<char16_t,struct std::char_traits<char16_t>,class std::allocator<char16_t> >"
		&& nameof_v<std::u32string>	== "std::basic_string<char32_t,struct std::char_traits<char32_t>,class std::allocator<char32_t> >"
#else
		&& nameof_v<std::string>	== "std::string"
		&& nameof_v<std::wstring>	== "std::wstring"
		&& nameof_v<std::u16string>	== "std::u16string"
		&& nameof_v<std::u32string>	== "std::u32string"
#endif
	);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr auto flag_info{ enum_info<GL::Flag>
	{ {
		GL::Flag::CullFace,
		GL::Flag::DepthTest,
		GL::Flag::AlphaTest,
		GL::Flag::Blend,
		GL::Flag::Multisample,
		GL::Flag::FramebufferSRGB,
		GL::Flag::ScissorTest,
	},{
		"GL_CULL_FACE",
		"GL_DEPTH_TEST",
		"GL_ALPHA_TEST",
		"GL_BLEND",
		"GL_MULTISAMPLE",
		"GL_FRAMEBUFFER_SRGB",
		"GL_SCISSOR_TEST",
	},{
		"Cull Face",
		"Depth Test",
		"Alpha Test",
		"Blend",
		"Multisample",
		"Framebuffer sRGB",
		"Scissor Test",
	} } };

	static_assert(flag_info.get_index(GL::Blend) == 3);
	static_assert(flag_info.get_name(GL::Blend) == "GL_BLEND");
	static_assert(flag_info.get_desc(GL::Blend) == "Blend");
	static_assert(flag_info.find_by_index(3) == GL::Blend);
	static_assert(flag_info.find_by_name("GL_BLEND") == GL::Blend);
	static_assert(flag_info.find_by_desc("Blend") == GL::Blend);
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


// Settings
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
namespace ml
{
	static constexpr auto const window_title{ StringView {
		"libmeme"		// Title
	} };
	static constexpr auto const window_video{ Window::Video {
		1280,			// Width
		720,			// Height
		32				// Bits-per-Pixel
	} };
	static constexpr auto const window_style{ Window::Style {
		true,			// Resizable
		true,			// Visible
		true,			// Decorated
		true,			// Focused
		true,			// Auto Iconify
		false,			// Floating
		false,			// Maximized
		false,			// Fullscreen
		false,			// Vertical Sync
	} };
	static constexpr auto const window_context{ Window::Context {
		Window::OpenGL,	// API
		4,				// Major Version
		6,				// Minor Version
		Window::Compat,	// Profile
		24,				// Depth Bits
		8,				// Stencil Bits
		false,			// Multisample
		false			// sRGB Capable
	} };
}


// Main
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ml::int32_t main()
{
	using namespace ml;

	// Systems
	ML_MemoryTracker;
	ML_EventSystem;
	ML_PerformanceTracker;
	
	// State
	static struct Monostate final
	{
		RenderWindow window{};

		struct Colors final
		{
			Color bg{ colors::magenta };

		} colors{};

		struct Time final
		{
			Timer main{ true };

			Timer loop{ false };

			float64_t delta{ 0.0 };

		} time{};

	} state{};


	// Create Window
	/* * * * * * * * * * * * * * * * * * * * */
	if (!state.window.create(window_title.str(), window_video, window_style, window_context))
	{
		return Debug::logError("Failed initializing Window") | Debug::pause(1);
	}
	else
	{
		// Install Callbacks
		/* * * * * * * * * * * * * * * * * * * * */
		state.window.setCharCallback([](auto, auto ch)
		{
			ML_EventSystem.fireEvent<CharEvent>(ch);
		});
		state.window.setCursorEnterCallback([](auto, auto entered)
		{
			ML_EventSystem.fireEvent<CursorEnterEvent>(entered);
		});
		state.window.setCursorPosCallback([](auto, auto x, auto y)
		{
			ML_EventSystem.fireEvent<CursorPosEvent>(x, y);
		});
		state.window.setErrorCallback([](auto code, auto desc)
		{
			ML_EventSystem.fireEvent<WindowErrorEvent>(code, desc);
		});
		state.window.setFrameSizeCallback([](auto, auto w, auto h)
		{
			ML_EventSystem.fireEvent<FrameSizeEvent>(w, h);
		});
		state.window.setKeyCallback([](auto, auto button, auto scan, auto action, auto mods)
		{
			ML_EventSystem.fireEvent<KeyEvent>(button, scan, action, mask8_t{ {
				(mods & ML_MOD_SHIFT),
				(mods & ML_MOD_CTRL),
				(mods & ML_MOD_ALT),
				(mods & ML_MOD_SUPER),
				(mods & ML_MOD_CAPSLOCK),
				(mods & ML_MOD_NUMLOCK)
			} });
		});
		state.window.setMouseCallback([](auto, auto button, auto action, auto mods)
		{
			ML_EventSystem.fireEvent<MouseEvent>(button, action, mods);
		});
		state.window.setScrollCallback([](auto, auto x, auto y)
		{
			ML_EventSystem.fireEvent<ScrollEvent>(x, y);
		});
		state.window.setWindowCloseCallback([](auto)
		{
			ML_EventSystem.fireEvent<WindowCloseEvent>();
		});
		state.window.setWindowFocusCallback([](auto, auto focused)
		{
			ML_EventSystem.fireEvent<WindowFocusEvent>(focused);
		});
		state.window.setWindowPosCallback([](auto, auto x, auto y)
		{
			ML_EventSystem.fireEvent<WindowPosEvent>(x, y);
		});
		state.window.setWindowSizeCallback([](auto, auto w, auto h)
		{
			ML_EventSystem.fireEvent<WindowSizeEvent>(w, h);
		});
	}


	// Start Editor
	/* * * * * * * * * * * * * * * * * * * * */
	if (!Editor::startup(state.window.getHandle()))
	{
		return Debug::logError("Failed initializing Window") | Debug::pause(1);
	}


	// Loop
	/* * * * * * * * * * * * * * * * * * * * */
	while (state.window.isOpen())
	{
		state.time.loop.start();
		
		// Begin Step
		/* * * * * * * * * * * * * * * * * * * * */
		{
			ML_BENCHMARK("STEP_BEGIN");

			Window::pollEvents();
		}
		// Update
		/* * * * * * * * * * * * * * * * * * * * */
		{
			ML_BENCHMARK("\tUPDATE");

			// update stuff, etc...
		}
		// Draw
		/* * * * * * * * * * * * * * * * * * * * */
		{
			ML_BENCHMARK("\tDRAW");
			
			ML_GL.clear(
				GL::DepthBufferBit |
				GL::ColorBufferBit
			);
			
			ML_GL.clearColor(
				state.colors.bg[0],
				state.colors.bg[1],
				state.colors.bg[2],
				state.colors.bg[3]
			);

			// draw stuff, etc...
		}
		// Begin Gui
		/* * * * * * * * * * * * * * * * * * * * */
		{
			ML_BENCHMARK("\tGUI_BEGIN");

			Editor::new_frame();
		}
		// Gui
		/* * * * * * * * * * * * * * * * * * * * */
		{
			ML_BENCHMARK("\t\tGUI");
			
			//Editor::show_about_window();
			Editor::show_imgui_demo();
			//Editor::show_user_guide();
			//Editor::show_style_editor();

			// Benchmarks
			ImGui::SetNextWindowSize({ 256, 256 }, ImGuiCond_Once);
			if (ImGui::Begin("Benchmarks", nullptr, ImGuiWindowFlags_None))
			{
				ImGui::Text("%.3f s", state.time.main.elapsed().count());
				ImGui::Text("%.7f s/frame", state.time.delta);
				ImGui::Text("%.4f fps", ImGui::GetIO().Framerate);
				if (auto const & prev{ ML_PerformanceTracker.previous() }; !prev.empty())
				{
					ImGui::Separator();
					ImGui::Columns(2);
					for (auto const & elem : prev)
					{
						ImGui::Text("%s", elem.first);
						ImGui::NextColumn();
						ImGui::Text("%.7fs", elem.second.count());
						ImGui::NextColumn();
					}
					ImGui::Columns(1);
				}
			}
			ImGui::End();
		}
		// End Gui
		/* * * * * * * * * * * * * * * * * * * * */
		{
			ML_BENCHMARK("\tGUI_END");

			Editor::render_frame();
		}
		// End Step
		/* * * * * * * * * * * * * * * * * * * * */
		{
			ML_BENCHMARK("STEP_END");

			if (state.window.getStyle().vertical_sync())
			{
				state.window.swapBuffers(); // Vsync
			}
			else
			{
				ML_GL.flush(); // Uncapped
			}
		}
		ML_PerformanceTracker.end_frame();
		state.time.delta = state.time.loop.stop().elapsed().count();
	}


	// Cleanup
	/* * * * * * * * * * * * * * * * * * * * */
	{
		Editor::shutdown();

		state.window.dispose();
	}


	// Goodbye!
	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */