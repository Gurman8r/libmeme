#ifndef _ML_MULTI_VECTOR_HPP_
#define _ML_MULTI_VECTOR_HPP_

#include <libmeme/Core/Meta.hpp>

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// tuple<vector<T>...>
	template <class ... _Ts
	> struct multi_vector final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type			= typename multi_vector<_Ts...>;
		using type_list			= typename meta::list<_Ts...>;
		using vector_types		= typename meta::remap<pmr::vector, type_list>;
		using storage_type		= typename meta::tuple<vector_types>;
		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		multi_vector(allocator_type const & alloc = {}) noexcept
			: m_data{ std::allocator_arg, alloc }
		{
		}

		multi_vector(storage_type const & value, allocator_type const & alloc = {})
			: m_data{ std::allocator_arg, alloc, value }
		{
		}

		multi_vector(storage_type && value, allocator_type const & alloc = {}) noexcept
			: m_data{ std::allocator_arg, alloc, std::move(value) }
		{
		}

		multi_vector(self_type const & value, allocator_type const & alloc = {})
			: m_data{ std::allocator_arg, alloc, value.m_data }
		{
		}

		multi_vector(self_type && value, allocator_type const & alloc = {}) noexcept
			: m_data{ std::allocator_arg, alloc, std::move(value.m_data) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		self_type & operator=(storage_type const & value)
		{
			self_type temp{ value };
			this->swap(temp);
			return (*this);
		}

		self_type & operator=(storage_type && value) noexcept
		{
			this->(std::move(value));
			return (*this);
		}

		self_type & operator=(self_type const & value)
		{
			self_type temp{ value };
			this->swap(temp);
			return (*this);
		}

		self_type & operator=(self_type && value) noexcept
		{
			this->swap(std::move(value));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> ML_NODISCARD decltype(auto) get() & noexcept
		{
			return std::get<I>(m_data);
		}

		template <size_t I> ML_NODISCARD decltype(auto) get() const & noexcept
		{
			return std::get<I>(m_data);
		}

		template <size_t I> ML_NODISCARD decltype(auto) get() && noexcept
		{
			return std::move(std::get<I>(m_data));
		}

		template <size_t I> ML_NODISCARD decltype(auto) get() const && noexcept
		{
			return std::move(std::get<I>(m_data));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> ML_NODISCARD decltype(auto) get() & noexcept
		{
			return std::get<T>(m_data);
		}

		template <class T> ML_NODISCARD decltype(auto) get() const & noexcept
		{
			return std::get<T>(m_data);
		}

		template <class T> ML_NODISCARD decltype(auto) get() && noexcept
		{
			return std::move(std::get<T>(m_data));
		}

		template <class T> ML_NODISCARD decltype(auto) get() const && noexcept
		{
			return std::move(std::get<T>(m_data));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> ML_NODISCARD decltype(auto) get(size_t const i) & noexcept
		{
			return std::get<I>(m_data)[i];
		}

		template <size_t I> ML_NODISCARD decltype(auto) get(size_t const i) const & noexcept
		{
			return std::get<I>(m_data)[i];
		}

		template <size_t I> ML_NODISCARD decltype(auto) get(size_t const i) && noexcept
		{
			return std::move(std::get<I>(m_data)[i]);
		}

		template <size_t I> ML_NODISCARD decltype(auto) get(size_t const i) const && noexcept
		{
			return std::move(std::get<I>(m_data)[i]);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> ML_NODISCARD decltype(auto) get(size_t const i) & noexcept
		{
			return std::get<pmr::vector<T>>(m_data)[i];
		}

		template <class T> ML_NODISCARD decltype(auto) get(size_t const i) const & noexcept
		{
			return std::get<pmr::vector<T>>(m_data)[i];
		}

		template <class T> ML_NODISCARD decltype(auto) get(size_t const i) && noexcept
		{
			return std::move(std::get<pmr::vector<T>>(m_data)[i]);
		}

		template <class T> ML_NODISCARD decltype(auto) get(size_t const i) const && noexcept
		{
			return std::move(std::get<pmr::vector<T>>(m_data)[i]);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn> void for_each(Fn && fn) noexcept
		{
			meta::for_tuple(ML_forward(fn), m_data);
		}

		template <class Fn> void for_each(Fn && fn) const noexcept
		{
			meta::for_tuple(ML_forward(fn), m_data);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear() noexcept
		{
			this->for_each([&](auto & v) { v.clear(); });
		}

		void pop_back()
		{
			this->for_each([&](auto & v) { v.pop_back(); });
		}

		void reserve(size_t const cap)
		{
			this->for_each([&](auto & v) { v.reserve(cap); });
		}

		void resize(size_t const cap)
		{
			this->for_each([&](auto & v) { v.resize(cap); });
		}

		void shrink_to_fit() noexcept
		{
			this->for_each([&](auto & v) { v.shrink_to_fit(); });
		}

		void swap(self_type & value) noexcept
		{
			if (this != std::addressof(value))
			{
				m_data.swap(value.m_data);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> void clear() noexcept
		{
			this->get<I>().clear();
		}

		template <size_t I> void pop_back()
		{
			this->get<I>().pop_back();
		}

		template <size_t I> void reserve(size_t const cap)
		{
			this->get<I>().reserve(cap);
		}

		template <size_t I> void resize(size_t const cap)
		{
			this->get<I>().resize(cap);
		}

		template <size_t I> void shrink_to_fit() noexcept
		{
			this->get<I>().shrink_to_fit();
		}

		template <size_t I, class Other> void swap(Other & other) noexcept
		{
			this->get<I>().swap(other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> void clear() noexcept
		{
			this->get<T>().clear();
		}

		template <class T> void pop_back()
		{
			this->get<T>().pop_back();
		}

		template <class T> void reserve(size_t const cap)
		{
			this->get<T>().reserve(cap);
		}

		template <class T> void resize(size_t const cap)
		{
			this->get<T>().resize(cap);
		}

		template <class T> void shrink_to_fit() noexcept
		{
			this->get<T>().shrink_to_fit();
		}

		template <class T, class Other> void swap(Other & other) noexcept
		{
			this->get<T>().swap(other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> ML_NODISCARD size_t capacity() const noexcept
		{
			return this->get<I>().capacity();
		}

		template <size_t I> ML_NODISCARD bool empty() const noexcept
		{
			return this->get<I>().empty();
		}

		template <size_t I> ML_NODISCARD size_t max_size() const noexcept
		{
			return this->get<I>().max_size();
		}

		template <size_t I> ML_NODISCARD size_t size() const noexcept
		{
			return this->get<I>().size();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> ML_NODISCARD size_t capacity() const noexcept
		{
			return this->get<T>().capacity();
		}

		template <class T> ML_NODISCARD bool empty() const noexcept
		{
			return this->get<T>().empty();
		}

		template <class T> ML_NODISCARD size_t max_size() const noexcept
		{
			return this->get<T>().max_size();
		}

		template <class T> ML_NODISCARD size_t size() const noexcept
		{
			return this->get<T>().size();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// TUPLE INTERFACE
namespace std
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t I, class ... Ts
	> ML_NODISCARD constexpr decltype(auto) get(_ML ds::multi_vector<Ts...> & value) noexcept
	{
		return value.get<I>();
	}

	template <size_t I, class ... Ts
	> ML_NODISCARD constexpr decltype(auto) get(_ML ds::multi_vector<Ts...> const & value) noexcept
	{
		return value.get<I>();
	}

	template <size_t I, class ... Ts
	> ML_NODISCARD constexpr decltype(auto) get(_ML ds::multi_vector<Ts...> && value) noexcept
	{
		return std::move(value.get<I>());
	}

	template <size_t I, class ... Ts
	> ML_NODISCARD constexpr decltype(auto) get(_ML ds::multi_vector<Ts...> const && value) noexcept
	{
		return std::move(value.get<I>());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class ... Ts
	> ML_NODISCARD constexpr decltype(auto) get(_ML ds::multi_vector<Ts...> & value) noexcept
	{
		return value.get<T>();
	}

	template <class T, class ... Ts
	> ML_NODISCARD constexpr decltype(auto) get(_ML ds::multi_vector<Ts...> const & value) noexcept
	{
		return value.get<T>();
	}

	template <class T, class ... Ts
	> ML_NODISCARD constexpr decltype(auto) get(_ML ds::multi_vector<Ts...> && value) noexcept
	{
		return std::move(value.get<T>());
	}

	template <class T, class ... Ts
	> ML_NODISCARD constexpr decltype(auto) get(_ML ds::multi_vector<Ts...> const && value) noexcept
	{
		return std::move(value.get<T>());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MULTI_VECTOR_HPP_