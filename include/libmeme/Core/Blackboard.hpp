#ifndef _ML_BLACKBOARD_HPP_
#define _ML_BLACKBOARD_HPP_

#include <libmeme/Core/BatchVector.hpp>

namespace ml
{
	struct blackboard final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;
		
		using variable = typename std::any;

		using element = typename shared<variable>;

		using category = typename ds::map<hash_t, element>;
		
		using storage = typename ds::map<hash_t, category>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		blackboard(allocator_type alloc = {}) noexcept
			: m_vars{ alloc }
		{
		}

		explicit blackboard(storage const & data, allocator_type alloc = {})
			: m_vars{ data, alloc }
		{
		}

		explicit blackboard(storage && data, allocator_type alloc = {}) noexcept
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

		ML_NODISCARD auto data() & noexcept -> storage & { return m_vars; }

		ML_NODISCARD auto data() const & noexcept -> storage const & { return m_vars; }

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

		ML_NODISCARD bool contains(hash_t type) const noexcept
		{
			return m_vars.contains(type);
		}

		ML_NODISCARD bool contains(hash_t type, hash_t id) const noexcept
		{
			if (auto const it{ m_vars.find(type) }; !it) { return false; }
			else { return (*it->second).contains(id); }
		}

		ML_NODISCARD bool contains(hash_t type, pmr::string const & id) const noexcept
		{
			return this->contains(type, util::hash(id));
		}

		template <class T> ML_NODISCARD bool contains() const noexcept
		{
			return this->contains(hashof_v<T>);
		}

		template <class T> ML_NODISCARD bool contains(hash_t id) const noexcept
		{
			return this->contains(hashof_v<T>, id);
		}

		template <class T> ML_NODISCARD bool contains(pmr::string const & id) const noexcept
		{
			return this->contains(hashof_v<T>, id);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto find(hash_t type) noexcept -> storage::optl_iterator_pair
		{
			return m_vars.find(type);
		}

		ML_NODISCARD auto find(hash_t type) const noexcept -> storage::optl_const_iterator_pair
		{
			return m_vars.find(type);
		}

		ML_NODISCARD auto find(hash_t type, hash_t id) noexcept -> category::optl_iterator_pair
		{
			return m_vars[type].find(id);
		}

		ML_NODISCARD auto find(hash_t type, hash_t id) const noexcept -> category::optl_const_iterator_pair
		{
			if (auto const it{ this->find(type) })
			{
				return it->second->find(id);
			}
			else
			{
				return std::nullopt;
			}
		}

		ML_NODISCARD auto find(hash_t type, pmr::string const & id) noexcept -> category::optl_iterator_pair
		{
			return this->find(type, util::hash(id));
		}

		ML_NODISCARD auto find(hash_t type, pmr::string const & id) const noexcept -> category::optl_const_iterator_pair
		{
			return this->find(type, util::hash(id));
		}

		template <class T> ML_NODISCARD auto find() noexcept -> storage::optl_iterator_pair
		{
			return this->find(hashof_v<T>);
		}

		template <class T> ML_NODISCARD auto find() const noexcept -> storage::optl_const_iterator_pair
		{
			return this->find(hashof_v<T>);
		}

		template <class T> ML_NODISCARD auto find(hash_t id) noexcept -> category::optl_iterator_pair
		{
			return this->find(hashof_v<T>, id);
		}

		template <class T> ML_NODISCARD auto find(hash_t id) const noexcept -> category::optl_const_iterator_pair
		{
			return this->find(hashof_v<T>, id);
		}

		template <class T> ML_NODISCARD auto find(pmr::string const & id) noexcept -> category::optl_iterator_pair
		{
			return this->find(hashof_v<T>, id);
		}

		template <class T> ML_NODISCARD auto find(pmr::string const & id) const noexcept -> category::optl_const_iterator_pair
		{
			return this->find(hashof_v<T>, id);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void erase(hash_t type) noexcept
		{
			if (auto const cat{ this->find(type) })
			{
				m_vars.erase(cat->first);
			}
		}

		void erase(hash_t type, hash_t id) noexcept
		{
			if (auto const cat{ this->find(type) })
			{
				if (auto const var{ cat->second->find(id) })
				{
					cat->second->erase(var->first);
				}
			}
		}

		void erase(hash_t type, pmr::string const & id) noexcept
		{
			this->erase(type, util::hash(id));
		}

		template <class T
		> void erase(hash_t id) noexcept
		{
			this->erase(hashof_v<T>, id);
		}

		template <class T
		> void erase(pmr::string const & id) noexcept
		{
			this->erase(hashof_v<T>, id);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD element & load(hash_t type, hash_t id, allocator_type alloc = {}) noexcept
		{
			return m_vars[type].find_or_add_fn(id, [&alloc]() noexcept
			{
				return memory::alloc_ref<variable>(alloc);
			});
		}

		ML_NODISCARD element & load(hash_t type, pmr::string const & id, allocator_type alloc = {}) noexcept
		{
			return this->load(type, util::hash(id), alloc);
		}

		template <class T
		> ML_NODISCARD element & load(hash_t id, allocator_type alloc = {}) noexcept
		{
			return this->load(hashof_v<T>, id, alloc);
		}

		template <class T
		> ML_NODISCARD element & load(pmr::string const & id, allocator_type alloc = {}) noexcept
		{
			return this->load(hashof_v<T>, id, alloc);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD T & get(hash_t id, allocator_type alloc = {}) noexcept
		{
			return std::any_cast<T>(*this->load<T>(id, alloc));
		}

		template <class T
		> ML_NODISCARD T & get(pmr::string const & id, allocator_type alloc = {}) noexcept
		{
			return std::any_cast<T>(*this->load<T>(id, alloc));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class ... Args
		> T & emplace(allocator_type alloc, hash_t id, Args && ... args) noexcept
		{
			return this->load<T>(id, alloc)->emplace<T>(ML_forward(args)..., alloc);
		}

		template <class T, class ... Args
		> T & emplace(allocator_type alloc, pmr::string const & id, Args && ... args) noexcept
		{
			return this->load<T>(id, alloc)->emplace<T>(ML_forward(args)..., alloc);
		}

		template <class T, class ... Args
		> T & emplace(hash_t id, Args && ... args) noexcept
		{
			return this->load<T>(id)->emplace<T>(ML_forward(args)...);
		}

		template <class T, class ... Args
		> T & emplace(pmr::string const & id, Args && ... args) noexcept
		{
			return this->load<T>(id)->emplace<T>(ML_forward(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void reset(hash_t type, hash_t id) noexcept
		{
			this->load(type, id)->reset();
		}

		void reset(hash_t type, pmr::string const & id) noexcept
		{
			this->reset(type, util::hash(id));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void swap(blackboard & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_vars.swap(other.m_vars);
			}
		}

		void swap(hash_t type, hash_t lhs, hash_t rhs) noexcept
		{
			this->load(type, lhs)->swap(*this->load(type, rhs));
		}

		void swap(hash_t type, hash_t lhs, pmr::string const & rhs) noexcept
		{
			this->swap(type, lhs, util::hash(rhs));
		}

		void swap(hash_t type, pmr::string const & lhs, hash_t rhs) noexcept
		{
			this->swap(type, util::hash(lhs), rhs);
		}

		void swap(hash_t type, pmr::string const & lhs, pmr::string const & rhs) noexcept
		{
			this->swap(type, util::hash(lhs), util::hash(rhs));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage m_vars; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_BLACKBOARD_HPP_