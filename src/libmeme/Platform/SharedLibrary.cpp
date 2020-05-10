#include <libmeme/Platform/SharedLibrary.hpp>

#if defined(ML_os_windows)
#	include <Windows.h>
#else
//	https://reemus.blogspot.com/2009/02/dynamic-load-library-linux.html
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool shared_library::open(fs::path const & path)
	{
		// already opened
		if (m_handle) { return false; }

		// set path
		if ((m_path = path).extension().empty())
		{
			m_path += default_extension; // no extension provided
		}

		// clear symbols
		m_symbols.clear();

		// open library
		return m_handle = ([&]() noexcept
		{
#if defined(ML_os_windows)
			return LoadLibraryExW(m_path.c_str(), nullptr, 0);
#else
			return nullptr;
#endif
		})();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool shared_library::close()
	{
		// not opened
		if (!m_handle) { return false; }

		// clear path
		m_path.clear();

		// clear symbols
		m_symbols.clear();

		// free library
		return ([&]() noexcept
		{
#if defined(ML_os_windows)
			return FreeLibrary(static_cast<HINSTANCE>(m_handle));
#else
			return false;
#endif
		})();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * shared_library::load_symbol(pmr::string const & name)
	{
		// not opened
		if (!m_handle) { return nullptr; }

		// load symbol
		return m_symbols.find_or_add_fn(util::hash(name), [&]() noexcept
		{
#if defined(ML_os_windows)
			return GetProcAddress(static_cast<HINSTANCE>(m_handle), name.c_str());
#else
			return nullptr;
#endif
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}