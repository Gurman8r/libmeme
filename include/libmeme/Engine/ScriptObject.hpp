#ifndef _ML_SCRIPT_OBJECT_HPP_
#define _ML_SCRIPT_OBJECT_HPP_

// WIP

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/BitSet.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Core/StringUtility.hpp>

#ifndef ML_EMBED_PYTHON
#define ML_EMBED_PYTHON
#endif
#include <libmeme/Engine/Embed.hpp>

namespace ml::embed
{
	enum script_object_flags_
	{
		script_object_flags_enabled,
		script_object_flags_MAX
	};

	struct ML_ENGINE_API script_object final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using callback = std::function<void()>;

		using funcs_type = ds::flat_map<hash_t, callback>;

		using flags_type = ds::bitset<script_object_flags_MAX>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr cstring func_names[] =
		{
			"on_activate",
			"on_deactivate",
			"on_tick",
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		script_object(py::object self, py::kwargs kwargs)
			: m_self{ self }
			, m_kwargs{ kwargs }
			, m_flags{}
			, m_funcs{}
		{
			for (cstring name : func_names) { load_fn(name); }

			set_enabled(get_opt<bool>("enabled"));

			call_fn("on_activate");
		}

		~script_object()
		{
			call_fn("on_deactivate");
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto & self() & noexcept { return m_self; }

		auto const & self() const & noexcept { return m_self; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> auto get_opt(cstring name, T dv = {}) const
		{
			return m_kwargs.contains(name) ? m_kwargs[name].cast<T>() : dv;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool get_flag(int32_t i) const noexcept { return m_flags.read(i); }

		ML_NODISCARD bool is_enabled() const noexcept { return get_flag(script_object_flags_enabled); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool set_flag(int32_t i, bool b) noexcept { return m_flags.write(i, b); }

		void set_enabled(bool b) noexcept { set_flag(script_object_flags_enabled, b); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool call_fn(cstring name)
		{
			if (auto const it{ m_funcs.find(util::hash(name, util::strlen(name))) })
			{
				if (*it->second) { std::invoke(*it->second); }

				return true;
			}
			return false;
		}

		bool load_fn(cstring name)
		{
			return py::hasattr(m_self, name) && m_funcs.try_emplace(
				util::hash(name, util::strlen(name)),
				m_self.attr(name).cast<callback>()
			).second;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void install(py::module & m)
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

	private:
		py::object m_self;
		py::kwargs m_kwargs;
		flags_type m_flags;
		funcs_type m_funcs;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SCRIPT_OBJECT_HPP_