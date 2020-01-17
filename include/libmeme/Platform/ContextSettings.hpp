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

		int32_t		api				{ Client_API::Unknown };
		int32_t		major			{ 0 };
		int32_t		minor			{ 0 };
		int32_t		profile			{ Client_API::Any };
		int32_t		depth_bits		{ 0 };
		int32_t		stencil_bits	{ 0 };
		bool		multisample		{ false };
		bool		sRGB_capable	{ false };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr ContextSettings() noexcept = default;
		constexpr ContextSettings(ContextSettings const &) = default;
		constexpr ContextSettings(ContextSettings &&) noexcept = default;
		constexpr ContextSettings & operator=(ContextSettings const &) = default;
		constexpr ContextSettings & operator=(ContextSettings &&) noexcept = default;

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