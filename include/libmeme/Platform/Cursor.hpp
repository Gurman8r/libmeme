#ifndef _ML_CURSOR_HPP_
#define _ML_CURSOR_HPP_

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias cursor_handle = typename void *;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum cursor_mode_ : int32_t
	{
		cursor_mode_normal,
		cursor_mode_hidden,
		cursor_mode_disabled,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum cursor_shape_ : int32_t
	{
		cursor_shape_arrow,
		cursor_shape_ibeam,
		cursor_shape_crosshair,
		cursor_shape_hand,
		cursor_shape_ew,
		cursor_shape_ns,
		cursor_shape_nesw,
		cursor_shape_nwse,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CURSOR_HPP_