#include <libmeme/Platform/SharedLibrary.hpp>
#include <libmeme/Core/Debug.hpp>

#ifdef ML_os_windows
#	include <Windows.h>
#else
// https://reemus.blogspot.com/2009/02/dynamic-load-library-linux.html
#endif

#ifdef ML_os_windows
#	define ML_LIB_EXT L".dll"
#else
#	define ML_LIB_EXT L".so"
#endif

namespace ml::impl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static inline void * load_library(fs::path const & path)
	{
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
		if ((m_path = path).extension().empty())
		{
			m_path += ML_LIB_EXT;
		}

		// clear functions
		m_funcs.clear();

		// open library
		return (m_inst = impl::load_library(m_path));
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
		return m_funcs.find_or_add_fn(
			util::hash(name),
			&impl::load_function, m_inst, name.c_str());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}