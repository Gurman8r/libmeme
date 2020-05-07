#ifndef _ML_CONTEXT_SETTINGS_HPP_
#define _ML_CONTEXT_SETTINGS_HPP_

#include <libmeme/Common.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class window_api final
	{
	public:
		enum { unknown, opengl, vulkan, directx };

		enum { any, core, compat, debug };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD context_settings final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t		api				{};
		int32_t		major			{};
		int32_t		minor			{};
		int32_t		profile			{};
		int32_t		depth_bits		{};
		int32_t		stencil_bits	{};
		bool		multisample		{};
		bool		srgb_capable	{};

		constexpr operator bool() const noexcept
		{
			return api && major && profile;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static void from_json(json const & j, context_settings & value)
	{
		j.at("api").get_to(value.api);
		j.at("major").get_to(value.major);
		j.at("minor").get_to(value.minor);
		j.at("profile").get_to(value.profile);
		j.at("depth_bits").get_to(value.depth_bits);
		j.at("stencil_bits").get_to(value.stencil_bits);
		j.at("multisample").get_to(value.multisample);
		j.at("srgb_capable").get_to(value.srgb_capable);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTEXT_SETTINGS_HPP_