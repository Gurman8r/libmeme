#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

#include <libmeme/Core/StringUtility.hpp>
#include <libmeme/Core/Matrix.hpp>
#include <libmeme/Core/EnumInfo.hpp>
#include <libmeme/Core/BitMask.hpp>
#include <libmeme/Core/EventListener.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/StringUtility.hpp>
#include <libmeme/Platform/Cursor.hpp>
#include <libmeme/Platform/KeyCode.hpp>
#include <libmeme/Platform/MouseButton.hpp>

#define ML_ASPECT(w, h) ((w != 0 && h != 0) ? ((float_t)w / (float_t)(h)) : 0.0f)
#define ML_ASPECT_2(v)	ML_ASPECT(v[0], v[1])

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_PLATFORM_API Window : public Trackable, public NonCopyable, public EventListener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct DisplayMode final
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			vec2u	 size;
			uint32_t depth;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr explicit DisplayMode(vec2u const & size, uint32_t depth)
				: size{ size }, depth{ depth }
			{
			}

			constexpr DisplayMode(uint32_t width, uint32_t height, uint32_t depth)
				: DisplayMode{ { width, height }, depth }
			{
			}

			constexpr DisplayMode(DisplayMode const & copy)
				: DisplayMode{ copy.size, copy.depth }
			{
			}

			constexpr DisplayMode()
				: DisplayMode{ { 0, 0 }, 0 }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr auto width()  const -> uint32_t const & { return size[0]; }

			constexpr auto height() const -> uint32_t const & { return size[1]; }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr bool operator==(DisplayMode const & other) const { return !((*this) < other) && !(other < (*this)); }

			constexpr bool operator!=(DisplayMode const & other) const { return !((*this) == other); }

			constexpr bool operator<(DisplayMode const & other) const { return (this->size < other.size) && (this->depth < other.depth); }

			constexpr bool operator>(DisplayMode const & other) const { return !((*this) < other); }

			constexpr bool operator<=(DisplayMode const & other) const { return ((*this) == other) || ((*this) < other); }

			constexpr bool operator>=(DisplayMode const & other) const { return ((*this) == other) || ((*this) > other); }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Style final
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			using base_type = typename mask16_t;
			using self_type = typename Style;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			enum Flag : uint32_t
			{
				Resizable,
				Visible,
				Decorated,
				Focused,
				AutoIconify,
				Floating,
				Maximized,
				Fullscreen,
				VerticalSync,
			};
			
			static constexpr auto flag_info{ enum_info<Flag>
			{ {
				Flag::Resizable, Flag::Visible, Flag::Decorated, Flag::Focused,
				Flag::AutoIconify, Flag::Floating, Flag::Maximized,
				Flag::Fullscreen, Flag::VerticalSync,
			}, {
				"Resizable", "Visible", "Decorated", "Focused",
				"AutoIconify", "Floating", "Maximized",
				"Fullscreen", "VerticalSync",
			}, {
				"Resizable", "Visible", "Decorated", "Focused",
				"Auto Iconify", "Floating", "Maximized",
				"Fullscreen", "Vertical Sync",
			} } };

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr explicit Style(
				bool resizable, bool visible, bool decorated, bool focused,
				bool autoIconify, bool floating, bool maximized, 
				bool fullscreen, bool vertical_sync
			) : self_type{ base_type { {
				resizable, visible, decorated, focused, 
				autoIconify, floating, maximized, 
				fullscreen, vertical_sync
			} } }
			{
			}

			constexpr Style(Style const & copy)
				: self_type{ copy.m_data }
			{
			}

			constexpr Style(base_type const & data)
				: m_data{ data }
			{
			}

			constexpr Style() noexcept = default;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr auto data() const -> base_type const & { return m_data; }
		
			constexpr auto data() -> base_type & { return m_data; }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr bool resizable()		const { return m_data[Flag::Resizable]; }
			constexpr bool visible()		const { return m_data[Flag::Visible]; }
			constexpr bool decorated()		const { return m_data[Flag::Decorated]; }
			constexpr bool focused()		const { return m_data[Flag::Focused]; }
			constexpr bool autoIconify()	const { return m_data[Flag::AutoIconify]; }
			constexpr bool floating()		const { return m_data[Flag::Floating]; }
			constexpr bool maximized()		const { return m_data[Flag::Maximized]; }
			constexpr bool fullscreen()		const { return m_data[Flag::Fullscreen]; }
			constexpr bool vertical_sync()	const { return m_data[Flag::VerticalSync]; }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		private: base_type m_data{};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Context final
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			enum API : uint32_t
			{
				Unknown, OpenGL, Vulkan, DirectX,
			};

			enum Profile : uint32_t
			{
				Any, Core, Compat, Debug,
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
			uint32_t	api			{ API::Unknown };
			uint32_t	major		{ 1 };
			uint32_t	minor		{ 1 };
			uint32_t	profile		{ Profile::Compat };
			uint32_t	depthBits	{ 32 };
			uint32_t	stencilBits	{ 8 };
			bool		multisample	{ false };
			bool		srgbCapable	{ false };

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr Context() = default;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using CharFun			= typename void(*)(void *, uint32_t);
		using CursorEnterFun	= typename void(*)(void *, int32_t);
		using CursorPosFun		= typename void(*)(void *, float64_t, float64_t);
		using ErrorFun			= typename void(*)(int32_t, C_String);
		using FrameSizeFun		= typename void(*)(void *, int32_t, int32_t);
		using KeyFun			= typename void(*)(void *, int32_t, int32_t, int32_t, int32_t);
		using MouseFun			= typename void(*)(void *, int32_t, int32_t, int32_t);
		using ScrollFun			= typename void(*)(void *, float64_t, float64_t);
		using CloseFun			= typename void(*)(void *);
		using FocusFun			= typename void(*)(void *, int32_t);
		using PositionFun		= typename void(*)(void *, int32_t, int32_t);
		using SizeFun			= typename void(*)(void *, int32_t, int32_t);
		using ProcFun			= typename void *(*)(void);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Window();

		virtual ~Window();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool create(
			std::string const & title, 
			DisplayMode const & videoMode,
			Style const & settings,
			Context const & context
		);

		virtual void onEvent(Event const & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Window & close();
		
		Window & destroy();

		Window & iconify();
		
		Window & makeContextCurrent();
		
		Window & maximize();
		
		Window & restore();
		
		Window & swapBuffers();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Window & setCentered();
		
		Window & setClipboardString(std::string const & value);
		
		Window & setCursor(void * value);
		
		Window & setCursorMode(const Cursor::Mode value);
		
		Window & setCursorPos(vec2i const & value);
		
		Window & setFullscreen(bool value);
		
		Window & setIcon(uint32_t w, uint32_t h, byte_t const * pixels);
		
		Window & setPosition(vec2i const & value);
		
		Window & setMonitor(void * value);
		
		Window & setSize(vec2u const & value);
		
		Window & setTitle(std::string const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose();

		bool isFocused() const;
		
		bool isFullscreen() const;
		
		bool isOpen() const;
		
		int32_t	getAttribute(int32_t value) const;
		
		C_String getClipboardString() const;
		
		vec2 getCursorPos() const;
		
		vec2i getFrameSize() const;
		
		void * getHandle() const;

		int32_t getKey(int32_t value) const;
		
		int32_t	getInputMode(int32_t value) const;
		
		int32_t	getMouseButton(int32_t value) const;
		
		vec2i getPosition() const;
		
		void * getRawHandle() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto getAspect() const -> float_t { return ML_ASPECT_2(getSize()); };
		
		inline auto getContext() const -> Context const & { return m_context; }
		
		inline auto getFrameAspect() const -> float_t { return ML_ASPECT_2(getFrameSize()); };
		
		inline auto getFrameHeight() const -> int32_t { return getFrameSize()[1]; }
		
		inline auto getFrameWidth()	const -> int32_t { return getFrameSize()[0]; }
		
		inline auto getHeight()	const -> uint32_t { return getSize()[1]; }
		
		inline auto getMonitor() const -> void * { return m_monitor; }

		inline auto getShare() const -> void * { return m_share; }
		
		inline auto getSize() const -> vec2u const & { return getDisplayMode().size; }

		inline auto getStyle() const -> Style const & { return m_style; }
		
		inline auto getTitle() const -> std::string const & { return m_title; }
		
		inline auto getDisplayMode() const -> DisplayMode const & { return m_video; }
		
		inline auto getWidth() const -> uint32_t { return getSize()[0]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void * createCustomCursor(uint32_t w, uint32_t h, byte_t const * pixels);
		
		static void * createStandardCursor(Cursor::Shape value);
		
		static bool	destroyCursor(void * value);

		static int32_t extensionSupported(C_String value);

		static void * getContextCurrent();

		static DisplayMode const & getDesktopMode();
		
		static std::vector<DisplayMode> const & getFullscreenModes();

		static ProcFun getProcAddress(C_String value);
		
		static std::vector<void *> const & getMonitors();

		static float64_t getTime();

		static void makeContextCurrent(void * value);

		static void pollEvents();

		static ErrorFun setErrorCallback(ErrorFun value);
		
		static void swapInterval(int32_t value);

		static void terminate();
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		CharFun			setCharCallback			(CharFun		value);
		CursorEnterFun	setCursorEnterCallback	(CursorEnterFun value);
		CursorPosFun	setCursorPosCallback	(CursorPosFun	value);
		FrameSizeFun	setFrameSizeCallback	(FrameSizeFun	value);
		KeyFun			setKeyCallback			(KeyFun			value);
		MouseFun		setMouseCallback		(MouseFun		value);
		ScrollFun		setScrollCallback		(ScrollFun		value);
		CloseFun		setWindowCloseCallback	(CloseFun		value);
		FocusFun		setWindowFocusCallback	(FocusFun		value);
		PositionFun		setWindowPosCallback	(PositionFun	value);
		SizeFun			setWindowSizeCallback	(SizeFun		value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		void * 		m_window;
		void * 		m_monitor;
		void * 		m_share;
		Context		m_context;
		Style		m_style;
		DisplayMode	m_video;
		std::string	m_title;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_HPP_