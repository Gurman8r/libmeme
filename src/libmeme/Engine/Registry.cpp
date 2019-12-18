#include <libmeme/Engine/Registry.hpp>
#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	ML_REGISTER(mat4, "primitive") { return std::make_optional(mat4{}); }
}