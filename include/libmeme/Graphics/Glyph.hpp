#ifndef _ML_GLYPH_HPP_
#define _ML_GLYPH_HPP_

#include <libmeme/Graphics/RenderAPI.hpp>

namespace ml
{
	struct ML_GRAPHICS_API glyph final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shared<gfx::texture2d> graphic{};

		float_rect bounds{};
		
		uint32_t advance{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		operator bool() const noexcept { return (bool)graphic; }

		auto bearing() const noexcept -> vec2 { return bounds.position(); }
		
		auto size() const noexcept -> vec2 { return bounds.size(); }
		
		auto left() const noexcept -> float_t { return bearing()[0]; }
		
		auto top() const noexcept -> float_t { return bearing()[1]; }
		
		auto width() const noexcept -> float_t { return size()[0]; }
		
		auto height() const noexcept -> float_t { return size()[1]; }
		
		auto offset() const noexcept -> vec2 { return { left(), -top() }; }
		
		auto step() const noexcept -> float_t { return (float_t)(advance >> 6); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_GLYPH_HPP_