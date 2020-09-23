#include <libmeme/Scene/SceneManager.hpp>

namespace ml
{
	scene_manager * scene_manager::g_instance{};

	scene_manager::scene_manager() noexcept
	{
		ML_assert(!g_instance && (g_instance = this));
	}

	scene_manager::~scene_manager() noexcept
	{
		ML_assert(g_instance == this && !(g_instance = nullptr));
	}
}