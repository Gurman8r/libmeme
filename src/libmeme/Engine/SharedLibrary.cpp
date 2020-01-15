#include <libmeme/Engine/SharedLibrary.hpp>
#include <libmeme/Core/FileSystem.hpp>

#ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
#else
// https://reemus.blogspot.com/2009/02/dynamic-load-library-linux.html
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SharedLibrary::SharedLibrary() noexcept
		: m_instance{ nullptr }
		, m_functions{}
	{
	}
	
	SharedLibrary::SharedLibrary(path_t const & path)
		: SharedLibrary{}
	{
		open(path);
	}

	SharedLibrary::SharedLibrary(SharedLibrary && copy) noexcept
		: SharedLibrary{}
	{
		swap(std::move(copy));
	}

	SharedLibrary::~SharedLibrary()
	{
		close();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SharedLibrary & SharedLibrary::operator=(SharedLibrary && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void SharedLibrary::swap(SharedLibrary & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_instance, other.m_instance);

			std::swap(m_functions, other.m_functions);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool SharedLibrary::open(path_t const & path)
	{
		if (!good())
		{
#ifdef ML_SYSTEM_WINDOWS
			return (m_instance = ::LoadLibraryA(path.string().c_str()));
#else
			return (m_instance = nullptr);
#endif
		}
		else
		{
			return false;
		}
	}

	bool SharedLibrary::close()
	{
		if (good())
		{
			m_functions.clear();

#ifdef ML_SYSTEM_WINDOWS
			return ::FreeLibrary(static_cast<HINSTANCE>(m_instance));
#else
			return (m_instance = nullptr);
#endif
		}
		else
		{
			return false;
		}
	}

	void * SharedLibrary::load_function(std::string const & name)
	{
		if (auto const it{ m_functions.find(name) })
		{
			return (*it->second);
		}
		else if (m_instance)
		{
			return (*m_functions.insert(name,
#ifdef ML_SYSTEM_WINDOWS
				::GetProcAddress(static_cast<HINSTANCE>(m_instance), name.c_str())
#else
				nullptr
#endif
			).first.second);
		}
		else
		{
			return nullptr;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}