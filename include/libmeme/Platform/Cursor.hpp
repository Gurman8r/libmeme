#ifndef _ML_CURSOR_HPP_
#define _ML_CURSOR_HPP_

namespace ml
{
	struct Cursor final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum class Mode
		{
			Normal,
			Hidden,
			Disabled,
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum class Shape
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_CURSOR_HPP_