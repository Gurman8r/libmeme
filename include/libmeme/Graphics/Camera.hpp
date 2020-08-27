#ifndef _ML_CAMERA_HPP_
#define _ML_CAMERA_HPP_

#include <libmeme/Graphics/Export.hpp>
#include <libmeme/Core/Matrix.hpp>
#include <libmeme/Core/Memory.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

namespace ml::testing
{
	mat4 frustum(float_t l, float_t r, float_t b, float_t t, float_t near, float_t far)
	{
		vec4 const temp{ 2.0f * near, r - l, t - b, far - near };
		return
		{
			temp[0] / temp[1],
			0.f,
			0.f,
			0.f,
			0.f,
			temp[0] / temp[2],
			0.f,
			0.f,
			(r + l) / temp[1],
			(t + b) / temp[2],
			(-far - near) / temp[3],
			-1.0f,
			0.f,
			0.f,
			(-temp[0] * far) / temp[3],
			0.f
		};
	}

	mat4 perspective(float_t fovyInDegrees, float_t aspectRatio, float_t znear, float_t zfar)
	{
		float_t ymax = znear * std::tanf(fovyInDegrees * 3.141592f / 180.0f);
		float_t xmax = ymax * aspectRatio;
		return frustum(-xmax, xmax, -ymax, ymax, znear, zfar);
	}

	vec3 cross(vec3 const & a, vec3 const & b)
	{
		return
		{
			a[1] * b[2] - a[2] * b[1],
			a[2] * b[0] - a[0] * b[2],
			a[0] * b[1] - a[1] * b[0]
		};
	}

	float_t dot(vec3 const & a, vec3 const & b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}

	vec3 normalize(vec3 const & a)
	{
		float_t const il{ 1.f / (std::sqrtf(dot(a, a)) + FLT_EPSILON) };
		return
		{
			a[0] * il,
			a[1] * il,
			a[2] * il
		};
	}

	mat4 look_at(vec3 const & eye, vec3 const & at, vec3 const & up)
	{
		vec3 X, Y, Z, temp{ eye[0] - at[0], eye[1] - at[1], eye[2] - at[2] };

		Z = normalize(temp);
		Y = normalize(up);

		temp = cross(Y, Z);
		X = normalize(temp);

		temp = cross(Z, X);
		Y = normalize(temp);

		return
		{
			X[0],
			Y[0],
			Z[0],
			0.f,
			X[1],
			Y[1],
			Z[1],
			0.f,
			X[2],
			Y[2],
			Z[2],
			0.f,
			-dot(X, eye),
			-dot(Y, eye),
			-dot(Z, eye),
			1.0f,
		};
	}

	mat4 orthographic(float_t l, float_t r, float_t b, float_t t, float_t zn, float_t zf)
	{
		return
		{
			2 / (r - l),
			0.f,
			0.f,
			0.f,
			0.f,
			2.f / (t - b),
			0.f,
			0.f,
			0.f,
			0.f,
			1.0f / (zf - zn),
			0.f,
			(l + r) / (l - r),
			(t + b) / (b - t),
			zn / (zn - zf),
			1.0f
		};
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// perspective
namespace ml
{
	struct camera : trackable
	{
	};

	struct perspective_camera final : camera
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

		perspective_camera(perspective_camera const & other)
			: m_position{ other.m_position	}
			, m_forward	{ other.m_forward	}
			, m_up		{ other.m_up		}
			, m_right	{ other.m_right		}
			, m_world_up{ other.m_world_up	}
			, m_pitch	{ other.m_pitch		}
			, m_yaw		{ other.m_yaw		}
			, m_zoom	{ other.m_zoom		}
		{
		}

		perspective_camera(perspective_camera && other) noexcept
			: perspective_camera{}
		{
			swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		perspective_camera & operator=(perspective_camera const & other)
		{
			perspective_camera temp{ other };
			swap(temp);
			return (*this);
		}

		perspective_camera & operator=(perspective_camera && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

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

#endif // !_ML_CAMERA_HPP_