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
	
	Script::Script(path_t const & filename)
		: m_language{ Language::Unknown }
		, m_text{}
	{
		loadFromFile(filename);
	}

	Script::Script(Language language, std::string const & text)
		: m_language{ language }
		, m_text{ text }
	{
	}
	
	Script::Script(Script const & copy)
		: m_language{ copy.m_language }
		, m_text{ copy.m_text }
	{
	}
	
	Script::Script(Script && copy) noexcept
		: m_language{}
		, m_text{}
	{
		swap(std::move(copy));
	}
	
	Script::~Script() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	Script & Script::operator=(Script const & other) noexcept
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
	
	void Script::swap(Script & other)
	{
		if (this != std::addressof(other))
		{
			std::swap(m_language, other.m_language);

			m_text.swap(other.m_text);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool Script::loadFromFile(path_t const & filename)
	{
		if (auto const o{ FS::read_file(filename) }; o && !o.value().empty())
		{
			m_text.assign(o.value().cbegin(), o.value().cend());
		}
		else
		{
			m_text.clear();
		}
		return !m_text.empty();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t Script::execute()
	{
		return execute({});
	}

	int32_t Script::execute(std::vector<std::string> const & args)
	{
		switch (m_language)
		{
		case Language::Python: return ML_Python.do_string(m_text);
		
		case Language::Lua: return ML_Lua.do_string(m_text);
		
		default: return 0;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}