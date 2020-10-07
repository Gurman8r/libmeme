#ifndef _ML_NODE_HPP_
#define _ML_NODE_HPP_

// WIP

#include <libmeme/Scene/Export.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	template <class T
	> struct node
	{
		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using value_type		= typename T;
		using self_type			= typename node<value_type>;

		node(allocator_type alloc = {}) noexcept
			: m_parent	{}
			, m_children{ alloc }
			, m_value	{}
		{
		}

		node(self_type * parent, allocator_type alloc = {})
			: m_parent	{ parent }
			, m_children{ alloc }
			, m_value	{}
		{
		}

		node(self_type const & other, allocator_type alloc = {})
			: m_parent	{ other.m_parent }
			, m_children{ other.m_children, alloc }
			, m_value	{ other.m_value }
		{
		}

		node(self_type && other, allocator_type alloc = {}) noexcept
			: m_parent	{}
			, m_children{ alloc }
			, m_value	{}
		{
			this->swap(std::move(other));
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

		void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_parent	, other.m_parent);
				std::swap(m_children, other.m_children);
				std::swap(m_value	, other.m_value);
			}
		}

		value_type * get_value()
		{
			return *m_value;
		}

	private:
		self_type *					m_parent	; // 
		pmr::vector<self_type *>	m_children	; // 
		std::optional<value_type>	m_value		; // 
	};
}

#endif // !_ML_NODE_HPP_