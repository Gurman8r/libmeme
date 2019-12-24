#ifndef _ML_PYTHON_HPP_
#define _ML_PYTHON_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Singleton.hpp>

#define ML_Python ::ml::Python::getInstance()

namespace ml
{
	struct ML_ENGINE_API Python final : public Singleton<Python>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool init(path_t const & name, path_t const & home);
		
		bool dispose();
		
		int32_t do_string(std::string const & value) const;
		
		int32_t do_file(path_t const & filename) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Singleton<Python>;

		Python();
		~Python();

		bool m_init{ false };
		path_t m_name{};
		path_t m_home{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PYTHON_HPP_