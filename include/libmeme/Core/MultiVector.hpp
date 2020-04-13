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
		using storage_type		= typename meta::tuple<meta::remap<pmr::vector, type_list>>;
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
			return std::get<pmr::vector<T>>(m_data);
		}

		template <class T> ML_NODISCARD decltype(auto) get() const & noexcept
		{
			return std::get<pmr::vector<T>>(m_data);
		}

		template <class T> ML_NODISCARD decltype(auto) get() && noexcept
		{
			return std::move(std::get<pmr::vector<T>>(m_data));
		}

		template <class T> ML_NODISCARD decltype(auto) get() const && noexcept
		{
			return std::move(std::get<pmr::vector<T>>(m_data));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> ML_NODISCARD decltype(auto) get(size_t const i) & noexcept
		{
			return this->get<I>()[i];
		}

		template <size_t I> ML_NODISCARD decltype(auto) get(size_t const i) const & noexcept
		{
			return this->get<I>()[i];
		}

		template <size_t I> ML_NODISCARD decltype(auto) get(size_t const i) && noexcept
		{
			return std::move(this->get<I>()[i]);
		}

		template <size_t I> ML_NODISCARD decltype(auto) get(size_t const i) const && noexcept
		{
			return std::move(this->get<I>()[i]);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> ML_NODISCARD decltype(auto) get(size_t const i) & noexcept
		{
			return this->get<T>()[i];
		}

		template <class T> ML_NODISCARD decltype(auto) get(size_t const i) const & noexcept
		{
			return this->get<T>()[i];
		}

		template <class T> ML_NODISCARD decltype(auto) get(size_t const i) && noexcept
		{
			return std::move(this->get<T>()[i]);
		}

		template <class T> ML_NODISCARD decltype(auto) get(size_t const i) const && noexcept
		{
			return std::move(this->get<T>()[i]);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t ... Is, class Fn> void expand(Fn && fn) noexcept
		{
			std::invoke(ML_forward(fn), this->get<Is>()...);
		}

		template <size_t ... Is, class Fn> void expand(Fn && fn) const noexcept
		{
			std::invoke(ML_forward(fn), this->get<Is>()...);
		}

		template <class ... Ts, class Fn> void expand(Fn && fn) noexcept
		{
			std::invoke(ML_forward(fn), this->get<Ts>()...);
		}

		template <class ... Ts, class Fn> void expand(Fn && fn) const noexcept
		{
			std::invoke(ML_forward(fn), this->get<Ts>()...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t ... Is, class Fn> void expand(size_t const i, Fn && fn) noexcept
		{
			std::invoke(ML_forward(fn), this->get<Is>(i)...);
		}

		template <size_t ... Is, class Fn> void expand(size_t const i, Fn && fn) const noexcept
		{
			std::invoke(ML_forward(fn), this->get<Is>(i)...);
		}

		template <class ... Ts, class Fn> void expand(size_t const i, Fn && fn) noexcept
		{
			std::invoke(ML_forward(fn), this->get<Ts>(i)...);
		}

		template <class ... Ts, class Fn> void expand(size_t const i, Fn && fn) const noexcept
		{
			std::invoke(ML_forward(fn), this->get<Ts>(i)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn, class ... Args
		> void for_each(Fn && fn, Args && ... args) noexcept
		{
			meta::for_tuple(m_data, ML_forward(fn), ML_forward(args)...);
		}

		template <class Fn, class ... Args
		> void for_each(Fn && fn, Args && ... args) const noexcept
		{
			meta::for_tuple(m_data, ML_forward(fn), ML_forward(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t ... Is, class Fn, class ... Args
		> void for_(Fn && fn, Args && ... args) noexcept
		{
			this->expand<Is...>([&](auto && ... vs)
			{
				meta::for_args([&](auto & v)
				{
					std::invoke(ML_forward(fn), v, ML_forward(args)...);
				}
				, ML_forward(vs)...);
			});
		}

		template <size_t ... Is, class Fn, class ... Args
		> void for_(Fn && fn, Args && ... args) const noexcept
		{
			this->expand<Is...>([&](auto const && ... vs)
			{
				meta::for_args([&](auto const & v)
				{
					std::invoke(ML_forward(fn), v, ML_forward(args)...);
				}
				, ML_forward(vs)...);
			});
		}

		template <class ... Ts, class Fn, class ... Args
		> void for_(Fn && fn, Args && ... args) noexcept
		{
			this->expand<Ts...>([&](auto && ... vs)
			{
				meta::for_args([&](auto & v)
				{
					std::invoke(ML_forward(fn), v, ML_forward(args)...);
				}
				, ML_forward(vs)...);
			});
		}

		template <class ... Ts, class Fn, class ... Args
		> void for_(Fn && fn, Args && ... args) const noexcept
		{
			this->expand<Ts...>([&](auto const && ... vs)
			{
				meta::for_args([&](auto const & v)
				{
					std::invoke(ML_forward(fn), v, ML_forward(args)...);
				}
				, ML_forward(vs)...);
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear() noexcept
		{
			this->for_each([&](auto & v) { v.clear(); });
		}

		template <size_t ... Is> void clear() noexcept
		{
			this->for_<Is...>([&](auto & v) { v.clear(); });
		}

		template <class ... Ts> void clear() noexcept
		{
			this->for_<Ts...>([&](auto & v) { v.clear(); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void reserve(size_t const value)
		{
			this->for_each([&](auto & v) { v.reserve(value); });
		}

		template <size_t ... Is> void reserve(size_t const value)
		{
			this->for_<Is...>([&](auto & v) { v.reserve(value); });
		}

		template <class ... Ts> void reserve(size_t const value)
		{
			this->for_<Ts...>([&](auto & v) { v.reserve(value); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void resize(size_t const value)
		{
			this->for_each([&](auto & v) { v.resize(value); });
		}

		template <size_t ... Is> void resize(size_t const value)
		{
			this->for_<Is...>([&](auto & v) { v.resize(value); });
		}

		template <class ... Ts> void resize(size_t const value)
		{
			this->for_<Ts...>([&](auto & v) { v.resize(value); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void shrink_to_fit() noexcept
		{
			this->for_each([&](auto & v) { v.shrink_to_fit(); });
		}

		template <size_t ... Is> void shrink_to_fit() noexcept
		{
			this->for_<Is...>([&](auto & v) { v.shrink_to_fit(); });
		}

		template <class ... Ts> void shrink_to_fit() noexcept
		{
			this->for_<Ts...>([&](auto & v) { v.shrink_to_fit(); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void swap(self_type & value) noexcept
		{
			if (this != std::addressof(value))
			{
				m_data.swap(value.m_data);
			}
		}

		template <size_t I, class Other> void swap(Other & value) noexcept
		{
			this->get<I>().swap(value);
		}

		template <class T, class Other> void swap(Other & value) noexcept
		{
			this->get<T>().swap(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> ML_NODISCARD size_t capacity() const noexcept
		{
			return this->get<I>().capacity();
		}

		template <class T> ML_NODISCARD size_t capacity() const noexcept
		{
			return this->get<T>().capacity();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> ML_NODISCARD bool empty() const noexcept
		{
			return this->get<I>().empty();
		}

		template <class T> ML_NODISCARD bool empty() const noexcept
		{
			return this->get<T>().empty();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> ML_NODISCARD size_t max_size() const noexcept
		{
			return this->get<I>().max_size();
		}

		template <class T> ML_NODISCARD size_t max_size() const noexcept
		{
			return this->get<T>().max_size();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> ML_NODISCARD size_t size() const noexcept
		{
			return this->get<I>().size();
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

#endif // !_ML_MULTI_VECTOR_HPP_