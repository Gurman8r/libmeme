#ifndef _ML_RENDER_STATES_HPP_
#define _ML_RENDER_STATES_HPP_

#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API AlphaState final
	{
		bool enabled;

		GL::Predicate func;

		float_t coeff;

		explicit constexpr AlphaState(bool enabled, GL::Predicate func, float_t coeff) noexcept
			: enabled{ enabled }
			, func{ func }
			, coeff{ coeff }
		{
		}

		constexpr AlphaState(AlphaState const & other) noexcept
			: AlphaState{ other.enabled, other.func, other.coeff }
		{
		}

		constexpr AlphaState(bool enabled) noexcept
			: AlphaState{ enabled, GL::Greater, 0.001f }
		{
		}

		constexpr AlphaState() noexcept
			: AlphaState{ true }
		{
		}

		AlphaState const & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API BlendState final
	{
		bool enabled;

		GL::Factor sfactorRGB;

		GL::Factor sfactorAlpha;

		GL::Factor dfactorRGB;

		GL::Factor dfactorAlpha;

		constexpr explicit BlendState(bool enabled, GL::Factor sfactorRGB, GL::Factor sfactorAlpha, GL::Factor dfactorRGB, GL::Factor dfactorAlpha) noexcept
			: enabled{ enabled }
			, sfactorRGB{ sfactorRGB }
			, sfactorAlpha{ sfactorAlpha }
			, dfactorRGB{ dfactorRGB }
			, dfactorAlpha{ dfactorAlpha }
		{
		}

		constexpr BlendState(BlendState const & other) noexcept
			: BlendState{ other.enabled, other.sfactorRGB, other.sfactorAlpha, other.dfactorRGB, other.dfactorAlpha }
		{
		}

		constexpr BlendState(bool enabled, GL::Factor rgb, GL::Factor alpha) noexcept
			: BlendState{ enabled, rgb, alpha, rgb, alpha }
		{
		}

		constexpr BlendState(bool enabled) noexcept
			: BlendState{ enabled, GL::SrcAlpha, GL::OneMinusSrcAlpha }
		{
		}

		constexpr BlendState() noexcept
			: BlendState{ true }
		{
		}

		BlendState const & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API CullState final
	{
		bool enabled;

		GL::Face mode;

		constexpr explicit CullState(bool enabled, GL::Face mode) noexcept
			: enabled{ enabled }
			, mode{ mode }
		{
		}

		constexpr CullState(CullState const & other) noexcept
			: CullState{ other.enabled, other.mode }
		{
		}

		constexpr CullState(bool enabled) noexcept
			: CullState{ enabled, GL::Back }
		{
		}

		constexpr CullState() noexcept
			: CullState{ true }
		{
		}

		CullState const & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API DepthState final
	{
		bool enabled;

		GL::Predicate func;

		bool mask;

		constexpr explicit DepthState(bool enabled, GL::Predicate func, bool mask) noexcept
			: enabled{ enabled }
			, func{ func }
			, mask{ mask }
		{
		}

		constexpr DepthState(bool enabled, bool mask) noexcept
			: DepthState{ enabled, GL::Less, mask }
		{
		}

		constexpr DepthState(DepthState const & other) noexcept
			: DepthState{ other.enabled, other.func, other.mask }
		{
		}

		constexpr DepthState(bool enabled) noexcept
			: DepthState{ enabled, GL::Less, true }
		{
		}

		constexpr DepthState() noexcept
			: DepthState{ true }
		{
		}

		DepthState const & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API RenderStates final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr RenderStates(
			bool enabled,
			AlphaState const & alpha,
			BlendState const & blend,
			CullState const & cull,
			DepthState const & depth
		) noexcept
			: m_enabled{ enabled }
			, m_alpha{ alpha }
			, m_blend{ blend }
			, m_cull{ cull }
			, m_depth{ depth }
		{
		}

		constexpr RenderStates(
			AlphaState const & alpha,
			BlendState const & blend,
			CullState const & cull,
			DepthState const & depth
		) noexcept : RenderStates{ true, alpha, blend, cull, depth }
		{
		}

		constexpr RenderStates(RenderStates const & other) noexcept : RenderStates{
			other.m_enabled,
			other.m_alpha,
			other.m_blend,
			other.m_cull,
			other.m_depth
		}
		{
		}

		constexpr RenderStates(bool enabled) noexcept
			: RenderStates{ enabled, {}, {}, {}, {} }
		{
		}

		constexpr RenderStates() noexcept
			: RenderStates{ true }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderStates const & operator()() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto alpha()			-> AlphaState & { return m_alpha; }
		constexpr auto alpha()	const	-> AlphaState const & { return m_alpha; }

		constexpr auto blend()			-> BlendState & { return m_blend; }
		constexpr auto blend()	const	-> BlendState const & { return m_blend; }

		constexpr auto cull()			-> CullState & { return m_cull; }
		constexpr auto cull()	const	-> CullState const & { return m_cull; }

		constexpr auto depth()			-> DepthState & { return m_depth; }
		constexpr auto depth()	const	-> DepthState const & { return m_depth; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool		m_enabled;
		AlphaState	m_alpha;
		BlendState	m_blend;
		CullState	m_cull;
		DepthState	m_depth;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_STATES_HPP_