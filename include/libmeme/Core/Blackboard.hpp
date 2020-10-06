#ifndef _ML_BLACKBOARD_HPP_
#define _ML_BLACKBOARD_HPP_

#include <libmeme/Core/Memory.hpp>

namespace ml
{
	struct blackboard final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using variable_type		= typename std::any;
		using element_type		= typename shared<variable_type>;
		using id_type			= typename hash_t;
		using category_type		= typename ds::map<id_type, element_type>;
		using catagories_type	= typename ds::map<id_type, category_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> ML_NODISCARD static constexpr id_type make_id() noexcept
		{
			return hashof_v<T>;
		}

		template <class T> ML_NODISCARD static constexpr id_type make_id(T && id) noexcept
		{
			if constexpr (std::is_integral_v<std::decay_t<decltype(id)>>)
			{
				return static_cast<id_type>(id);
			}
			else
			{
				return util::hash(ML_forward(id));
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> struct handle final
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			using value_type		= typename T;
			using self_type			= typename blackboard::handle<value_type>;
			using wrapper_type		= typename std::reference_wrapper<value_type>;
			using pointer			= typename value_type *;
			using const_pointer		= typename value_type const *;
			using reference			= typename value_type &;
			using const_reference	= typename value_type const &;

			template <class ID
			> handle(blackboard * bb, ID && id, allocator_type alloc = {})
				: m_bb	{ ML_check(bb) }
				, m_id	{ make_id(ML_forward(id)) }
				, m_ref	{ m_bb->emplace<value_type>(alloc, m_id) }
			{
			}

			~handle() noexcept
			{
				ML_check(m_bb)->erase<value_type>(m_id);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			variable_type & update(allocator_type alloc = {}) noexcept
			{
				ML_assert_msg(m_bb, "BAD PARENT BLACKBOARD");
				return (m_ref = m_bb->load<value_type>(m_id, alloc));
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			auto id		() const noexcept	-> id_type				{ return m_id; }
			auto parent	() const noexcept	-> blackboard *			{ return m_bb; }
			auto ref	() & noexcept		-> reference			{ return m_ref.get(); }
			auto ref	() const & noexcept	-> const_reference		{ return m_ref.get(); }
			auto cref	() const & noexcept	-> const_reference		{ return ref(); }
			auto ptr	() noexcept			-> pointer				{ return &ref(); }
			auto ptr	() const noexcept	-> const_pointer		{ return &ref(); }
			auto cptr	() const noexcept	-> const_pointer		{ return ptr(); }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			ML_NODISCARD pointer operator->() noexcept { return ptr(); }

			ML_NODISCARD const_pointer operator->() const noexcept { return cptr(); }

			ML_NODISCARD reference operator*() & noexcept { return ref(); }

			ML_NODISCARD const_reference operator*() const & noexcept { return cref(); }

			ML_NODISCARD operator reference () & noexcept { return ref(); }

			ML_NODISCARD operator const_reference () const & noexcept { return cref(); }
			
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <class I
			> decltype(auto) operator[](I && i) noexcept
			{
				return ref()[ML_forward(i)];
			}

			template <class I
			> decltype(auto) operator[](I && i) const noexcept
			{
				return cref()[ML_forward(i)];
			}

			template <class ... Args
			> decltype(auto) operator()(Args && ... args) noexcept
			{
				return std::invoke(ref(), ML_forward(args)...);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			ML_NODISCARD auto begin() noexcept { return ref().begin(); }
			
			ML_NODISCARD auto begin() const noexcept { return ref().begin(); }
			
			ML_NODISCARD auto cbegin() const noexcept { return cref().cbegin(); }

			ML_NODISCARD auto end() noexcept { return ref().end(); }
			
			ML_NODISCARD auto end() const noexcept { return ref().end(); }
			
			ML_NODISCARD auto cend() const noexcept { return cref().cend(); }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		private:
			blackboard *	m_bb	; // 
			id_type			m_id	; // 
			wrapper_type	m_ref	; // 

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		blackboard(allocator_type alloc = {}) noexcept
			: m_vars{ alloc }
		{
		}

		explicit blackboard(catagories_type const & data, allocator_type alloc = {})
			: m_vars{ data, alloc }
		{
		}

		explicit blackboard(catagories_type && data, allocator_type alloc = {}) noexcept
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

		ML_NODISCARD auto data() & noexcept -> catagories_type & { return m_vars; }

		ML_NODISCARD auto data() const & noexcept -> catagories_type const & { return m_vars; }

		ML_NODISCARD auto capacity() const noexcept -> size_t { return m_vars.capacity(); }

		ML_NODISCARD bool empty() const noexcept { return m_vars.empty(); }

		ML_NODISCARD auto get_allocator() const noexcept -> allocator_type { return m_vars.get_allocator(); }

		ML_NODISCARD auto max_size() const noexcept -> size_t { return m_vars.max_size(); }

		ML_NODISCARD auto size() const noexcept -> size_t { return m_vars.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto find(id_type type) noexcept -> catagories_type::optl_iterator_pair
		{
			return m_vars.find(type);
		}

		ML_NODISCARD auto find(id_type type) const noexcept -> catagories_type::optl_const_iterator_pair
		{
			return m_vars.find(type);
		}

		template <class T
		> ML_NODISCARD auto find() noexcept -> catagories_type::optl_iterator_pair
		{
			return this->find(make_id<T>());
		}

		template <class T
		> ML_NODISCARD auto find() const noexcept -> catagories_type::optl_const_iterator_pair
		{
			return this->find(make_id<T>());
		}

		template <class ID
		> ML_NODISCARD auto find(id_type type, ID && id) noexcept -> category_type::optl_iterator_pair
		{
			return m_vars[type].find(make_id(ML_forward(id)));
		}

		template <class ID
		> ML_NODISCARD auto find(id_type type, ID && id) const noexcept -> category_type::optl_const_iterator_pair
		{
			if (auto const it{ this->find(type) }; !it) { return std::nullopt; }
			else { return it->second->find(make_id(ML_forward(id))); }
		}

		template <class T, class ID
		> ML_NODISCARD auto find(ID && id) noexcept -> category_type::optl_iterator_pair
		{
			return this->find(make_id<T>(), ML_forward(id));
		}

		template <class T, class ID
		> ML_NODISCARD auto find(ID && id) const noexcept -> category_type::optl_const_iterator_pair
		{
			return this->find(make_id<T>(), ML_forward(id));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool contains(id_type type) const noexcept
		{
			return m_vars.contains(type);
		}

		template <class T> ML_NODISCARD bool contains() const noexcept
		{
			return this->contains(make_id<T>());
		}

		template <class ID
		> ML_NODISCARD bool contains(id_type type, ID && id) const noexcept
		{
			if (auto const it{ this->find(type) }; !it) { return false; }
			else { return it->second->contains(make_id(ML_forward(id))); }
		}

		template <class T, class ID
		> ML_NODISCARD bool contains(ID && id) const noexcept
		{
			return this->contains(make_id<T>(), ML_forward(id));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void erase(id_type type) noexcept
		{
			if (auto const cat{ this->find(type) })
			{
				m_vars.erase(cat->first);
			}
		}

		template <class T
		> void erase() noexcept
		{
			this->erase(make_id<T>());
		}

		template <class ID
		> void erase(id_type type, ID && id) noexcept
		{
			if (auto const cat{ this->find(type) })
			{
				if (auto const var{ cat->second->find(make_id(ML_forward(id))) })
				{
					cat->second->erase(var->first);
				}
			}
		}

		template <class T, class ID
		> void erase(ID && id) noexcept
		{
			this->erase(make_id<T>(), ML_forward(id));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ID
		> ML_NODISCARD element_type & load(id_type type, ID && id, allocator_type alloc = {}) noexcept
		{
			return m_vars[type].find_or_add_fn(make_id(ML_forward(id)), [&]() noexcept
			{
				return memory::alloc_ref<variable_type>(alloc);
			});
		}

		template <class T, class ID
		> ML_NODISCARD element_type & load(ID && id, allocator_type alloc = {}) noexcept
		{
			return this->load(make_id<T>(), ML_forward(id), alloc);
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
		> void reset(id_type type, ID && id, allocator_type alloc = {}) noexcept
		{
			this->load(type, ML_forward(id), alloc)->reset();
		}

		template <class T, class ID
		> void reset(ID && id, allocator_type alloc = {}) noexcept
		{
			this->reset(make_id<T>(), ML_forward(id), alloc);
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
		> void swap(id_type type, LHS && lhs, RHS && rhs, allocator_type alloc = {}) noexcept
		{
			this->load(type, ML_forward(lhs), alloc)
				->swap(*this->load(type, ML_forward(rhs), alloc));
		}

		template <class T, class LHS, class RHS = LHS
		> void swap(LHS && lhs, RHS && rhs, allocator_type alloc = {}) noexcept
		{
			this->swap(make_id<T>(), ML_forward(lhs), ML_forward(rhs), alloc);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear() noexcept { m_vars.clear(); }

		void clear(id_type type) noexcept { m_vars[type].clear(); }

		template <class T
		> void clear() { this->clear(make_id<T>()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void resize(size_t const count) { m_vars.resize(count); }
		
		void resize(id_type type, size_t const count) { m_vars[type].resize(count); }

		template <class T
		> void resize(size_t const count) { this->resize(make_id<T>(), count); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void reserve(size_t const count) { m_vars.reserve(count); }

		void reserve(id_type type, size_t const count) { m_vars[type].reserve(count); }
		
		template <class T
		> void reserve(size_t const count) { this->reserve(make_id<T>(), count); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void shrink_to_fit() noexcept { m_vars.shrink_to_fit(); }

		void shrink_to_fit(id_type type) noexcept { m_vars[type].shrink_to_fit(); }

		template <class T
		> void shrink_to_fit() { this->shrink_to_fit(make_id<T>()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		std::mutex m_mutex	; // 
		catagories_type m_vars	; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_BLACKBOARD_HPP_