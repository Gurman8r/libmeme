#ifndef _ML_CAMERA_HPP_
#define _ML_CAMERA_HPP_

#include <libmeme/Graphics/Export.hpp>
#include <libmeme/Core/Matrix.hpp>
#include <libmeme/System/Memory.hpp>

// perspective
namespace ml
{
	struct perspective_camera final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		perspective_camera() noexcept
			: m_position{ 0, 0, 3 }
			, m_forward	{ 0, 0, -1 }
			, m_up		{ 0, 1, 0 }
			, m_right	{ 1, 0, 0 }
			, m_world_up{ 0, 1, 0 }
			, m_pitch	{ 0.f }
			, m_yaw		{ -90.f }
			, m_zoom	{ 45.f }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void swap(perspective_camera & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_position	, other.m_position	);
				std::swap(m_forward		, other.m_forward	);
				std::swap(m_up			, other.m_up		);
				std::swap(m_right		, other.m_right		);
				std::swap(m_world_up	, other.m_world_up	);
				std::swap(m_pitch		, other.m_pitch		);
				std::swap(m_yaw			, other.m_yaw		);
				std::swap(m_zoom		, other.m_zoom		);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		vec3	m_position	;
		vec3	m_forward	;
		vec3	m_up		;
		vec3	m_right		;
		vec3	m_world_up	;
		float_t m_pitch		;
		float_t m_yaw		;
		float_t m_zoom		;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}


// orthographic
namespace ml
{
	struct orthographic_camera final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void swap(orthographic_camera & other) noexcept
		{
			if (this != std::addressof(other))
			{
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_CAMERA_HPP_