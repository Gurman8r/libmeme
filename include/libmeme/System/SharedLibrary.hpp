#ifndef _ML_SHARED_LIBRARY_HPP_
#define _ML_SHARED_LIBRARY_HPP_

#include <libmeme/System/Export.hpp>
#include <libmeme/System/Memory.hpp>

namespace ml
{
	ML_decl_handle(library_handle);

	struct ML_SYSTEM_API shared_library final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using function_table	= typename ds::map<hash_t, void *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto default_extension // native library extension
		{
#if defined(ML_os_windows)
			L".dll"
#elif defined(ML_os_unix)
			L".so"
#else
			L""
#endif
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shared_library(allocator_type alloc = {}) noexcept
			: m_handle{}, m_path{}, m_functions{ alloc }
		{
		}

		explicit shared_library(fs::path const & path, allocator_type alloc = {}) noexcept
			: shared_library{ alloc }
		{
			this->open(path);
		}

		explicit shared_library(shared_library && value, allocator_type alloc = {}) noexcept
			: shared_library{ alloc }
		{
			this->swap(std::move(value));
		}

		~shared_library() noexcept
		{
			this->close();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shared_library & operator=(shared_library && value) noexcept
		{
			this->swap(std::move(value));
			return (*this);
		}

		void swap(shared_library & value) noexcept
		{
			if (this != std::addressof(value))
			{
				std::swap(m_handle, value.m_handle);
				m_path.swap(value.m_path);
				m_functions.swap(value.m_functions);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool open(fs::path const & path);

		bool close();

		void * proc(cstring value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ret, class ... Args
		> decltype(auto) call(cstring name, Args && ... args) noexcept
		{
			if (auto const fn{ reinterpret_cast<Ret(*)(Args...)>(proc(name)) })
			{
				if constexpr (!std::is_same_v<Ret, void>)
				{
					return std::make_optional<Ret>(std::invoke(fn, ML_forward(args)...));
				}
				else
				{
					std::invoke(fn, ML_forward(args)...);
				}
			}
			else if constexpr (!std::is_same_v<Ret, void>)
			{
				return (std::optional<Ret>)std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD operator bool() const noexcept { return m_handle; }

		ML_NODISCARD bool good() const noexcept { return m_handle; }

		ML_NODISCARD auto get_handle() const noexcept -> library_handle const & { return m_handle; }

		ML_NODISCARD auto get_path() const noexcept -> fs::path const & { return m_path; }

		ML_NODISCARD auto get_functions() const noexcept -> function_table const & { return m_functions; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U = shared_library
		> ML_NODISCARD auto compare(U const & value) const noexcept
		{
			if constexpr (std::is_same_v<U, shared_library>)
			{
				return (this != std::addressof(value)) ? compare(value.m_path) : 0;
			}
			else if constexpr (std::is_same_v<U, fs::path>)
			{
				return compare(util::hash(value.filename().string()));
			}
			else
			{
				static_assert(std::is_same_v<U, hash_t>);
				return util::compare(util::hash(m_path.filename().string()), value);
			}
		}

		template <class U = shared_library
		> ML_NODISCARD bool operator==(U const & value) const noexcept
		{
			return compare(value) == 0;
		}

		template <class U = shared_library
		> ML_NODISCARD bool operator!=(U const & value) const noexcept
		{
			return compare(value) != 0;
		}

		template <class U = shared_library
		> ML_NODISCARD bool operator<(U const & value) const noexcept
		{
			return compare(value) < 0;
		}

		template <class U = shared_library
		> ML_NODISCARD bool operator>(U const & value) const noexcept
		{
			return compare(value) > 0;
		}

		template <class U = shared_library
		> ML_NODISCARD bool operator<=(U const & value) const noexcept
		{
			return compare(value) <= 0;
		}

		template <class U = shared_library
		> ML_NODISCARD bool operator>=(U const & value) const noexcept
		{
			return compare(value) >= 0;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		library_handle	m_handle;
		fs::path		m_path;
		function_table	m_functions;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SHARED_LIBRARY_HPP_