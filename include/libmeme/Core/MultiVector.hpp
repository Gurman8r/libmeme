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

		ML_NODISCARD decltype(auto) data() & noexcept { return m_data; }

		ML_NODISCARD decltype(auto) data() const & noexcept { return m_data; }

		ML_NODISCARD decltype(auto) data() && noexcept { return std::move(m_data); }

		ML_NODISCARD decltype(auto) data() const && noexcept { return std::move(m_data); }

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

		template <size_t ... Is, class Fn
		> void expand(Fn && fn) noexcept
		{
			std::invoke(ML_forward(fn), this->get<Is>()...);
		}

		template <size_t ... Is, class Fn
		> void expand(Fn && fn) const noexcept
		{
			std::invoke(ML_forward(fn), this->get<Is>()...);
		}

		template <class ... Ts, class Fn
		> void expand(Fn && fn) noexcept
		{
			std::invoke(ML_forward(fn), this->get<Ts>()...);
		}

		template <class ... Ts, class Fn
		> void expand(Fn && fn) const noexcept
		{
			std::invoke(ML_forward(fn), this->get<Ts>()...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t ... Is, class Fn
		> void expand(size_t const i, Fn && fn) noexcept
		{
			std::invoke(ML_forward(fn), this->get<Is>(i)...);
		}

		template <size_t ... Is, class Fn
		> void expand(size_t const i, Fn && fn) const noexcept
		{
			std::invoke(ML_forward(fn), this->get<Is>(i)...);
		}

		template <class ... Ts, class Fn
		> void expand(size_t const i, Fn && fn) noexcept
		{
			std::invoke(ML_forward(fn), this->get<Ts>(i)...);
		}

		template <class ... Ts, class Fn
		> void expand(size_t const i, Fn && fn) const noexcept
		{
			std::invoke(ML_forward(fn), this->get<Ts>(i)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn, class ... Args
		> void for_tuple(Fn && fn, Args && ... args) noexcept
		{
			meta::for_tuple(m_data, ML_forward(fn), ML_forward(args)...);
		}

		template <class Fn, class ... Args
		> void for_tuple(Fn && fn, Args && ... args) const noexcept
		{
			meta::for_tuple(m_data, ML_forward(fn), ML_forward(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t ... Is, class Fn, class ... Args
		> void for_each(Fn && fn, Args && ... args) noexcept
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
		> void for_each(Fn && fn, Args && ... args) const noexcept
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
		> void for_each(Fn && fn, Args && ... args) noexcept
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
		> void for_each(Fn && fn, Args && ... args) const noexcept
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
			this->for_tuple([&](auto & v) { v.clear(); });
		}

		template <size_t ... Is
		> void clear() noexcept
		{
			this->for_each<Is...>([&](auto & v) { v.clear(); });
		}

		template <class ... Ts
		> void clear() noexcept
		{
			this->for_each<Ts...>([&](auto & v) { v.clear(); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void pop_back() noexcept
		{
			this->for_tuple([&](auto & v) { v.pop_back(); });
		}

		template <size_t ... Is
		> void pop_back() noexcept
		{
			this->for_each<Is...>([&](auto & v) { v.pop_back(); });
		}

		template <class ... Ts
		> void pop_back() noexcept
		{
			this->for_each<Ts...>([&](auto & v) { v.pop_back(); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void reserve(size_t const value)
		{
			this->for_tuple([&](auto & v) { v.reserve(value); });
		}

		template <size_t ... Is
		> void reserve(size_t const value)
		{
			this->for_each<Is...>([&](auto & v) { v.reserve(value); });
		}

		template <class ... Ts
		> void reserve(size_t const value)
		{
			this->for_each<Ts...>([&](auto & v) { v.reserve(value); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void resize(size_t const value)
		{
			this->for_tuple([&](auto & v) { v.resize(value); });
		}

		template <size_t ... Is
		> void resize(size_t const value)
		{
			this->for_each<Is...>([&](auto & v) { v.resize(value); });
		}

		template <class ... Ts
		> void resize(size_t const value)
		{
			this->for_each<Ts...>([&](auto & v) { v.resize(value); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void shrink_to_fit() noexcept
		{
			this->for_tuple([&](auto & v) { v.shrink_to_fit(); });
		}

		template <size_t ... Is
		> void shrink_to_fit() noexcept
		{
			this->for_each<Is...>([&](auto & v) { v.shrink_to_fit(); });
		}

		template <class ... Ts
		> void shrink_to_fit() noexcept
		{
			this->for_each<Ts...>([&](auto & v) { v.shrink_to_fit(); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void swap(self_type & value) noexcept
		{
			if (this != std::addressof(value))
			{
				m_data.swap(value.m_data);
			}
		}

		template <size_t I, class Other = meta::nth<I, vector_types>
		> void swap(Other & value) noexcept
		{
			this->get<I>().swap(value);
		}

		template <class T, class Other = T
		> void swap(Other & value) noexcept
		{
			this->get<T>().swap(value);
		}

		void swap(size_t const lhs, size_t const rhs) noexcept
		{
			this->for_tuple([&](auto & v) { std::swap(v[lhs], v[rhs]); });
		}

		template <size_t ... Is
		> void swap(size_t const lhs, size_t const rhs) noexcept
		{
			this->for_each<Is...>([&](auto & v) { std::swap(v[lhs], v[rhs]); });
		}

		template <class ... Ts
		> void swap(size_t const lhs, size_t const rhs) noexcept
		{
			this->for_each<Ts...>([&](auto & v) { std::swap(v[lhs], v[rhs]); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void erase(size_t const loc)
		{
			this->for_tuple([&](auto & v)
			{
				v.erase(v.begin() + loc);
			});
		}

		void erase(size_t const first, size_t const last)
		{
			this->for_tuple([&](auto & v)
			{
				v.erase(v.begin() + first, v.begin() + last);
			});
		}

		template <size_t ... Is
		> void erase(size_t const loc)
		{
			this->for_each<Is...>([&](auto & v)
			{
				v.erase(v.begin() + loc);
			});
		}

		template <size_t ... Is
		> void erase(size_t const first, size_t const last)
		{
			this->for_each<Is...>([&](auto & v)
			{
				v.erase(v.begin() + first, v.begin() + last);
			});
		}

		template <class ... Ts
		> void erase(size_t const loc)
		{
			this->for_each<Ts...>([&](auto & v)
			{
				v.erase(v.begin() + loc);
			});
		}

		template <class ... Ts
		> void erase(size_t const first, size_t const last)
		{
			this->for_each<Ts...>([&](auto & v)
			{
				v.erase(v.begin() + first, v.begin() + last);
			});
		}

		template <size_t I, class It
		> auto erase(It loc)
		{
			return this->get<I>().erase(loc);
		}

		template <size_t I, class It
		> auto erase(It first, It last)
		{
			return this->get<I>().erase(first, last);
		}

		template <class T, class It
		> auto erase(It loc)
		{
			return this->get<T>().erase(loc);
		}

		template <class T, class It
		> auto erase(It first, It last)
		{
			return this->get<T>().erase(first, last);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class It, class ... Args
		> decltype(auto) emplace(It loc, Args && ... args) noexcept
		{
			if constexpr (std::is_integral_v<It>)
			{
				return this->get<I>().emplace(this->begin<I>() + loc, ML_forward(args)...);
			}
			else
			{
				return this->get<I>().emplace(loc, ML_forward(args)...);
			}
		}

		template <class T, class It, class ... Args
		> decltype(auto) emplace(It loc, Args && ... args) noexcept
		{
			if constexpr (std::is_integral_v<It>)
			{
				return this->get<T>().emplace(this->begin<T>() + loc, ML_forward(args)...);
			}
			else
			{
				return this->get<T>().emplace(loc, ML_forward(args)...);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class ... Args
		> decltype(auto) emplace_back(Args && ... args) noexcept
		{
			return this->get<I>().emplace_back(ML_forward(args)...);
		}

		template <class T, class ... Args
		> decltype(auto) emplace_back(Args && ... args) noexcept
		{
			return this->get<T>().emplace_back(ML_forward(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class It, class Other = meta::nth<I, type_list>
		> auto insert(It loc, Other && value) noexcept
		{
			if constexpr (std::is_integral_v<It>)
			{
				return this->get<I>().insert(this->begin<I>() + loc, ML_forward(value));
			}
			else
			{
				return this->get<I>().insert(loc, ML_forward(value));
			}
		}

		template <class T, class It, class Other = T
		> auto insert(It loc, Other && value) noexcept
		{
			if constexpr (std::is_integral_v<It>)
			{
				return this->get<T>().insert(this->begin<T>() + loc, ML_forward(value));
			}
			else
			{
				return this->get<T>().insert(loc, ML_forward(value));
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class Other = meta::nth<I, type_list>
		> void push_back(Other && value) noexcept
		{
			this->get<I>().push_back(ML_forward(value));
		}

		template <class T, class Other = T
		> void push_back(Other && value) noexcept
		{
			this->get<T>().push_back(ML_forward(value));
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

		template <size_t I> ML_NODISCARD decltype(auto) back() & noexcept
		{
			return this->get<I>().back();
		}

		template <size_t I> ML_NODISCARD decltype(auto) back() const & noexcept
		{
			return this->get<I>().back();
		}

		template <size_t I> ML_NODISCARD decltype(auto) back() && noexcept
		{
			return std::move(this->get<I>().back());
		}

		template <size_t I> ML_NODISCARD decltype(auto) back() const && noexcept
		{
			return std::move(this->get<I>().back());
		}

		template <class T> ML_NODISCARD decltype(auto) back() & noexcept
		{
			return this->get<T>().back();
		}

		template <class T> ML_NODISCARD decltype(auto) back() const & noexcept
		{
			return this->get<T>().back();
		}

		template <class T> ML_NODISCARD decltype(auto) back() && noexcept
		{
			return std::move(this->get<T>().back());
		}

		template <class T> ML_NODISCARD decltype(auto) back() const && noexcept
		{
			return std::move(this->get<T>().back());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> ML_NODISCARD decltype(auto) front() & noexcept
		{
			return this->get<I>().front();
		}

		template <size_t I> ML_NODISCARD decltype(auto) front() const & noexcept
		{
			return this->get<I>().front();
		}

		template <size_t I> ML_NODISCARD decltype(auto) front() && noexcept
		{
			return std::move(this->get<I>().front());
		}

		template <size_t I> ML_NODISCARD decltype(auto) front() const && noexcept
		{
			return std::move(this->get<I>().front());
		}

		template <class T> ML_NODISCARD decltype(auto) front() & noexcept
		{
			return this->get<T>().front();
		}

		template <class T> ML_NODISCARD decltype(auto) front() const & noexcept
		{
			return this->get<T>().front();
		}

		template <class T> ML_NODISCARD decltype(auto) front() && noexcept
		{
			return std::move(this->get<T>().front());
		}

		template <class T> ML_NODISCARD decltype(auto) front() const && noexcept
		{
			return std::move(this->get<T>().front());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> ML_NODISCARD auto begin() noexcept { return this->get<I>().begin(); }

		template <size_t I> ML_NODISCARD auto begin() const noexcept { return this->get<I>().begin(); }

		template <size_t I> ML_NODISCARD auto cbegin() noexcept { return this->get<I>().cbegin(); }

		template <class T> ML_NODISCARD auto begin() noexcept { return this->get<T>().begin(); }

		template <class T> ML_NODISCARD auto begin() const noexcept { return this->get<T>().begin(); }

		template <class T> ML_NODISCARD auto cbegin() noexcept { return this->get<T>().cbegin(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> ML_NODISCARD auto end() noexcept { return this->get<I>().end(); }

		template <size_t I> ML_NODISCARD auto end() const noexcept { return this->get<I>().end(); }

		template <size_t I> ML_NODISCARD auto cend() noexcept { return this->get<I>().cend(); }

		template <class T> ML_NODISCARD auto end() noexcept { return this->get<T>().end(); }

		template <class T> ML_NODISCARD auto end() const noexcept { return this->get<T>().end(); }

		template <class T> ML_NODISCARD auto cend() noexcept { return this->get<T>().cend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> ML_NODISCARD auto rbegin() noexcept { return this->get<I>().rbegin(); }

		template <size_t I> ML_NODISCARD auto rbegin() const noexcept { return this->get<I>().rbegin(); }

		template <size_t I> ML_NODISCARD auto crbegin() noexcept { return this->get<I>().crbegin(); }

		template <class T> ML_NODISCARD auto rbegin() noexcept { return this->get<T>().rbegin(); }

		template <class T> ML_NODISCARD auto rbegin() const noexcept { return this->get<T>().rbegin(); }

		template <class T> ML_NODISCARD auto crbegin() noexcept { return this->get<T>().crbegin(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> ML_NODISCARD auto rend() noexcept { return this->get<I>().rend(); }

		template <size_t I> ML_NODISCARD auto rend() const noexcept { return this->get<I>().rend(); }

		template <size_t I> ML_NODISCARD auto crend() noexcept { return this->get<I>().crend(); }

		template <class T> ML_NODISCARD auto rend() noexcept { return this->get<T>().rend(); }

		template <class T> ML_NODISCARD auto rend() const noexcept { return this->get<T>().rend(); }

		template <class T> ML_NODISCARD auto crend() noexcept { return this->get<T>().crend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MULTI_VECTOR_HPP_