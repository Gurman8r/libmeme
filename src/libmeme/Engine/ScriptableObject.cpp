#include <libmeme/Engine/ScriptableObject.hpp>

namespace ml::embed
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	py::class_<scriptable_object> & scriptable_object::install(py::module & m, cstring name)
	{
		return py::class_<self_type>(m, name)
			// init
			.def(py::init<py::object, py::args, py::kwargs>())

			// convert
			.def("__bool__"		, &self_type::operator bool, py::is_operator())
			.def("__nonzero__"	, &self_type::operator bool, py::is_operator())
			.def("__repr__"		, &self_type::repr)
			.def("__str__"		, &self_type::str)
			
			// execution
			.def("__enter__"	, &self_type::enter)
			.def("__exit__"		, [](self_type & self, py::args) { return self.exit(); })
			.def("__call__"		, &self_type::operator(), py::is_operator())
			.def("hook"			, &self_type::hook)
			
			// properties
			.def_readwrite("args"	, &self_type::m_args)
			.def_readwrite("kwargs"	, &self_type::m_kwargs)
			.def_readonly("flags"	, &self_type::m_flags)
			.def_property("active"	, &self_type::is_active, &self_type::set_active)
			.def_property("enabled"	, &self_type::is_enabled, &self_type::set_enabled)

			;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	scriptable_object::scriptable_object(py::object self, py::args args, py::kwargs kwargs)
		: m_self{ self }
		, m_args{ args }, m_kwargs{ kwargs }, m_flags{ scriptable_flags_default }
		, m_clbk{}
	{
		// load functions
		hook("awake");
		hook("on_destroy");
		hook("on_disable");
		hook("on_enable");
		hook("reset");
		hook("start");
		hook("update");

		set_active(true);
		set_enabled(true);
		operator()("start");
	}

	scriptable_object::~scriptable_object()
	{
		operator()("on_destroy");
		set_enabled(false);
		set_active(false);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	scriptable_object::operator bool() const
	{
		return (bool)m_self;
	}

	py::str scriptable_object::repr() const
	{
		return py::repr(m_self);
	}

	py::str scriptable_object::str() const
	{
		return py::str{ m_self };
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
		return name && py::hasattr(m_self, name) && m_clbk.find_or_add(
			util::hash(name, util::strlen(name)),
			m_self.attr(name).cast<callback>()
		);
	}

	py::object scriptable_object::operator()(cstring name)
	{
		if (name && py::hasattr(m_self, name))
		{
			if (auto const it{ m_clbk.find(util::hash(name, util::strlen(name))) })
			{
				return std::invoke(*it->second);
			}
		}
		return py::none{};
	}

	void scriptable_object::set_flag(int32_t i, bool b)
	{
		if (get_flag(i) == b) return;
		m_flags = b ? (m_flags | i) : (m_flags & ~i);
		switch (i)
		{
		case scriptable_flags_active:
			if (b) operator()("awake");
			break;
		case scriptable_flags_enabled:
			if (b) operator()("on_enable");
			else operator()("on_disable");
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}