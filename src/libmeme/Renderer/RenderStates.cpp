#include <libmeme/Renderer/RenderStates.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderStates const & RenderStates::operator()() const
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

	AlphaState const & AlphaState::operator()() const
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

	BlendState const & BlendState::operator()() const
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

	CullState const & CullState::operator()() const
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

	DepthState const & DepthState::operator()() const
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