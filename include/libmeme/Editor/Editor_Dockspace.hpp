#ifndef _ML_DOCKSPACE_HPP_
#define _ML_DOCKSPACE_HPP_

#include <libmeme/Editor/Export.hpp>
#include <libmeme/Core/Matrix.hpp>
#include <libmeme/Core/NonCopyable.hpp>

namespace ml
{
	// built-in dockspace
	struct ML_EDITOR_API editor_dockspace final : non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using nodes_t = typename pmr::vector<uint32_t>;
		
		static constexpr auto title{ "##libmeme##editor##dockspace" };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		bool render();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t begin_builder(int32_t flags);
		uint32_t end_builder(uint32_t root);
		uint32_t dock_window(cstring name, uint32_t id);
		uint32_t split_node(uint32_t index, uint32_t id, int32_t dir, float_t ratio, uint32_t * other);
		uint32_t split_node(uint32_t id, int32_t dir, float_t ratio, uint32_t * other);
		uint32_t split_node(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * other);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool good() const noexcept { return m_good; }

		inline bool open() const noexcept { return m_open; }

		inline nodes_t & nodes() noexcept { return m_nodes; }

		inline nodes_t const & nodes() const noexcept { return m_nodes; }

		inline uint32_t & operator[](const int32_t i) noexcept { return m_nodes[i]; }

		inline uint32_t const & operator[](const int32_t i) const noexcept { return m_nodes[i]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline editor_dockspace & resize(size_t const cap)
		{
			m_nodes.resize(cap);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend class editor;

		bool		m_open			{ true }	; // 
		bool		m_good			{ false }	; // 
		nodes_t		m_nodes			{}			; // 
		float_t		m_border		{ 0.f }		; // 
		vec2		m_padding		{ 0.f }		; // 
		float_t		m_rounding		{ 0.f }		; // 
		vec2		m_size			{ 0.f }		; // 
		float_t		m_transparency	{ 1.f }		; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_DOCKSPACE_HPP_