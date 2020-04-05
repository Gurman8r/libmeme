#include <libmeme/Engine/ScriptObject.hpp>
#include <libmeme/Engine/Engine.hpp>

namespace ml::embed
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	script_object::script_object(py::object self, py::kwargs kwargs)
		: m_self{ self }
		, m_kwargs{ kwargs }
		, m_flags{}
		, m_funcs{}
	{
		for (cstring name : func_names) { load_fn(name); }

		set_enabled(get_opt<bool>("enabled"));

		call_fn("on_activate");
	}

	script_object::~script_object()
	{
		call_fn("on_deactivate");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void script_object::install(py::module & m)
	{
		py::class_<script_object>(m, "script_object")
			.def(py::init<py::object, py::kwargs>())

			.def("get_flag", &script_object::get_flag)
			.def("set_flag", &script_object::set_flag)
			.def_property("enabled", &script_object::is_enabled, &script_object::set_enabled)

			.def("call", &script_object::call_fn)
			.def("load", &script_object::load_fn)
			;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}