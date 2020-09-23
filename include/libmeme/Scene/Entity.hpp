#ifndef _ML_ENTITY_HPP_
#define _ML_ENTITY_HPP_

#include <libmeme/Scene/Export.hpp>
#include <libmeme/Core/Memory.hpp>
#include <entt/entt.hpp>

namespace ml
{
	struct scene;

	struct entity final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		entity(scene * scene, entt::entity handle, pmr::string const & name, allocator_type alloc = {}) noexcept
			: m_scene	{ scene }
			, m_handle	{ handle }
			, m_name	{ name, alloc }
		{
		}

		entity(scene * scene, entt::entity handle, allocator_type alloc = {}) noexcept
			: m_scene	{ scene }
			, m_handle	{ handle }
			, m_name	{ alloc }
		{
		}

		entity(entity const & other, allocator_type alloc = {})
			: m_handle	{ other.m_handle }
			, m_name	{ other.m_name, alloc }
			, m_scene	{ other.m_scene }
		{
		}

		entity(entity && other, allocator_type alloc = {}) noexcept
			: m_handle	{}
			, m_name	{ alloc }
			, m_scene	{}
		{
			this->swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		entity & operator=(entity const & other)
		{
			entity temp{ other };
			this->swap(temp);
			return (*this);
		}

		entity & operator=(entity && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		void swap(entity & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_handle, other.m_handle);
				std::swap(m_scene, other.m_scene);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Component, class ... Args
		> decltype(auto) add(Args && ... args) noexcept
		{
			return (*ML_check(m_scene))->emplace<Component>(m_handle, ML_forward(args)...);
		}

		template <class ... Component
		> ML_NODISCARD decltype(auto) get() noexcept
		{
			return (*ML_check(m_scene))->get<Component...>(m_handle);
		}

		template <class ... Component
		> ML_NODISCARD bool has() const noexcept
		{
			return (*ML_check(m_scene))->has<Component...>(m_handle);
		}

		template <class ... Component
		> void remove() noexcept
		{
			(*ML_check(m_scene))->remove<Component...>(m_handle);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD operator entt::entity() const noexcept { return m_handle; }

		ML_NODISCARD auto get_handle() const noexcept -> entt::entity { return m_handle; }

		ML_NODISCARD auto get_name() const noexcept -> pmr::string const & { return m_name; }
		
		ML_NODISCARD auto get_scene() const noexcept -> scene * { return m_scene; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		entt::entity	m_handle	; // 
		pmr::string		m_name		; // 
		scene *			m_scene		; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ENTITY_HPP_