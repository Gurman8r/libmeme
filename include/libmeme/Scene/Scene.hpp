#ifndef _ML_SCENE_HPP_
#define _ML_SCENE_HPP_

#include <libmeme/Scene/Entity.hpp>

namespace ml
{
	struct ML_SCENE_API scene final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		scene(allocator_type alloc = {}) noexcept : m_reg{}
		{
		}

		explicit scene(scene && other, allocator_type alloc = {}) noexcept : scene{ alloc }
		{
			this->swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		scene & operator=(scene && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		void swap(scene & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_reg, other.m_reg);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		entity create(pmr::string const & name) noexcept
		{
			return { this, m_reg.create(), name };
		}

		void destroy(entity const & value) noexcept
		{
			m_reg.destroy(value.get_handle());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_registry() & noexcept -> entt::registry & { return m_reg; }

		ML_NODISCARD auto operator->() noexcept { return std::addressof(m_reg); }

		ML_NODISCARD auto operator->() const noexcept { return std::addressof(m_reg); }

		ML_NODISCARD auto operator *() noexcept -> entt::registry & { return m_reg; }
		
		ML_NODISCARD auto operator *() const noexcept -> entt::registry const & { return m_reg; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		entt::registry m_reg;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SCENE_HPP_