#include <libmeme/Engine/Engine.hpp>
#include <libmeme/Core/Debug.hpp>

#define ML_EMBED_PYTHON
#include <libmeme/Scripting/Embed.hpp>

namespace ml::embedded
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PYBIND11_EMBEDDED_MODULE(LIBMEME_ENGINE, inst)
	{
		inst.def("close"		, []()			{ engine::get_window()->close(); })
			.def("load_plugin"	, [](cstring s) { return engine::load_plugin(s); })
			.def("total_time"	, []()			{ return engine::get_time().count<float_t>(); })
			.def("delta_time"	, []()			{ return engine::get_io().delta_time; })
			.def("frame_rate"	, []()			{ return engine::get_io().frame_rate; })
			.def("frame_count"	, []()			{ return engine::get_io().frame_count; })
			;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}