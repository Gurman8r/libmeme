#ifndef _ML_CONTEXT_SETTINGS_HPP_
#define _ML_CONTEXT_SETTINGS_HPP_

#include <libmeme/Common.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Client_API final
	{
		enum API { Unknown, OpenGL, Vulkan, DirectX };

		enum Profile { Any, Core, Compat, Debug };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ContextSettings final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t		api			{ Client_API::Unknown };
		int32_t		major		{ 0 };
		int32_t		minor		{ 0 };
		int32_t		profile		{ Client_API::Any };
		int32_t		depthBits	{ 0 };
		int32_t		stencilBits	{ 0 };
		bool		multisample	{ false };
		bool		sRGBCapable	{ false };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr ContextSettings() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_context_settings(Args && ... args)
	{
		return ContextSettings{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTEXT_SETTINGS_HPP_