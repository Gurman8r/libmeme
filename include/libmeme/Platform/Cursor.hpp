#ifndef _ML_CURSOR_HPP_
#define _ML_CURSOR_HPP_

#include <libmeme/Platform/Export.hpp>
#include <libmeme/Core/EnumInfo.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Cursor final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Cursor() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum class Mode : int32_t
		{
			Normal,
			Hidden,
			Disabled,
		};

		static constexpr auto const Modes{ enum_info<Mode>{
		{
			Mode::Normal,
			Mode::Hidden,
			Mode::Disabled
		},{
			"Normal",
			"Hidden",
			"Disabled",
		},{
			"The arrow cursor is used and motion is not limited",
			"Hide the cursor, but it behaves normally otherwise",
			"Hide the cursor and lock it to the window",
		} } };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum class Shape : int32_t
		{
			Arrow,
			TextInput,
			Crosshair,
			Hand,
			ResizeEW,
			ResizeNS,
			ResizeNESW,
			ResizeNWSE,
		};

		static constexpr auto const Shapes{ enum_info<Shape>{
		{
			Shape::Arrow,
			Shape::TextInput,
			Shape::Crosshair,
			Shape::Hand,
			Shape::ResizeEW,
			Shape::ResizeNS,
			Shape::ResizeNESW,
			Shape::ResizeNWSE,
		},{
			"Arrow",
			"Text Input",
			"Cross Hair",
			"Hand",
			"Resize EW",
			"Resize NS",
			"Resize NE/SW",
			"Resize NW/SE",
		},{
			"Regular arrow cursor shape",
			"Text input I-beam cursor shape",
			"Crosshair cursor shape",
			"Hand cursor shape",
			"Horizontal resize cursor shape",
			"Vertical resize cursor shape",
			"Bottom-left resize cursor shape",
			"Bottom-right resize cursor shape",
		} } };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CURSOR_HPP_