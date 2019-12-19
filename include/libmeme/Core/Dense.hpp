#ifndef _ML_SMALL_MAP_HPP_
#define _ML_SMALL_MAP_HPP_

#include <libmeme/Core/Core.hpp>

namespace ml
{
	template <class _Key, class _Val> struct dense_map final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_type					= typename _Key;
		using mapped_type				= typename _Val;
		using self_type					= typename _ML dense_map<key_type, mapped_type>;
		using value_type				= typename _STD pair<key_type, mapped_type>;
		using storage_type				= typename _STD vector<value_type>;
		using initializer_type			= typename _STD initializer_list<value_type>;

		using allocator_type			= typename storage_type::allocator_type;
		using difference_type			= typename storage_type::difference_type;
		using size_type					= typename storage_type::size_type;
		using pointer					= typename storage_type::pointer;
		using reference					= typename storage_type::reference;
		using const_pointer				= typename storage_type::const_pointer;
		using const_reference			= typename storage_type::const_reference;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		dense_map() noexcept : m_storage{} {}

		~dense_map() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit dense_map(allocator_type const & al)
			: m_storage{ al }
		{
		}

		explicit dense_map(size_type const count)
			: m_storage{ count }
		{
		}

		explicit dense_map(size_type const count, allocator_type const & al)
			: m_storage{ count, al }
		{
		}

		explicit dense_map(size_type const count, const_reference value)
			: m_storage{ count, value, allocator_type{} }
		{
		}

		explicit dense_map(size_type const count, const_reference value, allocator_type const & al)
			: m_storage{ count, value, al }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		dense_map(initializer_type init)
			: m_storage{ init }
		{
		}

		dense_map(initializer_type init, _STD input_iterator_tag)
			: m_storage{ init, _STD input_iterator_tag }
		{
		}

		dense_map(initializer_type init, _STD forward_iterator_tag)
			: m_storage{ init, _STD forward_iterator_tag }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class It> dense_map(It first, It last)
			: m_storage{ first, last }
		{
		}

		template <class It> dense_map(It first, It last, _STD input_iterator_tag)
			: m_storage{ first, last, _STD input_iterator_tag }
		{
		}

		template <class It> dense_map(It first, It last, _STD forward_iterator_tag)
			: m_storage{ first, last, _STD forward_iterator_tag }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		dense_map(storage_type const & value)
			: m_storage{ value }
		{
		}

		dense_map(storage_type const & value, allocator_type const & al)
			: m_storage{ value, al }
		{
		}

		dense_map(storage_type && value) noexcept
			: m_storage{ _STD move(value) }
		{
		}

		dense_map(storage_type && value, allocator_type const & al) noexcept
			: m_storage{ _STD move(value), al }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		dense_map(self_type const & value)
			: m_storage{}
		{
			this->assign(value);
		}

		dense_map(self_type && value) noexcept
			: m_storage{}
		{
			this->swap(_STD move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline mapped_type & operator[](key_type const & key)
		{
			if (auto it{ this->find(key) }; it != this->end())
			{
				return it->second;
			}
			else
			{
				return this->insert(_STD make_pair(key, mapped_type{}))->second;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(storage_type const & value)
		{
			this->assign(value);
			return (*this);
		}

		inline self_type & operator=(self_type const & value)
		{
			this->assign(value);
			return (*this);
		}

		inline self_type & operator=(storage_type && value) noexcept
		{
			this->swap(_STD move(value));
			return (*this);
		}

		inline self_type & operator=(self_type && value) noexcept
		{
			this->swap(_STD move(value));
			return (*this);
		}

		inline self_type & operator=(initializer_type value)
		{
			this->assign(value);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void assign(size_type const count, mapped_type const & value)
		{
			m_storage.assign(count, value);
		}

		template <class It> inline void assign(It first, It last)
		{
			m_storage.assign(first, last);
		}

		inline void assign(initializer_type init)
		{
			m_storage.assign(init);
		}

		inline void assign(storage_type const & value)
		{
			if (_STD addressof(m_storage) != _STD addressof(value))
			{
				m_storage.assign(value.begin(), value.end());
			}
		}

		inline void assign(self_type const & value)
		{
			if (this != _STD addressof(value))
			{
				this->assign(value.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void swap(storage_type & value) noexcept
		{
			if (_STD addressof(m_storage) != _STD addressof(value))
			{
				m_storage.swap(value);
			}
		}

		inline void swap(self_type & value) noexcept
		{
			if (this != _STD addressof(value))
			{
				m_storage.swap(value.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Pr> inline iterator find_if(Pr pr) noexcept
		{
			return _STD find_if<iterator, Pr>(this->begin(), this->end(), pr);
		}

		template <class Pr> inline const_iterator find_if(Pr pr) const noexcept
		{
			return _STD find_if<const_iterator, Pr>(this->cbegin(), this->cend(), pr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(key_type const & value) noexcept
		{
			return this->find_if([value](auto && elem) { return elem.first == value; });
		}

		inline const_iterator find(key_type const & value) const noexcept
		{
			return this->find_if([value](auto && elem) { return elem.first == value; });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args>
		inline decltype(auto) emplace(const_iterator whereptr, Args && ... args)
		{
			return m_storage.emplace(whereptr, _STD forward<Args>(args)...);
		}

		template <class ... Args>
		inline decltype(auto) emplace_back(Args && ... args)
		{
			return m_storage.emplace_back(_STD forward<Args>(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator insert(value_type const & value)
		{
			if (find(value.first) == this->end())
			{
				m_storage.push_back(value);

				return (this->end() - 1);
			}
			return this->end();
		}

		inline iterator insert(value_type && value)
		{
			if (find(value.first) == this->end())
			{
				m_storage.push_back(_STD move(value));

				return (this->end() - 1);
			}
			return this->end();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto at(size_type const i) -> reference { return m_storage.at(i); }

		inline auto at(size_type const i) const -> const_reference { return m_storage.at(i); }

		inline auto back() noexcept -> reference { return m_storage.back(); }

		inline auto back() const noexcept -> const_reference { return m_storage.back(); }

		inline auto begin() noexcept -> iterator { return m_storage.begin(); }

		inline auto begin() const noexcept -> const_iterator { return m_storage.begin(); }

		inline auto capacity() const noexcept -> size_type { return m_storage.capacity(); }

		inline auto cbegin() const noexcept -> const_iterator { return m_storage.cbegin(); }

		inline auto cend() const noexcept -> const_iterator { return m_storage.cend(); }

		inline auto crbegin() const noexcept -> const_reverse_iterator { return m_storage.crbegin(); }

		inline auto crend() const noexcept -> const_reverse_iterator { return m_storage.crend(); }

		inline void clear() noexcept { return m_storage.clear(); }

		inline auto data() noexcept -> pointer { return m_storage.data(); }

		inline auto data() const noexcept -> const_pointer { return m_storage.data(); }

		inline bool empty() const noexcept { return m_storage.empty(); }

		inline auto end() noexcept -> iterator { return m_storage.end(); }

		inline auto end() const noexcept -> const_iterator { return m_storage.end(); }

		inline auto erase(const_iterator it) -> iterator { return m_storage.erase(it); }

		inline auto front() noexcept -> reference { return m_storage.front(); }

		inline auto front() const noexcept -> const_reference { return m_storage.front(); }

		inline auto get_allocator() const noexcept -> allocator_type { return m_storage.get_allocator(); }

		inline auto max_size() const noexcept -> size_type { return m_storage.max_size(); }

		inline void pop_back() noexcept { return m_storage.pop_back(); }
		
		inline auto rbegin() noexcept -> reverse_iterator { return m_storage.rbegin(); }
		
		inline auto rbegin() const noexcept -> const_reverse_iterator { return m_storage.rbegin(); }
		
		inline auto rend() noexcept -> reverse_iterator { return m_storage.rend(); }
		
		inline auto rend() const noexcept -> const_reverse_iterator { return m_storage.rend(); }

		inline void reserve(size_type const size) { return m_storage.reserve(size); }

		inline void resize(size_type const size) { return m_storage.resize(size); }

		inline auto size() const noexcept -> size_type { return m_storage.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		union { storage_type m_storage; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SMALL_MAP_HPP_