#include <libmeme/Platform/SharedLibrary.hpp>
#include <libmeme/Core/Debug.hpp>

#ifdef ML_os_windows
#	include <Windows.h>
#else
// https://reemus.blogspot.com/2009/02/dynamic-load-library-linux.html
#endif

#ifdef ML_os_windows
#	define LIB_EXT L".dll"
#else
#	define LIB_EXT L".so"
#endif

namespace ml::impl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static inline void * load_library(fs::path path)
	{
		if (path.extension().empty()) { path += LIB_EXT; } // append extension
#ifdef ML_os_windows
		return LoadLibraryExW(path.c_str(), nullptr, 0);
#else
		return nullptr;
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static inline bool free_library(void * instance)
	{
#ifdef ML_os_windows
		return FreeLibrary(static_cast<HINSTANCE>(instance));
#else
		return false;
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static inline void * load_function(void * instance, cstring name)
	{
#ifdef ML_os_windows
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

	shared_library::shared_library(allocator_type const & alloc) noexcept
		: m_inst{}, m_path{}, m_funcs{ alloc }
	{
	}

	shared_library::shared_library(fs::path const & path, allocator_type const & alloc) noexcept
		: shared_library{ alloc }
	{
		open(path);
	}

	shared_library::shared_library(shared_library && other, allocator_type const & alloc) noexcept
		: shared_library{ alloc }
	{
		swap(std::move(other));
	}

	shared_library::~shared_library() noexcept
	{
		impl::free_library(m_inst);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool shared_library::open(fs::path const & path)
	{
		// already opened
		if (m_inst) { return false; }

		// set path
		m_path = path;

		// clear functions
		m_funcs.clear();

		// open library
		return (m_inst = impl::load_library(path));
	}

	bool shared_library::close()
	{
		// not opened
		if (!m_inst) { return false; }

		// clear path
		m_path.clear();

		// clear functions
		m_funcs.clear();

		// free library
		return impl::free_library(m_inst);
	}

	void * shared_library::load_function(pmr::string const & name)
	{
		// not opened
		if (!m_inst) { return nullptr; }

		// load function
		return m_funcs.find_or_invoke(name, [&]()
		{
			return impl::load_function(m_inst, name.c_str());
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}