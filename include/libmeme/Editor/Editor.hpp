#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <libmeme/Editor/Export.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/Matrix.hpp>
#include <libmeme/Core/NonCopyable.hpp>

namespace ml
{
	class ML_EDITOR_API editor final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		class context;
		class config;
		class runtime;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// startup variables
		class config final : trackable, non_copyable
		{
			friend class context;
		public:
			pmr::string			api_version		{}			; // shading language version
			cstring				ini_file		{}			; // imgui ini file
			cstring				log_file		{}			; // imgui log file
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// runtime variables
		class runtime final : trackable, non_copyable
		{
			friend class context;
		public:
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			// EDITOR MAIN MENU BAR
			class ML_EDITOR_API main_menu_t final : trackable, non_copyable
			{ friend runtime;
			public:
				static constexpr auto title{ "libmeme##editor##main_menu" };
				
				using menus_t = typename pmr::vector<std::pair<
					cstring, pmr::vector<std::function<void()>>
				>>;

				bool	open	{ true };
				menus_t	menus	{};

				void add_menu(cstring label, std::function<void()> && fn);

			} main_menu;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			// EDITOR DOCKSPACE
			class ML_EDITOR_API dockspace_t final : trackable, non_copyable
			{ friend runtime;
			public:
				static constexpr auto title{ "libmeme##editor##dockspace" };

				using nodes_t = pmr::vector<uint32_t>;

				bool	open		{ true };
				float_t	border		{};
				vec2	padding		{};
				float_t	rounding	{};
				vec2	size		{};
				float_t	alpha		{ 1.f };
				nodes_t	nodes		{};

				uint32_t begin_builder(int32_t flags = 0);
				uint32_t end_builder(uint32_t root);
				uint32_t dock(cstring name, uint32_t id);
				uint32_t split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * other);
				uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * other);
				uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * other);

				inline auto & operator[](int32_t const i) { return nodes[(size_t)i]; }

				inline auto const & operator[](int32_t const i) const { return nodes[(size_t)i]; }

			} dockspace;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// editor context
		class context final : trackable, non_copyable
		{
			friend class		editor						;
			editor::config		m_config		{}			; // startup variables
			editor::runtime		m_io			{}			; // runtime variables
			void *				m_imgui_context	{}			; // current imgui context
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool running() noexcept;

		ML_NODISCARD static bool create_context();

		ML_NODISCARD static bool destroy_context();

		ML_NODISCARD static editor::context * const get_context() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool startup();

		ML_NODISCARD static bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void new_frame();

		static void render();

		static void render_frame();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline editor::config & get_config() noexcept
		{
			ML_assert(running());
			return get_context()->m_config;
		}

		ML_NODISCARD static inline editor::runtime & get_io() noexcept
		{
			ML_assert(running());
			return get_context()->m_io;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void show_imgui_demo(bool * p_open = {});

		static void show_imgui_metrics(bool * p_open = {});

		static void show_imgui_about(bool * p_open = {});

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_EDITOR_HPP_