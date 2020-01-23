#include <libmeme/Platform/SharedLibrary.hpp>

#ifdef ML_OS_WINDOWS
#	include <Windows.h>
#else
// https://reemus.blogspot.com/2009/02/dynamic-load-library-linux.html
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared_library::shared_library() noexcept
		: m_instance{ nullptr }
		, m_functions{}
	{
	}
	
	shared_library::shared_library(fs::path const & filename)
		: shared_library{}
	{
		open(filename);
	}

	shared_library::shared_library(shared_library && copy) noexcept
		: shared_library{}
	{
		swap(std::move(copy));
	}

	shared_library::~shared_library()
	{
		close();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared_library & shared_library::operator=(shared_library && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void shared_library::swap(shared_library & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_instance, other.m_instance);

			std::swap(m_functions, other.m_functions);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool shared_library::open(fs::path const & filename)
	{
		if (!good())
		{
#ifdef ML_OS_WINDOWS
			if (filename.extension().string() != ".dll") return false;
			return (m_instance = ::LoadLibraryA(filename.string().c_str()));
#else
			if (filename.extension().string() != ".so") return false;
			return (m_instance = nullptr);
#endif
		}
		else
		{
			return false;
		}
	}

	bool shared_library::close()
	{
		if (good())
		{
			m_functions.clear();

#ifdef ML_OS_WINDOWS
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

	void * shared_library::load_function(C_string name)
	{
		if (good())
		{
			if (auto const it{ m_functions.find(name) })
			{
				return (*it->second);
			}
			else
			{
				return (*m_functions.insert(name,
#ifdef ML_OS_WINDOWS
					::GetProcAddress(static_cast<HINSTANCE>(m_instance), name)
#else
					nullptr
#endif
				).first.second);
			}
		}
		else
		{
			return nullptr;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}