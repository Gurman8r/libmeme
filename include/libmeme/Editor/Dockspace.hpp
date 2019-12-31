#ifndef _ML_DOCKSPACE_HPP_
#define _ML_DOCKSPACE_HPP_

#include <libmeme/Editor/Export.hpp>
#include <libmeme/Core/Matrix.hpp>
#include <libmeme/Core/NonCopyable.hpp>

namespace ml
{
	struct ML_EDITOR_API Dockspace final : public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using nodes_t = typename std::vector<uint32_t>;
		
		enum NodeID : uint32_t
		{
			Root,
			Left, LeftUp, LeftDn,
			Right, RightUp, RightDn,
		};
		
		static constexpr auto dockspace_title{ "libmeme##editor##dockspace" };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		bool render();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t begin_builder(int32_t flags);
		uint32_t end_builder(uint32_t root);
		uint32_t dock_window(C_String name, uint32_t id);
		uint32_t split_node(uint32_t id, int32_t dir, float_t ratio, uint32_t * other);
		uint32_t split_node(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * other);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool good() const noexcept { return m_good; }

		inline bool open() const noexcept { return m_open; }

		inline auto nodes() const noexcept -> nodes_t const & { return m_nodes; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Editor;

		Dockspace();

		~Dockspace();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool		m_good;			//
		bool		m_open;			//
		nodes_t		m_nodes;		//
		float_t		m_border;		//
		vec2		m_padding;		//
		float_t		m_rounding;		//
		vec2		m_size;			//
		float_t		m_transparency;	//

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_DOCKSPACE_HPP_