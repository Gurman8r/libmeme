#ifndef _ML_MOUSE_BUTTON_HPP_
#define _ML_MOUSE_BUTTON_HPP_

#include <libmeme/Platform/Export.hpp>
#include <libmeme/Common.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class mouse_button final
	{
	public:
		enum : int32_t
		{
			Button0,
			Button1,
			Button2,
			Button3,
			Button4,
			Button5,
			Button6,
			Button7,
		};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MOUSE_BUTTON_HPP_