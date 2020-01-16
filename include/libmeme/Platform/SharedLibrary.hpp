#ifndef _ML_SHARED_LIBRARY_HPP_
#define _ML_SHARED_LIBRARY_HPP_

#include <libmeme/Platform/Export.hpp>
#include <libmeme/Platform/FileSystem.hpp>
#include <libmeme/Core/MemoryTracker.hpp>

namespace ml
{
	struct ML_PLATFORM_API SharedLibrary final : public Trackable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using functions_t = typename ds::flat_map<std::string, void *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		SharedLibrary() noexcept;
		
		explicit SharedLibrary(path_t const & path);
		
		SharedLibrary(SharedLibrary && copy) noexcept;
		
		~SharedLibrary();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		SharedLibrary & operator=(SharedLibrary && other) noexcept;

		void swap(SharedLibrary & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool open(path_t const & path);

		bool close();

		void * load_function(std::string const & name);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ret, class ... Args
		> inline decltype(auto) load_function(std::string const & name)
		{
			return reinterpret_cast<Ret(*)(Args...)>(load_function(name));
		}

		template <class Ret, class ... Args
		> inline decltype(auto) call_function(std::string const & name, Args && ... args)
		{
			return std::invoke(load_function<Ret, Args...>(name), std::forward<Args>(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline operator bool() const noexcept
		{
			return good();
		}

		ML_NODISCARD inline decltype(auto) address() const noexcept
		{
			return std::addressof(m_instance);
		}

		ML_NODISCARD inline bool good() const noexcept
		{
			return m_instance;
		}

		ML_NODISCARD inline functions_t const & functions() const noexcept
		{
			return m_functions;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool operator==(SharedLibrary const & other) const
		{
			return !(*this < other) && !(other < *this);
		}

		ML_NODISCARD inline bool operator!=(SharedLibrary const & other) const
		{
			return !(*this == other);
		}

		ML_NODISCARD inline bool operator<(SharedLibrary const & other) const
		{
			return (m_instance < other.m_instance);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void * m_instance;

		functions_t m_functions;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static inline auto make_shared_library(Args && ... args)
	{
		return SharedLibrary{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHARED_LIBRARY_HPP_