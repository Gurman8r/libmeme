#include <libmeme/Engine/Registry.hpp>
#include <libmeme/Renderer/Material.hpp>
#include <libmeme/Renderer/Shader.hpp>
#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	decltype(registrar::m_storage) registrar::m_storage{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_REGISTER(material)	{ return std::make_optional(make_material()); }
	ML_REGISTER(shader)		{ return std::make_optional(make_shader()); }
	ML_REGISTER(texture)	{ return std::make_optional(make_texture()); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}