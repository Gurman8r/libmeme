#include <libmeme/Engine/Script.hpp>
#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Engine/Python.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	script::script()
		: m_language{ language::unknown }
		, m_text{ allocator_type{} }
	{
	}

	script::script(allocator_type const & alloc)
		: m_language{ language::unknown }
		, m_text{ alloc }
	{
	}
	
	script::script(path_t const & path, allocator_type const & alloc)
		: m_language{ language::unknown }
		, m_text{ alloc }
	{
		load_from_file(path);
	}

	script::script(language lang, pmr::string const & text, allocator_type const & alloc)
		: m_language{ lang }
		, m_text{ text, alloc }
	{
	}
	
	script::script(script const & other, allocator_type const & alloc)
		: m_language{ other.m_language }
		, m_text{ other.m_text, alloc }
	{
	}
	
	script::script(script && other, allocator_type const & alloc) noexcept
		: m_language{ std::move(other.m_language) }
		, m_text{ std::move(other.m_text), alloc }
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
			std::swap(m_language, other.m_language);

			m_text.swap(other.m_text);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool script::load_from_file(path_t const & path)
	{
		return !(m_text = FS::get_file_contents(path)).empty();
	}

	int32_t script::execute()
	{
		return execute({});
	}

	int32_t script::execute(pmr::vector<pmr::string> const & args)
	{
		switch (m_language)
		{
		case language::python:
			return Python::do_string(m_text);
		
		case language::lua:
			return Lua::do_string(m_text);
		}
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}