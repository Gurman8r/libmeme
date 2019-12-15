#ifndef _ML_PYTHON_HPP_
#define _ML_PYTHON_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/FileSystem.hpp>
#include <libmeme/Core/StringUtility.hpp>

namespace ml
{
	class ML_ENGINE_API Py final : public Singleton<Py>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend Singleton<Py>;

		Py() {}
		~Py() {}
		
		bool m_init { false };
		std::string	m_name {};
		std::string	m_home {};

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool	init(std::string const & name, std::string const & home);
		bool	restart();
		bool	dispose();
		int32_t doString(std::string const & value) const;
		int32_t doFile(std::string const & filename) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PYTHON_HPP_