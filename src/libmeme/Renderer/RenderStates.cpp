#include <libmeme/Renderer/RenderStates.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	render_states const & render_states::operator()() const
	{
		if (m_enabled)
		{
			m_alpha();
			m_blend();
			m_cull();
			m_depth();
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	alpha_state const & alpha_state::operator()() const
	{
		if (!this->enabled)
		{
			GL::disable(GL::AlphaTest);
		}
		else if (GL::enable(GL::AlphaTest, this->enabled))
		{
			GL::alphaFunc(this->func, this->coeff);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	blend_state const & blend_state::operator()() const
	{
		if (!this->enabled)
		{
			GL::disable(GL::Blend);
		}
		else if (GL::enable(GL::Blend, this->enabled))
		{
			GL::blendFuncSeparate(
				this->sfactorRGB, this->sfactorAlpha,
				this->dfactorRGB, this->dfactorAlpha
			);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	cull_state const & cull_state::operator()() const
	{
		if (!this->enabled)
		{
			GL::disable(GL::CullFace);
		}
		else if (GL::enable(GL::CullFace, this->enabled))
		{
			GL::cullFace(this->mode);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	depth_state const & depth_state::operator()() const
	{
		if (!this->enabled)
		{
			GL::disable(GL::DepthTest);
		}
		else if (GL::enable(GL::DepthTest, this->enabled))
		{
			GL::depthFunc(this->func);

			GL::depthMask(this->mask);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}