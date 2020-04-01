#include <libmeme/Engine/ScriptableObject.hpp>

namespace ml::embed
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	py::class_<scriptable_object> & scriptable_object::install(py::module & m, cstring name)
	{
		return py::class_<self_type>{ m, name }
			.def(py::init<py::object, py::args, py::kwargs>())

			.def("__bool__"		, &self_type::operator bool, py::is_operator())
			.def("__nonzero__"	, &self_type::operator bool, py::is_operator())
			.def("__call__"		, &self_type::operator(), py::is_operator())

			.def("__enter__"	, [](self_type & self) { return self.m_self; })
			.def("__exit__"		, [](self_type & self, py::args) { return py::none{}; })

			.def_readwrite(			"args"		, &self_type::m_args)
			.def_readwrite(			"kwargs"	, &self_type::m_kwargs)
			.def_readonly(			"flags"		, &self_type::m_flags)
			.def_property(			"enabled"	, &self_type::is_enabled, &self_type::set_enabled)
			.def_property_readonly(	"active"	, &self_type::is_active)
			;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	scriptable_object::scriptable_object(py::object self, py::args args, py::kwargs kwargs)
		: m_self{ self }, m_args{ args }, m_kwargs{ kwargs }, m_flags{}
	{
		load_fn(m_awake		, "awake");
		load_fn(m_on_disable, "on_disable");
		load_fn(m_on_enable	, "on_enable");
		load_fn(m_reset		, "reset");
		load_fn(m_start		, "start");
		load_fn(m_update	, "update",
			[&]() { if (set_flag(scriptable_flags_active, true)) { call_fn("start"); } }
		);

		call_fn("awake");

		set_enabled(get_opt<bool>("enabled", true));
	}

	scriptable_object::~scriptable_object()
	{
		set_enabled(false);

		call_fn("on_destroy");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool scriptable_object::operator()(cstring name)
	{
		return call_fn(name);
	}

	bool scriptable_object::call_fn(cstring name)
	{
		if (!name || !*name) { return false; }

		auto const o{ ([&]() noexcept -> std::optional<std::reference_wrapper<callback>>
		{
			switch (util::hash(name, util::strlen(name)))
			{
			default:						return std::nullopt;
			case util::hash("awake"):		return std::ref(m_awake);
			case util::hash("on_destroy"):	return std::ref(m_on_destroy);
			case util::hash("on_disable"):	return std::ref(m_on_disable);
			case util::hash("on_enable"):	return std::ref(m_on_enable);
			case util::hash("reset"):		return std::ref(m_reset);
			case util::hash("start"):		return std::ref(m_start);
			case util::hash("update"):		return std::ref(m_update);
			}
		})() };

		if (!o.has_value()) { return false; }

		auto & fn{ o->get() };

		if (fn || (!fn && load_fn(fn, name)))
		{
			std::invoke(fn);
		}

		return (bool)fn;
	}

	bool scriptable_object::load_fn(callback & fn, cstring name, callback pre, callback post)
	{
		if (!py::hasattr(m_self, name))
		{
			return false;
		}
		else if (auto const temp{ m_self.attr(name).cast<callback>() }; !temp)
		{
			return false;
		}
		else if (!pre && !post)
		{
			fn = temp;
		}
		else if (pre && !post)
		{
			fn = [temp, pre]()
			{
				std::invoke(pre);
				std::invoke(temp);
			};
		}
		else if (!pre && post)
		{
			fn = [temp, post]()
			{
				std::invoke(temp);
				std::invoke(post);
			};
		}
		else
		{
			fn = [temp, pre, post]()
			{
				std::invoke(pre);
				std::invoke(temp);
				std::invoke(post);
			};
		}
		return true;
	}

	void scriptable_object::on_flag(int32_t i, bool b)
	{
		switch (i)
		{
		case scriptable_flags_enabled:
			if (b) call_fn("on_enable");
			else call_fn("on_disable");
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}