#include <libmeme/Engine/Script.hpp>
#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Engine/Python.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	script::script()
		: m_lang{ language::unknown }
		, m_str{ allocator_type{} }
	{
	}

	script::script(allocator_type const & alloc)
		: m_lang{ language::unknown }
		, m_str{ alloc }
	{
	}
	
	script::script(path_t const & path, allocator_type const & alloc)
		: m_lang{ language::unknown }
		, m_str{ alloc }
	{
		load_from_file(path);
	}

	script::script(language lang, pmr::string const & text, allocator_type const & alloc)
		: m_lang{ lang }
		, m_str{ text, alloc }
	{
	}
	
	script::script(script const & other, allocator_type const & alloc)
		: m_lang{ other.m_lang }
		, m_str{ other.m_str, alloc }
	{
	}
	
	script::script(script && other, allocator_type const & alloc) noexcept
		: m_lang{ std::move(other.m_lang) }
		, m_str{ std::move(other.m_str), alloc }
	{
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

	void script::swap(script & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_lang, other.m_lang);

			m_str.swap(other.m_str);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool script::load_from_file(path_t const & path)
	{
		if (path.empty())
			return false;
		switch (util::hash(path.extension().string()))
		{
		case util::hash(".lua"): m_lang = language::lua;
			break;
		case util::hash(".py"): m_lang = language::python;
			break;
		default:
			return false;
		}
		return !(m_str = FS::get_file_contents(path)).empty();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t script::execute()
	{
		return execute({});
	}

	int32_t script::execute(pmr::vector<pmr::string> const & args)
	{
		switch (m_lang)
		{
		case language::python:
			return Python::do_string(m_str);
		
		case language::lua:
			return Lua::do_string(m_str);
		
		default:
			return 0;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}