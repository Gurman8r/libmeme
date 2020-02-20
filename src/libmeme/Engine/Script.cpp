#include <libmeme/Engine/Script.hpp>
#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Engine/Python.hpp>
#include <libmeme/Core/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	script::script()
		: m_lang{ unknown }
		, m_text{ allocator_type{} }
	{
	}

	script::script(allocator_type const & alloc)
		: m_lang{ unknown }
		, m_text{ alloc }
	{
	}
	
	script::script(fs::path const & path, allocator_type const & alloc)
		: m_lang{ unknown }
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
		: m_lang{ std::move(other.m_lang) }
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
			std::swap(m_lang, other.m_lang);

			m_text.swap(other.m_text);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool script::load_from_file(fs::path const & path)
	{
		if (path.empty() || !fs::exists(path))
			return false;
		switch (util::hash(path.extension().string()))
		{
		default					: return false;
		case util::hash(".lua")	: m_lang = lua; break;
		case util::hash(".py")	: m_lang = python; break;
		}
		return !(m_text = FS::get_file_contents(path)).empty();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t script::execute(pmr::vector<pmr::string> const & args)
	{
		switch (m_lang)
		{
		default		: return 0;
		case lua	: return ml_lua::do_string(m_text);
		case python	: return ml_python::do_string(m_text);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}