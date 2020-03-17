#include <libmeme/Engine/ScriptableObject.hpp>

namespace ml::embed
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	py::class_<scriptable_object> & scriptable_object::install(py::module & m, cstring name)
	{
		return py::class_<self_type>(m, name)
			.def(py::init<py::object, py::args, py::kwargs>())

			.def("__bool__"		, &self_type::operator bool, py::is_operator())
			.def("__nonzero__"	, &self_type::operator bool, py::is_operator())
			.def("__enter__"	, &self_type::enter)
			.def("__exit__"		, [](self_type & self, py::args) { return self.exit(); })
			.def("__call__"		, &self_type::broadcast, py::is_operator())

			.def_readwrite("args"	, &self_type::m_args)
			.def_readwrite("kwargs"	, &self_type::m_kwargs)
			.def_property("enabled"	, &self_type::is_enabled, &self_type::set_enabled)

			;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	scriptable_object::scriptable_object(py::object self, py::args args, py::kwargs kwargs)
		: m_self{ self }
		, m_args{ args }, m_kwargs{ kwargs }, m_flags{ scriptable_flags_default }
		, m_clbk{}
	{
		broadcast("awake");
		
		set_enabled(!m_kwargs.contains("enabled") || py::bool_{ m_kwargs["enabled"] });

		if (is_enabled())
		{
			broadcast("start");
		}
	}

	scriptable_object::~scriptable_object()
	{
		set_enabled(false);

		broadcast("on_destroy");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	py::object scriptable_object::enter()
	{
		return m_self;
	}

	py::object scriptable_object::exit()
	{
		return py::none{};
	}

	bool scriptable_object::hook(cstring name)
	{
		return name && py::hasattr(m_self, name) && m_clbk.try_emplace(
			util::hash(name, util::strlen(name)),
			m_self.attr(name).cast<callback>()
		).second;
	}

	py::object scriptable_object::broadcast(cstring name)
	{
		if (auto const it{ m_clbk.find(util::hash(name, util::strlen(name))) })
		{
			return std::invoke(*it->second);
		}
		else if (hook(name))
		{
			return broadcast(name);
		}
		else
		{
			return py::none{};
		}
	}

	void scriptable_object::set_flag(int32_t i, bool b)
	{
		if (get_flag(i) == b) return;
		m_flags = b ? (m_flags | i) : (m_flags & ~i);
		switch (i)
		{
		case scriptable_flags_enabled:
			if (b) broadcast("on_enable");
			else broadcast("on_disable");
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}