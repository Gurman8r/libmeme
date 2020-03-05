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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// startup
		class config final : trackable, non_copyable
		{
		public:

			pmr::string			api_version		{}			; // 
			pmr::string			style			{}			; // 
			cstring				ini_file		{}			; // 
			cstring				log_file		{}			; // 
		
		private: friend class context;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// runtime
		class io final : trackable, non_copyable
		{
		public:
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			class ML_EDITOR_API main_menu_t final : trackable, non_copyable
			{
			public:
				using storage_type = typename pmr::vector<
					std::pair<cstring, pmr::vector<std::function<void()>>>
				>;

				static constexpr auto title{ "libmeme##editor##main_menu" };

				bool			open			{ true }	; // 
				storage_type	menus			{}			; // 

				void add_menu(cstring label, std::function<void()> && fn);

			private: friend io;
			} main_menu;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			class ML_EDITOR_API dockspace_t final : trackable, non_copyable
			{
			public:
				using storage_type = pmr::vector<uint32_t>;

				static constexpr auto title{ "libmeme##editor##dockspace" };

				bool			open			{ true }	; // 
				float_t			border			{}			; // 
				vec2			padding			{}			; // 
				float_t			rounding		{}			; // 
				vec2			size			{}			; // 
				float_t			alpha			{ 1.f }		; // 
				storage_type	nodes			{}			; // 

				uint32_t begin_builder(int32_t flags = 0);
				uint32_t end_builder(uint32_t root);
				uint32_t dock(cstring name, uint32_t id);
				uint32_t split(uint32_t i, uint32_t id, int32_t dir, float_t ratio, uint32_t * other);
				uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * other);
				uint32_t split(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * other);

				inline auto & operator[](int32_t const i) { return nodes[(size_t)i]; }

				inline auto const & operator[](int32_t const i) const { return nodes[(size_t)i]; }

			private: friend io;
			} dockspace;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		private: friend class context;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// context
		class context final : trackable, non_copyable
		{
		private:
			friend class		editor						;
			editor::config		m_config		{}			; // startup settings
			editor::io			m_io			{}			; // runtime variables
			void *				m_imgui_context	{}			; // current imgui context
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool initialized() noexcept;

		ML_NODISCARD static bool create_context();

		static bool destroy_context();

		ML_NODISCARD static editor::context * const get_context() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static bool startup(bool install_callbacks);

		static bool shutdown();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void new_frame();

		static void render();

		static void render_frame();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static inline editor::config & get_config() noexcept
		{
			return get_context()->m_config;
		}

		ML_NODISCARD static inline editor::io & get_io() noexcept
		{
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