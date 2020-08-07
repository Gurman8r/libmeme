#ifndef _ML_CONTEXT_SETTINGS_HPP_
#define _ML_CONTEXT_SETTINGS_HPP_

#include <libmeme/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// context api
	enum context_client_ : int32_t
	{
		context_api_unknown		, // unknown
		context_api_opengl		, // opengl
		context_api_vulkan		, // vulkan
		context_api_directx		, // directx
	};

	inline void from_json(json const & j, context_client_ & v)
	{
		if (j.is_number())
		{
			v = (context_client_)j.get<int32_t>();
		}
		else if (j.is_string())
		{
			using namespace util;
			switch (auto const s{ j.get<pmr::string>() }; hash(to_lower(s)))
			{
			case hash("unknown"	): v = context_api_unknown	; break;
			case hash("opengl"	): v = context_api_opengl	; break;
			case hash("vulkan"	): v = context_api_vulkan	; break;
			case hash("directx"	): v = context_api_directx	; break;
			}
		}
	}

	inline void to_json(json & j, context_client_ const & v)
	{
		switch (v)
		{
		case context_api_unknown: j = "unknown"	; break;
		case context_api_opengl	: j = "opengl"	; break;
		case context_api_vulkan	: j = "vulkan"	; break;
		case context_api_directx: j = "directx"	; break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// context profile
	enum context_profile_ : int32_t
	{
		context_profile_any		, // any
		context_profile_core	, // core
		context_profile_compat	, // compat
		context_profile_debug	, // debug
	};

	inline void from_json(json const & j, context_profile_ & v)
	{
		if (j.is_number())
		{
			v = (context_profile_)j.get<int32_t>();
		}
		else if (j.is_string())
		{
			using namespace util;
			switch (auto const s{ j.get<pmr::string>() }; hash(to_lower(s)))
			{
			case hash("any"		): v = context_profile_any		; break;
			case hash("core"	): v = context_profile_core		; break;
			case hash("compat"	): v = context_profile_compat	; break;
			case hash("debug"	): v = context_profile_debug	; break;
			}
		}
	}

	inline void to_json(json & j, context_profile_ const & v)
	{
		switch (v)
		{
		case context_profile_any	: j = "any"		; break;
		case context_profile_core	: j = "core"	; break;
		case context_profile_compat	: j = "compat"	; break;
		case context_profile_debug	: j = "debug"	; break;
		}
	}

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

	inline void from_json(json const & j, context_settings & v)
	{
		j["client"		].get_to((context_client_ & )v.api);
		j["major"		].get_to(v.major);
		j["minor"		].get_to(v.minor);
		j["profile"		].get_to((context_profile_ &)v.profile);
		j["depth_bits"	].get_to(v.depth_bits);
		j["stencil_bits"].get_to(v.stencil_bits);
		j["multisample"	].get_to(v.multisample);
		j["srgb_capable"].get_to(v.srgb_capable);
	}

	inline void to_json(json & j, context_settings const & v)
	{
		j["client"		] = (context_client_)v.api;
		j["major"		] = v.major;
		j["minor"		] = v.minor;
		j["profile"		] = (context_profile_)v.profile;
		j["depth_bits"	] = v.depth_bits;
		j["stencil_bits"] = v.stencil_bits;
		j["multisample"	] = v.multisample;
		j["srgb_capable"] = v.srgb_capable;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTEXT_SETTINGS_HPP_