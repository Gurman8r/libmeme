#include <libmeme/Engine/Registry.hpp>
#include <libmeme/Renderer/Material.hpp>
#include <libmeme/Renderer/Shader.hpp>
#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Registry<>::Registry() noexcept : m_storage{}
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_REGISTER(Material,	"") { return std::make_optional(make_material()); }
	ML_REGISTER(Shader,		"") { return std::make_optional(make_shader()); }
	ML_REGISTER(Texture,	"") { return std::make_optional(make_texture()); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}