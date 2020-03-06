#include <libmeme/Core/Debug.hpp>

#define ML_EMBED_PYTHON
#include <libmeme/Scripting/Embed.hpp>

namespace ml::embedded
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(LIBMEME_CONFIG, inst)
	{
		inst.def("proj_author"	, []() { return ML__AUTHOR; })
			.def("proj_date"	, []() { return ML__DATE; })
			.def("proj_name"	, []() { return ML__NAME; })
			.def("proj_time"	, []() { return ML__TIME; })
			.def("proj_url"		, []() { return ML__URL; })
			.def("proj_version"	, []() { return ML__VERSION; })
			.def("arch"			, []() { return ML_ARCH; })
			.def("arch_name"	, []() { return ML_ARCH_NAME; })
			.def("cc_name"		, []() { return ML_CC_NAME; })
			.def("cc_ver"		, []() { return ML_CC_VER; })
			.def("config"		, []() { return ML_CONFIGURATION; })
			.def("debug"		, []() { return ML_DEBUG; })
			.def("lang"			, []() { return ML_LANG; })
			.def("system"		, []() { return ML_OS_NAME; })
			;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(LIBMEME_STDIO, inst)
	{
		inst.def("args"			, []()			{ return pmr::vector<cstring>{ ML_argv, ML_argv + ML_argc }; })
			.def("clear"		, []()			{ debug::clear(); })
			.def("pause"		, []()			{ debug::pause(); })
			.def("write"		, [](cstring s) { std::cout << s; })
			.def("info"			, [](cstring s) { debug::log::info(s); })
			.def("error"		, [](cstring s) { debug::log::error(s); })
			.def("warning"		, [](cstring s) { debug::log::warning(s); })
			;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}