#include <libmeme/Engine/Script.hpp>
#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Engine/Python.hpp>
#include <libmeme/Core/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Script::Script()
		: m_language{ Language::Unknown }
		, m_text{}
	{
	}
	
	Script::Script(path_t const & path)
		: m_language{ Language::Unknown }
		, m_text{}
	{
		loadFromFile(path);
	}

	Script::Script(Language language, std::string const & text)
		: m_language{ language }
		, m_text{ text }
	{
	}
	
	Script::Script(Script const & other)
		: m_language{ other.m_language }
		, m_text{ other.m_text }
	{
	}
	
	Script::Script(Script && other) noexcept
		: m_language{ std::move(other.m_language) }
		, m_text{ std::move(other.m_text) }
	{
	}
	
	Script::~Script() {}

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

	bool Script::loadFromFile(path_t const & path)
	{
		return !(m_text = FS::get_file_contents(path)).empty();
	}

	int32_t Script::execute()
	{
		return execute({});
	}

	int32_t Script::execute(std::vector<std::string> const & args)
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