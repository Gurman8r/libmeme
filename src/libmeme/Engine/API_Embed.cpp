#include <libmeme/Engine/Application.hpp>
#include <libmeme/Engine/API_Embed.hpp>

namespace ml::impl
{
	static py::int_ memget(intptr_t ptr, size_t size)
	{
		switch (size)
		{
		default	: return 0;
		case 1	: return *(uint8_t *)ptr;
		case 2	: return *(uint16_t *)ptr;
		case 3	: return *(uint32_t *)ptr;
		case 4	: return *(uint64_t *)ptr;
		}
	}
}

// memelib
PYBIND11_EMBEDDED_MODULE(memelib, m)
{
	using namespace ml;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// EXIT
	m.def("exit", [](py::args) { application::get()->window().close(); });
	py::module::import("builtins").attr("exit") = m.attr("exit");
	py::module::import("sys").attr("exit") = m.attr("exit");

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	py::class_<non_copyable, scoped<non_copyable, no_delete>>(m, "non_copyable");

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ml_project {};
	py::class_<ml_project>(m, "prj")
		.def_property_readonly_static("author"		, [](py::object) { return ML__auth; })
		.def_property_readonly_static("date"		, [](py::object) { return ML__date; })
		.def_property_readonly_static("name"		, [](py::object) { return ML__name; })
		.def_property_readonly_static("time"		, [](py::object) { return ML__time; })
		.def_property_readonly_static("url"			, [](py::object) { return ML__url; })
		.def_property_readonly_static("ver"			, [](py::object) { return ML__ver; })
		.def_property_readonly_static("arch"		, [](py::object) { return ML_arch; })
		.def_property_readonly_static("cc_name"		, [](py::object) { return ML_cc_name; })
		.def_property_readonly_static("cc_version"	, [](py::object) { return ML_cc_version; })
		.def_property_readonly_static("is_debug"	, [](py::object) { return ML_is_debug; })
		.def_property_readonly_static("config"		, [](py::object) { return ML_configuration; })
		.def_property_readonly_static("lang"		, [](py::object) { return ML_lang; })
		.def_property_readonly_static("platform"	, [](py::object) { return ML_platform; })
		.def_property_readonly_static("os_name"		, [](py::object) { return ML_os_name; })
		;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct stdio final
	{
		struct output final
		{
			std::reference_wrapper<std::ostream> m_os;

			output(std::ostream & os = std::cout) noexcept : m_os{ os } {}

			int32_t fileno() const noexcept
			{
				if (auto const addr{ std::addressof(m_os.get()) }; !addr) { return -2; }
				else if (addr == std::addressof(std::cout)) { return 1; }
				else if (addr == std::addressof(std::cerr)) { return 2; }
				else { return -1; }
			}

			void flush() noexcept { m_os.get().flush(); }

			void write(py::object o) noexcept { m_os << o; }

			void writelines(py::list l) noexcept { for (auto const & e : l) { m_os << e; } }
		};

		static auto & cerr(py::object) noexcept { static output temp{ std::cerr }; return temp; }

		static auto & cout(py::object) noexcept { static output temp{ std::cout }; return temp; }
	};
	py::class_<stdio::output>(m, "stdout")
		.def(py::init<>())
		.def("closed"		, []() { return false; })
		.def("isatty"		, []() { return false; })
		.def("readable"		, []() { return false; })
		.def("seekable"		, []() { return false; })
		.def("writable"		, []() { return true; })
		.def("fileno"		, &stdio::output::fileno)
		.def("flush"		, &stdio::output::flush)
		.def("write"		, &stdio::output::write)
		.def("writelines"	, &stdio::output::writelines)
		;
	py::class_<stdio>(m, "stdio")
		.def(py::init<>())
		.def_property_readonly_static("cerr", &stdio::cerr)
		.def_property_readonly_static("cout", &stdio::cout)
		;
	std::invoke([&m, s = py::module::import("sys")]()
	{
		s.attr("stdout") = m.attr("stdio").attr("cout");
		s.attr("stderr") = m.attr("stdio").attr("cout");
		s.attr("stdin") = py::none{};
	});

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	py::class_<json>(m, "json")
		.def(py::init<>())
		.def(py::init([](json const & j) { return json{ j }; }))
		.def(py::init([](cstring s) { return json::parse(s); }))
		.def(py::init([](py::object o) { return json::parse((std::string)py_json_str(o)); }))

		.def_static("parse", [](cstring s) { return json::parse(s); })
		.def_static("dumps", [](py::object o) { return py_json_str(o); })

		.def("__getitem__", [](json & j, int32_t k) -> json & { return j[k]; }, py::is_operator())
		.def("__getitem__", [](json & j, cstring k) -> json & { return j[k]; }, py::is_operator())
		
		.def("__setitem__", [](json & j, cstring k, bool v) { j[k] = v; }, py::is_operator())
		.def("__setitem__", [](json & j, cstring k, int32_t v) { j[k] = v; }, py::is_operator())
		.def("__setitem__", [](json & j, cstring k, float64_t v) { j[k] = v; }, py::is_operator())
		.def("__setitem__", [](json & j, cstring k, cstring v) { j[k] = v; }, py::is_operator())
		.def("__setitem__", [](json & j, cstring k, json const & v) { j[k] = v; }, py::is_operator())

		.def("__repr__", [](json const & j) { return j.dump(); })
		.def("__str__", [](json const & j) { return j.dump(); })
		;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	py::class_<trackable, scoped<trackable>>(m, "trackable");

	py::class_<memory::record>(m, "memory_record")
		.def(py::init<>())
		.def(py::init<memory::record const &>())
		.def_property_readonly("addr", [](memory::record const & o) { return (intptr_t)o.addr; })
		.def_readonly("index", &memory::record::index)
		.def_readonly("size", &memory::record::size)
		.def("__nonzero__", &memory::record::operator bool)
		.def("__repr__", [](memory::record const & o) { return py_json_str(o); })
		.def("__str__", [](memory::record const & o) { return py_json_str(o); })
		;

	m // memory

		// allocation
		.def("malloc"	, [](size_t s) { return (intptr_t)memory::allocate(s); })
		.def("calloc"	, [](size_t c, size_t s) { return (intptr_t)memory::allocate(c, s); })
		.def("free"		, [](intptr_t p) { memory::deallocate((void *)p); })
		.def("realloc"	, [](intptr_t p, size_t s) { return (intptr_t)memory::reallocate((void *)p, s); })
		.def("realloc"	, [](intptr_t p, size_t o, size_t n) { return (intptr_t)memory::reallocate((void *)p, o, n); })

		// utilities
		.def("memrec"	, [](intptr_t p)
		{
			if (auto const it{ memory::get_records().find((void *)p) }) {
				return *it->second;
			} else {
				return memory::record{};
			}
		})
		.def("memstr"	, [](intptr_t p) { return (cstring)p; })
		.def("memstr"	, [](intptr_t l, intptr_t r) { return std::string{ (cstring)l, (cstring)r }; })
		.def("memget"	, [](intptr_t p) { return impl::memget(p, 1); })
		.def("memget"	, [](intptr_t p, size_t s) { return impl::memget(p, s); })
		.def("memget"	, [](intptr_t p, size_t s, size_t n)
		{
			py::list temp{};
			for (intptr_t first = p, last = p + (ptrdiff_t)(s * n); first != last; ++first)
			{
				temp.append(impl::memget(first, s));
			}
			return temp;
		})

		// raw manipulation
		.def("memchr"	, [](intptr_t p, int32_t v, size_t n) { return (intptr_t)std::memchr((void *)p, v, n); })
		.def("memcmp"	, [](intptr_t l, intptr_t r, size_t n) { return std::memcmp((void *)l, (void *)r, n); })
		.def("memcpy"	, [](intptr_t d, intptr_t s, size_t n) { return (intptr_t)std::memcpy((void *)d, (void *)s, n); })
		.def("memmove"	, [](intptr_t d, intptr_t s, size_t n) { return (intptr_t)std::memmove((void *)d, (void *)s, n); })
		.def("memset"	, [](intptr_t d, int32_t v, size_t n) { return (intptr_t)std::memset((void *)d, v, n); })

		// strcpy
		.def("strcpy"	, [](intptr_t d, intptr_t s) { return (intptr_t)std::strcpy((char *)d, (char *)s); })
		.def("strcpy"	, [](intptr_t d, cstring s) { return (intptr_t)std::strcpy((char *)d, s); })
		// strncpy
		.def("strncpy"	, [](intptr_t d, intptr_t s, size_t n) { return (intptr_t)std::strncpy((char *)d, (char *)s, n); })
		.def("strncpy"	, [](intptr_t d, cstring s, size_t n) { return (intptr_t)std::strncpy((char *)d, s, n); })
		// strcat
		.def("strcat"	, [](intptr_t d, intptr_t s) { return (intptr_t)std::strcat((char *)d, (char *)s); })
		.def("strcat"	, [](intptr_t d, cstring s) { return (intptr_t)std::strcat((char *)d, s); })
		// strncat
		.def("strncat"	, [](intptr_t d, intptr_t s, size_t n) { return (intptr_t)std::strncat((char *)d, (char *)s, n); })
		.def("strncat"	, [](intptr_t d, cstring s, size_t n) { return (intptr_t)std::strncat((char *)d, s, n); })
		// strxfrm
		.def("strxfrm"	, [](intptr_t d, intptr_t s, size_t n) { return std::strxfrm((char *)d, (char *)s, n); })
		.def("strxfrm"	, [](intptr_t d, cstring s, size_t n) { return std::strxfrm((char *)d, s, n); })
		
		// strlen
		.def("strlen"	, [](intptr_t s) { return std::strlen((cstring)s); })
		.def("strlen"	, [](cstring s) { return std::strlen(s); })
		// strcmp
		.def("strcmp"	, [](intptr_t l, intptr_t r) { return std::strcmp((cstring)l, (cstring)r); })
		.def("strcmp"	, [](cstring l, intptr_t r) { return std::strcmp(l, (cstring)r); })
		.def("strcmp"	, [](intptr_t l, cstring r) { return std::strcmp((cstring)l, r); })
		.def("strcmp"	, [](cstring l, cstring r) { return std::strcmp(l, r); })
		// strncmp
		.def("strncmp"	, [](intptr_t l, intptr_t r, size_t n) { return std::strncmp((cstring)l, (cstring)r, n); })
		.def("strncmp"	, [](cstring l, intptr_t r, size_t n) { return std::strncmp(l, (cstring)r, n); })
		.def("strncmp"	, [](intptr_t l, cstring r, size_t n) { return std::strncmp((cstring)l, r, n); })
		.def("strncmp"	, [](cstring l, cstring r, size_t n) { return std::strncmp(l, r, n); })
		// strcoll
		.def("strcoll"	, [](intptr_t l, intptr_t r) { return std::strcoll((cstring)l, (cstring)r); })
		.def("strcoll"	, [](cstring l, intptr_t r) { return std::strcoll(l, (cstring)r); })
		.def("strcoll"	, [](intptr_t l, cstring r) { return std::strcoll((cstring)l, r); })
		.def("strcoll"	, [](cstring l, cstring r) { return std::strcoll(l, r); })
		// strchr
		.def("strchr"	, [](intptr_t s, int32_t c) { return (intptr_t)std::strchr((cstring)s, c); })
		.def("strchr"	, [](cstring s, int32_t c) { return (intptr_t)std::strchr(s, c); })
		// strrchr
		.def("strrchr"	, [](intptr_t s, int32_t c) { return (intptr_t)std::strrchr((cstring)s, c); })
		.def("strrchr"	, [](cstring s, int32_t c) { return (intptr_t)std::strrchr(s, c); })
		// strspn
		.def("strspn"	, [](intptr_t d, intptr_t s) { return std::strspn((cstring)d, (cstring)s); })
		.def("strspn"	, [](cstring d, intptr_t s) { return std::strspn(d, (cstring)s); })
		.def("strspn"	, [](intptr_t d, cstring s) { return std::strspn((cstring)d, s); })
		.def("strspn"	, [](cstring d, cstring s) { return std::strspn(d, s); })
		// strcspn
		.def("strcspn"	, [](intptr_t d, intptr_t s) { return std::strcspn((cstring)d, (cstring)s); })
		.def("strcspn"	, [](cstring d, intptr_t s) { return std::strcspn(d, (cstring)s); })
		.def("strcspn"	, [](intptr_t d, cstring s) { return std::strcspn((cstring)d, s); })
		.def("strcspn"	, [](cstring d, cstring s) { return std::strcspn(d, s); })
		// strpbrk
		.def("strpbrk"	, [](intptr_t d, intptr_t s) { return (intptr_t)std::strpbrk((cstring)d, (cstring)s); })
		.def("strpbrk"	, [](cstring d, intptr_t s) { return (intptr_t)std::strpbrk(d, (cstring)s); })
		.def("strpbrk"	, [](intptr_t d, cstring s) { return (intptr_t)std::strpbrk((cstring)d, s); })
		.def("strpbrk"	, [](cstring d, cstring s) { return (intptr_t)std::strpbrk(d, s); })
		// strstr
		.def("strstr"	, [](intptr_t l, intptr_t r) { return (intptr_t)std::strstr((cstring)l, (cstring)r); })
		.def("strstr"	, [](cstring l, intptr_t r) { return (intptr_t)std::strstr(l, (cstring)r); })
		.def("strstr"	, [](intptr_t l, cstring r) { return (intptr_t)std::strstr((cstring)l, r); })
		.def("strstr"	, [](cstring l, cstring r) { return (intptr_t)std::strstr(l, r); })
		// strtok
		.def("strtok"	, [](intptr_t s, intptr_t d) { return (intptr_t)std::strtok((char *)s, (cstring)d); })
		.def("strtok"	, [](intptr_t s, cstring d) { return (intptr_t)std::strtok((char *)s, d); })
		;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	py::class_<event, non_copyable>(m, "event")
		.def("__int__", &event::operator hash_t)
		;

	struct py_event_listener : event_listener
	{
		void on_event(event const & ev) override
		{
			PYBIND11_OVERLOAD_PURE(void, event_listener, on_event, ev);
		}
	};

	py::class_<event_listener, py_event_listener>(m, "event_listener")
		.def(py::init<>())
		.def("on_event", &event_listener::on_event)
		;

	m // event_bus
		.def("bus_fire"		, []() {})
		.def("bus_add"		, [](hash_t t, intptr_t l) { event_bus::add_listener(t, (event_listener *)l); })
		.def("bus_remove"	, [](hash_t t, intptr_t l) { event_bus::remove_listener(t, (event_listener *)l); })
		.def("bus_remove"	, [](intptr_t l) { event_bus::remove_listener((event_listener *)l); })
		;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// CONTEXT CLIENT
	py::class_<context_client_>(m, "context_client")
		.def_property_readonly_static("unknown"	, [](py::object) { return (int32_t)context_api_unknown; })
		.def_property_readonly_static("opengl"	, [](py::object) { return (int32_t)context_api_opengl; })
		.def_property_readonly_static("vulkan"	, [](py::object) { return (int32_t)context_api_vulkan; })
		.def_property_readonly_static("directx"	, [](py::object) { return (int32_t)context_api_directx; })
		;

	// CONTEXT PROFILE
	py::class_<context_profile_>(m, "context_profile")
		.def_property_readonly_static("any"			, [](py::object) { return (int32_t)context_profile_any; })
		.def_property_readonly_static("core"		, [](py::object) { return (int32_t)context_profile_core; })
		.def_property_readonly_static("compat"		, [](py::object) { return (int32_t)context_profile_compat; })
		.def_property_readonly_static("debug"		, [](py::object) { return (int32_t)context_profile_debug; })
		;

	// CURSOR MODE
	py::class_<cursor_mode_>(m, "cursor_mode")
		.def_property_readonly_static("normal"		, [](py::object) { return (int32_t)cursor_mode_normal; })
		.def_property_readonly_static("hidden"		, [](py::object) { return (int32_t)cursor_mode_hidden; })
		.def_property_readonly_static("disabled"	, [](py::object) { return (int32_t)cursor_mode_disabled; })
		;

	// CURSOR SHAPE
	py::class_<cursor_shape_>(m, "cursor_shape")
		.def_property_readonly_static("arrow"		, [](py::object) { return (int32_t)cursor_shape_arrow; })
		.def_property_readonly_static("ibeam"		, [](py::object) { return (int32_t)cursor_shape_ibeam; })
		.def_property_readonly_static("crosshair"	, [](py::object) { return (int32_t)cursor_shape_crosshair; })
		.def_property_readonly_static("hand"		, [](py::object) { return (int32_t)cursor_shape_hand; })
		.def_property_readonly_static("ew"			, [](py::object) { return (int32_t)cursor_shape_ew; })
		.def_property_readonly_static("ns"			, [](py::object) { return (int32_t)cursor_shape_ns; })
		.def_property_readonly_static("nesw"		, [](py::object) { return (int32_t)cursor_shape_nesw; })
		.def_property_readonly_static("nwse"		, [](py::object) { return (int32_t)cursor_shape_nwse; })
		;

	// KEY CODE
	py::class_<key_code_>(m, "key_code")
		.def_property_readonly_static("space"			, [](py::object) { return (int32_t)key_code_space; })
		.def_property_readonly_static("apostrophe"		, [](py::object) { return (int32_t)key_code_apostrophe; })
		.def_property_readonly_static("comma"			, [](py::object) { return (int32_t)key_code_comma; })
		.def_property_readonly_static("minus"			, [](py::object) { return (int32_t)key_code_minus; })
		.def_property_readonly_static("period"			, [](py::object) { return (int32_t)key_code_period; })
		.def_property_readonly_static("slash"			, [](py::object) { return (int32_t)key_code_slash; })
		.def_property_readonly_static("num_0"			, [](py::object) { return (int32_t)key_code_num_0; })
		.def_property_readonly_static("num_1"			, [](py::object) { return (int32_t)key_code_num_1; })
		.def_property_readonly_static("num_2"			, [](py::object) { return (int32_t)key_code_num_2; })
		.def_property_readonly_static("num_3"			, [](py::object) { return (int32_t)key_code_num_3; })
		.def_property_readonly_static("num_4"			, [](py::object) { return (int32_t)key_code_num_4; })
		.def_property_readonly_static("num_5"			, [](py::object) { return (int32_t)key_code_num_5; })
		.def_property_readonly_static("num_6"			, [](py::object) { return (int32_t)key_code_num_6; })
		.def_property_readonly_static("num_7"			, [](py::object) { return (int32_t)key_code_num_7; })
		.def_property_readonly_static("num_8"			, [](py::object) { return (int32_t)key_code_num_8; })
		.def_property_readonly_static("num_9"			, [](py::object) { return (int32_t)key_code_num_9; })
		.def_property_readonly_static("semicolon"		, [](py::object) { return (int32_t)key_code_semicolon; })
		.def_property_readonly_static("equal"			, [](py::object) { return (int32_t)key_code_equal; })
		.def_property_readonly_static("a"				, [](py::object) { return (int32_t)key_code_a; })
		.def_property_readonly_static("b"				, [](py::object) { return (int32_t)key_code_b; })
		.def_property_readonly_static("c"				, [](py::object) { return (int32_t)key_code_c; })
		.def_property_readonly_static("d"				, [](py::object) { return (int32_t)key_code_d; })
		.def_property_readonly_static("e"				, [](py::object) { return (int32_t)key_code_e; })
		.def_property_readonly_static("f"				, [](py::object) { return (int32_t)key_code_f; })
		.def_property_readonly_static("g"				, [](py::object) { return (int32_t)key_code_g; })
		.def_property_readonly_static("h"				, [](py::object) { return (int32_t)key_code_h; })
		.def_property_readonly_static("i"				, [](py::object) { return (int32_t)key_code_i; })
		.def_property_readonly_static("j"				, [](py::object) { return (int32_t)key_code_j; })
		.def_property_readonly_static("k"				, [](py::object) { return (int32_t)key_code_k; })
		.def_property_readonly_static("l"				, [](py::object) { return (int32_t)key_code_l; })
		.def_property_readonly_static("m"				, [](py::object) { return (int32_t)key_code_m; })
		.def_property_readonly_static("n"				, [](py::object) { return (int32_t)key_code_n; })
		.def_property_readonly_static("o"				, [](py::object) { return (int32_t)key_code_o; })
		.def_property_readonly_static("p"				, [](py::object) { return (int32_t)key_code_p; })
		.def_property_readonly_static("q"				, [](py::object) { return (int32_t)key_code_q; })
		.def_property_readonly_static("r"				, [](py::object) { return (int32_t)key_code_r; })
		.def_property_readonly_static("s"				, [](py::object) { return (int32_t)key_code_s; })
		.def_property_readonly_static("t"				, [](py::object) { return (int32_t)key_code_t; })
		.def_property_readonly_static("u"				, [](py::object) { return (int32_t)key_code_u; })
		.def_property_readonly_static("v"				, [](py::object) { return (int32_t)key_code_v; })
		.def_property_readonly_static("w"				, [](py::object) { return (int32_t)key_code_w; })
		.def_property_readonly_static("x"				, [](py::object) { return (int32_t)key_code_x; })
		.def_property_readonly_static("y"				, [](py::object) { return (int32_t)key_code_y; })
		.def_property_readonly_static("z"				, [](py::object) { return (int32_t)key_code_z; })
		.def_property_readonly_static("left_bracket"	, [](py::object) { return (int32_t)key_code_left_bracket; })
		.def_property_readonly_static("backslash"		, [](py::object) { return (int32_t)key_code_backslash; })
		.def_property_readonly_static("right_bracket"	, [](py::object) { return (int32_t)key_code_right_bracket; })
		.def_property_readonly_static("grave_accent"	, [](py::object) { return (int32_t)key_code_grave_accent; })
		.def_property_readonly_static("world_1"			, [](py::object) { return (int32_t)key_code_world_1; })
		.def_property_readonly_static("world_2"			, [](py::object) { return (int32_t)key_code_world_2; })
		.def_property_readonly_static("escape"			, [](py::object) { return (int32_t)key_code_escape; })
		.def_property_readonly_static("enter"			, [](py::object) { return (int32_t)key_code_enter; })
		.def_property_readonly_static("tab"				, [](py::object) { return (int32_t)key_code_tab; })
		.def_property_readonly_static("backspace"		, [](py::object) { return (int32_t)key_code_backspace; })
		.def_property_readonly_static("insert"			, [](py::object) { return (int32_t)key_code_insert; })
		.def_property_readonly_static("del"				, [](py::object) { return (int32_t)key_code_del; })
		.def_property_readonly_static("right"			, [](py::object) { return (int32_t)key_code_right; })
		.def_property_readonly_static("left"			, [](py::object) { return (int32_t)key_code_left; })
		.def_property_readonly_static("down"			, [](py::object) { return (int32_t)key_code_down; })
		.def_property_readonly_static("up"				, [](py::object) { return (int32_t)key_code_up; })
		.def_property_readonly_static("page_up"			, [](py::object) { return (int32_t)key_code_page_up; })
		.def_property_readonly_static("page_down"		, [](py::object) { return (int32_t)key_code_page_down; })
		.def_property_readonly_static("home"			, [](py::object) { return (int32_t)key_code_home; })
		.def_property_readonly_static("end"				, [](py::object) { return (int32_t)key_code_end; })
		.def_property_readonly_static("caps_lock"		, [](py::object) { return (int32_t)key_code_caps_lock; })
		.def_property_readonly_static("scroll_lock"		, [](py::object) { return (int32_t)key_code_scroll_lock; })
		.def_property_readonly_static("num_lock"		, [](py::object) { return (int32_t)key_code_num_lock; })
		.def_property_readonly_static("print_screen"	, [](py::object) { return (int32_t)key_code_print_screen; })
		.def_property_readonly_static("pause"			, [](py::object) { return (int32_t)key_code_pause; })
		.def_property_readonly_static("fn_1"			, [](py::object) { return (int32_t)key_code_fn_1; })
		.def_property_readonly_static("fn_2"			, [](py::object) { return (int32_t)key_code_fn_2; })
		.def_property_readonly_static("fn_3"			, [](py::object) { return (int32_t)key_code_fn_3; })
		.def_property_readonly_static("fn_4"			, [](py::object) { return (int32_t)key_code_fn_4; })
		.def_property_readonly_static("fn_5"			, [](py::object) { return (int32_t)key_code_fn_5; })
		.def_property_readonly_static("fn_6"			, [](py::object) { return (int32_t)key_code_fn_6; })
		.def_property_readonly_static("fn_7"			, [](py::object) { return (int32_t)key_code_fn_7; })
		.def_property_readonly_static("fn_8"			, [](py::object) { return (int32_t)key_code_fn_8; })
		.def_property_readonly_static("fn_9"			, [](py::object) { return (int32_t)key_code_fn_9; })
		.def_property_readonly_static("fn_10"			, [](py::object) { return (int32_t)key_code_fn_10; })
		.def_property_readonly_static("fn_11"			, [](py::object) { return (int32_t)key_code_fn_11; })
		.def_property_readonly_static("fn_12"			, [](py::object) { return (int32_t)key_code_fn_12; })
		.def_property_readonly_static("fn_13"			, [](py::object) { return (int32_t)key_code_fn_13; })
		.def_property_readonly_static("fn_14"			, [](py::object) { return (int32_t)key_code_fn_14; })
		.def_property_readonly_static("fn_15"			, [](py::object) { return (int32_t)key_code_fn_15; })
		.def_property_readonly_static("fn_16"			, [](py::object) { return (int32_t)key_code_fn_16; })
		.def_property_readonly_static("fn_17"			, [](py::object) { return (int32_t)key_code_fn_17; })
		.def_property_readonly_static("fn_18"			, [](py::object) { return (int32_t)key_code_fn_18; })
		.def_property_readonly_static("fn_19"			, [](py::object) { return (int32_t)key_code_fn_19; })
		.def_property_readonly_static("fn_20"			, [](py::object) { return (int32_t)key_code_fn_20; })
		.def_property_readonly_static("fn_21"			, [](py::object) { return (int32_t)key_code_fn_21; })
		.def_property_readonly_static("fn_22"			, [](py::object) { return (int32_t)key_code_fn_22; })
		.def_property_readonly_static("fn_23"			, [](py::object) { return (int32_t)key_code_fn_23; })
		.def_property_readonly_static("fn_24"			, [](py::object) { return (int32_t)key_code_fn_24; })
		.def_property_readonly_static("fn_25"			, [](py::object) { return (int32_t)key_code_fn_25; })
		.def_property_readonly_static("kp_0"			, [](py::object) { return (int32_t)key_code_kp_0; })
		.def_property_readonly_static("kp_1"			, [](py::object) { return (int32_t)key_code_kp_1; })
		.def_property_readonly_static("kp_2"			, [](py::object) { return (int32_t)key_code_kp_2; })
		.def_property_readonly_static("kp_3"			, [](py::object) { return (int32_t)key_code_kp_3; })
		.def_property_readonly_static("kp_4"			, [](py::object) { return (int32_t)key_code_kp_4; })
		.def_property_readonly_static("kp_5"			, [](py::object) { return (int32_t)key_code_kp_5; })
		.def_property_readonly_static("kp_6"			, [](py::object) { return (int32_t)key_code_kp_6; })
		.def_property_readonly_static("kp_7"			, [](py::object) { return (int32_t)key_code_kp_7; })
		.def_property_readonly_static("kp_8"			, [](py::object) { return (int32_t)key_code_kp_8; })
		.def_property_readonly_static("kp_9"			, [](py::object) { return (int32_t)key_code_kp_9; })
		.def_property_readonly_static("kp_decimal"		, [](py::object) { return (int32_t)key_code_kp_decimal; })
		.def_property_readonly_static("kp_divide"		, [](py::object) { return (int32_t)key_code_kp_divide; })
		.def_property_readonly_static("kp_multiply"		, [](py::object) { return (int32_t)key_code_kp_multiply; })
		.def_property_readonly_static("kp_subtract"		, [](py::object) { return (int32_t)key_code_kp_subtract; })
		.def_property_readonly_static("kp_add"			, [](py::object) { return (int32_t)key_code_kp_add; })
		.def_property_readonly_static("kp_enter"		, [](py::object) { return (int32_t)key_code_kp_enter; })
		.def_property_readonly_static("kp_equal"		, [](py::object) { return (int32_t)key_code_kp_equal; })
		.def_property_readonly_static("left_shift"		, [](py::object) { return (int32_t)key_code_left_shift; })
		.def_property_readonly_static("left_control"	, [](py::object) { return (int32_t)key_code_left_control; })
		.def_property_readonly_static("left_alt"		, [](py::object) { return (int32_t)key_code_left_alt; })
		.def_property_readonly_static("left_super"		, [](py::object) { return (int32_t)key_code_left_super; })
		.def_property_readonly_static("right_shift"		, [](py::object) { return (int32_t)key_code_right_shift; })
		.def_property_readonly_static("right_control"	, [](py::object) { return (int32_t)key_code_right_control; })
		.def_property_readonly_static("right_alt"		, [](py::object) { return (int32_t)key_code_right_alt; })
		.def_property_readonly_static("right_super"		, [](py::object) { return (int32_t)key_code_right_super; })
		.def_property_readonly_static("menu"			, [](py::object) { return (int32_t)key_code_menu; })
		;

	// KEY MODS
	py::class_<key_mods_>(m, "key_mods")
		.def_property_readonly_static("none", [](py::object) { return (int32_t)key_mods_none; })
		.def_property_readonly_static("shift", [](py::object) { return (int32_t)key_mods_shift; })
		.def_property_readonly_static("ctrl", [](py::object) { return (int32_t)key_mods_ctrl; })
		.def_property_readonly_static("alt", [](py::object) { return (int32_t)key_mods_alt; })
		.def_property_readonly_static("super", [](py::object) { return (int32_t)key_mods_super; })
		.def_property_readonly_static("caps_lock", [](py::object) { return (int32_t)key_mods_caps_lock; })
		.def_property_readonly_static("num_lock", [](py::object) { return (int32_t)key_mods_num_lock; })
		;

	// KEY STATE
	py::class_<input_state_>(m, "key_state")
		.def_property_readonly_static("release", [](py::object) { return (int32_t)input_state_release; })
		.def_property_readonly_static("press", [](py::object) { return (int32_t)input_state_press; })
		.def_property_readonly_static("repeat", [](py::object) { return (int32_t)input_state_repeat; })
		;

	// WINDOW HINTS
	py::class_<window_hints_>(m, "window_hints")
		.def_property_readonly_static("none"			, [](py::object) { return (int32_t)window_hints_none; })
		.def_property_readonly_static("resizable"		, [](py::object) { return (int32_t)window_hints_resizable; })
		.def_property_readonly_static("visible"			, [](py::object) { return (int32_t)window_hints_visible; })
		.def_property_readonly_static("decorated"		, [](py::object) { return (int32_t)window_hints_decorated; })
		.def_property_readonly_static("focused"			, [](py::object) { return (int32_t)window_hints_focused; })
		.def_property_readonly_static("is_auto_iconify"	, [](py::object) { return (int32_t)window_hints_auto_iconify; })
		.def_property_readonly_static("floating"		, [](py::object) { return (int32_t)window_hints_floating; })
		.def_property_readonly_static("maximized"		, [](py::object) { return (int32_t)window_hints_maximized; })
		.def_property_readonly_static("doublebuffer"	, [](py::object) { return (int32_t)window_hints_doublebuffer; })
		.def_property_readonly_static("center_cursor"	, [](py::object) { return (int32_t)window_hints_center_cursor; })
		.def_property_readonly_static("focus_on_show"	, [](py::object) { return (int32_t)window_hints_focus_on_show; })
		.def_property_readonly_static("default"			, [](py::object) { return (int32_t)window_hints_default; })
		.def_property_readonly_static("default_max"		, [](py::object) { return (int32_t)window_hints_default_max; })
		;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// CONTEXT SETTINGS
	py::class_<context_settings>(m, "context_settings")
		.def(py::init<>())
		.def(py::init<int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, bool, bool>())
		.def_readwrite("api"			, &context_settings::api)
		.def_readwrite("major"			, &context_settings::major)
		.def_readwrite("minor"			, &context_settings::minor)
		.def_readwrite("profile"		, &context_settings::profile)
		.def_readwrite("depth_bits"		, &context_settings::depth_bits)
		.def_readwrite("stencil_bits"	, &context_settings::stencil_bits)
		.def_readwrite("multisample"	, &context_settings::multisample)
		.def_readwrite("srgb_capable"	, &context_settings::srgb_capable)
		;

	// VIDEO MODE
	py::class_<video_mode>(m, "video_mode")
		.def(py::init<>())
		.def(py::init<vec2i const &>())
		.def(py::init<vec2i const &, vec4b const &>())
		.def(py::init<vec2i const &, vec4b const &, int32_t>())
		.def_readwrite("resolution"			, &video_mode::resolution)
		.def_readwrite("bits_per_pixel"		, &video_mode::bits_per_pixel)
		.def_readwrite("refresh_rate"		, &video_mode::refresh_rate)
		.def_static("get_desktop_mode"		, &video_mode::get_desktop_mode)
		.def_static("get_fullscreen_modes"	, &video_mode::get_fullscreen_modes)
		;

	// WINDOW SETTINGS
	py::class_<window_settings>(m, "window_settings")
		.def(py::init<>())
		.def(py::init<pmr::string const &, video_mode const &, context_settings const &, int32_t>())
		.def_readwrite("title"	, &window_settings::title)
		.def_readwrite("video"	, &window_settings::video)
		.def_readwrite("context", &window_settings::context)
		.def_readwrite("hints"	, &window_settings::hints)
		;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}