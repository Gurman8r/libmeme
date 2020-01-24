#ifndef _ML_SHARED_LIBRARY_HPP_
#define _ML_SHARED_LIBRARY_HPP_

#include <libmeme/Platform/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>

namespace ml
{
	struct ML_PLATFORM_API shared_library final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using functions_type = typename ds::flat_map<pmr::string, void *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr C_string ext{
#ifdef ML_OS_WINDOWS
			".dll"
#else
			".so"
#endif
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shared_library() noexcept;
		
		explicit shared_library(fs::path const & path);
		
		shared_library(shared_library && copy) noexcept;
		
		~shared_library();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shared_library & operator=(shared_library && other) noexcept;

		void swap(shared_library & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool open(fs::path const & path);

		bool close();

		void * load_function(C_string name);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ret, class ... Args
		> inline decltype(auto) load_function(C_string name)
		{
			return reinterpret_cast<Ret(*)(Args...)>(load_function(name));
		}

		template <class Ret, class ... Args
		> inline decltype(auto) call_function(C_string name, Args && ... args)
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

		ML_NODISCARD inline functions_type const & functions() const noexcept
		{
			return m_functions;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool operator==(shared_library const & other) const
		{
			return !(*this < other) && !(other < *this);
		}

		ML_NODISCARD inline bool operator!=(shared_library const & other) const
		{
			return !(*this == other);
		}

		ML_NODISCARD inline bool operator<(shared_library const & other) const
		{
			return (m_instance < other.m_instance);
		}

		ML_NODISCARD inline bool operator>(shared_library const & other) const
		{
			return !(*this < other);
		}

		ML_NODISCARD inline bool operator<=(shared_library const & other) const
		{
			return (*this < other) || (*this == other);
		}

		ML_NODISCARD inline bool operator>=(shared_library const & other) const
		{
			return (*this > other) || (*this == other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void * m_instance;

		functions_type m_functions;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static inline auto make_shared_library(Args && ... args)
	{
		return shared_library{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHARED_LIBRARY_HPP_