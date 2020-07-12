#ifndef _ML_CONTEXT_SETTINGS_HPP_
#define _ML_CONTEXT_SETTINGS_HPP_

#include <libmeme/Common.hpp>
#include <libmeme/Core/JSON.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// context api
	enum context_api_ : int32_t
	{
		context_api_unknown		, // unknown
		context_api_opengl		, // opengl
		context_api_vulkan		, // vulkan
		context_api_directx		, // directx
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// context profile
	enum context_profile_ : int32_t
	{
		context_profile_any		, // any
		context_profile_core	, // core
		context_profile_compat	, // compat
		context_profile_debug	, // debug
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// context settings
	struct ML_NODISCARD context_settings final
	{
		int32_t		api				{ context_api_unknown };
		int32_t		major			{};
		int32_t		minor			{};
		int32_t		profile			{ context_profile_any };
		int32_t		depth_bits		{};
		int32_t		stencil_bits	{};
		bool		multisample		{};
		bool		srgb_capable	{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline void from_json(json const & j, context_settings & value)
	{
		j["api"			].get_to(value.api);
		j["major"		].get_to(value.major);
		j["minor"		].get_to(value.minor);
		j["profile"		].get_to(value.profile);
		j["depth_bits"	].get_to(value.depth_bits);
		j["stencil_bits"].get_to(value.stencil_bits);
		j["multisample"	].get_to(value.multisample);
		j["srgb_capable"].get_to(value.srgb_capable);
	}

	inline void to_json(json & j, context_settings const & value)
	{
		j["api"			] = value.api;
		j["major"		] = value.major;
		j["minor"		] = value.minor;
		j["profile"		] = value.profile;
		j["depth_bits"	] = value.depth_bits;
		j["stencil_bits"] = value.stencil_bits;
		j["multisample"	] = value.multisample;
		j["srgb_capable"] = value.srgb_capable;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTEXT_SETTINGS_HPP_