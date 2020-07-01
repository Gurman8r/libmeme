#include <libmeme/Platform/SharedLibrary.hpp>
#include <libmeme/Platform/Native.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool shared_library::open(fs::path const & path)
	{
		// already open
		if (m_handle) { return false; }

		// set path
		if ((m_path = path).extension().empty())
		{
			m_path += default_extension; // no extension provided
		}

		// open library
		return m_handle = std::invoke([&]() noexcept
		{
#if defined(ML_os_windows)
			return LoadLibraryExW(m_path.c_str(), nullptr, 0);

#elif defined(ML_os_unix)
			return dlopen(path.string().c_str(), RTLD_LOCAL | RTLD_LAZY);

#else
			return nullptr;
#endif
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool shared_library::close()
	{
		// not open
		if (!m_handle) { return false; }

		// clear path
		m_path.clear();

		// clear symbols
		m_symbols.clear();

		// close library
		return std::invoke([&]() noexcept
		{
#if defined(ML_os_windows)
			return FreeLibrary(static_cast<HINSTANCE>(m_handle));

#elif defined(ML_os_unix)
			return dlclose(m_handle);

#else
			return false;
#endif
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * shared_library::load_symbol(pmr::string const & name)
	{
		// not open
		if (!m_handle) { return nullptr; }

		// load symbol
		return m_symbols.find_or_add_fn(util::hash(name), [&]() noexcept
		{
#if defined(ML_os_windows)
			return GetProcAddress(static_cast<HINSTANCE>(m_handle), name.c_str());

#elif defined(ML_os_unix)
			return dlsym(m_handle, name.c_str());

#else
			return nullptr;
#endif
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}