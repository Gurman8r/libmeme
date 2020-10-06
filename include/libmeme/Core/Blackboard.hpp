#ifndef _ML_BLACKBOARD_HPP_
#define _ML_BLACKBOARD_HPP_

#include <libmeme/Core/Memory.hpp>

namespace ml
{
	struct blackboard final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using variable			= typename std::any;
		using element			= typename shared<variable>;
		using category			= typename ds::map<hash_t, element>;
		using catagories		= typename ds::map<hash_t, category>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ID
		> ML_NODISCARD static constexpr hash_t getid(ID && id) noexcept
		{
			if constexpr (std::is_integral_v<std::decay_t<decltype(id)>>)
			{
				return static_cast<hash_t>(id);
			}
			else
			{
				return util::hash(ML_forward(id));
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> struct ref final : trackable
		{
			using wrapper = typename std::reference_wrapper<T>;

			template <class ID> ref(blackboard * parent, ID && id, allocator_type alloc = {})
				: m_parent	{ ML_check(parent) }
				, m_id		{ getid(ML_forward(id)) }
				, m_ref		{ m_parent->get<T>(m_id, alloc) }
			{
			}

			ref(ref const & other)
				: m_parent	{ other.m_parent }
				, m_id		{ other.m_id }
				, m_ref		{ other.m_ref }
			{
			}

			~ref() noexcept
			{
				ML_check(m_parent)->erase<T>(m_id);
			}

			ML_NODISCARD auto parent() const noexcept -> blackboard * { return m_parent; }
			ML_NODISCARD auto id() const noexcept -> hash_t { return m_id; }
			ML_NODISCARD operator bool() const noexcept { return ML_check(m_parent)->contains<T>(m_id); }

			ML_NODISCARD auto get() & noexcept -> T & { return m_ref.get(); }
			ML_NODISCARD auto get() && noexcept -> T && { return std::move(m_ref.get()); }
			ML_NODISCARD auto get() const & noexcept -> T const & { return m_ref.get(); }
			ML_NODISCARD auto get() const && noexcept -> T const && { return std::move(m_ref.get()); }

			ML_NODISCARD operator T & () & noexcept -> T & { return get(); }
			ML_NODISCARD operator T && () && noexcept -> T && { return std::move(get()); }
			ML_NODISCARD operator T const & () const & noexcept -> T const & { return get(); }
			ML_NODISCARD operator T const && () const && noexcept -> T const && { return std::move(get()); }

			ML_NODISCARD auto operator*() & noexcept -> T & { return get(); }
			ML_NODISCARD auto operator*() && noexcept -> T & { return std::move(get()); }
			ML_NODISCARD auto operator*() const & noexcept -> T const & { return get(); }
			ML_NODISCARD auto operator*() const && noexcept -> T const & { return std::move(get()); }

			ML_NODISCARD auto operator->() noexcept -> T * { return std::addressof(get()); }
			ML_NODISCARD auto operator->() const noexcept -> T const * { return std::addressof(get()); }
			
			template <class I
			> ML_NODISCARD decltype(auto) operator[](I && i) noexcept { return get()[ML_forward(i)]; }

			template <class I
			> ML_NODISCARD decltype(auto) operator[](I && i) const noexcept { return get()[ML_forward(i)]; }

			template <class ... Args
			> decltype(auto) operator()(Args && ... args) noexcept { return std::invoke(get(), ML_forward(args)...); }

		private:
			blackboard * const	m_parent	; // 
			hash_t const		m_id		; // 
			wrapper				m_ref		; // 
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		blackboard(allocator_type alloc = {}) noexcept
			: m_vars{ alloc }
		{
		}

		explicit blackboard(catagories const & data, allocator_type alloc = {})
			: m_vars{ data, alloc }
		{
		}

		explicit blackboard(catagories && data, allocator_type alloc = {}) noexcept
			: m_vars{ std::move(data), alloc }
		{
		}

		blackboard(blackboard const & other, allocator_type alloc = {})
			: blackboard{ other.m_vars, alloc }
		{
		}

		blackboard(blackboard && other, allocator_type alloc = {}) noexcept
			: blackboard{ std::move(other.m_vars), alloc }
		{
		}

		blackboard & operator=(blackboard const & other)
		{
			blackboard temp{ other };
			this->swap(temp);
			return (*this);
		}

		blackboard & operator=(blackboard && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto data() & noexcept -> catagories & { return m_vars; }

		ML_NODISCARD auto data() const & noexcept -> catagories const & { return m_vars; }

		ML_NODISCARD auto capacity() const noexcept -> size_t { return m_vars.capacity(); }

		ML_NODISCARD bool empty() const noexcept { return m_vars.empty(); }

		ML_NODISCARD auto get_allocator() const noexcept -> allocator_type { return m_vars.get_allocator(); }

		ML_NODISCARD auto max_size() const noexcept -> size_t { return m_vars.max_size(); }

		ML_NODISCARD auto size() const noexcept -> size_t { return m_vars.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear() noexcept { m_vars.clear(); }

		void clear(hash_t type) noexcept { m_vars[type].clear(); }

		template <class T>
		void clear() { this->clear(hashof_v<T>); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void resize(size_t const count) { m_vars.resize(count); }
		
		void resize(hash_t type, size_t const count) { m_vars[type].resize(count); }

		template <class T>
		void resize(size_t const count) { this->resize(hashof_v<T>, count); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void reserve(size_t const count) { m_vars.reserve(count); }

		void reserve(hash_t type, size_t const count) { m_vars[type].reserve(count); }
		
		template <class T>
		void reserve(size_t const count) { this->reserve(hashof_v<T>, count); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void shrink_to_fit() noexcept { m_vars.shrink_to_fit(); }

		void shrink_to_fit(hash_t type) noexcept { m_vars[type].shrink_to_fit(); }

		template <class T>
		void shrink_to_fit() { this->shrink_to_fit(hashof_v<T>); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto find(hash_t type) noexcept -> catagories::optl_iterator_pair
		{
			return m_vars.find(type);
		}

		ML_NODISCARD auto find(hash_t type) const noexcept -> catagories::optl_const_iterator_pair
		{
			return m_vars.find(type);
		}

		template <class T
		> ML_NODISCARD auto find() noexcept -> catagories::optl_iterator_pair
		{
			return this->find(hashof_v<T>);
		}

		template <class T
		> ML_NODISCARD auto find() const noexcept -> catagories::optl_const_iterator_pair
		{
			return this->find(hashof_v<T>);
		}

		template <class ID
		> ML_NODISCARD auto find(hash_t type, ID && id) noexcept -> category::optl_iterator_pair
		{
			return m_vars[type].find(getid(ML_forward(id)));
		}

		template <class ID
		> ML_NODISCARD auto find(hash_t type, ID && id) const noexcept -> category::optl_const_iterator_pair
		{
			if (auto const it{ this->find(type) }; !it) { return std::nullopt; }
			else { return it->second->find(getid(ML_forward(id))); }
		}

		template <class T, class ID
		> ML_NODISCARD auto find(ID && id) noexcept -> category::optl_iterator_pair
		{
			return this->find(hashof_v<T>, ML_forward(id));
		}

		template <class T, class ID
		> ML_NODISCARD auto find(ID && id) const noexcept -> category::optl_const_iterator_pair
		{
			return this->find(hashof_v<T>, ML_forward(id));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool contains(hash_t type) const noexcept
		{
			return m_vars.contains(type);
		}

		template <class T> ML_NODISCARD bool contains() const noexcept
		{
			return this->contains(hashof_v<T>);
		}

		template <class ID
		> ML_NODISCARD bool contains(hash_t type, ID && id) const noexcept
		{
			if (auto const it{ this->find(type) }; !it) { return false; }
			else { return it->second->contains(getid(ML_forward(id))); }
		}

		template <class T, class ID
		> ML_NODISCARD bool contains(ID && id) const noexcept
		{
			return this->contains(hashof_v<T>, ML_forward(id));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void erase(hash_t type) noexcept
		{
			if (auto const cat{ this->find(type) })
			{
				m_vars.erase(cat->first);
			}
		}

		template <class T
		> void erase() noexcept
		{
			this->erase(hashof_v<T>);
		}

		template <class ID
		> void erase(hash_t type, ID && id) noexcept
		{
			if (auto const cat{ this->find(type) })
			{
				if (auto const var{ cat->second->find(getid(ML_forward(id))) })
				{
					cat->second->erase(var->first);
				}
			}
		}

		template <class T, class ID
		> void erase(ID && id) noexcept
		{
			this->erase(hashof_v<T>, ML_forward(id));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ID
		> ML_NODISCARD element & load(hash_t type, ID && id, allocator_type alloc = {}) noexcept
		{
			return m_vars[type].find_or_add_fn(getid(ML_forward(id)), [&]() noexcept
			{
				return memory::alloc_ref<variable>(alloc);
			});
		}

		template <class T, class ID
		> ML_NODISCARD element & load(ID && id, allocator_type alloc = {}) noexcept
		{
			return this->load(hashof_v<T>, ML_forward(id), alloc);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class ID
		> ML_NODISCARD T * cast(ID && id, allocator_type alloc = {}) noexcept
		{
			return std::any_cast<T>(this->load<T>(ML_forward(id), alloc).get());
		}

		template <class T, class ID
		> ML_NODISCARD T & get(ID && id, allocator_type alloc = {}) noexcept
		{
			return *this->cast<T>(ML_forward(id), alloc);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class ID, class ... Args
		> T & emplace(allocator_type alloc, ID && id, Args && ... args) noexcept
		{
			return this->load<T>(ML_forward(id), alloc)->emplace<T>(ML_forward(args)..., alloc);
		}

		template <class T, class ID, class ... Args
		> T & emplace(ID && id, Args && ... args) noexcept
		{
			return this->load<T>(ML_forward(id))->emplace<T>(ML_forward(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ID
		> void reset(hash_t type, ID && id, allocator_type alloc = {}) noexcept
		{
			this->load(type, ML_forward(id), alloc)->reset();
		}

		template <class T, class ID
		> void reset(ID && id, allocator_type alloc = {}) noexcept
		{
			this->reset(hashof_v<T>, ML_forward(id), alloc);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void swap(blackboard & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_vars.swap(other.m_vars);
			}
		}

		template <class LHS, class RHS = LHS
		> void swap(hash_t type, LHS && lhs, RHS && rhs, allocator_type alloc = {}) noexcept
		{
			this->load(type, ML_forward(lhs), alloc)
				->swap(*this->load(type, ML_forward(rhs), alloc));
		}

		template <class T, class LHS, class RHS = LHS
		> void swap(LHS && lhs, RHS && rhs, allocator_type alloc = {}) noexcept
		{
			this->swap(hashof_v<T>, ML_forward(lhs), ML_forward(rhs), alloc);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		std::mutex m_mutex	; // 
		catagories m_vars	; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_BLACKBOARD_HPP_