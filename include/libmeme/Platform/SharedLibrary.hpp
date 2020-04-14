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
		
		shared_library(shared_library && value, allocator_type const & alloc = {}) noexcept;
		
		~shared_library() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shared_library & operator=(shared_library && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		void swap(shared_library & value) noexcept
		{
			if (this != std::addressof(value))
			{
				std::swap(m_inst, value.m_inst);
				m_path.swap(value.m_path);
				m_funcs.swap(value.m_funcs);
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
			return reinterpret_cast<Ret(*)(Args...)>(this->load_function(name));
		}

		template <class Ret, class ... Args
		> auto call_function(pmr::string const & name, Args && ... args)
		{
			if (auto const fn{ this->load_function<Ret, Args...>(name) })
			{
				if constexpr (!std::is_same_v<Ret, void>)
				{
					return std::make_optional<Ret>(std::invoke(fn, ML_forward(args)...));
				}
			}
			else if constexpr (!std::is_same_v<Ret, void>)
			{
				return (std::optional<Ret>)std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD operator bool() const noexcept { return good(); }

		ML_NODISCARD bool good() const noexcept { return m_inst; }

		ML_NODISCARD auto functions() const noexcept -> function_map const & { return m_funcs; }

		ML_NODISCARD auto instance() const noexcept -> void const * { return m_inst; }

		ML_NODISCARD auto path() const noexcept -> fs::path const & { return m_path; }

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
				return compare(util::hash(value.string()));
			}
			else
			{
				static_assert(std::is_same_v<U, hash_t>);
				return util::compare(util::hash(m_path.string()), value);
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