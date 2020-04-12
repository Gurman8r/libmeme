#ifndef _ML_CURSOR_HPP_
#define _ML_CURSOR_HPP_

namespace ml
{
	class cursor final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum class mode
		{
			normal,
			hidden,
			disabled,
		};

		enum class shape
		{
			arrow,
			text_input,
			crosshair,
			hand,
			resize_ew,
			resize_ns,
			resize_nesw,
			resize_nwse,
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_CURSOR_HPP_