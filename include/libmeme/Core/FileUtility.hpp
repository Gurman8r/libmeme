#ifndef _ML_FILE_UTILITY_HPP_
#define _ML_FILE_UTILITY_HPP_

#include <libmeme/Core/StringUtility.hpp>

namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// load file contents into vector
	template <class Ch = char, class Tr = std::char_traits<Ch>, class Vec = pmr::vector<Ch>
	> static inline std::optional<Vec> get_file_contents(filesystem::path const & path)
	{
		std::basic_ifstream<Ch, Tr> file{ path, std::ios_base::binary };
		ML_defer{ file.close(); };
		if (!file) { return std::nullopt; }

		Vec temp{};
		file.seekg(0, std::ios_base::end);
		if (std::streamsize size{ file.tellg() }; size > 0)
		{
			file.seekg(0, std::ios_base::beg);
			
			temp.resize(static_cast<size_t>(size));
			
			file.read(&temp[0], size);
		}
		return std::make_optional(std::move(temp));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// load file contents into string
	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Str)
	> inline Str read_file(filesystem::path const & path) noexcept
	{
		if (auto const contents{ util::get_file_contents<Ch>(path) })
		{
			return Str{ contents->begin(), contents->end() };
		}
		else
		{
			return {};
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FILE_UTILITY_HPP_