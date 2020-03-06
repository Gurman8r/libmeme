#include <libmeme/Scripting/Script.hpp>
#include <libmeme/Scripting/Lua.hpp>
#include <libmeme/Scripting/Python.hpp>
#include <libmeme/Core/FileSystem.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	script::script(allocator_type const & alloc)
		: m_lang{}
		, m_text{ alloc }
	{
	}
	
	script::script(filesystem::path const & path, allocator_type const & alloc)
		: m_lang{}
		, m_text{ alloc }
	{
		load_from_file(path);
	}

	script::script(int32_t lang, pmr::string const & text, allocator_type const & alloc)
		: m_lang{ lang }
		, m_text{ text, alloc }
	{
	}
	
	script::script(script const & other, allocator_type const & alloc)
		: m_lang{ other.m_lang }
		, m_text{ other.m_text, alloc }
	{
	}
	
	script::script(script && other, allocator_type const & alloc) noexcept
		: script{ alloc }
	{
		swap(std::move(other));
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	script & script::operator=(script const & other)
	{
		script temp{ other };
		swap(temp);
		return (*this);
	}
	
	script & script::operator=(script && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	script & script::load_from_file(filesystem::path const & path)
	{
		if (path.empty() || !filesystem::exists(path))
		{
			return (*this)
				.set_lang(api::unknown)
				.set_text({});
		}
		else
		{
			return (*this)
				.set_lang(api::language_ext(path.extension().string()))
				.set_text(util::read_file(path));
		}
	}

	void script::swap(script & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_lang, other.m_lang);

			m_text.swap(other.m_text);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t script::startup()
	{
		int32_t status{};

		ML_bitwrite(status, 0, (!ml_lua::is_initialized() && ml_lua::initialize()));

		ML_bitwrite(status, 1, (!ml_python::is_initialized() && ml_python::initialize()));

		return status;
	}

	int32_t script::shutdown()
	{
		int32_t status{};

		ML_bitwrite(status, 0, (ml_lua::is_initialized() && ml_lua::finalize()));

		ML_bitwrite(status, 1, (ml_python::is_initialized() && ml_python::finalize()));

		return status;
	}

	int32_t script::execute(int32_t lang, pmr::string const & text) noexcept
	{
		switch (lang)
		{
		case api::lua: return ml_lua::do_string(text);
		
		case api::python: return ml_python::do_string(text);
		
		default: return 0;
		}
	}

	int32_t script::execute(filesystem::path const & path) noexcept
	{
		if (path.empty() || !filesystem::exists(path))
		{
			return 0;
		}
		else
		{
			return execute(api::language_ext(path.extension().string()), util::read_file(path));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}