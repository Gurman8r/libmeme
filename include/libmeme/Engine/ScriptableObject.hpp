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
	struct scriptable_object;

	enum scriptable_flags_
	{
		scriptable_flags_invalid	= -1,		// invalid
		scriptable_flags_none		= 0,		// none
		scriptable_flags_enabled	= 1 << 0,	// enabled
		scriptable_flags_default	= 0			// default
	};

	struct ML_ENGINE_API scriptable_object final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static py::class_<scriptable_object> & install(py::module & m, cstring name);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = scriptable_object;

		using callback = std::function<py::object()>;

		using fn_table = ds::flat_map<hash_t, callback>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit scriptable_object(py::object self, py::args args, py::kwargs kwargs);

		~scriptable_object();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		py::object enter();

		py::object exit();

		bool hook(cstring name);

		py::object broadcast(cstring name);

		void set_flag(int32_t i, bool b);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline operator bool() const { return (bool)m_self; }

		ML_NODISCARD inline auto self() noexcept -> py::object const & { return m_self; }

		ML_NODISCARD inline auto args() noexcept -> py::args & { return m_args; }

		ML_NODISCARD inline auto args() const noexcept -> py::args const & { return m_args; }

		ML_NODISCARD inline auto kwargs() noexcept -> py::kwargs & { return m_kwargs; }

		ML_NODISCARD inline auto kwargs() const noexcept -> py::kwargs const & { return m_kwargs; }

		ML_NODISCARD inline auto flags() const noexcept -> int32_t const & { return m_flags; }

		ML_NODISCARD inline auto callbacks() const noexcept -> fn_table const & { return m_clbk; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool get_flag(int32_t const i) const noexcept { return m_flags & i; }

		ML_NODISCARD inline bool is_enabled() const noexcept { return get_flag(scriptable_flags_enabled); }

		ML_NODISCARD inline void set_enabled(bool b) noexcept { return set_flag(scriptable_flags_enabled, b); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		py::object	m_self		; // self
		py::args	m_args		; // args
		py::kwargs	m_kwargs	; // kwargs
		int32_t		m_flags		; // flags
		fn_table	m_clbk		; // callbacks

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