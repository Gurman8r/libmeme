#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Core/Debug.hpp>

#define ML_EMBED_PYTHON
#include <libmeme/Engine/Embed.hpp>

namespace ml::embed
{
	PYBIND11_EMBEDDED_MODULE(LIBMEME_ENGINE, m)
	{
		m /* LIBMEME_ENGINE */
			
			// general
			.def("close"		, [](int32_t i = 0) { return engine::close(); })
			.def("is_running"	, []() { return engine::is_running(); })

			// config
			.def("path_to"		, [](cstring s) { return engine::path_to(s).native(); })
			.def("content_home"	, []() { return engine::get_config().content_home; })
			.def("command_line"	, []() { return engine::get_config().command_line; })
			.def("library_home"	, []() { return engine::get_config().library_home; })
			.def("program_name"	, []() { return engine::get_config().program_name; })
			.def("program_path"	, []() { return engine::get_config().program_path; })
			
			// runtime
			.def("total_time"	, []() { return engine::get_time().count(); })
			.def("delta_time"	, []() { return engine::get_runtime().delta_time; })
			.def("frame_count"	, []() { return engine::get_runtime().frame_count; })
			.def("frame_rate"	, []() { return engine::get_runtime().frame_rate; })

			// plugins
			.def("load_plugin"	, [](cstring s) { return engine::load_plugin(s); })

			// scripting
			.def("do_string"	, [](int32_t l, cstring s) { return engine::do_script(l, s); })
			.def("do_file"		, [](cstring s) { return engine::do_script(s); })
			
			;
	};
}