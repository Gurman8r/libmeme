#ifndef _ML_SHARED_LIBRARY_HPP_
#define _ML_SHARED_LIBRARY_HPP_

#include <libmeme/Platform/Export.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	ML_decl_handle(library_handle);

	struct ML_PLATFORM_API shared_library final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using self_type			= typename shared_library;
		using symbol_table		= typename ds::map<hash_t, void *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto default_extension // native library extension
		{
#if defined(ML_os_windows)
			L".dll"
#else
			L".so"
#endif
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shared_library(allocator_type alloc = {}) noexcept
			: m_handle{}, m_path{}, m_symbols{ alloc }
		{
		}

		explicit shared_library(fs::path const & path, allocator_type alloc = {}) noexcept
			: self_type{ alloc }
		{
			this->open(path);
		}

		explicit shared_library(self_type && value, allocator_type alloc = {}) noexcept
			: self_type{ alloc }
		{
			this->swap(std::move(value));
		}

		~shared_library() noexcept
		{
			this->close();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		self_type & operator=(self_type && value) noexcept
		{
			this->swap(std::move(value));
			return (*this);
		}

		void swap(self_type & value) noexcept
		{
			if (this != std::addressof(value))
			{
				std::swap(m_handle, value.m_handle);
				m_path.swap(value.m_path);
				m_symbols.swap(value.m_symbols);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool open(fs::path const & path);

		bool close();

		void * read(cstring value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ret, class ... Args
		> auto call(cstring method_name, Args && ... args) noexcept
		{
			if (auto const fn{ reinterpret_cast<Ret(*)(Args...)>(read(method_name)) })
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

		ML_NODISCARD bool good() const noexcept { return m_handle; }

		ML_NODISCARD operator bool() const noexcept { return this->good(); }

		ML_NODISCARD auto handle() const noexcept -> library_handle const & { return m_handle; }

		ML_NODISCARD auto path() const noexcept -> fs::path const & { return m_path; }

		ML_NODISCARD auto symbols() const noexcept -> symbol_table const & { return m_symbols; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U = self_type
		> ML_NODISCARD auto compare(U const & value) const noexcept
		{
			if constexpr (std::is_same_v<U, self_type>)
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

		template <class U = self_type
		> ML_NODISCARD bool operator==(U const & value) const noexcept
		{
			return compare(value) == 0;
		}

		template <class U = self_type
		> ML_NODISCARD bool operator!=(U const & value) const noexcept
		{
			return compare(value) != 0;
		}

		template <class U = self_type
		> ML_NODISCARD bool operator<(U const & value) const noexcept
		{
			return compare(value) < 0;
		}

		template <class U = self_type
		> ML_NODISCARD bool operator>(U const & value) const noexcept
		{
			return compare(value) > 0;
		}

		template <class U = self_type
		> ML_NODISCARD bool operator<=(U const & value) const noexcept
		{
			return compare(value) <= 0;
		}

		template <class U = self_type
		> ML_NODISCARD bool operator>=(U const & value) const noexcept
		{
			return compare(value) >= 0;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		library_handle	m_handle;
		fs::path		m_path;
		symbol_table	m_symbols;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD inline auto make_shared_library(Args && ... args)
	{
		return shared_library{ ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHARED_LIBRARY_HPP_