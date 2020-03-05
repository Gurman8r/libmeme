#ifndef _ML_FILE_SYSTEM_HPP_
#define _ML_FILE_SYSTEM_HPP_

#include <libmeme/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class FS final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ch = char
		> static inline std::optional<pmr::vector<Ch>> get_file_contents(fs::path const & path)
		{
			std::basic_ifstream<Ch, std::char_traits<Ch>> file{ path, std::ios_base::binary };
			ML_DEFER{ file.close(); };
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

		template <class Ch = char
		> static inline pmr::basic_string<Ch> get_file_string(fs::path const & path)
		{
			if (auto const o{ FS::get_file_contents<Ch>(path.string()) })
			{
				return pmr::basic_string<Ch>{ o->cbegin(), o->cend() };
			}
			else
			{
				return {};
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FILE_SYSTEM_HPP_