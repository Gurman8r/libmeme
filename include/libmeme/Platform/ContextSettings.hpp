#ifndef _ML_CONTEXT_SETTINGS_HPP_
#define _ML_CONTEXT_SETTINGS_HPP_

#include <libmeme/Common.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct client_api final
	{
		enum API { Unknown, OpenGL, Vulkan, DirectX };

		enum Profile { Any, Core, Compat, Debug };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct context_settings final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t		api				{ client_api::Unknown };
		int32_t		major			{ 0 };
		int32_t		minor			{ 0 };
		int32_t		profile			{ client_api::Any };
		int32_t		depth_bits		{ 0 };
		int32_t		stencil_bits	{ 0 };
		bool		multisample		{ false };
		bool		sRGB_capable	{ false };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr context_settings() noexcept = default;
		constexpr context_settings(context_settings const &) = default;
		constexpr context_settings(context_settings &&) noexcept = default;
		constexpr context_settings & operator=(context_settings const &) = default;
		constexpr context_settings & operator=(context_settings &&) noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_context_settings(Args && ... args)
	{
		return context_settings{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTEXT_SETTINGS_HPP_