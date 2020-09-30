#ifndef _ML_BLACKBOARD_HPP_
#define _ML_BLACKBOARD_HPP_

#include <libmeme/Core/BatchVector.hpp>

namespace ml
{
	struct blackboard final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using variable_type		= typename std::any;
		using element_type		= typename shared<variable_type>;
		using catagory_type		= typename ds::map<hash_t, element_type>;
		using storage_type		= typename ds::map<hash_t, catagory_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr hash_t getid(hash_t id) noexcept { return id; }

		template <size_t N
		> static constexpr hash_t getid(const char(&id)[N]) noexcept { return util::hash(id, N); }

		template <class ID
		> static constexpr hash_t getid(ID && id) noexcept { return util::hash(ML_forward(id)); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		blackboard(allocator_type alloc = {}) noexcept
			: m_vars{ alloc }
		{
		}

		explicit blackboard(storage_type const & data, allocator_type alloc = {})
			: m_vars{ data, alloc }
		{
		}

		explicit blackboard(storage_type && data, allocator_type alloc = {}) noexcept
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

		ML_NODISCARD auto data() & noexcept -> storage_type & { return m_vars; }

		ML_NODISCARD auto data() const & noexcept -> storage_type const & { return m_vars; }

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

		ML_NODISCARD auto find(hash_t type) noexcept -> storage_type::optl_iterator_pair
		{
			return m_vars.find(type);
		}

		ML_NODISCARD auto find(hash_t type) const noexcept -> storage_type::optl_const_iterator_pair
		{
			return m_vars.find(type);
		}

		template <class T
		> ML_NODISCARD auto find() noexcept -> storage_type::optl_iterator_pair
		{
			return this->find(hashof_v<T>);
		}

		template <class T
		> ML_NODISCARD auto find() const noexcept -> storage_type::optl_const_iterator_pair
		{
			return this->find(hashof_v<T>);
		}

		template <class ID
		> ML_NODISCARD auto find(hash_t type, ID && id) noexcept -> catagory_type::optl_iterator_pair
		{
			return m_vars[type].find(this->getid(ML_forward(id)));
		}

		template <class ID
		> ML_NODISCARD auto find(hash_t type, ID && id) const noexcept -> catagory_type::optl_const_iterator_pair
		{
			if (auto const it{ this->find(type) }; !it) { return std::nullopt; }
			else { return it->second->find(this->getid(ML_forward(id))); }
		}

		template <class T, class ID
		> ML_NODISCARD auto find(ID && id) noexcept -> catagory_type::optl_iterator_pair
		{
			return this->find(hashof_v<T>, ML_forward(id));
		}

		template <class T, class ID
		> ML_NODISCARD auto find(ID && id) const noexcept -> catagory_type::optl_const_iterator_pair
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
			else { return it->second->contains(this->getid(ML_forward(id))); }
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
				if (auto const var{ cat->second->find(this->getid(ML_forward(id))) })
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
		> ML_NODISCARD element_type & load(hash_t type, ID && id, allocator_type alloc = {}) noexcept
		{
			return m_vars[type].find_or_add_fn(this->getid(ML_forward(id)), [&alloc]() noexcept
			{
				return memory::alloc_ref<variable_type>(alloc);
			});
		}

		template <class T, class ID
		> ML_NODISCARD element_type & load(ID && id, allocator_type alloc = {}) noexcept
		{
			return this->load(hashof_v<T>, ML_forward(id), alloc);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class ID
		> ML_NODISCARD T & get(ID && id, allocator_type alloc = {}) noexcept
		{
			return std::any_cast<T>(*this->load<T>(ML_forward(id), alloc));
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

		template <class T, class ID, class ... Args
		> auto & new_list(allocator_type alloc, ID && id, Args && ... args) noexcept
		{
			return this->emplace<pmr::vector<T>>(alloc, ML_forward(id), ML_forward(args)...);
		}

		template <class T, class ID, class ... Args
		> auto & new_list(ID && id, Args && ... args) noexcept
		{
			return this->emplace<pmr::vector<T>>(ML_forward(id), ML_forward(args)...);
		}

		template <class T, class ID
		> void del_list(ID && id) noexcept
		{
			return this->erase<pmr::vector<T>>(ML_forward(id));
		}

		template <class T, class ID
		> ML_NODISCARD auto & get_list(ID && id) noexcept
		{
			return this->get<pmr::vector<T>>(ML_forward(id));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class K, class V, class ID, class ... Args
		> auto & new_map(allocator_type alloc, ID && id, Args && ... args) noexcept
		{
			return this->emplace<ds::map<K, V>>(alloc, ML_forward(id), ML_forward(args)...);
		}

		template <class K, class V, class ID, class ... Args
		> auto & new_map(ID && id, Args && ... args) noexcept
		{
			return this->emplace<ds::map<K, V>>(ML_forward(id), ML_forward(args)...);
		}

		template <class K, class V, class ID
		> void del_map(ID && id) noexcept
		{
			return this->erase<ds::map<K, V>>(ML_forward(id));
		}

		template <class K, class V, class ID
		> ML_NODISCARD auto & get_map(ID && id) noexcept
		{
			return this->get<ds::map<K, V>>(ML_forward(id));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class K, class V, class ID, class ... Args
		> auto & new_hashmap(allocator_type alloc, ID && id, Args && ... args) noexcept
		{
			return this->emplace<pmr::unordered_map<K, V>>(alloc, ML_forward(id), ML_forward(args)...);
		}

		template <class K, class V, class ID, class ... Args
		> auto & new_hashmap(ID && id, Args && ... args) noexcept
		{
			return this->emplace<pmr::unordered_map<K, V>>(ML_forward(id), ML_forward(args)...);
		}

		template <class K, class V, class ID
		> void del_hashmap(ID && id) noexcept
		{
			return this->erase<pmr::unordered_map<K, V>>(ML_forward(id));
		}

		template <class K, class V, class ID
		> ML_NODISCARD auto & get_hashmap(ID && id) noexcept
		{
			return this->get<pmr::unordered_map<K, V>>(ML_forward(id));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Ts, class ID, class ... Args
		> auto & new_batch(allocator_type alloc, ID && id, Args && ... args) noexcept
		{
			return this->emplace<ds::batch_vector<Ts...>>(alloc, ML_forward(id), ML_forward(args)...);
		}

		template <class ... Ts, class ID, class ... Args
		> auto & new_batch(ID && id, Args && ... args) noexcept
		{
			return this->emplace<ds::batch_vector<Ts...>>(ML_forward(id), ML_forward(args)...);
		}

		template <class ... Ts, class ID
		> void del_batch(ID && id) noexcept
		{
			return this->erase<ds::batch_vector<Ts...>>(ML_forward(id));
		}

		template <class ... Ts, class ID
		> ML_NODISCARD auto & get_batch(ID && id) noexcept
		{
			return this->get<ds::batch_vector<Ts...>>(ML_forward(id));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_vars; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_BLACKBOARD_HPP_