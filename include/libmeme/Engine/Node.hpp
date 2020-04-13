#ifndef _ML_NODE_HPP_
#define _ML_NODE_HPP_

// WIP

#include <libmeme/Core/Memory.hpp>

namespace ml
{
	struct node final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		node() noexcept : node{ allocator_type{} }
		{
		}

		explicit node(allocator_type const & alloc) noexcept
			: m_children{ alloc }
		{
		}

		node(node const & value, allocator_type const & alloc = {})
			: node{ alloc }
		{
		}

		node(node && value, allocator_type const & alloc = {}) noexcept
			: node{ alloc }
		{
			swap(std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		node & operator=(node const & value)
		{
			node temp{ value };
			swap(temp);
			return (*this);
		}

		node & operator=(node && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void swap(node & value) noexcept
		{
			if (this != std::addressof(value))
			{
				m_children.swap(value.m_children);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto children() & noexcept -> pmr::vector<node> & { return m_children; }

		auto children() const & noexcept -> pmr::vector<node> const & { return m_children; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		pmr::vector<node> m_children;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_NODE_HPP_