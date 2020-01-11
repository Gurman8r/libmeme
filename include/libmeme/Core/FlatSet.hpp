#ifndef _ML_FLAT_HPP_
#define _ML_FLAT_HPP_

#include <libmeme/Core/DS.hpp>

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// BASIC FLAT SET TRAITS
	template <class Elem, class Comp, class Alloc, bool Multi
	> struct basic_flat_set_traits
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename Elem;
		using compare_type		= typename Comp;
		using allocator_type	= typename Alloc;

		static constexpr bool multi{ Multi };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_type				= typename std::vector<value_type, allocator_type>;
		using pointer					= typename storage_type::pointer;
		using reference					= typename storage_type::reference;
		using const_pointer				= typename storage_type::const_pointer;
		using const_reference			= typename storage_type::const_reference;
		using difference_type			= typename storage_type::difference_type;
		using size_type					= typename storage_type::size_type;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;
		using initializer_type			= typename std::initializer_list<value_type>;
		using iterator_pair				= typename std::pair<iterator, iterator>;
		using const_iterator_pair		= typename std::pair<const_iterator, const_iterator>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// BASIC FLAT SET
	template <class Traits
	> struct basic_flat_set
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type				= typename Traits;
		using self_type					= typename basic_flat_set<traits_type>;
		using value_type				= typename traits_type::value_type;
		using compare_type				= typename traits_type::compare_type;
		using allocator_type			= typename traits_type::allocator_type;
		using storage_type				= typename traits_type::storage_type;
		using pointer					= typename traits_type::pointer;
		using reference					= typename traits_type::reference;
		using const_pointer				= typename traits_type::const_pointer;
		using const_reference			= typename traits_type::const_reference;
		using difference_type			= typename traits_type::difference_type;
		using size_type					= typename traits_type::size_type;
		using iterator					= typename traits_type::iterator;
		using const_iterator			= typename traits_type::const_iterator;
		using reverse_iterator			= typename traits_type::reverse_iterator;
		using const_reverse_iterator	= typename traits_type::const_reverse_iterator;
		using initializer_type			= typename traits_type::initializer_type;
		using iterator_pair				= typename traits_type::iterator_pair;
		using const_iterator_pair		= typename traits_type::const_iterator_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr bool multi{ traits_type::multi };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_flat_set() noexcept
			: m_storage{}
		{
		}

		template <class It> basic_flat_set(It first, It last)
			: m_storage{ first, last }
		{
		}

		basic_flat_set(initializer_type init)
			: m_storage{ init }
		{
		}

		explicit basic_flat_set(storage_type const & value)
			: m_storage{ value }
		{
		}

		explicit basic_flat_set(storage_type && value) noexcept
			: m_storage{ std::move(value) }
		{
		}

		explicit basic_flat_set(storage_type const & value, allocator_type const & alloc)
			: m_storage{ value, alloc }
		{
		}

		explicit basic_flat_set(storage_type && value, allocator_type const & alloc) noexcept
			: m_storage{ std::move(value), alloc }
		{
		}

		basic_flat_set(self_type const & other)
			: m_storage{ other.m_storage }
		{
		}

		basic_flat_set(self_type && other) noexcept
			: m_storage{ std::move(other.m_storage) }
		{
		}

		basic_flat_set(self_type const & other, allocator_type const & alloc)
			: m_storage{ other.m_storage, alloc }
		{
		}

		basic_flat_set(self_type && other, allocator_type const & alloc) noexcept
			: m_storage{ std::move(other.m_storage), alloc }
		{
		}

		virtual ~basic_flat_set() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		inline self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline self_type & operator=(initializer_type init)
		{
			self_type temp{ init };
			swap(temp);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void clear() noexcept
		{
			return m_storage.clear();
		}

		inline void reserve(size_type const count)
		{
			return m_storage.reserve(count);
		}

		inline void resize(size_type const count)
		{
			return m_storage.resize(count);
		}

		inline void shrink_to_fit()
		{
			return m_storage.shrink_to_fit();
		}

		inline void swap(self_type & other) noexcept
		{
			return m_storage.swap(other.m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator erase(iterator location)
		{
			return m_storage.erase(location);
		}

		inline iterator erase(iterator first, iterator last)
		{
			return m_storage.erase(first, last);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline bool contains(U && u) const
		{
			return std::binary_search(cbegin(), cend(), std::forward<U>(u), compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline iterator_pair equal_range(U && u)
		{
			return std::equal_range(begin(), end(), std::forward<U>(u), compare_type{});
		}

		template <class U> inline const_iterator_pair equal_range(U && u) const
		{
			return std::equal_range(cbegin(), cend(), std::forward<U>(u), compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline iterator lower_bound(U && u)
		{
			return std::lower_bound(begin(), end(), std::forward<U>(u), compare_type{});
		}

		template <class U> inline const_iterator lower_bound(U && u) const
		{
			return std::lower_bound(cbegin(), cend(), std::forward<U>(u), compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline iterator upper_bound(U && u)
		{
			return std::upper_bound(begin(), end(), std::forward<U>(u), compare_type{});
		}

		template <class U> inline const_iterator upper_bound(U && u) const
		{
			return std::upper_bound(cbegin(), cend(), std::forward<U>(u), compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline iterator find(U && u)
		{
			auto const it{ equal_range(std::forward<U>(u)) };
			return (it.first != it.second) ? it.first : end();
		}

		template <class U> inline const_iterator find(U && u) const
		{
			auto const it{ equal_range(std::forward<U>(u)) };
			return (it.first != it.second) ? it.first : cend();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline reference at(size_type const i) { return m_storage.at(i); }

		inline const_reference at(size_type const i) const { return m_storage.at(i); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline reference operator[](size_type const i) { return m_storage[i]; }

		inline const_reference operator[](size_type const i) const { return m_storage[i]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto back() noexcept -> reference { return m_storage.back(); }

		inline auto back() const noexcept -> const_reference { return m_storage.back(); }

		inline auto begin() noexcept -> iterator { return m_storage.begin(); }

		inline auto begin() const noexcept -> const_iterator { return m_storage.begin(); }

		inline auto capacity() const noexcept -> size_type { return m_storage.capacity(); }

		inline auto cbegin() const noexcept -> const_iterator { return m_storage.cbegin(); }

		inline auto cend() const noexcept -> const_iterator { return m_storage.cend(); }

		inline auto crbegin() const noexcept -> const_reverse_iterator { return m_storage.crbegin(); }

		inline auto crend() const noexcept -> const_reverse_iterator { return m_storage.crend(); }

		inline auto data() noexcept -> pointer { return m_storage.data(); }

		inline auto data() const noexcept -> const_pointer { return m_storage.data(); }

		inline bool empty() const noexcept { return m_storage.empty(); }

		inline auto end() noexcept -> iterator { return m_storage.end(); }

		inline auto end() const noexcept -> const_iterator { return m_storage.end(); }

		inline auto front() noexcept -> reference { return m_storage.front(); }

		inline auto front() const noexcept -> const_reference { return m_storage.front(); }

		inline auto get_allocator() const noexcept -> allocator_type { return m_storage.get_allocator(); }

		inline auto max_size() const noexcept -> size_type { return m_storage.max_size(); }

		inline auto rbegin() noexcept -> reverse_iterator { return m_storage.rbegin(); }

		inline auto rbegin() const noexcept -> const_reverse_iterator { return m_storage.rbegin(); }

		inline auto rend() noexcept -> reverse_iterator { return m_storage.rend(); }

		inline auto rend() const noexcept -> const_reverse_iterator { return m_storage.rend(); }

		inline auto size() const noexcept -> size_type { return m_storage.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		storage_type m_storage;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT SET - sorted vector of unique elements
	template <class Elem, class Comp, class Alloc
	> struct flat_set final : public basic_flat_set<
		basic_flat_set_traits<Elem, Comp, Alloc, false>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename flat_set<Elem, Comp, Alloc>;
		using base_type					= typename basic_flat_set<basic_flat_set_traits<Elem, Comp, Alloc, false>>;
		using traits_type				= typename base_type::traits_type;
		using value_type				= typename base_type::value_type;
		using compare_type				= typename base_type::compare_type;
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
		using pointer					= typename base_type::pointer;
		using reference					= typename base_type::reference;
		using const_pointer				= typename base_type::const_pointer;
		using const_reference			= typename base_type::const_reference;
		using difference_type			= typename base_type::difference_type;
		using size_type					= typename base_type::size_type;
		using iterator					= typename base_type::iterator;
		using const_iterator			= typename base_type::const_iterator;
		using reverse_iterator			= typename base_type::reverse_iterator;
		using const_reverse_iterator	= typename base_type::const_reverse_iterator;
		using initializer_type			= typename base_type::initializer_type;
		using iterator_pair				= typename base_type::iterator_pair;
		using const_iterator_pair		= typename base_type::const_iterator_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		flat_set() noexcept
			: base_type{}
		{
		}

		flat_set(initializer_type init)
			: base_type{ init }
		{
			sort_internal();
		}

		template <class It> flat_set(It first, It last)
			: base_type{ first, last }
		{
			sort_internal();
		}

		explicit flat_set(storage_type const & other, allocator_type const & alloc = {})
			: base_type{ other, alloc }
		{
			sort_internal();
		}

		explicit flat_set(storage_type && other, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(other), alloc }
		{
			sort_internal();
		}

		flat_set(self_type const & other, allocator_type const & alloc = {})
			: base_type{ other.m_storage, alloc }
		{
		}

		flat_set(self_type && other, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(other.m_storage), alloc }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline self_type operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline self_type operator=(initializer_type init)
		{
			self_type temp{ init };
			swap(temp);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator, bool> insert(const_reference value)
		{
			if (auto const it{ equal_range(value) }; it.first != it.second)
			{
				return std::make_pair(it.second, false);
			}
			else
			{
				return std::make_pair(m_storage.emplace(it.second, value), true);
			}
		}

		inline std::pair<iterator, bool> insert(value_type && value)
		{
			if (auto const it{ equal_range(std::move(value)) }; it.first != it.second)
			{
				return std::make_pair(it.second, false);
			}
			else
			{
				return std::make_pair(m_storage.emplace(it.second, std::move(value)), true);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		inline void sort_internal()
		{
			std::sort(begin(), end(), compare_type{});

			erase(std::unique(begin(), end(), compare_type{}), end());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT MULTISET - sorted vector of elements
	template <class Elem, class Comp, class Alloc
	> struct flat_multiset final : public basic_flat_set<
		basic_flat_set_traits<Elem, Comp, Alloc, true>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename flat_multiset<Elem, Comp, Alloc>;
		using base_type					= typename basic_flat_set<basic_flat_set_traits<Elem, Comp, Alloc, true>>;
		using traits_type				= typename base_type::traits_type;
		using value_type				= typename base_type::value_type;
		using compare_type				= typename base_type::compare_type;
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
		using pointer					= typename base_type::pointer;
		using reference					= typename base_type::reference;
		using const_pointer				= typename base_type::const_pointer;
		using const_reference			= typename base_type::const_reference;
		using difference_type			= typename base_type::difference_type;
		using size_type					= typename base_type::size_type;
		using iterator					= typename base_type::iterator;
		using const_iterator			= typename base_type::const_iterator;
		using reverse_iterator			= typename base_type::reverse_iterator;
		using const_reverse_iterator	= typename base_type::const_reverse_iterator;
		using initializer_type			= typename base_type::initializer_type;
		using iterator_pair				= typename base_type::iterator_pair;
		using const_iterator_pair		= typename base_type::const_iterator_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		flat_multiset() noexcept
			: base_type{}
		{
		}

		flat_multiset(initializer_type init)
			: base_type{ init }
		{
			sort_internal();
		}

		template <class It> flat_multiset(It first, It last)
			: base_type{ first, last }
		{
			sort_internal();
		}

		explicit flat_multiset(storage_type const & other, allocator_type const & alloc = {})
			: base_type{ other, alloc }
		{
			sort_internal();
		}

		explicit flat_multiset(storage_type && other, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(other), alloc }
		{
			sort_internal();
		}

		flat_multiset(self_type const & other, allocator_type const & alloc = {})
			: base_type{ other.m_storage, alloc }
		{
		}

		flat_multiset(self_type && other, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(other.m_storage), alloc }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline self_type operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline self_type operator=(initializer_type init)
		{
			self_type temp{ init };
			swap(temp);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator insert(const_reference value)
		{
			return m_storage.emplace(upper_bound(value), value);
		}

		inline iterator insert(value_type && value)
		{
			return m_storage.emplace(upper_bound(std::move(value)), std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		inline void sort_internal()
		{
			std::sort(begin(), end(), compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// SET
	template <class Elem,
		class Comp = std::less<Elem>,
		class Alloc = std::allocator<Elem>
	> ML_USING set = _ML_DS flat_set<Elem, Comp, Alloc>;


	// MULTISET
	template <class Elem,
		class Comp = std::less<Elem>,
		class Alloc = std::allocator<Elem>
	> ML_USING multiset = _ML_DS flat_multiset<Elem, Comp, Alloc>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FLAT_HPP_