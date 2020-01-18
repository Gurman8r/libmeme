#include <libmeme/Engine/Script.hpp>
#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Engine/Python.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Script::Script()
		: m_language{ Language::Unknown }
		, m_text{ allocator_type{} }
	{
	}

	Script::Script(allocator_type const & alloc)
		: m_language{ Language::Unknown }
		, m_text{ alloc }
	{
	}
	
	Script::Script(path_t const & path, allocator_type const & alloc)
		: m_language{ Language::Unknown }
		, m_text{ alloc }
	{
		load_from_file(path);
	}

	Script::Script(Language language, pmr::string const & text, allocator_type const & alloc)
		: m_language{ language }
		, m_text{ text, alloc }
	{
	}
	
	Script::Script(Script const & other, allocator_type const & alloc)
		: m_language{ other.m_language }
		, m_text{ other.m_text, alloc }
	{
	}
	
	Script::Script(Script && other, allocator_type const & alloc) noexcept
		: m_language{ std::move(other.m_language) }
		, m_text{ std::move(other.m_text), alloc }
	{
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	Script & Script::operator=(Script const & other)
	{
		Script temp{ other };
		swap(temp);
		return (*this);
	}
	
	Script & Script::operator=(Script && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void Script::swap(Script & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_language, other.m_language);

			m_text.swap(other.m_text);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Script::load_from_file(path_t const & path)
	{
		return !(m_text = FS::get_file_contents(path)).empty();
	}

	int32_t Script::execute()
	{
		return execute({});
	}

	int32_t Script::execute(pmr::vector<pmr::string> const & args)
	{
		switch (m_language)
		{
		case Language::Python: return Python::do_string(m_text);
		
		case Language::Lua: return Lua::do_string(m_text);
		
		default: return 0;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}