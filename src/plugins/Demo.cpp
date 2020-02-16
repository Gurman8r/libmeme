#include <libmeme/Core/PerformanceTracker.hpp>
#include <libmeme/Core/Wrapper.hpp>
#include <libmeme/Core/ECS.hpp>
#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Engine/Script.hpp>
#include <libmeme/Engine/EngineEvents.hpp>
#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/EditorEvents.hpp>
#include <libmeme/Platform/PlatformEvents.hpp>
#include <libmeme/Renderer/RenderStates.hpp>
#include <libmeme/Renderer/Binder.hpp>
#include <libmeme/Renderer/RenderWindow.hpp>
#include <libmeme/Renderer/Model.hpp>
#include <libmeme/Renderer/Shader.hpp>
#include <libmeme/Renderer/RenderTexture.hpp>
#include <libmeme/Renderer/Font.hpp>

namespace ml
{
	struct demo final : plugin
	{
		// (T) TAGS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct t_object {};

		// (C) COMPONENTS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct c_shader		: ds::value_wrapper<shader	> {};
		struct c_material	: ds::value_wrapper<material> {};
		struct c_model		: ds::value_wrapper<model	> {};
		struct c_transform	{ vec3 pos; vec4 rot; vec3 scl; };


		// (S) SIGNATURES
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using s_apply_transforms = meta::list<c_material, c_transform
		>;
		using s_apply_materials = meta::list<c_shader, c_material
		>;
		using s_draw_renderers = meta::list<c_shader, c_model
		>;


		// (X) SYSTEMS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class> struct x_apply_transforms final : ecs::util::x_base<s_apply_transforms>
		{
			void update(c_material & mat, c_transform const & tf)
			{
				mat->set<vec3>("u_position", tf.pos)
					.set<vec4>("u_rotation", tf.rot)
					.set<vec3>("u_scale", tf.scl);
			}
		};

		template <class> struct x_apply_materials final : ecs::util::x_base<s_apply_materials>
		{
			void update(c_shader & shd, c_material const & mat)
			{
				shd->bind(false);
				for (uniform const & u : *mat)
					shd->set_uniform(u);
				shd->unbind();
			}
		};

		template <class> struct x_draw_renderers final : ecs::util::x_base<s_draw_renderers>
		{
			void update(render_target const & target, c_shader const & shd, c_model const & mdl)
			{
				shd->bind(true);
				target.draw(*mdl);
				shd->unbind();
			}
		};


		// (M) MANAGER
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using entity_traits = ecs::traits<

			// tags
			ecs::cfg::tags<
			t_object
			>,

			// components
			ecs::cfg::components<
			c_shader, c_material, c_model, c_transform
			>,

			// signatures
			ecs::cfg::signatures<
			s_apply_transforms, s_apply_materials, s_draw_renderers
			>,

			// systems
			ecs::cfg::systems<
			x_apply_transforms, x_apply_materials, x_draw_renderers
			>
		>;
		
		using entity_manager = ecs::manager<entity_traits>;

		entity_manager m_ecs{};

		pmr::vector<entity_manager::handle> m_handles;


		// GUI
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct gui_window final
		{
			cstring title;
			bool	open;
			cstring shortcut;
			int32_t flags;

			template <class Fn> inline void render(Fn && fn)
			{
				if (!open) return;
				ML_ImGui_ScopeID(ML_ADDRESSOF(this));
				ML_DEFER{ ImGui::End(); };
				if (ImGui::Begin(title, &open, flags))
				{
					std::invoke(ML_FWD(fn), flags);
				}
			}
		};

		gui_window m_imgui_demo		{ "Dear ImGui Demo"	, 0, "", ImGuiWindowFlags_None };
		gui_window m_gui_profiler	{ "profiler"		, 1, "", ImGuiWindowFlags_None };
		gui_window m_gui_ecs		{ "ecs"				, 1, "", ImGuiWindowFlags_None };
		gui_window m_gui_display	{ "display"			, 1, "", ImGuiWindowFlags_NoScrollbar };
		gui_window m_gui_memory		{ "memory"			, 1, "", ImGuiWindowFlags_MenuBar };
		gui_window m_gui_content	{ "content"			, 1, "", ImGuiWindowFlags_None };

		// CONTENT
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pmr::vector<				render_texture	> m_pipeline	{};
		ds::flat_map<pmr::string,	font			> m_fonts		{};
		ds::flat_map<pmr::string,	image			> m_images		{};
		ds::flat_map<pmr::string,	material		> m_materials	{};
		ds::flat_map<pmr::string,	model			> m_models		{};
		ds::flat_map<pmr::string,	script			> m_scripts		{};
		ds::flat_map<pmr::string,	shader			> m_shaders		{};
		ds::flat_map<pmr::string,	texture			> m_textures	{};


		// DEMO
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		demo()
		{
			event_system::add_listener<enter_event		>(this);
			event_system::add_listener<update_event		>(this);
			event_system::add_listener<draw_event		>(this);
			event_system::add_listener<gui_dock_event	>(this);
			event_system::add_listener<gui_draw_event	>(this);
			event_system::add_listener<exit_event		>(this);
		}

		void on_event(event const & value) override
		{
			switch (value.id())
			{
			case hashof_v<enter_event	> : on_enter	(*value.as<	enter_event		>()); break;
			case hashof_v<update_event	> : on_update	(*value.as<	update_event	>()); break;
			case hashof_v<draw_event	> : on_draw		(*value.as<	draw_event		>()); break;
			case hashof_v<gui_dock_event> : on_gui_dock	(*value.as<	gui_dock_event	>()); break;
			case hashof_v<gui_draw_event> : on_gui_draw	(*value.as<	gui_draw_event	>()); break;
			case hashof_v<exit_event	> : on_exit		(*value.as<	exit_event		>()); break;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_enter(enter_event const & ev)
		{
			// load stuff, etc...

			// MENUS
			{
				// File Menu
				editor::get_main_menu().add_menu("file", [&]()
				{
					ML_ImGui_ScopeID(ML_ADDRESSOF(this));
					if (ImGui::MenuItem("quit", "Alt+F4"))
					{
						engine::get_window().close();
					}
				});

				// Tools Menu
				editor::get_main_menu().add_menu("tools", [&]()
				{
					ML_ImGui_ScopeID(ML_ADDRESSOF(this));
					ImGui::MenuItem(m_gui_content.title,	m_gui_content.shortcut,		&m_gui_content.open);
					ImGui::MenuItem(m_gui_display.title,	m_gui_display.shortcut,		&m_gui_display.open);
					ImGui::MenuItem(m_gui_ecs.title,		m_gui_ecs.shortcut,			&m_gui_ecs.open);
					ImGui::MenuItem(m_gui_memory.title,		m_gui_memory.shortcut,		&m_gui_memory.open);
					ImGui::MenuItem(m_gui_profiler.title,	m_gui_profiler.shortcut,	&m_gui_profiler.open);
				});

				// Window Menu
				editor::get_main_menu().add_menu("window", [&]()
				{
					ML_ImGui_ScopeID(ML_ADDRESSOF(this));
					bool fullscreen{ engine::get_window().is_fullscreen() };
					if (ImGui::MenuItem("fullscreen", "", &fullscreen))
					{
						engine::get_window().set_fullscreen(fullscreen);
					}
				});

				// Help Menu
				editor::get_main_menu().add_menu("help", [&]()
				{
					ML_ImGui_ScopeID(ML_ADDRESSOF(this));
					ImGui::MenuItem(m_imgui_demo.title, "", &m_imgui_demo.open);
				});
			}

			// PIPELINE
			{
				m_pipeline.emplace_back(
					make_render_texture(vec2i{ 1280, 720 })
				).create();
			}

			// IMAGES
			{
				if (auto const & img{ m_images["icon"] = make_image(
					fs::path{ "../../../../assets/textures/icon.png" }
				) }; !img.empty())
				{
					engine::get_window().set_icon(img.width(), img.height(), img.data());
				}
			}

			// TEXTURES
			{
				m_textures["doot"] = make_texture(
					fs::path{ "../../../../assets/textures/doot.png" }
				);

				m_textures["navball"] = make_texture(
					fs::path{ "../../../../assets/textures/navball.png" }
				);
			}

			// FONTS
			{
				m_fonts["clacon"] = make_font(
					fs::path{ "../../../../assets/fonts/clacon.ttf" }
				);

				m_fonts["consolas"] = make_font(
					fs::path{ "../../../../assets/fonts/consolas.ttf" }
				);

				m_fonts["lucida_console"] = make_font(
					fs::path{ "../../../../assets/fonts/lucida_console.ttf" }
				);

				m_fonts["minecraft"] = make_font(
					fs::path{ "../../../../assets/fonts/minecraft.ttf" }
				);
			}

			// SHADERS
			{
				m_shaders["2d"] = make_shader(
					fs::path{ "../../../../assets/shaders/2D.vs.shader" },
					fs::path{ "../../../../assets/shaders/basic.fs.shader" }
				);

				m_shaders["3d"] = make_shader(
					fs::path{ "../../../../assets/shaders/3D.vs.shader" },
					fs::path{ "../../../../assets/shaders/basic.fs.shader" }
				);
			}

			// MATERIALS
			{
				// timers
				auto const _timers = make_material(
					make_uniform<float_t>("u_time",		[]() { return engine::get_time().count<float_t>(); }),
					make_uniform<float_t>("u_delta",	[]() { return engine::get_runtime().delta_time; })
				);

				// MVP
				auto const _mvp = make_material(
					make_uniform<mat4	>("u_model",	mat4::identity()),
					make_uniform<mat4	>("u_view",		mat4::identity()),
					make_uniform<mat4	>("u_proj",		mat4::identity())
				);

				// camera
				auto const _camera = make_material(
					make_uniform<vec3	>("u_cam.pos",	vec3{ 0, 0, 3.f }),
					make_uniform<vec3	>("u_cam.dir",	vec3{ 0, 0, -1.f }),
					make_uniform<float_t>("u_cam.fov",	45.0f),
					make_uniform<float_t>("u_cam.near", 0.0001f),
					make_uniform<float_t>("u_cam.far",	1000.0f),
					make_uniform<vec2	>("u_cam.view", vec2{ 1280.f, 720.f })
				);

				// transform
				auto const _transform = make_material(
					make_uniform<vec3	>("u_position", vec3{}),
					make_uniform<vec4	>("u_rotation", vec4{}),
					make_uniform<vec3	>("u_scale",	vec3{})
				);

				// 2d
				m_materials["2d"] = make_material(
					make_uniform<color	>("u_color",	colors::white),
					make_uniform<texture>("u_texture0", &m_textures["doot"])
				) + _timers + _mvp;

				// 3d
				m_materials["3d"] = make_material(
					make_uniform<color	>("u_color",	colors::white),
					make_uniform<texture>("u_texture0", &m_textures["navball"])
				) + _timers + _camera + _transform;
			}

			// MODELS
			{
				m_models["sphere8x6"] = make_model(
					fs::path{ "../../../../assets/models/sphere8x6.obj" }
				);

				m_models["sphere32x24"] = make_model(
					fs::path{ "../../../../assets/models/sphere32x24.obj" }
				);

				// FIXME: upside down?
				m_models["monkey"] = make_model(
					fs::path{ "../../../../assets/models/monkey.obj" }
				);

				// FIXME: ibo broken
				m_models["triangle"] = make_model(make_mesh(
					{
						make_vertex({  0.0f,  0.5f, 0.0f }, vec3::one(), { 0.5f, 1.0f }),
						make_vertex({  0.5f, -0.5f, 0.0f }, vec3::one(), { 1.0f, 0.0f }),
						make_vertex({ -0.5f, -0.5f, 0.0f }, vec3::one(), { 0.0f, 0.0f }),
					},
					{
						0, 1, 2,
					}
				));

				// FIXME: ibo broken
				m_models["quad"] = make_model(make_mesh(
					{
						make_vertex({ +1.0f, +1.0f, 0.0f }, vec3::one(), { 1.0f, 1.0f }),
						make_vertex({ +1.0f, -1.0f, 0.0f }, vec3::one(), { 1.0f, 0.0f }),
						make_vertex({ -1.0f, -1.0f, 0.0f }, vec3::one(), { 0.0f, 0.0f }),
						make_vertex({ -1.0f, +1.0f, 0.0f }, vec3::one(), { 0.0f, 1.0f }),
					},
					{
						0, 1, 3,
						1, 2, 3,
					}
				));
			}

			// ENTITIES
			{
				ML_DEFER{ m_ecs.refresh(); };

				if (auto & h{ m_handles.emplace_back(m_ecs.create_handle()) })
				{
					h.add_tag<t_object>();
					h.add_component<c_shader>	(m_shaders	["3d"]);
					h.add_component<c_material>	(m_materials["3d"]);
					h.add_component<c_model>	(m_models	["sphere32x24"]);
					h.add_component<c_transform>(
						vec3{ 0.f, 0.f, 0.f },
						vec4{ 0.0f, 0.1f, 0.0f, 0.25f },
						vec3{ 1.f, 1.f, 1.f }
					);
				}
			}
		}

		void on_update(update_event const & ev)
		{
			// update stuff, etc...

			m_ecs.update_system<x_apply_transforms>();
			m_ecs.update_system<x_apply_materials>();
		}

		void on_draw(draw_event const & ev)
		{
			// draw stuff, etc...

			if (m_pipeline.empty()) return;

			if (render_texture const & rt{ m_pipeline[0] })
			{
				rt.bind();
				rt.clear_color(colors::magenta);
				rt.viewport(rt.bounds());
				constexpr render_states states{
					{}, {}, cull_state{ false }, {}
				}; states();
				m_ecs.update_system<x_draw_renderers>(rt);
				rt.unbind();
			}
		}

		void on_gui_dock(gui_dock_event const & ev)
		{
			// dock gui windows

			static auto & d{ ev.d.resize(7) };
			static auto
				& root		{ d.get_node(0) },
				& left		{ d.get_node(1) },
				& left_up	{ d.get_node(2) },
				& left_dn	{ d.get_node(3) },
				& right		{ d.get_node(4) },
				& right_up	{ d.get_node(5) },
				& right_dn	{ d.get_node(6) };

			if (root = d.begin_builder(ImGuiDockNodeFlags_AutoHideTabBar))
			{
				constexpr float_t lhs = 0.465f;
				constexpr float_t rhs = 1.f - lhs;

				d.split_node(1, root,	ImGuiDir_Left,	lhs, &root);	// left
				d.split_node(4, root,	ImGuiDir_Right,	rhs, &root);	// right

				d.split_node(2, left,	ImGuiDir_Up,	0.5f, &left);	// left-up
				d.split_node(3, left,	ImGuiDir_Down,	0.5f, &left);	// left-down
				d.split_node(5, right,	ImGuiDir_Up,	0.5f, &right);	// right-up
				d.split_node(6, right,	ImGuiDir_Down,	0.5f, &right);	// right-down

				d.dock_window(m_gui_content.title	, left_dn);
				d.dock_window(m_gui_display.title	, left_up);
				d.dock_window(m_gui_ecs.title		, left_dn);
				d.dock_window(m_gui_memory.title	, right);
				d.dock_window(m_gui_profiler.title	, left_dn);

				d.end_builder(root);
			}
		}

		void on_gui_draw(gui_draw_event const & ev)
		{
			// gui stuff, etc...

			ML_ImGui_ScopeID(ML_ADDRESSOF(this));

			// IMGUI DEMO
			if (m_imgui_demo.open)
			{
				editor::show_imgui_demo(&m_imgui_demo.open);
			}

			// CONTENT
			m_gui_content.render([&](int32_t flags) noexcept
			{
				show_content_gui();
			});

			// DISPLAY
			m_gui_display.render([&](int32_t flags) noexcept
			{
				editor::draw_texture_preview(m_pipeline.back().get_texture());
			});

			// ECS
			m_gui_ecs.render([&](int32_t flags) noexcept
			{
				show_ecs_gui(&m_ecs);
			});

			// PROFILER
			m_gui_profiler.render([&](int32_t flags) noexcept
			{
				show_profiler_gui();
			});

			// MEMORY
			m_gui_memory.render([&](int32_t flags) noexcept
			{
				static auto const & g_buffer{ memory_tracker::get_buffer() };
				static MemoryEditor m; ML_ONCE_CALL()
				{
					m.Open					= true;
					m.ReadOnly				= true;
					m.Cols					= engine::get_window().get_flags() & WindowFlags_Maximized ? 32 : 16;
					m.OptShowOptions		= true;
					m.OptShowDataPreview	= true;
					m.OptShowHexII			= false;
					m.OptShowAscii			= true;
					m.OptGreyOutZeroes		= true;
					m.OptUpperCaseHex		= true;
					m.OptMidColsCount		= 8;
					m.OptAddrDigitsCount	= 0;
					m.HighlightColor		= IM_COL32(0, 255, 255, 50);
					m.ReadFn				= nullptr;
					m.WriteFn				= nullptr;
					m.HighlightFn			= nullptr;
				}
				if (ImGui::BeginMenuBar())
				{
					ImGui::Checkbox("read only", &m.ReadOnly);
					ImGui::EndMenuBar();
				}
				m.DrawContents(g_buffer.first, g_buffer.second, (size_t)g_buffer.first);
			});
		}

		void on_exit(exit_event const & ev)
		{
			// cleanup stuff, etc...

			m_ecs.clear();
			m_images.clear();
			m_shaders.clear();
			m_materials.clear();
			m_models.clear();
			m_textures.clear();
			m_fonts.clear();
			m_pipeline.clear();
			m_scripts.clear();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void show_content_gui()
		{
			editor::help_marker("WIP");
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U = ecs::traits<>
		> static void show_ecs_gui(ecs::manager<U> * man)
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			// SHOW VALUE
			auto show_value = [&](auto const & value)
			{
				ML_ImGui_ScopeID(ML_ADDRESSOF(&value));

				using T = typename std::decay_t<decltype(value)>;
				static constexpr auto info{ typeof_v<T> };
				static constexpr auto type{ nameof<>::filter_all(info.name()) };

				// FLOAT
				if constexpr (std::is_floating_point_v<T>)
				{
					auto temp{ static_cast<float_t>(value) };
					if (ImGui::DragFloat("##value", &temp)) {}
				}
				// INTEGRAL
				else if constexpr (std::is_integral_v<T>)
				{
					// BOOL
					if constexpr (std::is_same_v<T, bool>)
					{
						auto temp{ static_cast<bool>(value) };
						ImGui::Checkbox("##value", &temp);
					}
					// INT64
					else if constexpr (std::is_same_v<T, int64_t>)
					{
						auto temp{ static_cast<int64_t>(value) };
						ImGui::DragScalar("##value", ImGuiDataType_S64, &temp, 0.2f);
					}
					// INT32
					else if constexpr (std::is_same_v<T, int32_t>)
					{
						auto temp{ static_cast<int32_t>(value) };
						ImGui::DragScalar("##value", ImGuiDataType_S32, &temp, 0.2f);
					}
					// UINT64
					else if constexpr (std::is_same_v<T, uint64_t>)
					{
						auto temp{ static_cast<uint64_t>(value) };
						ImGui::DragScalar("##value", ImGuiDataType_U64, &temp, 0.2f);
					}
					// UINT32
					else if constexpr (std::is_same_v<T, uint32_t>)
					{
						auto temp{ static_cast<uint32_t>(value) };
						ImGui::DragScalar("##value", ImGuiDataType_U32, &temp, 0.2f);
					}
				}
				// POINTER
				else if constexpr (std::is_pointer_v<T>)
				{
					char buf[sizeof(size_t) * 2 + 1] = "";
					std::sprintf(buf, "%p", value);
					ImGui::InputText("##value", buf, ML_ARRAYSIZE(buf),
						ImGuiInputTextFlags_ReadOnly
					);
				}
				// STRING
				else if constexpr (util::is_string_v<T>)
				{
					char buf[128] = "";
					if constexpr (std::is_pointer_v<T>)
					{
						std::strcpy(buf, value);
					}
					else
					{
						std::sprintf(buf, "%.*s", (int32_t)value.size(), value.data());
					}
					ImGui::InputText("##value", buf, ML_ARRAYSIZE(buf),
						ImGuiInputTextFlags_ReadOnly
					);
				}
				// SIGNATURE
				else if constexpr (std::is_same_v<T, entity_traits::signature>)
				{
					auto const & style			{ ImGui::GetStyle() };
					auto const button_width		{ ImGui::GetFrameHeight() };
					auto const button_count		{ static_cast<int32_t>(value.size()) };
					auto const window_visible	{ ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x };

					int32_t i{};
					meta::for_types<meta::concat<entity_traits::component_list, entity_traits::tag_list>
					>([&](auto type)
					{
						ML_ImGui_ScopeID(i);
						bool temp{ value.read((size_t)i) };
						ImGui::Checkbox("##value", &temp);

						using T = typename decltype(type)::type;
						using S = typename U::signature;
						static constexpr bool
							is_component{ U::has_component<T>() },
							is_tag{ U::has_tag<T>() };

						std::stringstream ss; ss
							<< nameof_v<T> << "\n"
							<< (is_component ? "component" : (is_tag ? "tag" : "?"))
							<< " [" << (is_component ? i : (is_tag ? i - U::component_count : -1)) << "]"
							<< " 0b" << S{}.set(i)
							;
						editor::tooltip(pmr::string{ ss.str() });

						float_t const last_button{ ImGui::GetItemRectMax().x };
						float_t const next_button{ last_button + style.ItemSpacing.x + button_width };
						if ((i + 1 < button_count) && (next_button < window_visible)) { ImGui::SameLine(); }
						++i;
					});
				}
				else
				{
					ImGui::Text("error type");
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			// SHOW FIELD
			auto show_field = [&](cstring label, auto const & value)
			{
				ML_ImGui_ScopeID(label);
				
				using V = typename std::decay_t<decltype(value)>;
				static constexpr auto info{ typeof_v<V> };
				static constexpr auto type{ nameof<>::filter_all(info.name()) };

				ImGui::AlignTextToFramePadding();
				ImGui::TreeNodeEx("field node",
					ImGuiTreeNodeFlags_Leaf |
					ImGuiTreeNodeFlags_NoTreePushOnOpen |
					ImGuiTreeNodeFlags_Bullet,
					label
				);

				editor::tooltip(pmr::string{ type });

				if (ImGui::BeginPopupContextItem())
				{
					editor::help_marker("WIP");
					ImGui::EndPopup();
				}
				ImGui::NextColumn();
				ImGui::SetNextItemWidth(-1);
				show_value(value);
				ImGui::NextColumn();
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			// SHOW COMPONENT
			auto show_component = [&](auto & c)
			{
				ML_ImGui_ScopeID(ML_ADDRESSOF(&c));

				using C = typename std::decay_t<decltype(c)>;
				static constexpr auto info{ typeof_v<C> };
				static constexpr auto type{ nameof<>::filter_all(info.name()) };

				ImGui::AlignTextToFramePadding();
				bool const node_open{ ImGui::TreeNode("component node",
					"[%u] %.*s",
					entity_traits::component_id<C>(),
					type.size(), type.data()
				) };

				editor::tooltip(pmr::string{ info.name() });

				if (ImGui::BeginPopupContextItem())
				{
					editor::help_marker("WIP");
					ImGui::EndPopup();
				}
				ImGui::NextColumn();

				ImGui::AlignTextToFramePadding();
				ImGui::Text("");
				ImGui::NextColumn();

				if (node_open)
				{
					show_field("addr", &c);
					show_field("size", sizeof(C));
					ImGui::TreePop();
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			// SHOW ENTITY
			auto show_entity = [&](size_t const e)
			{
				ML_ImGui_ScopeID(static_cast<int32_t>(e));

				ImGui::AlignTextToFramePadding();
				bool const node_open{ ImGui::TreeNode("entity node",
					"entity_%u", e
				) };
				if (ImGui::BeginPopupContextItem())
				{
					editor::help_marker("WIP");
					ImGui::EndPopup();
				}
				ImGui::NextColumn();

				ImGui::AlignTextToFramePadding();
				ImGui::Text("");
				ImGui::NextColumn();

				if (node_open)
				{
					show_field("index", e);
					show_field("alive", man->is_alive(e));
					show_field("signature", man->get_signature(e));

					man->for_components(e, [&](auto & c)
					{
						ImGui::Separator();
						show_component(c);
					});
					ImGui::TreePop();
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			enum { gui_layout_tree, gui_layout_list };
			
			static int32_t s_gui_layout{ 0 };

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			ImGui::BeginChildFrame(ImGui::GetID("ecs"),
				ImGui::GetContentRegionAvail(),
				ImGuiWindowFlags_NoScrollbar |
				ImGuiWindowFlags_MenuBar
			);
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::Combo("layout", &s_gui_layout, "tree\0list"))
					ImGui::CloseCurrentPopup();
				ImGui::EndMenuBar();
			}
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 2, 2 });

			switch (s_gui_layout)
			{
			case gui_layout_tree: {
				ImGui::Columns(2);
				ImGui::Separator();
				man->for_entities([&](size_t const e)
				{
					show_entity(e);
				});
				ImGui::Columns(1);
				ImGui::Separator();
			} break;

			case gui_layout_list: {
				man->for_entities([&](size_t const e)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 2, 2 });
					ImGui::Columns(4);
					ImGui::Text("guid"); ImGui::NextColumn();
					ImGui::Text("name"); ImGui::NextColumn();
					ImGui::Text("addr"); ImGui::NextColumn();
					ImGui::Columns(1);
					ImGui::Separator();
					man->for_components(e, [&](auto & c)
					{
						static constexpr auto info{
							typeof_v<std::decay_t<decltype(c)>>
						};
						ImGui::Columns(4);
						ImGui::Text("%u", e); ImGui::NextColumn();
						ImGui::Text("%.*s", info.name().size(), info.name().data()); ImGui::NextColumn();
						ImGui::Text("%p", &c); ImGui::NextColumn();
						ImGui::Columns(1);
					});
					ImGui::PopStyleVar();
				});
			} break;
			}

			ImGui::PopStyleVar();
			ImGui::EndChildFrame();

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void show_profiler_gui()
		{
			ImGui::Columns(2);

			// total time
			ImGui::Text("total time"); ImGui::NextColumn();
			ImGui::Text("%.3fs", engine::get_time().count()); ImGui::NextColumn();

			// delta time
			ImGui::Text("delta time"); ImGui::NextColumn();
			ImGui::Text("%.7fs", engine::get_runtime().delta_time); ImGui::NextColumn();

			// frame rate
			ImGui::Text("frame rate"); ImGui::NextColumn();
			ImGui::Text("%.4ffps", ImGui::GetIO().Framerate); ImGui::NextColumn();

			// benchmarks
			if (auto const & prev{ performance_tracker::last_frame() }; !prev.empty())
			{
				ImGui::Separator();
				for (auto const & elem : prev)
				{
					ImGui::Text("%s", elem.first); ImGui::NextColumn();
					ImGui::Text("%.7fs", elem.second.count()); ImGui::NextColumn();
				}
			}

			ImGui::Separator();
			ImGui::Columns(1);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

extern "C" ML_PLUGIN_API ml::plugin * ml_plugin_main()
{
	static ml::plugin * p{};
	if (!p) p = new ml::demo{};
	return p;
}