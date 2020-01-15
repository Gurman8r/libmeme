#ifndef _ML_CONTEXT_SETTINGS_HPP_
#define _ML_CONTEXT_SETTINGS_HPP_

#include <libmeme/Core/Core.hpp>

namespace ml
{
	struct ContextSettings final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Client_API : uint32_t { Unknown, OpenGL, Vulkan, DirectX };

		enum Profile : uint32_t { Any, Core, Compat, Debug };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Client_API	client_api	{ Unknown };
		uint32_t	major		{ 1 };
		uint32_t	minor		{ 1 };
		Profile		profile		{ Any };
		uint32_t	depthBits	{ 32 };
		uint32_t	stencilBits	{ 8 };
		bool		multisample	{ false };
		bool		srgbCapable	{ false };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr explicit ContextSettings(
			uint32_t client_api, uint32_t major, uint32_t minor,
			uint32_t profile, uint32_t depthBits, uint32_t stencilBits,
			bool multisample, bool srgbCapable
		) noexcept
			: client_api{ (Client_API)client_api }, major{ major }, minor{ minor }
			, profile{ (Profile)profile }, depthBits{ depthBits }, stencilBits{ stencilBits }
			, multisample{ multisample }, srgbCapable{ srgbCapable }
		{
		}

		constexpr ContextSettings() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD constexpr auto make_context_settings(
		uint32_t client_api, uint32_t major, uint32_t minor,
		uint32_t profile, uint32_t depthBits, uint32_t stencilBits,
		bool multisample, bool srgbCapable
	) noexcept
	{
		return ContextSettings{
			client_api, major, minor,
			profile, depthBits, stencilBits,
			multisample, srgbCapable
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTEXT_SETTINGS_HPP_