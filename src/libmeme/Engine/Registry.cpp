#include <libmeme/Engine/Registry.hpp>
#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Registry<>::Registry() noexcept : m_codes{}, m_funcs{}, m_infos{}, m_names{}
	{
	}

	Registry<>::~Registry() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_REGISTER(mat4, "primitive") { return std::make_optional(mat4::identity()); }

	// etc...

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}