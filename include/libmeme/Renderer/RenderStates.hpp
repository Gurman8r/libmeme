#ifndef _ML_RENDER_STATES_HPP_
#define _ML_RENDER_STATES_HPP_

// this whole system needs another rework

#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API alpha_state final
	{
		bool enabled;

		GL::Predicate func;

		float_t coeff;

		explicit constexpr alpha_state(bool enabled, GL::Predicate func, float_t coeff) noexcept
			: enabled{ enabled }
			, func{ func }
			, coeff{ coeff }
		{
		}

		constexpr alpha_state(alpha_state const & other) noexcept
			: alpha_state{ other.enabled, other.func, other.coeff }
		{
		}

		constexpr alpha_state(bool enabled) noexcept
			: alpha_state{ enabled, GL::Greater, 0.001f }
		{
		}

		constexpr alpha_state() noexcept
			: alpha_state{ true }
		{
		}

		alpha_state const & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API blend_state final
	{
		bool enabled;

		GL::Factor sfactorRGB;

		GL::Factor sfactorAlpha;

		GL::Factor dfactorRGB;

		GL::Factor dfactorAlpha;

		constexpr explicit blend_state(bool enabled, GL::Factor sfactorRGB, GL::Factor sfactorAlpha, GL::Factor dfactorRGB, GL::Factor dfactorAlpha) noexcept
			: enabled{ enabled }
			, sfactorRGB{ sfactorRGB }
			, sfactorAlpha{ sfactorAlpha }
			, dfactorRGB{ dfactorRGB }
			, dfactorAlpha{ dfactorAlpha }
		{
		}

		constexpr blend_state(blend_state const & other) noexcept
			: blend_state{ other.enabled, other.sfactorRGB, other.sfactorAlpha, other.dfactorRGB, other.dfactorAlpha }
		{
		}

		constexpr blend_state(bool enabled, GL::Factor rgb, GL::Factor alpha) noexcept
			: blend_state{ enabled, rgb, alpha, rgb, alpha }
		{
		}

		constexpr blend_state(bool enabled) noexcept
			: blend_state{ enabled, GL::SrcAlpha, GL::OneMinusSrcAlpha }
		{
		}

		constexpr blend_state() noexcept
			: blend_state{ true }
		{
		}

		blend_state const & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API cull_state final
	{
		bool enabled;

		GL::Face mode;

		constexpr explicit cull_state(bool enabled, GL::Face mode) noexcept
			: enabled{ enabled }
			, mode{ mode }
		{
		}

		constexpr cull_state(cull_state const & other) noexcept
			: cull_state{ other.enabled, other.mode }
		{
		}

		constexpr cull_state(bool enabled) noexcept
			: cull_state{ enabled, GL::Back }
		{
		}

		constexpr cull_state() noexcept
			: cull_state{ true }
		{
		}

		cull_state const & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API depth_state final
	{
		bool enabled;

		GL::Predicate func;

		bool mask;

		constexpr explicit depth_state(bool enabled, GL::Predicate func, bool mask) noexcept
			: enabled{ enabled }
			, func{ func }
			, mask{ mask }
		{
		}

		constexpr depth_state(bool enabled, bool mask) noexcept
			: depth_state{ enabled, GL::Less, mask }
		{
		}

		constexpr depth_state(depth_state const & other) noexcept
			: depth_state{ other.enabled, other.func, other.mask }
		{
		}

		constexpr depth_state(bool enabled) noexcept
			: depth_state{ enabled, GL::Less, true }
		{
		}

		constexpr depth_state() noexcept
			: depth_state{ true }
		{
		}

		depth_state const & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API render_states final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr render_states(
			bool enabled,
			alpha_state const & alpha,
			blend_state const & blend,
			cull_state const & cull,
			depth_state const & depth
		) noexcept
			: m_enabled{ enabled }
			, m_alpha{ alpha }
			, m_blend{ blend }
			, m_cull{ cull }
			, m_depth{ depth }
		{
		}

		constexpr render_states(
			alpha_state const & alpha,
			blend_state const & blend,
			cull_state const & cull,
			depth_state const & depth
		) noexcept : render_states{ true, alpha, blend, cull, depth }
		{
		}

		constexpr render_states(render_states const & other) noexcept : render_states{
			other.m_enabled,
			other.m_alpha,
			other.m_blend,
			other.m_cull,
			other.m_depth
		}
		{
		}

		constexpr render_states(bool enabled) noexcept
			: render_states{ enabled, {}, {}, {}, {} }
		{
		}

		constexpr render_states() noexcept
			: render_states{ true }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		render_states const & operator()() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto alpha() -> alpha_state & { return m_alpha; }

		ML_NODISCARD constexpr auto alpha() const -> alpha_state const & { return m_alpha; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto blend() -> blend_state & { return m_blend; }

		ML_NODISCARD constexpr auto blend() const -> blend_state const & { return m_blend; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto cull() -> cull_state & { return m_cull; }

		ML_NODISCARD constexpr auto cull() const -> cull_state const & { return m_cull; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto depth() -> depth_state & { return m_depth; }

		ML_NODISCARD constexpr auto depth() const -> depth_state const & { return m_depth; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool		m_enabled;
		alpha_state	m_alpha;
		blend_state	m_blend;
		cull_state	m_cull;
		depth_state	m_depth;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_STATES_HPP_