#ifndef _ML_SCENE_MANAGER_HPP_
#define _ML_SCENE_MANAGER_HPP_

#include <libmeme/Scene/Scene.hpp>

namespace ml
{
	struct scene_manager final : trackable, non_copyable
	{
		scene_manager() noexcept;

		~scene_manager() noexcept;

		ML_NODISCARD static scene_manager * get() noexcept { return g_instance; }

	private:
		static scene_manager * g_instance;
	};
}

#endif // !_ML_SCENE_MANAGER_HPP_