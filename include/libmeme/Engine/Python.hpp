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

		bool init(Path const & name, Path const & home);
		
		bool dispose();
		
		int32_t do_string(std::string const & value) const;
		
		int32_t do_file(Path const & filename) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend Singleton<Python>;

		Python();
		~Python();

		bool m_init{ false };
		Path m_name{};
		Path m_home{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PYTHON_HPP_