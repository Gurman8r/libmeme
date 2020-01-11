#ifndef _ML_CX_HPP_
#define _ML_CX_HPP_

// Source:
// https://github.com/lefticus/constexpr_all_the_things

#include <libmeme/Core/Array.hpp>
#include <libmeme/Core/TypeOf.hpp>

#define _ML_CX _ML cx::

namespace ml::cx
{
	// STATIC STRING
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct static_string
	{
		using self_type = typename static_string;
		using elem_type = typename char;
		using data_type = typename C_String;
		using size_type = typename size_t;

		constexpr static_string() noexcept
			: self_type{ "" }
		{
		}

		template <size_t N>
		constexpr static_string(const elem_type(&data)[N]) noexcept
			: self_type{ data, N - 1 }
		{
		}

		constexpr static_string(data_type data, size_type size) noexcept
			: m_data{ data }
			, m_size{ size }
		{
		}

		constexpr data_type data() const noexcept { return m_data; }

		constexpr size_type size() const noexcept { return m_size; }

	private:
		data_type m_data{ "" };
		size_type m_size{ 0 };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::cx
{
	// VECTOR
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class _Val, size_t _Cap = 8
	> struct vector
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename _Val;
		using self_type			= typename vector<value_type, _Cap>;
		using base_type			= typename Array<value_type, _Cap>;
		using size_type			= typename base_type::size_type;
		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;
		using iterator			= typename base_type::iterator;
		using const_iterator	= typename base_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr vector(const_iterator first, const_iterator last)
		{
			while (first != last)
			{
				this->push_back(*first);
				++first;
			}
		}

		constexpr vector(std::initializer_list<value_type> init)
			: self_type{ init.begin(), init.end() }
		{
		}

		constexpr vector() = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t N, std::enable_if_t<(_Cap != N), int> = 0>
		constexpr operator vector<value_type, N>() const
		{
			vector<value_type, N> temp{};
			for (size_t i = 0, imax = (m_size < N ? m_size : N); i < imax; i++)
			{
				temp.push_back(this->at(i));
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto back()			-> reference		{ return *(m_data.begin() + m_size); }
		constexpr auto back()	const	-> const_reference	{ return *(m_data.begin() + m_size); }
		constexpr auto begin()			-> iterator			{ return m_data.begin(); }
		constexpr auto begin()	const	-> const_iterator	{ return m_data.begin(); }
		constexpr auto cbegin()	const	-> const_iterator	{ return m_data.cbegin(); }
		constexpr auto cend()	const	-> const_iterator	{ return m_data.cend(); }
		constexpr auto data()			-> pointer			{ return m_data.data(); }
		constexpr auto data()	const	-> const_pointer	{ return m_data.data(); }
		constexpr auto empty()	const	-> bool				{ return m_data.empty(); }
		constexpr auto end()			-> iterator			{ return m_data.end(); }
		constexpr auto end()	const	-> const_iterator	{ return m_data.end(); }
		constexpr auto front()			-> reference		{ return m_data.front(); }
		constexpr auto front()	const	-> const_reference	{ return m_data.front(); }
		constexpr auto hash()	const	-> hash_t			{ return m_data.hash(); }
		constexpr auto size()	const	-> size_t			{ return m_size; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr reference at(size_t index)
		{
			if (index >= m_size)
			{
				ML_THROW std::range_error("Index past end of vector");
			}
			return m_data.at(index);
		}
		
		constexpr const_reference at(size_t index) const
		{
			if (index >= m_size)
			{
				ML_THROW std::range_error("Index past end of vector");
			}
			return m_data.at(index);
		}

		constexpr void push_back(_Val value)
		{
			if (m_size >= _Cap)
			{
				ML_THROW std::range_error("Index out of range");
			}
			else
			{
				m_data[m_size++] = std::move(value);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fun>
		constexpr const_iterator find_if(Fun && fun) const
		{
			return _ML_ALG find_if(cbegin(), cend(), fun);
		}

		constexpr const_iterator find(value_type const & value) const
		{
			return this->find_if([&](auto && elem)
			{
				return (elem == value);
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr reference operator[](size_t i) { return at(i); }

		constexpr const_reference operator[](size_t i) const { return at(i); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr bool operator==(self_type const & other) const
		{
			return !((*this) < other) && !(other < (*this));
		}

		constexpr bool operator!=(self_type const & other) const
		{
			return !((*this) == other);
		}

		constexpr bool operator<(self_type const & other) const
		{
			return _ML_ALG less(this->cbegin(), this->cend(), other.cbegin(), other.cend());
		}

		constexpr bool operator>(self_type const & other) const
		{
			return !((*this) < other);
		}

		constexpr bool operator<=(self_type const & other) const
		{
			return ((*this) == other) || ((*this) < other);
		}

		constexpr bool operator>=(self_type const & other) const
		{
			return ((*this) == other) || ((*this) > other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		base_type m_data{};
		size_t m_size{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::cx
{
	// PAIR
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _First, class _Second> struct pair
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using first_type = typename _First;
		using second_type = typename _Second;
		using self_type = typename _ML_CX pair<first_type, second_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		first_type	first;
		second_type	second;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr pair() noexcept
			: self_type{ first_type{}, second_type{} }
		{
		}

		constexpr pair(self_type const & copy) noexcept
			: self_type{ copy.first, copy.second }
		{
		}

		constexpr pair(first_type first, second_type second) noexcept
			: first{ first }
			, second{ second }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	template <class First, class Second>
	static constexpr auto make_pair(First first, Second second) noexcept
	{
		return pair<First, Second>{ first, second };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::cx
{
	// MAP
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class _Key, class _Val, size_t _Cap = 8
	> struct map
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_type			= typename _Key;
		using value_type		= typename _Val;
		using self_type			= typename map<key_type, value_type, _Cap>;
		using pair_type			= typename _ML_CX pair<key_type, value_type>;
		using base_type			= typename _ML_CX vector<pair_type, _Cap>;
		using size_type			= typename base_type::size_type;
		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;
		using iterator			= typename base_type::iterator;
		using const_iterator	= typename base_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Iter>
		constexpr map(Iter first, Iter last) noexcept
			: m_data{ first, last }
		{
		}

		constexpr map(std::initializer_list<pair_type> init) noexcept
			: self_type{ init.begin(), init.end() }
		{
		}

		constexpr map() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto back()			-> reference		{ return m_data.back(); }
		constexpr auto back()	const	-> const_reference	{ return m_data.back(); }
		constexpr auto begin()			-> iterator			{ return m_data.begin(); }
		constexpr auto begin()	const	-> const_iterator	{ return m_data.begin(); }
		constexpr auto cbegin()	const	-> const_iterator	{ return m_data.cbegin(); }
		constexpr auto cend()	const	-> const_iterator	{ return m_data.cend(); }
		constexpr auto data()			-> pointer			{ return m_data.data(); }
		constexpr auto data()	const	-> const_pointer	{ return m_data.data(); }
		constexpr auto empty()	const	-> bool				{ return m_data.empty(); }
		constexpr auto end()			-> iterator			{ return m_data.end(); }
		constexpr auto end()	const	-> const_iterator	{ return m_data.end(); }
		constexpr auto front()			-> reference		{ return m_data.front(); }
		constexpr auto front()	const	-> const_reference	{ return m_data.front(); }
		constexpr auto hash()	const	-> hash_t			{ return m_data.hash(); }
		constexpr auto size()	const	-> size_t			{ return m_data.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr void push_back(pair_type const & value)
		{
			return m_data.push_back(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fun>
		constexpr const_iterator find_if(Fun && fun) const
		{
			return _ML_ALG find_if(this->cbegin(), this->cend(), fun);
		}

		constexpr const_iterator find_by_key(key_type const & key) const
		{
			return this->find_if([&](auto && elem)
			{
				return (elem.first == key);
			});
		}

		constexpr const_iterator find_by_value(value_type const & value) const
		{
			return this->find_if([&](auto && elem)
			{
				return (elem.second == value);
			});
		}

		constexpr const_iterator find(key_type const & key) const
		{
			return this->find_by_key(key);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		base_type m_data{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CX_HPP_