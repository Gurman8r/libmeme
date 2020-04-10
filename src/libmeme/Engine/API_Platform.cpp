#include <libmeme/Engine/Engine.hpp>

#ifndef ML_EMBED_PYTHON
#define ML_EMBED_PYTHON
#endif
#include <libmeme/Engine/Embed.hpp>

namespace ml::embed
{
	PYBIND11_EMBEDDED_MODULE(libmeme_platform, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// CONTEXT SETTINGS
		py::class_<context_settings>(m, "context_settings")
			.def(py::init<int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, bool, bool>())
			.def("__nonzero__"				, &context_settings::operator bool, py::is_operator())
			.def("__bool__"					, &context_settings::operator bool, py::is_operator())
			.def_readwrite("api"			, &context_settings::api)
			.def_readwrite("major"			, &context_settings::major)
			.def_readwrite("minor"			, &context_settings::minor)
			.def_readwrite("profile"		, &context_settings::profile)
			.def_readwrite("depth_bits"		, &context_settings::depth_bits)
			.def_readwrite("stencil_bits"	, &context_settings::stencil_bits)
			.def_readwrite("multisample"	, &context_settings::multisample)
			.def_readwrite("sRGB_capable"	, &context_settings::sRGB_capable)
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// DISPLAY SETTINGS
		py::class_<display_settings>(m, "display_settings")
			.def(py::init<>())
			
			.def_readwrite("depth", &display_settings::depth)
			
			.def_property("width"
				, [&](display_settings & s) { return s.size[0]; }, [&](display_settings & s, int32_t v) { s.size[0] = v; })
			
			.def_property("height"
				, [&](display_settings & s) { return s.size[1]; }, [&](display_settings & s, int32_t v) { s.size[1] = v; })
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// WINDOW SETTINGS
		py::class_<window_settings>(m, "window_settings")
			.def(py::init<pmr::string const &, display_settings const &, context_settings const &, int32_t>())
			.def_readwrite("title"	, &window_settings::title)
			.def_readwrite("display", &window_settings::display)
			.def_readwrite("context", &window_settings::context)
			.def_readwrite("hints"	, &window_settings::hints)
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// SETUP
		m.attr("__good__") = ([&
			, sys = py::module::import("sys")]()
		{
			return true;
		})();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}