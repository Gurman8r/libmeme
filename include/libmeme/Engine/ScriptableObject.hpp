#ifndef _ML_SCRIPTABLE_OBJECT_HPP_
#define _ML_SCRIPTABLE_OBJECT_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Core/StringUtility.hpp>

#ifndef ML_EMBED_PYTHON
#define ML_EMBED_PYTHON
#endif
#include <libmeme/Engine/Embed.hpp>

namespace ml::embed
{
	enum scriptable_flags_ : int32_t
	{
		scriptable_flags_none		= 0,		// none
		scriptable_flags_enabled	= 1 << 0,	// enabled
		scriptable_flags_active		= 1 << 1,	// active
	};

	struct ML_ENGINE_API scriptable_object final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static py::class_<scriptable_object> & install(py::module & m, cstring name);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = scriptable_object;

		using callback = std::function<void()>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit scriptable_object(py::object self, py::args args, py::kwargs kwargs);

		~scriptable_object();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool operator()(cstring name);

		bool call_fn(cstring name);

		bool load_fn(callback & fn, cstring name, callback pre = 0, callback post = 0);

		void on_flag(int32_t i, bool b);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline operator bool() const { return (bool)m_self; }

		ML_NODISCARD inline auto args() const noexcept -> py::args { return m_args; }

		ML_NODISCARD inline auto kwargs() const noexcept -> py::kwargs { return m_kwargs; }

		ML_NODISCARD inline auto flags() const noexcept -> int32_t { return m_flags; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD inline T get_opt(cstring name, T const & dv = {}) const
		{
			if (m_kwargs.contains(name))
			{
				return m_kwargs[name].cast<T>();
			}
			else
			{
				return dv;
			}
		}

		ML_NODISCARD inline bool get_flag(int32_t const i) const noexcept { return m_flags & i; }

		ML_NODISCARD inline bool is_active() const noexcept { return get_flag(scriptable_flags_active); }

		ML_NODISCARD inline bool is_enabled() const noexcept { return get_flag(scriptable_flags_enabled); }

		inline bool set_flag(int32_t i, bool b) noexcept
		{
			if (get_flag(i) != b)
			{
				m_flags = b ? (m_flags | i) : (m_flags & ~i);
				on_flag(i, b);
				return true;
			}
			return false;
		}

		inline void set_enabled(bool b) noexcept { set_flag(scriptable_flags_enabled, b); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		py::object	m_self		;	// self
		py::args	m_args		;	// args
		py::kwargs	m_kwargs	;	// kwargs
		int32_t		m_flags		;	// flags
		callback					// callbacks
			m_awake,
			m_on_destroy,
			m_on_disable,
			m_on_enable,
			m_reset,
			m_start,
			m_update;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// JSON INTERFACE
namespace ml::embed
{
	static void to_json(json & j, scriptable_object const & value)
	{

	}

	static void from_json(json const & j, scriptable_object & value)
	{

	}
}

#endif // !_ML_SCRIPTABLE_OBJECT_HPP_