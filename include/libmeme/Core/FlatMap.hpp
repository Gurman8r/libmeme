#ifndef _ML_FLAT_MAP_HPP_
#define _ML_FLAT_MAP_HPP_

#include <libmeme/Core/FlatSet.hpp>

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT MAP TRAITS
	template <
		class	_Kt,	// key type
		class	_Vt,	// value type
		class	_Pr,	// key comparator predicate type
		size_t	_Th		// search algorithm selector threshold
	> struct flat_map_traits final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_type			= typename _Kt;
		using value_type		= typename _Vt;
		using compare_type		= typename _Pr;
		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using difference_type	= typename ptrdiff_t;
		using size_type			= typename size_t;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_storage				= typename ds::flat_set<key_type, compare_type, _Th>;
		using key_pointer				= typename key_storage::pointer;
		using key_const_pointer			= typename key_storage::const_pointer;
		using key_reference				= typename key_storage::reference;
		using key_const_reference		= typename key_storage::const_reference;
		using key_iterator				= typename key_storage::iterator;
		using key_const_iterator		= typename key_storage::const_iterator;

		using value_storage				= typename pmr::vector<value_type>;
		using value_pointer				= typename value_storage::pointer;
		using value_const_pointer		= typename value_storage::const_pointer;
		using value_reference			= typename value_storage::reference;
		using value_const_reference		= typename value_storage::const_reference;
		using value_iterator			= typename value_storage::iterator;
		using value_const_iterator		= typename value_storage::const_iterator;

		using storage_type				= typename std::pair<key_storage, value_storage>;
		using keyval_pair				= typename std::pair<key_type, value_type>;
		using init_type					= typename std::initializer_list<keyval_pair>;

		using pointer_pair				= typename std::pair<key_pointer, value_pointer>;
		using const_pointer_pair		= typename std::pair<key_const_pointer, value_const_pointer>;
		using reference_pair			= typename std::pair<key_reference, value_reference>;
		using const_reference_pair		= typename std::pair<key_const_reference, value_const_reference>;

		using iterator_pair				= typename std::pair<key_iterator, value_iterator>;
		using const_iterator_pair		= typename std::pair<key_const_iterator, value_const_iterator>;
		using optl_iterator_pair		= typename std::optional<iterator_pair>;
		using optl_const_iterator_pair	= typename std::optional<const_iterator_pair>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// BASIC FLAT MAP
	template <class _Traits
	> struct basic_flat_map final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type				= typename _Traits;
		using self_type					= typename basic_flat_map<traits_type>;
		using compare_type				= typename traits_type::compare_type;
		using allocator_type			= typename traits_type::allocator_type;
		using difference_type			= typename traits_type::difference_type;
		using size_type					= typename traits_type::size_type;

		using key_type					= typename traits_type::key_type;
		using key_storage				= typename traits_type::key_storage;
		using key_pointer				= typename traits_type::key_pointer;
		using key_const_pointer			= typename traits_type::key_const_pointer;
		using key_reference				= typename traits_type::key_reference;
		using key_const_reference		= typename traits_type::key_const_reference;
		using key_iterator				= typename traits_type::key_iterator;
		using key_const_iterator		= typename traits_type::key_const_iterator;

		using value_type				= typename traits_type::value_type;
		using value_storage				= typename traits_type::value_storage;
		using value_pointer				= typename traits_type::value_pointer;
		using value_const_pointer		= typename traits_type::value_const_pointer;
		using value_reference			= typename traits_type::value_reference;
		using value_const_reference		= typename traits_type::value_const_reference;
		using value_iterator			= typename traits_type::value_iterator;
		using value_const_iterator		= typename traits_type::value_const_iterator;

		using storage_type				= typename traits_type::storage_type;
		using keyval_pair				= typename traits_type::keyval_pair;
		using init_type					= typename traits_type::init_type;

		using pointer_pair				= typename traits_type::pointer_pair;
		using const_pointer_pair		= typename traits_type::const_pointer_pair;
		using reference_pair			= typename traits_type::reference_pair;
		using const_reference_pair		= typename traits_type::const_reference_pair;

		using iterator_pair				= typename traits_type::iterator_pair;
		using const_iterator_pair		= typename traits_type::const_iterator_pair;
		using optl_iterator_pair		= typename traits_type::optl_iterator_pair;
		using optl_const_iterator_pair	= typename traits_type::optl_const_iterator_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_flat_map(allocator_type const & alloc = {})
			: m_pair{ key_storage{ alloc }, value_storage{ alloc } }
		{
		}

		basic_flat_map(init_type value, allocator_type const & alloc = {})
			: self_type{ alloc }
		{
			this->assign(value);
		}

		basic_flat_map(self_type const & other, allocator_type const & alloc = {})
			: self_type{ alloc }
		{
			this->assign(other);
		}

		basic_flat_map(self_type && other, allocator_type const & alloc = {}) noexcept
			: self_type{ alloc }
		{
			this->swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		self_type & operator=(init_type value)
		{
			self_type temp{ value };
			this->swap(temp);
			return (*this);
		}

		self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			this->swap(temp);
			return (*this);
		}

		self_type & operator=(self_type && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void assign(init_type value)
		{
			if (!this->empty()) { this->clear(); }

			for (auto it = value.begin(); it != value.end(); ++it)
			{
				this->insert(it->first, it->second);
			}
		}

		void assign(self_type const & other)
		{
			if (this != std::addressof(other))
			{
				m_pair = other.m_pair;
			}
		}

		void clear() noexcept
		{
			m_pair.first.clear();
			m_pair.second.clear();
		}

		void pop_back() noexcept
		{
			m_pair.first.pop_back();
			m_pair.second.pop_back();
		}

		void reserve(size_type const count)
		{
			m_pair.first.reserve(count);
			m_pair.second.reserve(count);
		}

		void resize(size_type const count)
		{
			m_pair.first.resize(count);
			m_pair.second.resize(count);
		}

		void resize(size_type const count, value_type const & value)
		{
			m_pair.first.resize(count);
			m_pair.second.resize(count, value);
		}

		void shrink_to_fit()
		{
			m_pair.first.shrink_to_fit();
			m_pair.second.shrink_to_fit();
		}

		void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_pair.swap(other.m_pair);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD key_storage const & keys() const & noexcept
		{
			return m_pair.first;
		}

		ML_NODISCARD value_storage const & values() const & noexcept
		{
			return m_pair.second;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD size_type capacity() const noexcept
		{
			return m_pair.first.capacity();
		}

		ML_NODISCARD bool empty() const noexcept
		{
			return m_pair.first.empty();
		}

		ML_NODISCARD size_type max_size() const noexcept
		{
			return m_pair.first.max_size();
		}

		ML_NODISCARD size_type size() const noexcept
		{
			return m_pair.first.size();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD reference_pair back() noexcept
		{
			return { m_pair.first.back(), m_pair.second.back() };
		}

		ML_NODISCARD const_reference_pair back() const noexcept
		{
			return { m_pair.first.back(), m_pair.second.back() };
		}

		ML_NODISCARD reference_pair front() noexcept
		{
			return { m_pair.first.front(), m_pair.second.front() };
		}

		ML_NODISCARD const_reference_pair front() const noexcept
		{
			return { m_pair.first.front(), m_pair.second.front() };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD value_iterator fetch(key_const_iterator it) noexcept
		{
			return std::next(m_pair.second.begin(), std::distance(m_pair.first.cbegin(), it));
		}

		ML_NODISCARD value_const_iterator fetch(key_const_iterator it) const noexcept
		{
			return std::next(m_pair.second.cbegin(), std::distance(m_pair.first.cbegin(), it));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		iterator_pair erase(key_iterator it)
		{
			auto const result{ m_pair.second.erase(this->fetch(it)) };
			return { m_pair.first.erase(it), result };
		}

		iterator_pair erase(key_iterator first, key_iterator last)
		{
			auto const result{ m_pair.second.erase(this->fetch(first), this->fetch(last)) };
			return { m_pair.first.erase(first, last), result };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other = self_type
		> ML_NODISCARD auto compare(Other const & other) const
		{
			if constexpr (std::is_same_v<Other, self_type>)
			{
				return this->compare(other.m_pair);
			}
			else
			{
				return (m_pair != other) ? ((m_pair < other) ? -1 : 1) : 0;
			}
		}

		ML_NODISCARD bool contains(key_type const & key) const
		{
			return m_pair.first.contains(key);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD optl_iterator_pair find(key_type const & key)
		{
			if (auto const k{ m_pair.first.find(key) }; k != m_pair.first.end())
			{
				return iterator_pair{ k, this->fetch(k) };
			}
			else
			{
				return std::nullopt;
			}
		}

		ML_NODISCARD optl_const_iterator_pair find(key_type const & key) const
		{
			if (auto const k{ m_pair.first.find(key) }; k != m_pair.first.cend())
			{
				return const_iterator_pair{ k, this->fetch(k) };
			}
			else
			{
				return std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> std::pair<iterator_pair, bool> try_emplace(key_type const & key, Args && ... args)
		{
			if (auto const k{ m_pair.first.insert(key) }; k.second)
			{
				return { this->impl_emplace_hint(k.first, ML_forward(args)...), true };
			}
			else
			{
				return { { k.first, this->fetch(k.first) }, false };
			}
		}

		template <class ... Args
		> std::pair<iterator_pair, bool> try_emplace(key_type && key, Args && ... args)
		{
			if (auto const k{ m_pair.first.insert(std::move(key)) }; k.second)
			{
				return { this->impl_emplace_hint(k.first, ML_forward(args)...), true };
			}
			else
			{
				return { { k.first, this->fetch(k.first) }, false };
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Key, class Value
		> iterator_pair insert(Key && key, Value && value)
		{
			return this->try_emplace(ML_forward(key), ML_forward(value)).first;
		}

		iterator_pair insert(keyval_pair && pair)
		{
			return this->try_emplace(ML_forward(pair.first), ML_forward(pair.second)).first;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> value_type & find_or_add(key_type const & key, Args && ... args)
		{
			if (auto const it{ this->find(key) })
			{
				return (*it->second);
			}
			else
			{
				return (*this->insert(key, ML_forward(args)...).second);
			}
		}

		template <class ... Args
		> value_type & find_or_add(key_type && key, Args && ... args)
		{
			if (auto const it{ this->find(key) })
			{
				return (*it->second);
			}
			else
			{
				return (*this->insert(std::move(key), ML_forward(args)...).second);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD value_type & at(key_type const & key)
		{
			return this->find_or_add(key, value_type{});
		}

		ML_NODISCARD value_type & at(key_type && key)
		{
			return this->find_or_add(std::move(key), value_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD value_type & operator[](key_type const & key) noexcept
		{
			return this->at(key);
		}

		ML_NODISCARD value_type & operator[](key_type && key) noexcept
		{
			return this->at(std::move(key));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> decltype(auto) apply(key_const_iterator it, Fn && fn)
		{
			return std::invoke(ML_forward(fn), *it, *this->fetch(it));
		}

		template <class Fn
		> decltype(auto) apply(key_const_iterator it, Fn && fn) const
		{
			return std::invoke(ML_forward(fn), *it, *this->fetch(it));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> auto for_each(key_const_iterator first, key_const_iterator last, Fn && fn) noexcept
		{
			for (; first != last; ++first)
			{
				this->apply(first, ML_forward(fn));
			}
			return first;
		}

		template <class Fn
		> auto for_each(key_const_iterator first, Fn && fn) noexcept
		{
			return this->for_each(first, m_pair.first.cend(), fn);
		}

		template <class Fn
		> auto for_each(Fn && fn) noexcept
		{
			return this->for_each(m_pair.first.cbegin(), fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> auto for_each(key_const_iterator first, key_const_iterator last, Fn && fn) const noexcept
		{
			for (; first != last; ++first)
			{
				this->apply(first, ML_forward(fn));
			}
			return first;
		}

		template <class Fn
		> auto for_each(key_const_iterator first, Fn && fn) const noexcept
		{
			return this->for_each(first, m_pair.first.cend(), fn);
		}

		template <class Fn
		> auto for_each(Fn && fn) const noexcept
		{
			return this->for_each(m_pair.first.cbegin(), fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> auto for_each_n(key_const_iterator first, ptrdiff_t count, Fn && fn) noexcept
		{
			if (0 < count)
			{
				do {
					this->apply(first, ML_forward(fn));
					--count;
					++first;
				} while (0 < count);
			}
			return first;
		}

		template <class Fn
		> auto for_each_n(ptrdiff_t count, Fn && fn) noexcept
		{
			return this->for_each_n(m_pair.first.cbegin(), count, fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> auto for_each_n(key_const_iterator first, ptrdiff_t count, Fn && fn) const noexcept
		{
			if (0 < count)
			{
				do {
					this->apply(first, ML_forward(fn));
					--count;
					++first;
				} while (0 < count);
			}
			return first;
		}

		template <class Fn
		> auto for_each_n(ptrdiff_t count, Fn && fn) const noexcept
		{
			return this->for_each_n(m_pair.first.cbegin(), count, fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other = self_type
		> ML_NODISCARD bool operator==(Other const & other) const
		{
			return this->compare(other) == 0;
		}

		template <class Other = self_type
		> ML_NODISCARD bool operator!=(Other const & other) const
		{
			return this->compare(other) != 0;
		}

		template <class Other = self_type
		> ML_NODISCARD bool operator<(Other const & other) const
		{
			return this->compare(other) < 0;
		}

		template <class Other = self_type
		> ML_NODISCARD bool operator>(Other const & other) const
		{
			return this->compare(other) > 0;
		}

		template <class Other = self_type
		> ML_NODISCARD bool operator<=(Other const & other) const
		{
			return this->compare(other) <= 0;
		}

		template <class Other = self_type
		> ML_NODISCARD bool operator>=(Other const & other) const
		{
			return this->compare(other) >= 0;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// emplace_hint implementation
		template <class ... Args
		> iterator_pair impl_emplace_hint(key_const_iterator it, Args && ... args)
		{
			// must be private or the map could become unsorted
			return {
				std::next(m_pair.first.begin(), std::distance(m_pair.first.cbegin(), it)),
				m_pair.second.emplace(this->fetch(it), ML_forward(args)...)
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT MAP | flat ordered associative container
	template <
		class	_Kt,					// key type
		class	_Vt,					// value type
		class	_Pr = std::less<_Kt>,	// key comparator predicate type
		size_t	_Th = 42				// search algorithm selector threshold
	> ML_alias flat_map = typename basic_flat_map
	<
		flat_map_traits<_Kt, _Vt, _Pr, _Th>
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FLAT_MAP_HPP_