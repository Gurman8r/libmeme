#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Core/Debug.hpp>

#define ML_EMBED_PYTHON
#include <libmeme/Engine/Embed.hpp>

namespace ml::embed
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(LIBMEME_CONFIG, m)
	{
		m /* LIBMEME_CONFIG */

			// project
			.def("proj_author"	, []() { return ML__AUTHOR; })
			.def("proj_date"	, []() { return ML__DATE; })
			.def("proj_name"	, []() { return ML__NAME; })
			.def("proj_time"	, []() { return ML__TIME; })
			.def("proj_url"		, []() { return ML__URL; })
			.def("proj_version"	, []() { return ML__VERSION; })

			// operating system
			.def("os"			, []() { return ML_OS_NAME; })

			// compiler
			.def("cc_lang"		, []() { return ML_LANG; })
			.def("cc_name"		, []() { return ML_CC_NAME; })
			.def("cc_ver"		, []() { return ML_CC_VER; })

			// architecture
			.def("arch"			, []() { return ML_ARCH; })
			.def("arch_name"	, []() { return ML_ARCH_NAME; })

			// configuration
			.def("debug"		, []() { return ML_DEBUG; })
			.def("config"		, []() { return ML_CONFIGURATION; })
			
			;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(LIBMEME_STDIO, m)
	{
		m /* LIBMEME_STDIO */

			// stdio
			.def("write"		, [](cstring s) { std::cout << s; })
			
			// console
			.def("clear"		, []() { debug::clear(); })
			.def("pause"		, []() { debug::pause(); })

			// logging
			.def("info"			, [](cstring s) { debug::log::info(s); })
			.def("error"		, [](cstring s) { debug::log::error(s); })
			.def("warning"		, [](cstring s) { debug::log::warning(s); })

			;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(LIBMEME_ENGINE, m)
	{
		m /* LIBMEME_ENGINE */
			
			// config
			.def("library_home"	, []() { return engine::get_config().library_home; })
			.def("command_line"	, []() { return engine::get_config().command_line; })
			.def("content_path"	, []() { return engine::get_config().content_path; })
			.def("program_name"	, []() { return engine::get_config().program_name; })
			.def("program_path"	, []() { return engine::get_config().program_path; })
			
			// timers
			.def("total_time"	, []() { return engine::get_time().count(); })
			.def("delta_time"	, []() { return engine::get_io().delta_time; })
			.def("frame_count"	, []() { return engine::get_io().frame_count; })
			.def("frame_rate"	, []() { return engine::get_io().frame_rate; })
			
			// general
			.def("is_open"		, []() { return engine::is_open(); })
			.def("close"		, []() { return engine::close(); })
			.def("path_to"		, [](cstring s) { return engine::path_to(s).native(); })

			// plugins
			.def("load_plugin"	, [](cstring s) { return engine::load_plugin(s); })

			;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}