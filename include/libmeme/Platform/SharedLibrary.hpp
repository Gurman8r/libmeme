#ifndef _ML_SHARED_LIBRARY_HPP_
#define _ML_SHARED_LIBRARY_HPP_

#include <libmeme/Platform/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>

namespace ml
{
	struct ML_PLATFORM_API shared_library final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		using function_map = typename ds::flat_map<fs::path, void *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit shared_library(allocator_type const & alloc);

		shared_library() noexcept;
		
		shared_library(fs::path const & path, allocator_type const & alloc = {});
		
		shared_library(shared_library && other, allocator_type const & alloc = {}) noexcept;
		
		~shared_library();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shared_library & operator=(shared_library && other) noexcept;

		void swap(shared_library & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool open(fs::path const & path);

		bool close();

		void * load_function(cstring name);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ret, class ... Args
		> ML_NODISCARD inline decltype(auto) load_function(cstring name)
		{
			return reinterpret_cast<Ret(*)(Args...)>(load_function(name));
		}

		template <class Ret, class ... Args
		> inline std::optional<Ret> call_function(cstring name, Args && ... args)
		{
			if (auto const fn{ load_function<Ret, Args...>(name) })
			{
				return std::make_optional(std::invoke(fn, ML_FWD(args)...));
			}
			else
			{
				return std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline operator bool() const noexcept { return good(); }

		ML_NODISCARD inline auto address() const noexcept -> void * const * { return std::addressof(m_instance); }

		ML_NODISCARD inline auto functions() const noexcept -> function_map const & { return m_funcs; }

		ML_NODISCARD inline bool good() const noexcept { return m_instance; }

		ML_NODISCARD inline auto instance() const noexcept -> void const * { return m_instance; }

		ML_NODISCARD inline auto path() const noexcept -> fs::path const & { return m_path; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto compare(shared_library const & other) const noexcept
		{
			return m_path.compare(other.m_path);
		}

		ML_NODISCARD inline bool operator==(shared_library const & other) const
		{
			return compare(other) == 0;
		}

		ML_NODISCARD inline bool operator!=(shared_library const & other) const
		{
			return compare(other) != 0;
		}

		ML_NODISCARD inline bool operator<(shared_library const & other) const
		{
			return compare(other) < 0;
		}

		ML_NODISCARD inline bool operator>(shared_library const & other) const
		{
			return compare(other) > 0;
		}

		ML_NODISCARD inline bool operator<=(shared_library const & other) const
		{
			return compare(other) <= 0;
		}

		ML_NODISCARD inline bool operator>=(shared_library const & other) const
		{
			return compare(other) >= 0;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void *			m_instance;
		fs::path		m_path;
		function_map	m_funcs;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static inline auto make_shared_library(Args && ... args)
	{
		return shared_library{ ML_FWD(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHARED_LIBRARY_HPP_