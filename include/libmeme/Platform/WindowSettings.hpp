#ifndef _ML_WINDOW_SETTINGS_HPP_
#define _ML_WINDOW_SETTINGS_HPP_

#include <libmeme/Platform/ContextSettings.hpp>
#include <libmeme/Platform/VideoMode.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD window_settings final
	{
		pmr::string			title	{};
		video_mode			video	{};
		context_settings	context	{};
		int32_t				hints	{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static void from_json(json const & j, window_settings & value)
	{
		j.at("title").get_to(value.title);
		j.at("video").get_to(value.video);
		j.at("context").get_to(value.context);
		j.at("hints").get_to(value.hints);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_SETTINGS_HPP_