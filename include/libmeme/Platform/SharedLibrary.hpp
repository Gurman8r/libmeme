#ifndef _ML_SHARED_LIBRARY_HPP_
#define _ML_SHARED_LIBRARY_HPP_

#include <libmeme/Platform/Export.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	struct ML_PLATFORM_API shared_library final : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		using function_map = typename ds::flat_map<hash_t, void *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shared_library() noexcept : shared_library{ allocator_type{} } {}

		explicit shared_library(allocator_type const & alloc) noexcept;

		shared_library(fs::path const & path, allocator_type const & alloc = {}) noexcept;
		
		shared_library(shared_library && other, allocator_type const & alloc = {}) noexcept;
		
		~shared_library() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shared_library & operator=(shared_library && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		void swap(shared_library & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_inst, other.m_inst);
				m_path.swap(other.m_path);
				m_funcs.swap(other.m_funcs);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool open(fs::path const & path);

		bool close();

		void * load_function(pmr::string const & name);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ret, class ... Args
		> ML_NODISCARD auto load_function(pmr::string const & name)
		{
			return reinterpret_cast<Ret(*)(Args...)>(load_function(name));
		}

		template <class Ret, class ... Args
		> std::optional<Ret> call_function(pmr::string const & name, Args && ... args)
		{
			if (auto const fn{ load_function<Ret, Args...>(name) })
			{
				return std::make_optional(std::invoke(fn, ML_forward(args)...));
			}
			else
			{
				return std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD operator bool() const noexcept { return good(); }

		ML_NODISCARD bool good() const noexcept { return m_inst; }

		ML_NODISCARD auto functions() const noexcept -> function_map const & { return m_funcs; }

		ML_NODISCARD auto instance() const noexcept -> void const * { return m_inst; }

		ML_NODISCARD auto path() const noexcept -> fs::path const & { return m_path; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other = shared_library
		> ML_NODISCARD auto compare(Other const & other) const noexcept
		{
			if constexpr (std::is_same_v<Other, shared_library>)
			{
				if (this != std::addressof(other))
				{
					return compare(other.m_path);
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return m_path.compare(other);
			}
		}

		template <class Other = shared_library
		> ML_NODISCARD bool operator==(Other const & other) const noexcept
		{
			return compare(other) == 0;
		}

		template <class Other = shared_library
		> ML_NODISCARD bool operator!=(Other const & other) const noexcept
		{
			return compare(other) != 0;
		}

		template <class Other = shared_library
		> ML_NODISCARD bool operator<(Other const & other) const noexcept
		{
			return compare(other) < 0;
		}

		template <class Other = shared_library
		> ML_NODISCARD bool operator>(Other const & other) const noexcept
		{
			return compare(other) > 0;
		}

		template <class Other = shared_library
		> ML_NODISCARD bool operator<=(Other const & other) const noexcept
		{
			return compare(other) <= 0;
		}

		template <class Other = shared_library
		> ML_NODISCARD bool operator>=(Other const & other) const noexcept
		{
			return compare(other) >= 0;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void *			m_inst;
		fs::path		m_path;
		function_map	m_funcs;

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