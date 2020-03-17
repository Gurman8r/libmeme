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
			.def("__call__"		, &self_type::call, py::is_operator())

			.def_readwrite(			"args"		, &self_type::m_args)
			.def_readwrite(			"kwargs"	, &self_type::m_kwargs)
			.def_readonly(			"flags"		, &self_type::m_flags)
			.def_property(			"enabled"	, &self_type::is_enabled, &self_type::set_enabled)
			;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	scriptable_object::scriptable_object(py::object self, py::args args, py::kwargs kwargs)
		: m_self{ self }
		, m_args{ args }, m_kwargs{ kwargs }
		, m_flags{ scriptable_flags_none }
		, m_clbk{}
	{
		call("awake");
		set_enabled(!m_kwargs.contains("enabled") || py::bool_{ m_kwargs["enabled"] });
	}

	scriptable_object::~scriptable_object()
	{
		set_enabled(false);

		call("on_destroy");
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

	py::object scriptable_object::call(cstring name)
	{
		auto const code{ util::hash(name, util::strlen(name)) };

		if (auto const it{ m_clbk.find(code) })
		{
			switch (code)
			{
			case util::hash("update"):
				if (set_flag(scriptable_flags_active, true))
				{
					call("start");
					return call("update");
				}
				break;
			}
			return (*it->second) ? std::invoke(*it->second) : py::none{};
		}
		else if (py::hasattr(m_self, name) && m_clbk.find_or_add(
			code, m_self.attr(name).cast<callback>()
		))
		{
			return call(name);
		}
		else
		{
			return py::none{};
		}
	}

	bool scriptable_object::set_flag(int32_t i, bool b)
	{
		if (get_flag(i) == b) return false;
		m_flags = b ? (m_flags | i) : (m_flags & ~i);
		switch (i)
		{
		case scriptable_flags_enabled:
			if (b) call("on_enable"); else call("on_disable");
			break;
		}
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}