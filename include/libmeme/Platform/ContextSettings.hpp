#ifndef _ML_CONTEXT_SETTINGS_HPP_
#define _ML_CONTEXT_SETTINGS_HPP_

#include <libmeme/Core/Core.hpp>

namespace ml
{
	struct ContextSettings final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum API : uint32_t
		{
			Unknown, OpenGL, Vulkan, DirectX,
		};

		enum Profile : uint32_t
		{
			Any, Core, Compat, Debug,
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t	api{ API::Unknown };
		uint32_t	major{ 1 };
		uint32_t	minor{ 1 };
		uint32_t	profile{ Profile::Compat };
		uint32_t	depthBits{ 32 };
		uint32_t	stencilBits{ 8 };
		bool		multisample{ false };
		bool		srgbCapable{ false };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr ContextSettings() = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_CONTEXT_SETTINGS_HPP_