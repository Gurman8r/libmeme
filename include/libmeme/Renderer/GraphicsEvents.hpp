#ifndef _ML_GRAPHICS_EVENTS_HPP_
#define _ML_GRAPHICS_EVENTS_HPP_

#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Shader;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class GraphicsEventType
	{
		MIN_GRAPHICS_EVENT = Event::EV_RENDERER,

		EV_RenderError, // Render Error
		EV_ShaderError, // Shader Error

		MAX_GRAPHICS_EVENT
	};

	static_assert(
		(int32_t)GraphicsEventType::MAX_GRAPHICS_EVENT <
		(int32_t)GraphicsEventType::MIN_GRAPHICS_EVENT + Event::MAX_LIBRARY_EVENTS,
		"too many library event types specified in " __FILE__
	);

	template <GraphicsEventType ID> struct GraphicsEvent : public T_Event<GraphicsEventType, ID>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct RenderErrorEvent final : public GraphicsEvent<GraphicsEventType::EV_RenderError>
	{
		union
		{
			C_string const file;
			uint32_t const line;
			C_string const expr;
			uint32_t const code;
		};

		constexpr RenderErrorEvent(C_string file, uint32_t line, C_string expr, uint32_t code) noexcept
			: file { file }
			, line { line }
			, expr { expr }
			, code { code }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ShaderErrorEvent final : public GraphicsEvent<GraphicsEventType::EV_ShaderError>
	{
		union
		{
			struct Shader const * const shader;
			uint32_t const type;
			C_string const error;
		};

		constexpr ShaderErrorEvent(struct Shader const * shader, uint32_t type, C_string error) noexcept
			: shader{ shader }
			, type	{ type }
			, error { error } 
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GRAPHICS_EVENTS_HPP_