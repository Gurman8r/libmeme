#include <libmeme/Platform/SharedLibrary.hpp>
#include <libmeme/Core/Debug.hpp>

#ifdef ML_OS_WINDOWS
#	include <Windows.h>
#else
// https://reemus.blogspot.com/2009/02/dynamic-load-library-linux.html
#endif

namespace ml::impl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static inline void * load_library(fs::path const & path)
	{
#ifdef ML_OS_WINDOWS
		return LoadLibraryExW(path.native().c_str(), nullptr, 0);
#else
		return nullptr;
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static inline bool free_library(void * instance)
	{
#ifdef ML_OS_WINDOWS
		return FreeLibrary(static_cast<HINSTANCE>(instance));
#else
		return false;
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static inline void * load_function(void * instance, cstring name)
	{
#ifdef ML_OS_WINDOWS
		return GetProcAddress(static_cast<HINSTANCE>(instance), name);
#else
		return nullptr;
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shared_library::shared_library(allocator_type const & alloc)
		: m_instance{ nullptr }
		, m_path	{}
		, m_funcs	{ alloc }
	{
	}
	
	shared_library::shared_library() noexcept
		: shared_library{ allocator_type{} }
	{
	}

	shared_library::shared_library(fs::path const & path, allocator_type const & alloc)
		: shared_library{ alloc }
	{
		open(path);
	}

	shared_library::shared_library(shared_library && copy, allocator_type const & alloc) noexcept
		: shared_library{ alloc }
	{
		swap(std::move(copy));
	}

	shared_library::~shared_library()
	{
		impl::free_library(m_instance);
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
			
			m_path.swap(other.m_path);
			
			m_funcs.swap(other.m_funcs);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool shared_library::open(fs::path const & path)
	{
		// already opened
		if (good()) return false;

		// set path
		m_path = path;

		// open library
		return (m_instance = impl::load_library(path));
	}

	bool shared_library::close()
	{
		// not opened
		if (!good()) return false;

		// cleanup
		m_path.clear();
		m_funcs.clear();

		// free library
		return impl::free_library(m_instance);
	}

	void * shared_library::load_function(cstring name)
	{
		// not opened
		if (!good()) return nullptr;

		// already loaded
		if (auto const it{ m_funcs.find(name) })
		{
			return (*it->second);
		}
		
		// load function
		return (*m_funcs.insert(name, impl::load_function(m_instance, name)).second);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}