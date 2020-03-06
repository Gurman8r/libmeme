#ifndef _ML_FILE_UTILITY_HPP_
#define _ML_FILE_UTILITY_HPP_

#include <libmeme/Core/StringUtility.hpp>

namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch = char, class Tr = std::char_traits<Ch>
	> static inline std::optional<pmr::vector<Ch>> get_file_contents(filesystem::path const & path)
	{
		std::basic_ifstream<Ch, Tr> file{ path, std::ios_base::binary };
		ML_defer{ file.close(); };
		if (!file) { return std::nullopt; }

		pmr::vector<Ch> temp{};
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

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Str)
	> inline Str read_file(filesystem::path const & path) noexcept
	{
		if (auto const o{ util::get_file_contents<Ch>(path.string()) })
		{
			return Str{ o->begin(), o->end() };
		}
		else
		{
			return {};
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FILE_UTILITY_HPP_